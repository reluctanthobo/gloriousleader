#include "monster.h"

#include "camera.h"
#include "image.h"
#include "sprite.h"

#include <stdlib.h>

struct monster *monster_new(unsigned pos, const struct image *img) {
  struct monster *ret;
  ret = malloc(sizeof(*ret));
  ret->pos = pos;
  ret->offset_x = TILE_W/2;
  ret->offset_y = rand()%TILE_W - img->h/2;
  ret->sprite = sprite_new(img, ret->pos * TILE_W + ret->offset_x, ret->offset_y);
  return ret;
}

void monster_free(struct monster *p)  {
  if( !p ) {
    return;
  }
  sprite_free(p->sprite);
  free(p);
}

void monster_update(struct monster *p) {
  p->sprite->x = p->pos * TILE_W + p->offset_x;
}

struct sprite *monster_draw(struct monster *p) {
  return p->sprite;
}
