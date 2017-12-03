#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED

struct image;

struct monster {
  unsigned pos;
 int offset_x;
 int offset_y;
  struct sprite * sprite;
};

struct monster *monster_new(unsigned pos, const struct image *img);
void monster_free(struct monster *p);
struct sprite *monster_draw(struct monster *p);
void monster_update(struct monster *p);

#endif
