#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "draw.h"
#include "map.h"
#include "window.h"

uint8_t first_buf[1 << 20] = {};
uint8_t second_buf[1 << 20] = {};
uint8_t third_buf[1 << 20] = {};

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  map_registry_t reg = {};

  for (size_t i = 0; i < sizeof(first_buf); i++)
    first_buf[i] = (uint8_t)i;

  for (size_t i = 0; i < sizeof(second_buf); i++)
    second_buf[i] = (uint8_t)i;

  for (size_t i = 0; i < sizeof(third_buf); i++)
    third_buf[i] = (uint8_t)i;

  register_map(&reg, &(map_t){
                         .buf = first_buf,
                         .len = sizeof(first_buf),
                         .remote_addr = 4096,
                     });
  register_map(&reg, &(map_t){
                         .buf = second_buf,
                         .len = sizeof(second_buf),
                         .remote_addr = 2 << 20,
                     });
  register_map(&reg, &(map_t){
                         .buf = third_buf,
                         .len = sizeof(third_buf),
                         .remote_addr = 3 << 20,
                     });

  create_window();

  draw_loop(&reg);

  destroy_window();

  return EXIT_SUCCESS;
}
