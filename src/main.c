#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "draw.h"
#include "map.h"
#include "window.h"

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  map_registry_t reg = {};

  uint8_t one_page[1 << 12] = {};
  for (size_t i = 0; i < sizeof(one_page); i++)
    one_page[i] = (uint8_t)i;
  register_map(&reg, &(map_t){one_page, sizeof(one_page), 123, NULL});

  create_window();

  draw_loop(&reg);

  destroy_window();

  return EXIT_SUCCESS;
}
