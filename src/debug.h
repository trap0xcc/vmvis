#include <stdio.h>

#include "raylib.h"

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
