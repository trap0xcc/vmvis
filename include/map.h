#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>
#include <unistd.h>

typedef struct map {
  uintptr_t remote_addr;
  size_t len;
} map;

#endif
