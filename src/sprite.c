#include "sprite.h"

#include "camera.h"
#include "image.h"

struct sprite *sprite_new(const struct image *img, int x, int y) {
  struct sprite *ret;
  ret = malloc(sizeof(*ret));
  ret->x = x;
  ret->y = y;
  ret->flip = 0;
  ret->image = img;
  return ret;
}

void sprite_free(struct sprite *s) {
  if( !s ) {
    return;
  }
  free(s);
}

void sprite_draw(const struct sprite *s, struct camera *cam) {
  image_draw(s->image, cam->renderer, s->x + s->y - cam->x, s->y + TILE_Y, s->flip);
}
