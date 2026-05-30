
#ifndef _SPACE_H
#define _SPACE_H

#include "coords.h"

// space encodes the information for the translation offset and zoom scale
// amount for coordinates to be mapped to other coordinate systems.
typedef struct {
  vec2 origin;
  double zoom;
} space;

extern const space space_relative_default;

void space_relative_init(space *s);
vec2 vec_apply_space(vec2 v, space *s);
vec2 vec_undo_space(vec2 v, space *s);
rect rect_apply_space(rect r, space *s);
double double_apply_space(double font_size, space *s);

#endif
