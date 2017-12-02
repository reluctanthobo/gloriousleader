#include "level.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>

int run_game(SDL_Renderer *renderer) {
  struct level *level;
  int done;
  
  level = level_new_random(100);

  done = 0;
  while( !done ) {
    SDL_Event ev;
    if( SDL_PollEvent(&ev) ) {
      switch(ev.type) {
      case SDL_QUIT:
	done = 1;
	break;
      }
    }

    level_draw(level, renderer, 20);
    SDL_RenderPresent(renderer);
  }

  level_free(level);
  return 0;
}

int main(int argc, char **argv) {
  SDL_Renderer *renderer;
  SDL_Window *screen;

  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  IMG_Init(IMG_INIT_PNG);
  
  SDL_GameControllerEventState(SDL_ENABLE);
  screen = SDL_CreateWindow("Save the citizens",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    1280, 720,
			    SDL_WINDOW_SHOWN);
  
  renderer = SDL_CreateRenderer(screen, -1, 0);

  for( ;; ) {
    if( !run_game(renderer) ) {
      break;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);
  IMG_Quit();
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
