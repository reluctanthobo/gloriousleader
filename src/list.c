#include "list.h"

#include <stdlib.h>

struct list *list_new(void) {
  struct list *ret;
  ret = malloc(sizeof(*ret));
  ret->head = NULL;
  ret->tail = NULL;
  return ret;
}

void list_free(struct list *list) {
  struct list_entry *cur;
  struct list_entry *next;
  if( !list ) {
    return;
  }

  for( cur=list->head; cur; cur=next ) {
    next = cur->next;
    free(cur);
  }
  free(list);
}

void list_append(struct list *list, void *element) {
  struct list_entry *ent;
  ent = malloc(sizeof(*ent));
  ent->next = NULL;
  ent->prev = NULL;
  ent->data = element;

  if( !list->tail ) {
    list->head = ent;
    list->tail = ent;
  } else {
    ent->prev = list->tail;
    list->tail->next = ent;
    list->tail = ent;
  }
}

void list_remove(struct list *list, void *element) {
  struct list_entry *cur;

  for( cur = list->head; cur; cur = cur->next ) {
    if( cur->data == element ) {
      if( cur->prev ) {
	cur->prev->next = cur->next;
      } else {
	list->head = cur->next;
      }
      if( cur->next ) {
	cur->next->prev = cur->prev;
      } else {
	list->tail = cur->prev;
      }
    }
  }
}
