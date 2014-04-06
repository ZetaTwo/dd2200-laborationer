/* processlist
 *
 * This module implements a singly linked list
 * of process ids which are used to keep
 * track of the background processes
 */

#include "../include/processlist.h"

#include <stdlib.h>

/** init_processlist
 *
 * Sets up the linked list for usage.
 */
int init_processlist(
  struct processlist *list) { /* IN: The process list to prepare */

  list->start.pid = -1;
  list->start.next = &list->end;

  list->end.pid = -1;

  list->last = &list->start;

  return 0;
}

/** free_processlist
 *
 * Frees up the resources used by the linked list.
 */
int free_processlist(
  struct processlist *list) { /* IN: The process list to free up */

  struct process *prev = &list->start;
  struct process *current = prev->next;
  while(current != &list->end) {
    current = current->next;
    free(prev->next);
  }

  return 0;
}

/** add_process
 *
 * Adds a process id, pid, to the list
 */
int add_process(
  struct processlist *list, /* IN: The list to add the pid to. */
  pid_t pid) {              /* IN: The pid to add to the list. */

  struct process *new = (struct process *)malloc(sizeof(struct process));
  new->pid = pid;
  new->next = &list->end;

  list->last->next = new;
  list->last = new;

  return 0;
}

/** remove_process
 *
 * Removes the process id following next from the list.
 */
int remove_process(
  struct processlist *list, /* IN: The list from which to remove the pid */
  struct process *prev) {   /* IN: The node before the one which is to be removed. */

  struct process *del = prev->next;
  prev->next = del->next;
  list->last = prev;
  free(del);

  return 0;
}