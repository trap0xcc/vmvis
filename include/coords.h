#ifndef _COORDS_H
#define _COORDS_H

#include <raylib.h>

#include "debug_print_macros.h"

typedef struct {
  double x;
  double y;
} vec2;

typedef struct {
  double x;
  double y;
  double width;
  double height;
} rect;

Vector2 vec2_to_raylib(vec2 v);
Rectangle rect_to_raylib(rect r);
vec2 raylib_to_vec2(Vector2 v);
rect raylib_to_rect(Rectangle r);

DEFINE_PRINTER_HEADER(vec2)
DEFINE_PRINTER_HEADER(rect)

#endif
