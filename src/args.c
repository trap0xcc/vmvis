
#include <stdlib.h>

#include "args.h"

void parse_args(int argc, char *argv[], args_t *args) {
  if (argc > 1)
    args->pid = atoi(argv[1]);
}
