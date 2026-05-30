
#include "debug.h"
#include "map.h"

void register_test_maps(map_registry *reg) {
  register_map(reg, (map){4096, 1 << 20});
  register_map(reg, (map){2 << 20, 1 << 20});
  register_map(reg, (map){3 << 20, 1 << 20});
}
