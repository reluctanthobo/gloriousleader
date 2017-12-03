#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <SDL2/SDL.h>

struct camera;
struct image;

struct sprite {
  int x;
  int y;
  int flip;
  const struct image *image;
};

struct sprite *sprite_new(const struct image *img, int x, int y);
void sprite_free(struct sprite *s);
void sprite_draw(const struct sprite *s, struct camera *cam);

#endif
