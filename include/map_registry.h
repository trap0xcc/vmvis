#ifndef _MAP_REGISTRY_H
#define _MAP_REGISTRY_H

#include <pthread.h>

#include "map.h"

typedef struct {
  pthread_mutex_t mu;
  struct map_registry_entry *first;
  pthread_t t;
} map_registry;

typedef struct map_registry_entry {
  map map;
  struct map_registry_entry *next;
} map_registry_entry;

typedef void (*map_registy_visit_fn)(map *map, void *userdata);

void registry_init(map_registry *reg);
void register_map(map_registry *reg, map map);
void registry_visit(map_registry *reg, map_registy_visit_fn fn, void *userdata);

#endif
