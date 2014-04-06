/* processlist
 *
 * This module implements a singly linked list
 * of process ids which are used to keep
 * track of the background processes
 */

#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <sys/types.h>

/* struct process
 *
 * Struct representing a node in the
 * linked list of process ids.
 */
struct process {
  struct process *next;
  pid_t pid;
};

/* struct processlist
 *
 * A linked list of process ids.
 */
struct processlist {
  struct process start;
  struct process *last;
  struct process end;
};

/** init_processlist
 *
 * Sets up the linked list for usage.
 */
int init_processlist(struct processlist *list);

/** free_processlist
 *
 * Frees up the resources used by the linked list.
 */
int free_processlist(struct processlist *list);

/** add_process
 *
 * Adds a process id to the list
 */
int add_process(struct processlist *list, pid_t pid);

/** remove_process
 *
 * Removes a process id from the list
 */
int remove_process(struct processlist *list, struct process *prev);

#endif