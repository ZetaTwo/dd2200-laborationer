#ifndef __TIMER__
#define __TIMER__

#include <sys/time.h>

struct timer {
  struct timeval start;
  struct timeval end;
};

int start_timer(struct timer *timer);
int end_timer(struct timer *timer);

#endif