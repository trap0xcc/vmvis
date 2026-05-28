
#ifndef _ARGS_H
#define _ARGS_H

#include <sys/types.h>

typedef struct {
  pid_t pid;
} args_t;

void parse_args(int argc, char *argv[], args_t *args);

#endif
