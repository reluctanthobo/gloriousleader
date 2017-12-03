#include "image.h"

#include <SDL2/SDL_image.h>

struct image *image_new(const char *filename, SDL_Renderer *r) {
  struct image *ret;
  SDL_Surface *surf;
  ret = malloc(sizeof(*ret));
  surf = IMG_Load(filename);
  ret->w = surf->w;
  ret->h = surf->h;
  ret->tex = SDL_CreateTextureFromSurface(r, surf);
  SDL_FreeSurface(surf);
  return ret;
}

void image_free(struct image *img) {
  if( !img ) {
    return;
  }
  SDL_DestroyTexture(img->tex);
  free(img);
}

void image_draw(const struct image *img, SDL_Renderer *renderer, int x, int y, int flip) {
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = img->w;
  r.h = img->h;

  //printf("Render image to %d,%d %dx%d\n", r.x, r.y, r.w, r.h);

  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  //SDL_RenderFillRect(renderer, &r);

  SDL_RenderCopyEx(renderer, img->tex, NULL, &r, 0,0, flip ? SDL_FLIP_HORIZONTAL : 0);
}

