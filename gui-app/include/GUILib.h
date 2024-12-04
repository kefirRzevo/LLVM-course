#pragma once

#define GUI_WIDTH 512
#define GUI_HEIGHT 512
  
#ifdef __cplusplus
extern "C" {
#endif

// This function is used to initialize gui console.
extern void gui_init();

// This function is used to clear resources after an application is closed.
extern void gui_exit();

// This function flushes all set pixels.
extern void gui_flush();

// This function returns true when user asked to quit.
extern int gui_quit_event();

// This function sets pixel at point with coordinates (x, y) with rgb color.
extern void gui_set_pixel(int x, int y, int color);

// This function returns random integer.
extern int gui_rand();

// This function is provided by user and it is executed in main loop.
extern void app();

#ifdef __cplusplus
}
#endif

// Here are some predefined colors.
#define GUI_BLACK 0x000000FF
#define GUI_WHITE 0xFFFFFFFF
#define GUI_RED 0xFF0000FF
#define GUI_GREEN 0x00FF00FF
#define GUI_BLUE 0x0000FFFF
#define GUI_AQUA 0x00FFFFFF
#define GUI_YELLOW 0xFFFF00FF
#define GUI_PURPLE 0xFF00FFFF
