#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"
#include "lang.h"
#include "map_registry.h"
#include "process.h"

void update_maps_from_file(pid_t pid, map_registry *reg) {
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
  if (f == nullptr) {
    fprintf(stderr, "%s\n", file_path);
    perror("fopen in update_maps_from_file");
    exit(EXIT_FAILURE);
  }

  ul start, end, offset;
  char perms[5];
  char path[256];

  while (fscanf(f, "%lx-%lx %4s %lx %*s %*d %255[^\n]\n", &start, &end, perms,
                &offset, path) >= 4) {
    // printf("start: %p, end: %p, perms: %s, offset: %lx, path: %s\n",
    //        (void *)start, (void *)end, perms, offset, path);

    // TODO: use offset, perms, and path
    register_map(reg, (map){.remote_addr = start, .len = end - start});
  }
}

typedef struct {
  map_registry *reg;
  process_info *pi;
} _map_monitor_args;

void *map_monitor(void *args) {
  map_registry *reg = ((_map_monitor_args *)args)->reg;
  process_info *pi = ((_map_monitor_args *)args)->pi;

  auto pid = process_get_pid(pi);

  for (;;) {
    update_maps_from_file(pid, reg);
    sleep(2);
  }

  return nullptr;
}

void start_map_monitor(map_registry *reg, process_info *pi) {
  _map_monitor_args *args = malloc(sizeof(_map_monitor_args));
  args->reg = reg;
  args->pi = pi;
  pthread_create(&reg->t, nullptr, map_monitor, args);
}
