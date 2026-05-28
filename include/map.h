
#ifndef _MAP_H
#define _MAP_H

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "process.h"

typedef struct map {
  // TODO: this buf can't stay because it will be to large to copy from tracee
  // there needs to be some organization that allows me to copy only the data
  // needed for what is visible on screen
  uint8_t *buf;
  size_t len;
  uintptr_t remote_addr;
  struct map *next;
} map_t;

typedef struct {
  pthread_mutex_t mu;
  map_t *first;
  pthread_t t;
} map_registry_t;

void registry_init(map_registry_t *reg);

void start_map_monitor(map_registry_t *reg, process_t *proc);

void register_map(map_registry_t *reg, uintptr_t remote_addr, size_t len);

#endif
