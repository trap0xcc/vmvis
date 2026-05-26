
#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>
#include <unistd.h>

typedef struct map {
  uint8_t *buf;
  size_t len;
  uintptr_t remote_addr;
  struct map *next;
} map_t;

typedef struct {
  map_t *first;
} map_registry_t;

void register_map(map_registry_t *reg, map_t *map);

#endif
