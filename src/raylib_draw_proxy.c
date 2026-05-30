#include "raylib.h"

#include "raylib_draw_proxy.h"

void draw_rect(raylib_draw_proxy *rdp, rect rec, Color color, space *s) {
  rdp->di->draw_count++;
  DrawRectangleRec(rect_to_raylib(rect_apply_space(rec, s)), color);
}

void draw_text(raylib_draw_proxy *rdp, char *text, vec2 position,
               double font_size, double spacing, Color color, space *s) {
  position = vec_apply_space(position, s);
  font_size = double_apply_space(font_size, s);
  spacing = double_apply_space(spacing, s);

  auto font = GetFontDefault();

  rdp->di->draw_count++;
  DrawTextEx(font, text, vec2_to_raylib(position), (float)font_size,
             (float)spacing, color);
}

void draw_text_center(raylib_draw_proxy *rdp, char *text, vec2 position,
                      double font_size, double spacing, Color color, space *s) {
  position = vec_apply_space(position, s);
  font_size = double_apply_space(font_size, s);
  spacing = double_apply_space(spacing, s);

  auto font = GetFontDefault();

  auto text_size = raylib_to_vec2(
      MeasureTextEx(font, text, (float)font_size, (float)spacing));
  position.x -= text_size.x / 2;
  position.y -= text_size.y / 2 - 1;

  rdp->di->draw_count++;
  DrawTextEx(font, text, vec2_to_raylib(position), (float)font_size,
             (float)spacing, color);
}
