
#include <stdlib.h>

#include "args.h"

void parse_args(int argc, char *argv[], args *args) {
  if (argc > 1)
    args->pid = atoi(argv[1]);
}
