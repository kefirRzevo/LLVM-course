set(GUI_Lib_Sources ${CMAKE_SOURCE_DIR}/gui-app/lib/GUILib.c)
set(GUI_One_Cycle_App_Source ${CMAKE_SOURCE_DIR}/gui-app/src/OneCycleApplication.c)
set(GUI_App_Source ${CMAKE_SOURCE_DIR}/gui-app/src/Application.c)
set(GUI_App_Start_Source ${CMAKE_SOURCE_DIR}/gui-app/src/Start.c)
set(GUI_APP_Include_Dir ${CMAKE_SOURCE_DIR}/gui-app/include)

add_library(GUI_Lib STATIC ${GUI_Lib_Sources})
target_include_directories(GUI_Lib PRIVATE ${GUI_APP_Include_Dir} ${SDL2_INCLUDE_DIRS})
target_link_libraries(GUI_Lib PRIVATE ${SDL2_LIBRARIES})

add_executable(GUI_App ${GUI_App_Source} ${GUI_App_Start_Source})
target_include_directories(GUI_App PRIVATE ${GUI_APP_Include_Dir})
target_link_libraries(GUI_App PRIVATE GUI_Lib)

set(GUI_App_IR ${CMAKE_SOURCE_DIR}/gui-app/llvm-ir/Application.ll)
add_custom_command(
  OUTPUT ${GUI_App_IR}
  DEPENDS ${GUI_App_Source} ${GUI_APP_Include_Dir}
  COMMAND clang
          -O3 -std=c23 -emit-llvm -S
          ${GUI_App_Source}
          -I ${GUI_APP_Include_Dir}
          -o ${GUI_App_IR}
  COMMENT "Generate LLVM IR for Application" VERBATIM
)

add_custom_target(GUI_App_IR_Target ALL
  DEPENDS ${GUI_App_IR}
)

set(GUI_One_Cycle_App_IR ${CMAKE_SOURCE_DIR}/gui-app/llvm-ir/OneCycleApplication.ll)
add_custom_command(
  OUTPUT ${GUI_One_Cycle_App_IR}
  DEPENDS ${GUI_One_Cycle_App_Source} ${GUI_APP_Include_Dir}
  COMMAND clang
          -O3 -std=c23 -emit-llvm -S
          ${GUI_One_Cycle_App_Source}
          -I ${GUI_APP_Include_Dir}
          -o ${GUI_One_Cycle_App_IR}
  COMMENT "Generate LLVM IR for Application" VERBATIM
)

add_custom_target(GUI_One_Cycle_App_IR_Target ALL
  DEPENDS ${GUI_One_Cycle_App_IR}
)
