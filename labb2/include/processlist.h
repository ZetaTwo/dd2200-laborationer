#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <sys/types.h>

struct process {
  struct process *next;
  pid_t pid;
};

struct processlist {
  struct process start;
  struct process *last;
  struct process end;
};

int init_processlist(struct processlist *list);
int free_processlist(struct processlist *list);

int add_process(struct processlist *list, pid_t pid);
int remove_process(struct processlist *list, struct process *prev);

#endif