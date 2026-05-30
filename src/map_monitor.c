#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <inttypes.h>
#include <stdint.h>

#include "draw.h"
#include "map.h"
#include "map_registry.h"
#include "process.h"

void parse_range(const char *s, map *map) {
  sscanf(s, "%" SCNxPTR "-%" SCNxPTR, &map->start, &map->end);
  map->len = map->end - map->start;
  map->pages = map->len / PAGE_BYTE_SIZE;
}

void parse_prot_flags(const char *s, map *map) {
  if (s[0] == 'r')
    map->prot |= PROT_READ;
  if (s[1] == 'w')
    map->prot |= PROT_WRITE;
  if (s[2] == 'x')
    map->prot |= PROT_EXEC;
  if (s[3] == 's')
    map->flags |= MAP_SHARED;
  if (s[3] == 'p')
    map->flags |= MAP_PRIVATE;
}

void parse_dev(const char *s, map *map) {
  sscanf(s, "%x:%x", &map->dev_major, &map->dev_minor);
}

map parse_line(const char *line) {
  map map = {};

  // make a writable copy since we may split it
  char *tmp = strdup(line);
  if (!tmp)
    exit(1);

  // find pathname start (first non-fixed field after 5 tokens)
  // format:
  //   addr prot offset dev inode [pathname...]
  char *addr = strtok(tmp, " ");
  char *prot = strtok(NULL, " ");
  char *offset = strtok(NULL, " ");
  char *dev = strtok(NULL, " ");
  char *inode = strtok(NULL, " ");

  if (addr)
    parse_range(addr, &map);
  if (prot)
    parse_prot_flags(prot, &map);
  if (offset)
    map.offset = strtoull(offset, NULL, 16);
  if (dev)
    parse_dev(dev, &map);
  if (inode)
    map.inode = atoi(inode);

  // Remaining text (if any) is pathname
  char *rest = strtok(NULL, "");
  if (rest) {
    size_t len = strlen(rest);
    if (len > 1) {
      rest[len - 1] = '\0';
      while (*rest == ' ')
        rest++;
      if (*rest)
        map.path = strdup(rest);
    }
  }

  free(tmp);
  return map;
}

void update_maps_from_file(pid_t pid, map_registry *reg) {
  if (!pid) {
    // TODO: eventually create a UI to allow user to select pid
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
  if (!f) {
    fprintf(stderr, "%s\n", file_path);
    perror("fopen in update_maps_from_file");
    exit(EXIT_FAILURE);
  }

  char line[1 << 10];
  map_registry_entry *root_mre = {};
  map_registry_entry **prev_mre = {};
  while (fgets(line, sizeof(line), f)) {
    map_registry_entry *mre = calloc(1, sizeof(map_registry_entry));
    mre->map = parse_line(line);
    if (!root_mre)
      root_mre = mre;
    if (prev_mre)
      *prev_mre = mre;
    prev_mre = &mre->next;
  }
  registry_swap_maps(reg, root_mre);
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
