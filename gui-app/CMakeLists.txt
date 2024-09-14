find_package(SDL2 REQUIRED)
find_library(SDL2_LIBRARY NAME SDL2)

set(GUI_Project MyGUI)
set(GUI_LIB MyGUI_Lib)

add_library(${GUI_LIB} lib/GUIImpl.cc)
target_include_directories(${GUI_LIB} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${GUI_LIB} ${SDL2_LIBRARIES})

add_executable(${GUI_Project} Application.cc)
include_directories(include)
target_link_libraries(${GUI_Project} ${GUI_LIB})
