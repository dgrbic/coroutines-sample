#pragma once

#include <experimental/coroutine>
#include <exception>
#include <future>
#include <compare>

namespace dg::coro {

template <class T>
class generator {
 public:
  struct promise_type;  // Forward declaration
  class iterator;
  using handle_type = std::experimental::coroutine_handle<promise_type>;

 private:
  using value_type = std::remove_reference_t<T>;
  using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
  using pointer_type = value_type*;
  handle_type hnd_;

 public:
  explicit generator(handle_type h) : hnd_(h) {}
  generator(const generator&) = delete;  // No copy allowed
  generator& operator=(consr generator&) = delete;
  generator(generator&& g) noexcept : hnd_(g.hnd_) { g.hnd_ = nullptr; }
  generator& operator=(generator&& g) noexcept {
    hnd_ = g.hnd_;
    g.hnd_ = nullptr;
    return *this;
  }

  ~generator() {
    if (hnd_)
      hnd_.destroy();
  }
  // TODO Implement iterator interface

  struct iterator_sentinel{};
  class iterator : public std::iterator<std::input_iterator_tag, T> {
    iterator() noexcept : hnd_{nullptr} {};
    explicit iterator(handle_type h) noexcept : hnd_{h} {};
    iterator(const iterator&) = default;
    ~iterator() = default;

    iterator& operator=(const iterator& i) {
      hnd_ = i.hnd_;
      return *this;
    };
    bool operator==(const iterator_sentinel& s) {
      return !hnd_ || hnd_.done();
    }
    iterator& operator++() {
      hnd_.resume();
      // TODO handle errors
      return *this;
    };
    void operator++(int) {
        operator++();
    }
    reference operator*() const noexcept {
      return hnd_.promise().value();
    }
    pointer operator->() const noexcept {
      return &(operator*());
    }

   private:
    handle_type hnd_;

  };

  struct promise_type {
   private:
    pointer_type val_{}; // Ptr to value allows to use type without copy constructor
    friend class generator;

   public:
    promise_type() = default;
    ~promise_type() = default;
    primise_type(const promise_type&) = delete;
    promise_type(promise_type&&) = delete;
    promise_type& operator=(const promise_type&) = delete;
    promise_type& operator=(promise_type&&) = delete;

    auto initial_suspend() { return std::experimental::suspend_never{}; }
    auto final_suspend() { return std::experimental::suspend_always{}; }
    auto get_return_object() {
      return generator{handle_type::from_promise(*this)};
    }
    // return void is needed if coroutine ends execution without co_return
    // or return type is void
    auto return_void() { return std::experimental::suspend_never{}; }
    auto yield_value(value_type& some_value) {
      val_ = &some_value;
      return std::experimental::suspend_always{};
    }
    auto yield_value(value_type&& some_value) {
      val_ = &some_value;
      return std::experimental::suspend_always{};
    }
    reference_type value() {
      if (val_) return static_cast<reference_type>(*val_);
      else std::throw(std::future_error::future_error (std::future_errc::no_state) );
    }
    void unhandled_exception() { std::exit(1); }
  }
};

}  // namespace dg::coro
