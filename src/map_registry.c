#include <pthread.h>
#include <stdlib.h>

#include "draw.h"
#include "map_registry.h"

void registry_init(map_registry *reg) { pthread_mutex_init(&reg->mu, nullptr); }

void registry_swap_maps(map_registry *reg, map_registry_entry *mre) {
  pthread_mutex_lock(&reg->mu);

  auto curr = reg->first;

  while (curr) {
    auto next = curr->next;
    map_destroy(curr->map);
    free(curr);
    curr = next;
  }

  reg->first = mre;

  pthread_mutex_unlock(&reg->mu);
}

void register_map(map_registry *reg, map map) {
  pthread_mutex_lock(&reg->mu);

  // TODO: handle map insertions via mmap
  // TODO: handle map deletions via munmap
  // TODO: handle map resizes via mremap

  auto curr = &reg->first;
  auto curr_val = reg->first;

  while (curr_val) {
    // map already exists in list, just return
    if (curr_val->map.start == map.start)
      goto unlock;

    curr = &curr_val->next;
    curr_val = curr_val->next;
  }

  *curr = calloc(1, sizeof(map_registry_entry));
  (*curr)->map = map;

unlock:
  pthread_mutex_unlock(&reg->mu);
}

void registry_visit(map_registry *reg, map_registy_visit_fn fn,
                    void *userdata) {
  pthread_mutex_lock(&reg->mu);

  auto curr = reg->first;

  while (curr) {
    fn(&curr->map, userdata);
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);
}

reg_stats registry_stats(map_registry *reg) {
  reg_stats rs = {};

  pthread_mutex_lock(&reg->mu);

  auto curr = reg->first;

  while (curr) {
    rs.len++;
    rs.byte_size += curr->map.len;
    rs.page_size += curr->map.pages;
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);

  return rs;
}

void register_test_maps(map_registry *reg) {
  static const auto size = 1 << 20;
  static const auto pages = size / PAGE_BYTE_SIZE;
  register_map(reg, (map){.start = 1 << 12,
                          .end = (1 << 12) + size,
                          .len = size,
                          .pages = pages});
  register_map(reg, (map){.start = 2 << 20,
                          .end = (2 << 20) + size,
                          .len = size,
                          .pages = pages});
  register_map(reg, (map){.start = 3 << 20,
                          .end = (3 << 20) + size,
                          .len = size,
                          .pages = pages});
}
