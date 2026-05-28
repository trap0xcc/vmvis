#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"
#include "map.h"
#include "process.h"

void registry_init(map_registry_t *reg) { pthread_mutex_init(&reg->mu, NULL); }

void update_maps_from_file(pid_t pid, map_registry_t *reg) {
  if (!pid) {
    fputs("pid is zero, registering test maps\n", stderr);
    register_test_maps(reg);
    return;
  }

  char file_path[1 << 8] = {};
  auto n = snprintf(file_path, sizeof(file_path), "/proc/%d/maps", pid);
  if (n < 0) {
    perror("snprintf in update_maps_from_file");
    exit(EXIT_FAILURE);
  }
  if (n >= (int)sizeof(file_path)) {
    fputs("snprintf trucation in update_maps_from_file\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *f = fopen(file_path, "r");
  if (f == NULL) {
    fprintf(stderr, "%s\n", file_path);
    perror("fopen in update_maps_from_file");
    exit(EXIT_FAILURE);
  }

  unsigned long start, end, offset;
  char perms[5];
  char path[256];

  while (fscanf(f, "%lx-%lx %4s %lx %*s %*d %255[^\n]\n", &start, &end, perms,
                &offset, path) >= 4) {
    // printf("start: %p, end: %p, perms: %s, offset: %lx, path: %s\n",
    //        (void *)start, (void *)end, perms, offset, path);

    // TODO: use offset, perms, and path
    register_map(reg, start, end - start);
  }
}

typedef struct {
  map_registry_t *reg;
  process_t *proc;
} _map_monitor_args;

void *map_monitor(void *args) {
  map_registry_t *reg = ((_map_monitor_args *)args)->reg;
  process_t *proc = ((_map_monitor_args *)args)->proc;

  auto pid = process_get_pid(proc);

  for (;;) {
    update_maps_from_file(pid, reg);
    sleep(2);
  }

  return NULL;
}

void start_map_monitor(map_registry_t *reg, process_t *proc) {
  _map_monitor_args *args = malloc(sizeof(_map_monitor_args));
  args->reg = reg;
  args->proc = proc;
  pthread_create(&reg->t, NULL, map_monitor, args);
}

void register_map(map_registry_t *reg, uintptr_t remote_addr, size_t len) {
  pthread_mutex_lock(&reg->mu);

  // TODO: handle map insertions via mmap
  // TODO: handle map deletions via munmap
  // TODO: handle map resizes via mremap

  auto curr = &reg->first;
  auto curr_val = reg->first;

  while (curr_val != NULL) {
    // map already exists in list, just return
    if (curr_val->remote_addr == remote_addr)
      goto unlock;

    curr = &curr_val->next;
    curr_val = curr_val->next;
  }

  *curr = calloc(1, sizeof(map_t));
  (*curr)->len = len;
  (*curr)->remote_addr = remote_addr;

unlock:
  pthread_mutex_unlock(&reg->mu);
}
