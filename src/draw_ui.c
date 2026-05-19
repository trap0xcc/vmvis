#include <stdio.h>
#include <stdlib.h>

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
float zoom = 1;

void register_buf(void *buf, size_t buf_len) {
  _buf = buf;
  _buf_len = buf_len;
}

void update_zoom() {
  auto val = GetMouseWheelMove();
  if (val != 0) {
    if (val < 0) {
      zoom *= 0.9;
    } else {
      zoom *= 1.1;
    }
  }
}

void draw_buf(char *buf, size_t len) {
  size_t size = 50 * zoom;
  auto font_size = size / 8 * 5;
  size_t margin = 15 * zoom;
  auto offset = size + margin;
  size_t width_elements = 64;

  // TODO: pregenerate fonts based on sizes
  Font font =
      LoadFontEx("/usr/share/fonts/TTF/CaskaydiaMonoNerdFont-Regular.ttf",
                 font_size, NULL, 0);

  auto layout_width = width_elements * offset + margin;
  auto x_start = GetScreenWidth() / 2 - layout_width / 2;
  auto y_start = size;

  for (size_t i = 0; i < len; i++) {
    auto x_boost = 0;
    auto i_pos = i % width_elements;

    x_boost += i_pos / 4 * margin / 2;
    x_boost += i_pos / 8 * margin / 2;

    auto x_pos = i % width_elements * offset + x_start + x_boost;
    auto y_pos = i / width_elements * offset + y_start;

    Rectangle shadow_rec = {x_pos + 3, y_pos + 3, size, size};
    DrawRectangleRec(shadow_rec, BLACK);

    Rectangle rec = {x_pos, y_pos, size, size};
    DrawRectangleRec(rec, LIGHTGRAY);

    if (zoom < 0.45)
      continue;

    auto byte = buf[i];
    char text[3];
    snprintf(text, sizeof(text), "%.2X", byte);

    auto text_size = MeasureTextEx(font, text, font_size, 0);

    auto box_center_x_pos = x_pos + size / 2;
    auto box_center_y_pos = y_pos + size / 2;

    Vector2 text_pos = {
        box_center_x_pos - text_size.x / 2,
        box_center_y_pos - text_size.y / 2 + 1,
    };
    DrawTextEx(font, text, text_pos, font_size, 0, DARKGRAY);
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

void draw_loop() {
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(DARKGRAY);

    update_zoom();
    draw_buf(_buf, _buf_len);

    EndDrawing();
  }
}

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  int buf[1 << 20] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
                      13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 23, 24, 25};
  register_buf(buf, sizeof(buf));

  create_window();

  draw_loop();
  destroy_window();

  return EXIT_SUCCESS;
}
