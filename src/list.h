#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct list_entry {
  struct list_entry *next;
  struct list_entry *prev;
  void *data;
};

struct list {
  struct list_entry *head;
  struct list_entry *tail;
};

struct list *list_new(void);
void list_free(struct list *list);
void list_append(struct list *list, void *element);
void list_remove(struct list *list, void *element);

#endif
