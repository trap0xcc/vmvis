#include "raylib.h"

#include "raylib_draw_proxy.h"

void draw_rect(raylib_draw_proxy *rdp, rect rect, Color color, space *s) {
  rdp->di->draw_count++;
  DrawRectangleRec(rect_to_raylib(rect_apply_space(rect, s)), color);
}

void draw_rect_const_size_right_aligned(raylib_draw_proxy *rdp, rect rect,
                                        vec2 offset, Color color, space *s) {
  auto new_rect = rect_apply_space(rect, s);
  new_rect.width = rect.width;
  new_rect.height = rect.height;
  new_rect.x -= new_rect.width;
  new_rect.x += offset.x;
  new_rect.y += offset.y;

  rdp->di->draw_count++;
  DrawRectangleRec(rect_to_raylib(new_rect), color);
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

void draw_text_const_size_right_aligned(raylib_draw_proxy *rdp, char *text,
                                        vec2 position, vec2 offset,
                                        double font_size, double spacing,
                                        Color color, space *s) {
  position = vec_apply_space(position, s);
  position.x += offset.x;
  position.y += offset.y;

  auto font = GetFontDefault();

  auto text_size = raylib_to_vec2(
      MeasureTextEx(font, text, (float)font_size, (float)spacing));
  position.x -= text_size.x;

  rdp->di->draw_count++;
  DrawTextEx(font, text, vec2_to_raylib(position), (float)font_size,
             (float)spacing, color);
}
