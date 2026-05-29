
#include "coords.h"

Vector2 vec2_to_raylib(vec2 v) {
  return (Vector2){
      .x = (float)v.x,
      .y = (float)v.y,
  };
}
Rectangle rect_to_raylib(rect r) {
  return (Rectangle){
      .x = (float)r.x,
      .y = (float)r.y,
      .width = (float)r.width,
      .height = (float)r.height,
  };
}

vec2 raylib_to_vec2(Vector2 v) {
  return (vec2){
      .x = (double)v.x,
      .y = (double)v.y,
  };
}
rect raylib_to_rect(Rectangle r) {
  return (rect){
      .x = (double)r.x,
      .y = (double)r.y,
      .width = (double)r.width,
      .height = (double)r.height,
  };
}
