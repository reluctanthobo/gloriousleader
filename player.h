#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

struct image;

struct player {
  unsigned pos;
  int offset_x;
  int offset_y;
  unsigned bullets;
  struct sprite * sprite;
};

struct player *player_new(unsigned pos, unsigned bullets, const struct image *img);
void player_free(struct player *p);
struct sprite *player_draw(struct player *p);
void player_update(struct player *p);
#endif
