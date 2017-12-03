#include "level.h"

#include "camera.h"

enum colors {
  RED, 
  GREEN,
  BLUE
};

struct level *level_new_random(unsigned elements) {
  static const int color[10] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 2 };
  struct level *ret;
  unsigned i;

  ret = malloc(sizeof(*ret));
  ret->color = malloc(sizeof(*ret->color) * elements);
  ret->quality = malloc(sizeof(*ret->quality) * elements);
  ret->name = malloc(sizeof(*ret->name) * elements);
  ret->num_tiles = elements;

  for( i=0; i<elements; ++i ) {
    ret->name[i] = strdup("metro station etc etc");
    if( rand()/(float)RAND_MAX < 0.1 ) {
      ret->quality[i] = 0;
      ret->color[i] = 3;
    } else {
      ret->quality[i] = (rand() % 5) + 1;
      ret->color[i] = color[rand() % 10];
    }
  }
  return ret;
}
 
void level_free(struct level *level) {
  unsigned i;
  if( !level ) {
    return;
  }
  for( i=0; i<level->num_tiles;++i ) {
    free(level->name[i]);
  }
  free(level->color);
  free(level->name);
  free(level->quality);
  free(level);
}

static const int red_array[3] = { 192, 32, 32 };
static const int green_array[3] = { 16, 192, 16 };
static const int blue_array[3] = { 16, 16, 255 };

static void draw_one_tile(const struct level *level, unsigned tile, SDL_Renderer *renderer, 
			  int offset_x) {
  const unsigned color_index = level->color[tile];
  const unsigned quality = level->quality[tile];
  SDL_Point p[4];
  SDL_Rect r;
  r.x = offset_x;
  r.y = TILE_Y - 32*quality;
  r.w = TILE_W;
  r.h = 32*quality;
  if(quality > 0 ) {
    SDL_SetRenderDrawColor(renderer, 
			   red_array[color_index], 
			   green_array[color_index], 
			   blue_array[color_index],
			   255);
  }
  SDL_RenderFillRect(renderer, &r);
  SDL_SetRenderDrawColor(renderer, 0,0,0 ,255);
  //SDL_SetRenderDrawColor(renderer, 255 ,255, 255, 255);
  SDL_RenderDrawRect(renderer, &r);

  p[0].x = r.x;
  p[0].y = r.y + r.h;
  p[1].x = p[0].x + TILE_W;
  p[1].y = p[0].y + TILE_W;
  p[2].x = p[1].x + TILE_W-1;
  p[2].y = p[1].y;
  p[3].x = p[0].x + TILE_W-1;
  p[3].y = p[0].y;
  SDL_RenderDrawLines(renderer, p, 4);
}

void level_draw(const struct level *level, struct camera *cam) {
  int start;
  int end;
  int i;

  start = (cam->x / TILE_W) - 1;
  end = (cam->x + cam->w)/TILE_W + 1;

  {
    SDL_Rect r;
    r.x = 0;
    r.y = TILE_Y;
    r.w = cam->w;
    r.h = TILE_W;
    SDL_SetRenderDrawColor(cam->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(cam->renderer, &r);
  }

  for( i=start; i<end; ++i )  {
    if( i >= 0 && i < level->num_tiles ) {
      const int offset_x = i * TILE_W - cam->x;
      draw_one_tile(level, (unsigned)i, cam->renderer, offset_x);
    }
  }
}

