cmake_minimum_required(VERSION 3.16)

project(paracl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(INTERPETER "${PROJECT_NAME}")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
include_directories(src/include ${Boost_INCLUDE_DIR})

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

set(INTERPETER_SOURCES
  src/frontend/Driver.cc
  src/ParaCL.cc
  src/backend/IRGen.cc
)
add_executable(
  ${INTERPETER}
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  ${INTERPETER_SOURCES}
)
target_include_directories(
  ${INTERPETER} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(${INTERPETER} ${Boost_PROGRAM_OPTIONS_LIBRARY})

