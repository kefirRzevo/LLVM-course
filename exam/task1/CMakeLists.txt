cmake_path(APPEND SOURCE
  ${PROJECT_SOURCE_DIR}
  task1
  source.c
)

cmake_path(APPEND IR_WITH_PHIS
  ${CMAKE_CURRENT_BINARY_DIR}
  with_phis.ll
)

cmake_path(APPEND IR_WITHOUT_PHIS
  ${CMAKE_CURRENT_BINARY_DIR}
  without_phis.ll
)

add_custom_command(
  OUTPUT ${IR_WITH_PHIS}
  DEPENDS ${SOURCE}
  COMMAND clang -O3 -emit-llvm -S ${SOURCE} -o ${IR_WITH_PHIS}
  COMMENT "Generate LLVM IR with phis for ${SOURCE} " VERBATIM
)

add_custom_command(
  OUTPUT ${IR_WITHOUT_PHIS}
  DEPENDS ${SOURCE}
  COMMAND clang -O3 -emit-llvm -S ${SOURCE} -o ${IR_WITHOUT_PHIS}
  COMMAND opt -passes=reg2mem ${IR_WITHOUT_PHIS} -S -o ${IR_WITHOUT_PHIS}
  COMMENT "Generate LLVM IR without phis for ${SOURCE} " VERBATIM
)

add_custom_target(task1 ALL
  DEPENDS ${IR_WITH_PHIS} ${IR_WITHOUT_PHIS}
)
