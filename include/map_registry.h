#ifndef _MAP_REGISTRY_H
#define _MAP_REGISTRY_H

#include <pthread.h>

#include "lang.h"
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

typedef struct {
  ul len;
  ul byte_size;
  ul page_size;
} reg_stats;

typedef void (*map_registy_visit_fn)(map *map, void *userdata);

void registry_init(map_registry *reg);
void registry_swap_maps(map_registry *reg, map_registry_entry *mre);
void register_map(map_registry *reg, map map);
void registry_visit(map_registry *reg, map_registy_visit_fn fn, void *userdata);
reg_stats registry_stats(map_registry *reg);
void register_test_maps(map_registry *reg);

#endif
