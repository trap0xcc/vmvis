
#ifndef _TIME_UTIL_H
#define _TIME_UTIL_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#include <time.h>

static inline double now_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

#endif
