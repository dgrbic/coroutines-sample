cmake_minimum_required(VERSION 3.15)
project(coro-test)


include(${CMAKE_SOURCE_DIR}/Build/Output/conanbuildinfo.cmake)
set(CONAN_COMPILER ${CONAN_SETTINGS_COMPILER})
set(CONAN_LIBCXX ${CONAN_SETTINGS_COMPILER_LIBCXX})
conan_basic_setup()

add_subdirectory(basics)