cmake_minimum_required(VERSION 3.7)

project(LLVM-course)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(LLVM REQUIRED CONFIG)

if("${LLVM_VERSION_MAJOR}" VERSION_LESS 18)
  message(FATAL_ERROR "Found LLVM ${LLVM_VERSION_MAJOR}, but need LLVM 18 or above")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LT_LLVM_INSTALL_DIR}")
message(STATUS "LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
)

include(AddLLVM)

add_subdirectory(gui-app)
add_subdirectory(llvm-pass)
add_subdirectory(ir-gen)
add_subdirectory(asm-to-ir)
