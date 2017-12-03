#include "camera.h"
#include "image.h"
#include "level.h"
#include "list.h"
#include "monster.h"
#include "player.h"
#include "sprite.h"
#include "survivor.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>

static int sprite_cmp(const void *v_a, const void *v_b) {
  const struct sprite * a = v_a;
  const struct sprite * b = v_b;
  return a->y - b->y;
}

#if 0
static void use_survivors(struct player *p, struct list *survivors, struct level *lev) {
  int teleport = 0;
  struct list_entry *cur;
  
  for( cur = survivors->head; cur; cur = cur->next ) {
    struct survivor *s = cur->data;
    if( !s->frozen && s->pos == p->pos && lev->color[p->pos] == s->color ) {
    }
  }
}
#endif

int run_game(SDL_Renderer *renderer) {
  struct level *level;
  struct image *player_img;
  struct image *survivor_img[3];
  struct image *monster_img;
  struct image *bullet_img;
  struct image *instructions_img;
  struct camera *cam;
  struct list *survivors;
  struct list *monsters;
  struct player *player;
  unsigned live_survivors;
  unsigned live_monsters;
  unsigned total_survivors;
  unsigned power;
  unsigned end_of_turn;
  int done;

  player_img = image_new("player.png", renderer);
  survivor_img[0] = image_new("soldier.png", renderer);
  survivor_img[1] = image_new("engineer.png", renderer);
  survivor_img[2] = image_new("scientist.png", renderer);
  monster_img = image_new("alien.png", renderer);
  bullet_img = image_new("bullet.png", renderer);
  instructions_img = image_new("instructions.png", renderer);
  
  level = level_new_random(120);
  cam = camera_new(renderer, 1280, 720, 0);
  player = player_new(20, 3, player_img);

  total_survivors = 0;
  live_survivors = 0;
  live_monsters = 0;
  power = 0;

  survivors = list_new();
  monsters = list_new();

  {
    struct monster *m;
    m = monster_new(player->pos - 20, monster_img);
    list_append(monsters, m);
    ++live_monsters;
  }
  /* 1s turn */
  end_of_turn = SDL_GetTicks() + 500;

  done = 0;
  while( !done ) {
    SDL_Event ev;
    int new_turn;
    new_turn = 0;
    if( SDL_PollEvent(&ev) ) {
      switch(ev.type) {
      case SDL_QUIT:
	done = 4;
	break;
      case SDL_KEYDOWN:
	switch( ev.key.keysym.sym ) {
	case SDLK_ESCAPE:
	  done = 3;
	  break;
	case SDLK_LEFT:
	  player->pos -= 1;
	  new_turn = 1;
	  break;
	case SDLK_RIGHT:
	  player->pos += 1;
	  new_turn = 1;
	  break;
	case SDLK_RETURN:
	  new_turn = 1;
	  break;
	case SDLK_SPACE:
	  {
	    struct list_entry *cur;
	    const unsigned color = level->color[player->pos];
	    int port = 0;
	    for( cur = survivors->head; cur; cur=cur->next ) {
	      struct survivor *s = cur->data;
	      if( s->pos == player->pos && s->type == color && !s->frozen ) {
		s->frozen = 1;
		if( color == 0 ) {//RED
		  level->quality[player->pos] ++;
		} else if( color == 1 ) {//GREEN
		  power += 3*(level->quality[player->pos] + 1);
		  level->quality[player->pos] = 0;
		} else { //BLUE
		  port += level->quality[player->pos]+1;
		  level->quality[player->pos] = 0;
		}
		new_turn = 1;
	      }
	    }
	    if( port > 0 ) {
	      unsigned last = 0;
	      unsigned offset = 0;
	      while( player->pos + offset < level->num_tiles ) {
		const unsigned index = player->pos + offset;
		if( level->color[index] == 2 ) {
		  --port;
		  last = offset;
		  if( port == 0 ) {
		    break;
		  }
		}
		++offset;
	      }
	      for( cur = survivors->head; cur; cur = cur->next ) {
		struct survivor *s = cur->data;
		if( s->pos == player->pos && !s->frozen ) {
		  s->pos += last;
		}
	      }
	      player->pos += last;
	    }
	  }
	  break;
	}
      }
    }

    if( SDL_GetTicks() > end_of_turn ) {
      new_turn = 1;
    }
    if( new_turn ) {
      end_of_turn = SDL_GetTicks()+500;
    }

    if( player->pos >= level->num_tiles )  {
      done = 2;
      break;
    }

    if( new_turn ) {
      if( power > 0 ) {
	--power;
      }
      {
	struct list_entry *cur;
	for( cur = survivors->head; cur; cur=cur->next ) {
	  struct survivor *s = cur->data;
	  if( !s->frozen && rand()/(float)RAND_MAX < 0.75 ) {
	    s->pos += 1;
	  }
	  survivor_update(s);
	}
      }

      {
	const float scale = 25+(total_survivors < 50 ? total_survivors : 50 );
	if( rand()/(float)RAND_MAX < scale/100.0)  {
	  static const int colors[10] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 2 };
	  struct survivor *s;
	  int color;
	  color = colors[rand()%10];
	  s = survivor_new(color, "", player->pos, survivor_img[color]);
	  list_append(survivors, s);
	  ++live_survivors;
	  ++total_survivors;
	}
      }

      if( live_survivors > live_monsters*5 && rand()/(float)RAND_MAX < 0.5 ) {
	struct monster *m;
	m = monster_new(player->pos - 20, monster_img);
	list_append(monsters, m);
	++live_monsters;
      }

      {
	const int monster_speed = power > 0 ? 1 : 2;
	struct list_entry *cur;
	struct list_entry *next;
	for( cur = monsters->head; cur; cur = next ) {
	  struct list_entry *cur2;
	  struct list_entry *next2;
	  struct monster *m = cur->data;
	  unsigned monster_pos;
	  int defended;
	  next = cur->next;
	  monster_pos = m->pos + monster_speed;
	  /* find out where the monster stops */
	  for( cur2 = survivors->head; cur2; cur2= cur2->next ) {
	    struct survivor *s = cur2->data;
	    if( s->pos >= m->pos && s->pos < monster_pos && 
		s->frozen && level->quality[s->pos] > 0 ) {
	      monster_pos = s->pos;
	    }
	  }
	  defended = 0;
	  for( cur2 = survivors->head; cur2; cur2 = next2 ) {
	    struct survivor *s = cur2->data;
	    next2 = cur2->next;
	    if( s->pos == monster_pos && level->quality[monster_pos] > 0 && s->frozen ) {
	      defended++;
	    } else if( s->pos <= monster_pos ) {
	      list_remove(survivors, s);
	      survivor_free(s);
	      --live_survivors;
	    }
	  }
	  {
	    unsigned i;
	    for( i=m->pos; i<monster_pos; ++i ) {
	      level->quality[i] = 0;
	    }
	  }
	  if( level->quality[monster_pos] > 0 ) {
	    if( defended ) {
	      level->quality[monster_pos] -= 1;
	    } else {
	      level->quality[monster_pos] = 0;
	    }
	  }
	  m->pos = monster_pos;
	  monster_update(m);
	  if( m->pos >= player->pos && !defended ) {
	    if( player->bullets == 0 ) {
	      done = 1;
	    } else {
	      list_remove(monsters, m);
	      monster_free(m);
	      -- live_monsters;
	      --player->bullets;
	    }
	  }
	}
      }
    }

    camera_set_target(cam, player->pos);
    camera_update(cam);
    player_update(player);

    if( power > 0 ) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
    }
    SDL_RenderClear(renderer);

    {
      unsigned i;
      for( i=0; i<player->bullets; ++i ) {
	image_draw(bullet_img, renderer, 40+32*i, TILE_Y+TILE_W*3/2, 0);
      }
      image_draw(instructions_img, renderer, 640 - instructions_img->w,
		 TILE_Y + TILE_W*3/2, 0);
    }
    
    level_draw(level, cam);

    {
      unsigned count;
      unsigned i;
      const struct sprite **sprites;
      struct list_entry *ent;
      count = 0;
      ++count;
      for( ent = survivors->head; ent; ent = ent->next ) {
	++count;
      }
      for( ent = monsters->head; ent; ent = ent->next ) {
	++count;
      }
      sprites = malloc(sizeof(*sprites) * count);
      count = 0;
      sprites[count++] = player_draw(player);
      for( ent = survivors->head; ent; ent = ent->next ) {
	struct survivor *s = ent->data;
	sprites[count++] = survivor_draw(s);
      }
      for( ent = monsters->head; ent; ent = ent->next ) { 
	struct monster *m = ent->data;
	sprites[count++] = monster_draw(m);
      }
      qsort(sprites, count, sizeof(struct sprite *), sprite_cmp);
      for( i=0; i<count; ++i ) {
	sprite_draw(sprites[i], cam);
      }
      free(sprites);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(20);
  }

  {
    struct list_entry *ent;
    for( ent = survivors->head; ent; ent = ent->next ) {
      struct survivor *s;
      s = ent->data;
      survivor_free(s);
    }
    for( ent = monsters->head; ent; ent = ent->next ) {
      struct monster *m;
      m = ent->data;
      monster_free(m);
    }
  }
  camera_free(cam);
  player_free(player);
  list_free(survivors);
  list_free(monsters);
  image_free(bullet_img);
  image_free(instructions_img);
  image_free(player_img);
  image_free(monster_img);
  image_free(survivor_img[0]);
  image_free(survivor_img[1]);
  image_free(survivor_img[2]);
  level_free(level);
  return done;
}

static SDL_Texture *load_image(const char *filename, SDL_Renderer *r) {
  SDL_Surface *surf = IMG_Load(filename);
  SDL_Texture *ret;
  ret = SDL_CreateTextureFromSurface(r, surf);
  SDL_FreeSurface(surf);
  return ret;
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)) ) {
  SDL_Renderer *renderer;
  SDL_Window *screen;
  SDL_Texture *win_screen;
  SDL_Texture *fail_screen;
  SDL_Texture *title_screen;

  srand((unsigned)SDL_GetPerformanceCounter());

  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  IMG_Init(IMG_INIT_PNG);
  
  SDL_GameControllerEventState(SDL_ENABLE);
  screen = SDL_CreateWindow("Save the citizens",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    1280, 720,
			    SDL_WINDOW_SHOWN);
  
  renderer = SDL_CreateRenderer(screen, -1, 0);

  win_screen = load_image("win.png", renderer);
  fail_screen = load_image("fail.png", renderer);
  title_screen = load_image("title.png", renderer);

  for( ;; ) {
    int retcode;
    {
      SDL_Event ev;
      int done = 0;
      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, title_screen, NULL, NULL);
      SDL_RenderPresent(renderer);
      while( !done && SDL_WaitEvent(&ev) ) {
	switch( ev.type ) {
	case SDL_QUIT:
	  done = 1;
	  break;
	case SDL_KEYDOWN:
	  switch(ev.key.keysym.sym ) {
	  case SDLK_ESCAPE:
	    done = 1;
	    break;
	  default:
	    done = 2;
	    break;
	  }
	}
      }
      if( done == 1 ) {
	break;
      }
    }

    retcode = run_game(renderer);
    if( retcode == 4 ) {
      // QUIT
      break;
    }
    if( retcode < 3 ) {
      SDL_Event ev;
      int done = 0;
      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
      SDL_RenderClear(renderer);
      if( retcode == 1 ) {
	SDL_RenderCopy(renderer, fail_screen, NULL, NULL);
      } else if( retcode == 2 ) {
	SDL_RenderCopy(renderer, win_screen, NULL, NULL);
      }
      SDL_RenderPresent(renderer);
      while( !done && SDL_WaitEvent(&ev) ) {
	switch( ev.type ) {
	case SDL_QUIT:
	  done = 1;
	  break;
	case SDL_KEYDOWN:
	  switch(ev.key.keysym.sym ) {
	  case SDLK_ESCAPE:
	    done = 1;
	    break;
	  default:
	    done = 2;
	    break;
	  }
	}
      }
      if( done == 1 ) {
	break;
      }
    }
  }

  SDL_DestroyTexture(win_screen);
  SDL_DestroyTexture(fail_screen);
  SDL_DestroyTexture(title_screen);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);
  IMG_Quit();
  SDL_Quit();
  exit(EXIT_SUCCESS);
}
