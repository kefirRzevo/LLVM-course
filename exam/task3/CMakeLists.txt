
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(
  scanner
  lex.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(
  parser
  parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "-Wcounterexamples -v --defines=${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h"
)

add_flex_bison_dependency(scanner parser)

add_executable(
  Parser
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
  #${SOURCES}
)
