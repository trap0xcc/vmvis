#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "raylib.h"

const auto init_width = 1024;
const auto init_height = 768;

void create_window() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(init_width, init_height, "test");
  SetTargetFPS(60);
}

void destroy_window() { CloseWindow(); }

char *_buf;
size_t _buf_len;
Vector2 origin = {};
float _zoom = 1;

void register_buf(void *buf, size_t buf_len) {
  _buf = buf;
  _buf_len = buf_len;
}

Vector2 relative_vec(Vector2 vec) {
  // scale
  vec.x *= _zoom;
  vec.y *= _zoom;

  // translate
  vec.x += origin.x;
  vec.y += origin.y;

  return vec;
}

Vector2 inverse_relative_vec(Vector2 vec) {
  // translate
  vec.x -= origin.x;
  vec.y -= origin.y;

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
  rect.x += origin.x;
  rect.y += origin.y;

  return rect;
}

float relative_font_size(float font_size) { return font_size * _zoom; }

void draw_origin_debug() {
  auto rect = relative_rect((Rectangle){0, 0, 20, 20});
  DrawRectangleRec(rect, RED);
  rect = relative_rect((Rectangle){20, 20, 20, 20});
  DrawRectangleRec(rect, GREEN);
  rect = relative_rect((Rectangle){40, 40, 20, 20});
  DrawRectangleRec(rect, BLUE);
  rect = relative_rect((Rectangle){60, 60, 20, 20});
  DrawRectangleRec(rect, RED);
  rect = relative_rect((Rectangle){80, 80, 20, 20});
  DrawRectangleRec(rect, GREEN);
  rect = relative_rect((Rectangle){100, 100, 20, 20});
  DrawRectangleRec(rect, BLUE);
}

void draw_rect(Rectangle rec, Color color) {
  DrawRectangleRec(relative_rect(rec), color);
}

void draw_text_center(char *text, Vector2 position, float font_size,
                      Color color) {
  position = relative_vec(position);
  font_size = relative_font_size(font_size);
  auto font = GetFontDefault();
  float spacing = 1;
  auto text_size = MeasureTextEx(font, text, font_size, spacing);
  position.x -= text_size.x / 2;
  position.y -= text_size.y / 2 - 1;
  DrawTextEx(font, text, position, font_size, spacing, color);
}

void draw_line(Vector2 start, Vector2 end, float thick, Color color) {
  DrawLineEx(relative_vec(start), relative_vec(end), thick, color);
}

void draw_line_direct(Vector2 start, Vector2 end, float thick, Color color) {
  DrawLineEx(start, end, thick, color);
}

void draw_grid_debug() {
  for (size_t i = 0; i < 1 << 9; i++) {
    auto start = (Vector2){i * 10, 0};
    auto end = start;
    end.y += 1 << 14;
    draw_line(start, end, 1, BLACK);
  }

  for (size_t i = 0; i < 1 << 9; i++) {
    auto start = (Vector2){0, i * 10};
    auto end = start;
    end.x += 1 << 14;
    draw_line(start, end, 1, BLACK);
  }
}

void draw_buf(char *buf, size_t len) {
  size_t size = 50;
  auto font_size = size / 8 * 5;
  size_t margin = 15;
  auto offset = size + margin;
  size_t width_elements = 64;

  // TODO: pregenerate fonts based on sizes
  // Font font =
  //     LoadFontEx("/usr/share/fonts/TTF/CaskaydiaMonoNerdFont-Regular.ttf",
  //                font_size, NULL, 0);

  auto layout_width = width_elements * offset + margin;
  auto x_start = margin;
  auto y_start = margin;

  for (size_t i = 0; i < len; i++) {
    auto x_boost = 0;
    auto i_pos = i % width_elements;

    x_boost += i_pos / 4 * margin / 2;
    x_boost += i_pos / 8 * margin / 2;

    auto x_pos = i % width_elements * offset + x_start + x_boost;
    auto y_pos = i / width_elements * offset + y_start;

    Rectangle shadow_rec = {x_pos + 3, y_pos + 3, size, size};
    draw_rect(shadow_rec, BLACK);

    Rectangle rec = {x_pos, y_pos, size, size};
    draw_rect(rec, LIGHTGRAY);

    if (_zoom < 0.45)
      continue;

    auto byte = buf[i];
    char text[3];
    snprintf(text, sizeof(text), "%.2X", byte);

    Vector2 text_pos = {
        x_pos + size / 2,
        y_pos + size / 2,
    };
    draw_text_center(text, text_pos, font_size, DARKGRAY);
  }
}

void draw_bars() {
  Rectangle rec3 = {100, 100, 10, 4};
  DrawRectangleRec(rec3, BLACK);
  Rectangle rec4 = {100, 105, 10.25, 4};
  DrawRectangleRec(rec4, WHITE);
  Rectangle rec5 = {100, 110, 10.5, 4};
  DrawRectangleRec(rec5, BLACK);
  Rectangle rec6 = {100, 115, 10.75, 4};
  DrawRectangleRec(rec6, WHITE);
  Rectangle rec7 = {100, 120, 11, 4};
  DrawRectangleRec(rec7, BLACK);
}

void draw_circle() { DrawCircle(100, 100, 50, WHITE); }

Vector2 pressed_pos;
Vector2 pressed_origin;

void handle_input() {
  // TODO: fix minor glitch when translating and zooming at the same time
  auto mouse_pos = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    pressed_pos = mouse_pos;
    pressed_origin = origin;
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    origin.x = pressed_origin.x + mouse_pos.x - pressed_pos.x;
    origin.y = pressed_origin.y + mouse_pos.y - pressed_pos.y;
  }

  int key;
  while ((key = GetKeyPressed()) != 0) {
    switch (key) {
      // default:
      //   printf("UNKNOWN KEY PRESSED: '%c', (%x)", key, key);
    case KEY_UP:
      origin.y += 100;
      break;
    case KEY_DOWN:
      origin.y -= 100;
      break;
    case KEY_LEFT:
      origin.x += 100;
      break;
    case KEY_RIGHT:
      origin.x -= 100;
      break;
    case KEY_Q:
      exit(0);
      break;
    }
  }
}

void update_zoom() {
  const auto zoom_factor = 0.1;

  auto pos = GetMousePosition();
  auto i_pos = inverse_relative_vec(pos);

  auto val = GetMouseWheelMove();
  if (val != 0) {
    if (val < 0) {
      // zoom out
      _zoom *= 1 - zoom_factor;

      auto new_pos = relative_vec(i_pos);

      origin.x -= new_pos.x - pos.x;
      origin.y -= new_pos.y - pos.y;
    } else {
      // zoom in
      _zoom *= 1 + zoom_factor;

      auto new_pos = relative_vec(i_pos);

      origin.x -= new_pos.x - pos.x;
      origin.y -= new_pos.y - pos.y;
    }
  }
}

void draw_loop() {
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(DARKGRAY);

    handle_input();
    update_zoom();

    // draw_origin_debug();
    // draw_grid_debug();

    draw_buf(_buf, _buf_len);

    EndDrawing();
  }
}

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  char buf[1 << 12] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
                       13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 23, 24, 25};
  register_buf(buf, sizeof(buf));

  create_window();

  draw_loop();
  destroy_window();

  return EXIT_SUCCESS;
}
