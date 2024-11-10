include_directories(${LLVM_INCLUDE_DIRS} include)
add_llvm_executable(AsmToIR_Gen
  AsmToIRGen.cc
  src/Builder.cc
  src/Insn.cc
  ${CMAKE_SOURCE_DIR}/gui-app/lib/GUILib.c
)
target_link_libraries(AsmToIR_Gen PRIVATE ${SDL2_LIBRARIES})
target_include_directories(AsmToIR_Gen PRIVATE ${CMAKE_SOURCE_DIR}/gui-app/include)
