#include "camera.h"

struct camera *camera_new(SDL_Renderer *r, unsigned w, unsigned h, unsigned x) {
  struct camera *ret;
  ret = malloc(sizeof(*ret));
  ret->renderer = r;
  ret->x = x;
  ret->target = x;
  ret->w = w;
  ret->h = h;
  return ret;
}

void camera_free(struct camera *cam) {
  if( !cam ) {
    return;
  }
  free(cam);
}

void camera_set_target(struct camera *cam, unsigned pos) {
  cam->target = pos * TILE_W - cam->w/2;
}

void camera_update(struct camera *cam) {
  if( cam->x != cam->target ) {
    cam->x = (15*cam->x + cam->target)/16;
  }
}
