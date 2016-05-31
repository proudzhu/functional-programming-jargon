cmake_minimum_required(VERSION 3.1.0)

project(fp-jargon CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
add_test(NAME RunTests COMMAND arity_test)
