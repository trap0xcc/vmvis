
#ifndef _DEBUG_H
#define _DEBUG_H

#include "map.h"
#include "raylib.h"

#define DEFINE_PRINTER_HEADER(Type) void Type##_print(const Type *v);

DEFINE_PRINTER_HEADER(Rectangle)

void register_test_maps(map_registry *reg);

typedef struct {
  unsigned long global_frame_count;
  unsigned long frame_count;
  double frame_rate_start_time;
  double frame_rate;
  unsigned long draw_count;
} debug_info;

#endif
