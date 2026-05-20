
#include "relative.h"

static Vector2 _origin = {};
static float _zoom = 1;

Vector2 get_origin() { return _origin; }

void set_origin(Vector2 origin) { _origin = origin; }

float get_zoom() { return _zoom; }

void set_zoom(float zoom) { _zoom = zoom; }

Vector2 relative_vec(Vector2 vec) {
  // scale
  vec.x *= _zoom;
  vec.y *= _zoom;

  // translate
  vec.x += _origin.x;
  vec.y += _origin.y;

  return vec;
}

Vector2 inverse_relative_vec(Vector2 vec) {
  // translate
  vec.x -= _origin.x;
  vec.y -= _origin.y;

  // scale
  vec.x /= _zoom;
  vec.y /= _zoom;

  return vec;
}

Rectangle relative_rect(Rectangle rect) {
  // scale
  rect.x *= _zoom;
  rect.y *= _zoom;
  rect.width *= _zoom;
  rect.height *= _zoom;

  // translate
  rect.x += _origin.x;
  rect.y += _origin.y;

  return rect;
}

float relative_font_size(float font_size) { return font_size * _zoom; }
