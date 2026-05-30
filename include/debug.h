
#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#include "lang.h"
#include "map.h"

#define DEFINE_PRINTER_HEADER(Type) void Type##_print(const Type *v);

#define DEFINE_PRINTER(Type, FIELDS)                                           \
  void Type##_print(const Type *v) {                                           \
    printf(#Type " {\n");                                                      \
    FIELDS(PRINT_FIELD, v)                                                     \
    printf("}\n");                                                             \
  }

#define PRINT_FIELD(name, type, fmt, obj)                                      \
  printf("  " #name " = " fmt "\n", (type)((obj)->name));

typedef struct {
  ul global_frame_count;
  ul frame_count;
  double frame_rate_start_time;
  double frame_rate;
  ul draw_count;
} debug_info;

void register_test_maps(map_registry *reg);

#endif
