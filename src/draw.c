#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "input.h"
#include "map.h"
#include "relative.h"

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
  float spacing = get_zoom();
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
    auto start = (Vector2){(float)(i * 10), 0};
    auto end = start;
    end.y += 1 << 14;
    draw_line(start, end, 1, BLACK);
  }

  for (size_t i = 0; i < 1 << 9; i++) {
    auto start = (Vector2){0, (float)(i * 10)};
    auto end = start;
    end.x += 1 << 14;
    draw_line(start, end, 1, BLACK);
  }
}

void draw_map(map_t *map) {
  auto size = 50ul;
  auto font_size = size / 8 * 5;
  auto margin = 15ul;
  auto offset = size + margin;
  auto width_elements = 64ul;

  // TODO: pregenerate fonts based on sizes
  // Font font =
  //     LoadFontEx("/usr/share/fonts/TTF/CaskaydiaMonoNerdFont-Regular.ttf",
  //                font_size, NULL, 0);

  auto x_start = margin;
  auto y_start = margin;

  for (size_t i = 0; i < map->len; i++) {
    auto x_boost = 0ul;
    auto i_pos = i % width_elements;

    x_boost += i_pos / 4 * margin / 2;
    x_boost += i_pos / 8 * margin / 2;

    auto x_pos = i % width_elements * offset + x_start + x_boost;
    auto y_pos = i / width_elements * offset + y_start;

    Rectangle shadow_rec = {(float)(x_pos + 3), (float)(y_pos + 3), (float)size,
                            (float)size};
    draw_rect(shadow_rec, BLACK);

    Rectangle rec = {(float)x_pos, (float)y_pos, (float)size, (float)size};
    draw_rect(rec, LIGHTGRAY);

    if (get_zoom() < 0.45)
      continue;

    auto byte = map->start[i];
    char text[3];
    snprintf(text, sizeof(text), "%02X", byte);

    Vector2 text_pos = {
        (float)x_pos + (float)size / 2,
        (float)y_pos + (float)size / 2,
    };
    draw_text_center(text, text_pos, (float)font_size, DARKGRAY);
  }
}

void draw_maps(map_registry_t *reg) {
  auto curr = reg->start;
  while (curr != NULL) {
    draw_map(curr);
    curr = curr->next;
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

void draw_loop(map_registry_t *reg) {
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(DARKGRAY);

    handle_input();

    // draw_origin_debug();
    // draw_grid_debug();

    draw_maps(reg);

    EndDrawing();
  }
}
