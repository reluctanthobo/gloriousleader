#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <SDL2/SDL.h>

struct camera;

struct level {
  unsigned  * color;
  char     ** name;
  unsigned  * quality;
  unsigned    num_tiles;
};

struct level *level_new_random(unsigned elements);
void level_draw(const struct level *level, struct camera *cam);
void level_free(struct level *level);

#endif
