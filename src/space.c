
#include "space.h"

void space_relative_init(space *s) { s->zoom = 0.005; }

vec2 vec_apply_space(vec2 v, space *s) {
  // scale
  v.x *= s->zoom;
  v.y *= s->zoom;

  // translate
  v.x += s->origin.x;
  v.y += s->origin.y;

  return v;
}

vec2 vec_undo_space(vec2 v, space *s) {
  // translate
  v.x -= s->origin.x;
  v.y -= s->origin.y;

  // scale
  v.x /= s->zoom;
  v.y /= s->zoom;

  return v;
}

rect rect_apply_space(rect r, space *s) {
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

double font_size_apply_space(double font_size, space *s) {
  return font_size * s->zoom;
}
