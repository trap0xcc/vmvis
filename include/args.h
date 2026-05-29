
#ifndef _ARGS_H
#define _ARGS_H

#include <sys/types.h>

typedef struct {
  pid_t pid;
} args;

void parse_args(int argc, char *argv[], args *args);

#endif
