#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#include "debug_print_macros.h"
#include "lang.h"

typedef struct map {
  uintptr_t start;
  uintptr_t end;
  size_t len;
  size_t offset;
  size_t pages;
  int prot;
  int flags;
  ui dev_major;
  ui dev_minor;
  int inode;
  char *path;
} map;

void map_destroy(map map);

DEFINE_PRINTER_HEADER(map)

#endif
