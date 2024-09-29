#include "GUILib.h"

int main(int argc, const char * argv[]) {
  gui_init();
  app(argc, argv);
  gui_exit();
}
