find_package(SDL2 REQUIRED)
find_library(SDL2_LIBRARY NAME SDL2)

set(GUI_Lib_Sources lib/GUILib.c)
add_library(GUI_Lib STATIC ${GUI_Lib_Sources})
target_include_directories(GUI_Lib PRIVATE include ${SDL2_INCLUDE_DIRS})
target_link_libraries(GUI_Lib ${SDL2_LIBRARIES})

set(GUI_App_Sources src/Application.c src/Start.c)
add_executable(GUI_App ${GUI_App_Sources})
target_include_directories(GUI_App PRIVATE include)
target_link_libraries(GUI_App GUI_Lib)
