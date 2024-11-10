include_directories(${LLVM_INCLUDE_DIRS})
add_llvm_pass_plugin(MyPass ${CMAKE_SOURCE_DIR}/llvm-pass/src/MyPass.cc)

set(GUI_Logged_App_IR ${CMAKE_SOURCE_DIR}/llvm-pass/llvm-ir/LoggedOneCycleApplication.ll)
add_custom_command(
  OUTPUT ${GUI_Logged_App_IR}
  DEPENDS ${MyPass} ${GUI_One_Cycle_App_IR}
  COMMAND opt
          -load-pass-plugin=${CMAKE_CURRENT_BINARY_DIR}/MyPass.so
          -passes=my-pass -S
          ${GUI_One_Cycle_App_IR}
          -o ${GUI_Logged_App_IR}
  COMMENT "Generate LLVM IR for Logged One Cycle Application" VERBATIM
)

add_custom_target(GUI_Logged_App_IR_Target ALL
  DEPENDS ${GUI_Logged_App_IR}
)

set(Log_Source ${CMAKE_SOURCE_DIR}/llvm-pass/src/Log.c)
add_custom_command(
  OUTPUT GUI_Logged_App
  DEPENDS ${GUI_App_Start_Source} ${GUI_Logged_App_IR} ${GUI_Lib_Sources} ${Log_Source} ${GUI_APP_Include_Dir}
  COMMAND clang
          -O3 -std=c23
          ${GUI_App_Start_Source}
          ${GUI_Logged_App_IR}
          ${GUI_Lib_Sources}
          ${Log_Source}
          -I ${GUI_APP_Include_Dir}
          -o GUI_Logged_App
  COMMENT "Generate Logged One Cycle Application" VERBATIM
)

add_custom_target(GUI_Logged_App_Target ALL
    DEPENDS ${GUI_Logged_App}
)
