#include "../include/processlist.h"

#include <stdlib.h>

int init_processlist(struct processlist *list) {
  list->start.pid = -1;
  list->start.next = &list->end;

  list->end.pid = -1;

  list->last = &list->start;

  return 0;
}

int free_processlist(struct processlist *list) {
  struct process *prev = &list->start;
  struct process *current = prev->next;
  while(current != &list->end) {
    current = current->next;
    free(prev->next);
  }

  return 0;
}

int add_process(struct processlist *list, pid_t pid) {
  struct process *new = (struct process *)malloc(sizeof(struct process));
  new->pid = pid;
  new->next = &list->end;

  list->last->next = new;
  list->last = new;

  return 0;
}


int remove_process(struct processlist *list, struct process *prev) {
  struct process *del = prev->next;
  prev->next = del->next;
  list->last = prev;
  free(del);

  return 0;
}