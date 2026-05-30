#include <map.h>
#include <stdlib.h>

void map_destroy(map map) { free(map.path); }

#define MAP_FIELDS(X, obj)                                                     \
  X(start, uintptr_t, "0x%lx", obj)                                            \
  X(end, uintptr_t, "0x%lx", obj)                                              \
  X(len, size_t, "0x%lx", obj)                                                 \
  X(offset, size_t, "0x%lx", obj)                                              \
  X(pages, size_t, "%'ld", obj)                                                \
  X(prot, int, "%b", obj)                                                      \
  X(flags, int, "%b", obj)                                                     \
  X(dev_major, ui, "%2d", obj)                                                 \
  X(dev_minor, ui, "%2d", obj)                                                 \
  X(inode, int, "%'d", obj)                                                    \
  X(path, char *, "%s", obj)

DEFINE_PRINTER(map, MAP_FIELDS)
