#include <stdbool.h>
#include "GUILib.h"

void set_bound_cond(bool layer[GUI_WIDTH]) {
  for (int k = 0; k != GUI_WIDTH; ++k) {
    int need_set = gui_rand() % 2;
    if (need_set)
      layer[k] = true;
    else
      layer[k] = false;
  }
}

void apply_rule(bool prev[GUI_WIDTH], bool next[GUI_WIDTH]) {
  for (int k = 0; k != GUI_WIDTH; ++k) {
    int neighbors = 0;
    if (prev[(k - 1 + GUI_WIDTH) % GUI_WIDTH])
      neighbors += 4;
    if (prev[k])
      neighbors += 2;
    if (prev[(k + 1) % GUI_WIDTH])
      neighbors += 1;
    if (110 & (1 << neighbors))
      next[k] = true;
    else
      next[k] = false;
  }
}

void app() {
  bool data[GUI_HEIGHT][GUI_WIDTH] = {};
  set_bound_cond(data[0]);
  int cur = 0;
  for (;;) {
    if (gui_quit_event())
      break;
    apply_rule(data[cur % GUI_HEIGHT], data[(cur + 1) % GUI_HEIGHT]);
    for (int i = 0; i != GUI_HEIGHT; ++i) {
      for (int j = 0; j != GUI_WIDTH; ++j) {
        if (data[i][j])
          gui_set_pixel(j, i, GUI_BLACK);
        else
          gui_set_pixel(j, i, GUI_WHITE);
      }
    }
    gui_flush();
    cur++;
  }
}
