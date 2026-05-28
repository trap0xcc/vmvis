#include <stdlib.h>

#include "active_page_notifier.h"
#include "args.h"
#include "draw.h"
#include "map.h"
#include "page_monitor.h"
#include "process.h"
#include "window.h"

int main(int argc, char *argv[]) {
  args_t args = {};
  parse_args(argc, argv, &args);

  map_registry_t reg = {};
  registry_init(&reg);

  process_t proc = {};
  process_init(&proc);
  proc.pid = args.pid;

  start_map_monitor(&reg, &proc);

  active_page_notifier_t pn = {};
  start_page_monitor(&reg, &pn);

  create_window();

  draw_loop(&reg, &pn);

  destroy_window();

  return EXIT_SUCCESS;
}
