#include <stdio.h>

#include "../include/timer.h"


int start_timer(struct timer *timer) {
  if (gettimeofday(&timer->start, NULL) < 0) { perror("Error getting start time"); return -1; }

  return 0;
}

int end_timer(struct timer *timer) {
  if (gettimeofday(&timer->end, NULL) < 0) { perror("Error getting end time"); return -1; }

  struct timeval elapsed;
  /* Calculate time elapsed, beware that we might have: end < start */
  elapsed.tv_sec = (timer->end.tv_sec >= timer->start.tv_sec) ? timer->end.tv_sec - timer->start.tv_sec : timer->start.tv_sec - timer->end.tv_sec;
  elapsed.tv_usec = (timer->end.tv_usec >= timer->start.tv_usec) ? timer->end.tv_usec - timer->start.tv_usec : timer->start.tv_usec - timer->end.tv_usec;

  return 1000000*elapsed.tv_sec + elapsed.tv_usec;

}