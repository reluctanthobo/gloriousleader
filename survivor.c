#include "survivor.h"

#include "camera.h"
#include "image.h"
#include "sprite.h"

struct survivor *survivor_new(unsigned type, char *name, unsigned pos, const struct image *image) {
  struct survivor *ret;
  ret = malloc(sizeof(*ret));
  ret->type = type;
  ret->name = strdup(name);
  ret->pos = pos;
  ret->frozen = 0;
  ret->offset_x = TILE_W/2;
  ret->offset_y = (rand()%TILE_W) - image->h/2;
  ret->sprite = sprite_new(image, ret->pos*TILE_W+ret->offset_x, ret->offset_y);
  return ret;
}

void survivor_free(struct survivor *s) {
  if (!s ) {
    return;
  }
  sprite_free(s->sprite);
  free(s->name);
  free(s);
}

struct sprite *survivor_draw(struct survivor *s) {
  return s->sprite;
}

void survivor_update(struct survivor *s) {
  s->sprite->x = s->pos * TILE_W + s->offset_x;
  s->sprite->flip = s->frozen;
}
