#include "player.h"

#include "camera.h"
#include "image.h"
#include "sprite.h"

#include <stdlib.h>

struct player *player_new(unsigned pos, unsigned bullets, const struct image *img) {
  struct player *ret;
  ret = malloc(sizeof(*ret));
  ret->pos = pos;
  ret->offset_x = TILE_W/2;
  ret->offset_y = rand()%(TILE_W - img->h);
  ret->bullets = bullets;
  ret->sprite = sprite_new(img, ret->pos * TILE_W + ret->offset_x, ret->offset_y);
  return ret;
}

void player_free(struct player *p)  {
  if( !p ) {
    return;
  }
  sprite_free(p->sprite);
  free(p);
}

void player_update(struct player *p) {
  p->sprite->x = p->pos * TILE_W + p->offset_x;
}

struct sprite *player_draw(struct player *p) {
  return p->sprite;
}
