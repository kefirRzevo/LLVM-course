find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Boost 1.74 COMPONENTS program_options REQUIRED)

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

set(SOURCES
  src/frontend/Driver.cc
  src/frontend/DotGenerator.cc
  src/frontend/SemanticAnalyzer.cc
  src/frontend/LibraryLoader.cc
  src/ParaCL.cc
  src/backend/IRGen.cc
  ${CMAKE_SOURCE_DIR}/gui-app/lib/GUILib.c
)

add_executable(
  LangToIR_Gen
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  ${SOURCES}
)

target_include_directories(
  LangToIR_Gen
  PRIVATE
  ${LLVM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${LLVM_INCLUDE_DIRS}
  src/include
  ${Boost_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/gui-app/include
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
  LangToIR_Gen
  PRIVATE
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${SDL2_LIBRARIES}
  LLVM
)
