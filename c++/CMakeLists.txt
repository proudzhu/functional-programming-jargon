cmake_minimum_required(VERSION 3.1.0)

project(fp-jargon CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(src)

enable_testing()
add_test(NAME arity COMMAND arity_test)
add_test(NAME hof COMMAND hof_test)
add_test(NAME partial COMMAND partial_test)
add_test(NAME curry COMMAND curry_test)
add_test(NAME compose COMMAND compose_test)
