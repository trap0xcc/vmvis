#include <pthread.h>
#include <stdlib.h>

#include "map_registry.h"

void registry_init(map_registry *reg) { pthread_mutex_init(&reg->mu, nullptr); }

void register_map(map_registry *reg, map map) {
  pthread_mutex_lock(&reg->mu);

  // TODO: handle map insertions via mmap
  // TODO: handle map deletions via munmap
  // TODO: handle map resizes via mremap

  auto curr = &reg->first;
  auto curr_val = reg->first;

  while (curr_val != nullptr) {
    // map already exists in list, just return
    if (curr_val->map.remote_addr == map.remote_addr)
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

  while (curr != nullptr) {
    fn(&curr->map, userdata);
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);
}

reg_stats registry_stats(map_registry *reg) {
  reg_stats rs = {};

  pthread_mutex_lock(&reg->mu);

  auto curr = reg->first;

  while (curr != nullptr) {
    rs.total_size += curr->map.len;
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);

  return rs;
}

void register_test_maps(map_registry *reg) {
  register_map(reg, (map){4096, 1 << 20});
  register_map(reg, (map){2 << 20, 1 << 20});
  register_map(reg, (map){3 << 20, 1 << 20});
}
