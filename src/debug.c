
#include "stdio.h"

#include "debug.h"
#include "map.h"

#define DEFINE_PRINTER(Type, FIELDS)                                           \
  void Type##_print(const Type *v) {                                           \
    printf(#Type " {\n");                                                      \
    FIELDS(PRINT_FIELD, v)                                                     \
    printf("}\n");                                                             \
  }

#define PRINT_FIELD(name, type, fmt, obj)                                      \
  printf("  " #name " = " fmt "\n", (type)((obj)->name));

#define RECTANGLE_FIELDS(X, obj)                                               \
  X(x, double, "%f", obj)                                                      \
  X(y, double, "%f", obj)                                                      \
  X(width, double, "%f", obj)                                                  \
  X(height, double, "%f", obj)

DEFINE_PRINTER(Rectangle, RECTANGLE_FIELDS)

void register_test_maps(map_registry *reg) {
  register_map(reg, 4096, 1 << 20);
  register_map(reg, 2 << 20, 1 << 20);
  register_map(reg, 3 << 20, 1 << 20);
}
