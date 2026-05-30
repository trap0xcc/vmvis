
#include "space.h"

const space space_relative_default = {.zoom = 0.005};

vec2 vec_apply_space(vec2 v, space *s) {
  if (s == nullptr)
    return v;

  // scale
  v.x *= s->zoom;
  v.y *= s->zoom;

  // translate
  v.x += s->origin.x;
  v.y += s->origin.y;

  return v;
}

vec2 vec_undo_space(vec2 v, space *s) {
  if (s == nullptr)
    return v;

  // translate
  v.x -= s->origin.x;
  v.y -= s->origin.y;

  // scale
  v.x /= s->zoom;
  v.y /= s->zoom;

  return v;
}

rect rect_apply_space(rect r, space *s) {
  if (s == nullptr)
    return r;

  // scale
  r.x *= s->zoom;
  r.y *= s->zoom;
  r.width *= s->zoom;
  r.height *= s->zoom;

  // translate
  r.x += s->origin.x;
  r.y += s->origin.y;

  return r;
}

double double_apply_space(double d, space *s) {
  if (s == nullptr)
    return d;

  return d * s->zoom;
}
