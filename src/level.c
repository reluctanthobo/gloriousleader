#include "level.h"

enum colors {
  RED, 
  GREEN,
  BLUE
};

struct level *level_new_random(unsigned elements) {
  struct level *ret;
  unsigned i;

  ret = malloc(sizeof(*ret));
  ret->color = malloc(sizeof(*ret->color) * elements);
  ret->quality = malloc(sizeof(*ret->quality) * elements);
  ret->name = malloc(sizeof(*ret->name) * elements);
  ret->num_tiles = elements;

  for( i=0; i<elements; ++i ) {
    ret->color[i] = rand() % 3;
    ret->name[i] = strdup("metro station etc etc");
    ret->quality[i] = (rand() % 5) + 1;
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

static const int tile_w = 96;
static const int tile_y = 200;
static const int red_array[3] = { 192, 32, 32 };
static const int green_array[3] = { 16, 192, 16 };
static const int blue_array[3] = { 16, 16, 255 };

static void draw_one_tile(const struct level *level, unsigned tile, SDL_Renderer *renderer, 
			  int offset_x) {
  const unsigned color_index = level->color[tile];
  SDL_Point p[4];
  SDL_Rect r;
  r.x = offset_x;
  r.y = tile_y - 32;
  r.w = tile_w;
  r.h = 32;
  SDL_SetRenderDrawColor(renderer, 
			 red_array[color_index], 
			 green_array[color_index], 
			 blue_array[color_index],
			 255);
  SDL_RenderFillRect(renderer, &r);
  SDL_SetRenderDrawColor(renderer, 0,0,0 ,255);
  //SDL_SetRenderDrawColor(renderer, 255 ,255, 255, 255);
  SDL_RenderDrawRect(renderer, &r);

  p[0].x = r.x;
  p[0].y = r.y + r.h;
  p[1].x = p[0].x + tile_w;
  p[1].y = p[0].y + tile_w;
  p[2].x = p[1].x + tile_w-1;
  p[2].y = p[1].y;
  p[3].x = p[0].x + tile_w-1;
  p[3].y = p[0].y;
  SDL_RenderDrawLines(renderer, p, 4);
}

void level_draw(const struct level *level, SDL_Renderer *renderer, unsigned offset) {
  int i;
  int offset_x;
  int window;

  window = 1;
  offset_x = 640;
  while( offset_x > -tile_w ) {
    offset_x -= tile_w;
    window += 1;
  }
  {
    SDL_Rect r;
    r.x = 0;
    r.y = tile_y;
    r.w = 1280;
    r.h = tile_w;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &r);
  }

  for( i=offset-window; i<offset+window; ++i )  {
    if( i >= 0 ) {
      draw_one_tile(level, (unsigned)i, renderer, offset_x);
    }
    offset_x += tile_w;
  }
}

