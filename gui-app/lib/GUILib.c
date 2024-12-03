#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "GUILib.h"

#define FRAME_TICKS 50

static SDL_Renderer *Renderer = NULL;
static SDL_Window *Window = NULL;
static Uint32 Ticks = 0;

void gui_init() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(GUI_WIDTH, GUI_HEIGHT, 0, &Window, &Renderer);
  SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
  SDL_RenderClear(Renderer);
  srand(time(NULL));
  gui_set_pixel(0, 0, 0);
  gui_flush();
}

void gui_exit() {
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();
}

void gui_flush() {
  SDL_PumpEvents();
  Uint32 cur_ticks = SDL_GetTicks() - Ticks;
  if (cur_ticks < FRAME_TICKS)
    SDL_Delay(FRAME_TICKS - cur_ticks);
  SDL_RenderPresent(Renderer);
}

int gui_quit_event() {
  SDL_PumpEvents();
  return SDL_HasEvent(SDL_QUIT) == SDL_TRUE;
}

void gui_set_pixel(int x, int y, int color) {
  assert(0 <= x && x < GUI_WIDTH && "Out of range");
  assert(0 <= y && y < GUI_HEIGHT && "Out of range");
  Uint8 r = (color >> 24) & 0xFF;
  Uint8 g = (color >> 16) & 0xFF;
  Uint8 b = (color >> 8) & 0xFF;
  Uint8 a = color & 0xFF;
  SDL_SetRenderDrawColor(Renderer, r, g, b, a);
  SDL_RenderDrawPoint(Renderer, x, y);
  Ticks = SDL_GetTicks();
}

int gui_rand() { return rand(); }
