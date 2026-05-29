
#ifndef _MAP_H
#define _MAP_H

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "page_table.h"
#include "process.h"

typedef struct map {
  uintptr_t remote_addr;
  size_t len;
  struct map *next;
  page_table *root_pt;
} map;

typedef struct {
  pthread_mutex_t mu;
  map *first;
  pthread_t t;
} map_registry;

void registry_init(map_registry *reg);

// TODO: split out map monitor
void start_map_monitor(map_registry *reg, process_info *pi);

void register_map(map_registry *reg, uintptr_t remote_addr, size_t len);

#endif
