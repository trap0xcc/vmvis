
#include "map.h"

void register_map(map_registry_t *reg, map_t *map) {
  auto curr = &reg->start;
  auto curr_val = reg->start;

  while (curr_val != NULL) {
    curr = &curr_val->next;
    curr_val = curr_val->next;
  }

  *curr = map;
}
