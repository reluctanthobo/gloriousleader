#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <SDL2/SDL.h>

#define TILE_W 96
#define TILE_Y 200

struct camera {
  int      x;
  int      target;
  unsigned w;
  unsigned h;
  SDL_Renderer *renderer;
};

struct camera *camera_new(SDL_Renderer *r, unsigned w, unsigned h, unsigned x);
void camera_free(struct camera *cam);  
void camera_set_target(struct camera *cam, unsigned pos);
void camera_update(struct camera *cam);

#endif
