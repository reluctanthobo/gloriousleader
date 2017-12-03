#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <SDL2/SDL.h>

struct image {
  SDL_Texture *tex;
  int w;
  int h;
};

struct image *image_new(const char *filename, SDL_Renderer *renderer);
void image_free(struct image *img);
void image_draw(const struct image *img, SDL_Renderer *renderer, int x, int y, int flip);

#endif
