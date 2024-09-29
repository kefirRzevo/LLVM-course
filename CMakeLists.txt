cmake_minimum_required(VERSION 3.7)

project(LLVM-course)
set(CMAKE_C_COMPILER "clang-18")
set(CMAKE_CXX_COMPILER "clang++-18")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(gui-app)
add_subdirectory(llvm-pass)
