#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "args.h"
#include "draw.h"
#include "map.h"
#include "process.h"
#include "window.h"

int main(int argc, char *argv[]) {
  args_t args = {};
  parse_args(argc, argv, &args);

  SetConfigFlags(FLAG_MSAA_4X_HINT);

  map_registry_t reg = {};
  registry_init(&reg);

  process_t proc = {};
  process_init(&proc);
  proc.pid = args.pid;

  start_map_monitor(&reg, &proc);

  create_window();

  draw_loop(&reg);

  destroy_window();

  return EXIT_SUCCESS;
}
