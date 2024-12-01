cmake_minimum_required(VERSION 3.16)

project(paracl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(PARACL "${PROJECT_NAME}")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Boost 1.74 COMPONENTS program_options REQUIRED)
find_package(LLVM REQUIRED CONFIG)

add_definitions(-std=c++20 -stdlib=libstdc++)
set(CMAKE_BUILD_TYPE Debug)

flex_target(
  scanner
  src/frontend/Lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)
bison_target(
  parser
  src/frontend/Parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "-Wcounterexamples -v --defines=${CMAKE_CURRENT_BINARY_DIR}/parser.tab.hh"
)
add_flex_bison_dependency(scanner parser)

set(PARACL_SOURCES
  src/frontend/Driver.cc
  src/frontend/DotGenerator.cc
  src/frontend/SemanticAnalyzer.cc
  src/frontend/LibraryLoader.cc
  src/ParaCL.cc
  src/backend/IRGen.cc
)
add_executable(
  ${PARACL}
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  ${PARACL_SOURCES}
)

target_include_directories(
  ${PARACL} PRIVATE
  ${LLVM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${LLVM_INCLUDE_DIRS}
  src/include
  ${Boost_INCLUDE_DIR}
)
llvm_map_components_to_libnames(
  llvm_libs
  ${LLVM_TARGETS_TO_BUILD}
  support
  core
  codegen
  irreader
  mc
  mcparser
  option
)

target_link_libraries(
  ${PARACL}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${llvm_libs}
)
