include_directories(${LLVM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
add_llvm_executable(IR_Gen src/IRGen.cc ${CMAKE_SOURCE_DIR}/gui-app/lib/GUILib.c)
target_link_libraries(IR_Gen PRIVATE ${SDL2_LIBRARIES})
target_include_directories(IR_Gen PRIVATE ${CMAKE_SOURCE_DIR}/gui-app/include)
