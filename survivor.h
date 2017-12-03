#ifndef SURVIVOR_H_INCLUDED
#define SURVIVOR_H_INCLUDED

struct image;

struct survivor {
  unsigned  type;
  char     *name;
  unsigned  pos;
  int  offset_x;
  int  offset_y;
  int frozen;
  struct sprite *sprite;
};

struct survivor *survivor_new(unsigned type, char *name, unsigned position, const struct image *img);
void survivor_free(struct survivor *s);
struct sprite *survivor_draw(struct survivor *s);
void survivor_update(struct survivor *s);

#endif
