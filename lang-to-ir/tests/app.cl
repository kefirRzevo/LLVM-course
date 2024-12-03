
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
	return;
}

void func(int[512] prev, int[512] next): apply_rule {
	int k = 0;
	while (k != 512) {
    int neighbors = 0;
    if (prev[(k - 1 + 512) % 512])
      neighbors = neighbors + 4;
    if (prev[k])
      neighbors = neighbors + 2;
    if (prev[(k + 1) % 512])
      neighbors = neighbors + 1;
    if (110 & (1 << neighbors))
      next[k] = 1;
    else
      next[k] = 0;
		k = k + 1;
  }
	return;
}

gui_init();

int[512][512] data = {};
set_bound_cond(data[0]);
int cur = 0;
while (1) {
	if (gui_quit_event())
		break;
	apply_rule(data[cur % 512], data[(cur + 1) % 512]);
	int i = 0;
	while(i != 512) {
		int j = 0;
		while (j != 512) {
			if (data[i][j])
				gui_set_pixel(j, i, 255);
			else
				gui_set_pixel(j, i, -1);
			j = j +1;
		}
		i = i +1;
	}
	gui_flush();
	cur = cur + 1;
}

gui_exit();
