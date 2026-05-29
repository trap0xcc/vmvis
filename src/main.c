#include <stdlib.h>

#include "active_page_notifier.h"
#include "args.h"
#include "draw.h"
#include "map.h"
#include "page_monitor.h"
#include "process.h"
#include "window.h"

int main(int argc, char *argv[]) {
  args args = {};
  parse_args(argc, argv, &args);

  map_registry reg = {};
  registry_init(&reg);

  process_info pi = {};
  process_init(&pi);
  pi.pid = args.pid;

  start_map_monitor(&reg, &pi);

  active_page_notifier pn = {};
  start_page_monitor(&reg, &pn);

  create_window();

  draw_loop(&reg, &pn);

  destroy_window();

  return EXIT_SUCCESS;
}
