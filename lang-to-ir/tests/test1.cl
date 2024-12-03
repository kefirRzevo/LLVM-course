
void func(int[512] layer):set_bound_cond {
	int k = 0;
	while (k != 512) {
		int need_set = gui_rand() % 2;
    if (need_set)
      layer[k] = 1;
    else
      layer[k] = 0;
		k = k+1;
	}
}
/*
void apply_rule(int[512] prev, int[512] next) {
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
}*/

gui_init();

  int[512][512] data = {};
  set_bound_cond(data[0]);
  int cur = 0;
  while (1) {
    if (gui_quit_event())
      break;
    //apply_rule(data[cur % GUI_HEIGHT], data[(cur + 1) % GUI_HEIGHT]);
		/*
    for (int i = 0; i != GUI_HEIGHT; ++i) {
      for (int j = 0; j != GUI_WIDTH; ++j) {
        if (data[i][j])
          gui_set_pixel(j, i, GUI_BLACK);
        else
          gui_set_pixel(j, i, GUI_WHITE);
      }
    }*/
    gui_flush();
    cur = cur + 1;
  }

gui_exit();
