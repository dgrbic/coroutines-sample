#include <iostream>
#include "generator.h"

using namespace std;
using namespace dg::coro;

generator<int> g() {
    for (int i = 0; i < 10; ++i) {
        co_yield i;
    }
}

int main() {
    
    auto x = g();

    for (auto i : x)
    {
        cout << i << endl;
    }

}