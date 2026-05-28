
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
  page_table_t *root_pt;
} map_t;

typedef struct {
  pthread_mutex_t mu;
  map_t *first;
  pthread_t t;
} map_registry_t;

void registry_init(map_registry_t *reg);

// TODO: split out map monitor
void start_map_monitor(map_registry_t *reg, process_t *proc);

void register_map(map_registry_t *reg, uintptr_t remote_addr, size_t len);

#endif
