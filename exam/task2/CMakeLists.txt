cmake_path(APPEND SOURCE
  ${PROJECT_SOURCE_DIR}
  task2
  source.tb
)

cmake_path(APPEND TBLGEN_OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}
  output.tb
)

add_custom_command(
  OUTPUT ${TBLGEN_OUTPUT}
  DEPENDS ${SOURCE}
  COMMAND llvm-tblgen-18 ${SOURCE} > ${TBLGEN_OUTPUT}
  COMMENT "Generate LLVM TD file for ${SOURCE} " VERBATIM
)

add_custom_target(task2 ALL
  DEPENDS ${TBLGEN_OUTPUT}
)
