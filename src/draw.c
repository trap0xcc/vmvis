#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "raylib.h"

#include "active_page_notifier.h"
#include "input.h"
#include "map.h"
#include "page_table.h"
#include "relative.h"

#define ul unsigned long

static ul draw_count;

void draw_rect(Rectangle rec, Color color) {
  draw_count++;
  DrawRectangleRec(relative_rect_to_screen(rec), color);
}

void draw_text_center(char *text, Vector2 position, float font_size,
                      Color color) {
  position = relative_vec_to_screen(position);
  font_size = relative_font_size_to_screen(font_size);
  auto font = GetFontDefault();
  float spacing = get_zoom();
  auto text_size = MeasureTextEx(font, text, font_size, spacing);
  position.x -= text_size.x / 2;
  position.y -= text_size.y / 2 - 1;
  draw_count++;
  DrawTextEx(font, text, position, font_size, spacing, color);
}

void draw_line(Vector2 start, Vector2 end, float thick, Color color) {
  draw_count++;
  DrawLineEx(screen_vec_to_relative(start), screen_vec_to_relative(end), thick,
             color);
}

void draw_line_direct(Vector2 start, Vector2 end, float thick, Color color) {
  draw_count++;
  DrawLineEx(start, end, thick, color);
}

bool directional_overlap_1d(float a_a, float a_b, float b_a, float b_b) {
  return !(a_b < b_a || a_a > b_b);
}

bool visible_rect(Rectangle relative_rect) {
  auto screen_rect = relative_rect_to_screen(relative_rect);

  return directional_overlap_1d(screen_rect.y,
                                screen_rect.y + screen_rect.height, 0,
                                (float)GetScreenHeight()) &&
         directional_overlap_1d(screen_rect.x,
                                screen_rect.x + screen_rect.width, 0,
                                (float)GetScreenWidth());
}

/************************* Drawing Constants *************************/

static const auto cell_width = 50ul;
static const auto cell_height = cell_width;
static const auto cell_margin = 15ul;
static const auto cell_offset = cell_width + cell_margin;
static const auto cells_per_line = 64ul;
static const auto font_size = cell_height / 8 * 5;

static const auto cell_margin_boost_per_row =
    ((cells_per_line / 4) * cell_margin / 2) +
    ((cells_per_line / 8) * cell_margin / 2);
// static const auto cell_margin_boost_per_row = 1.5 * cells_per_line *
// cell_margin / 8;

// auto i_pos = i % cells_per_line;
// x_boost += i_pos / 4 * cell_margin / 2;
// x_boost += i_pos / 8 * cell_margin / 2;

static const auto page_byte_size = 1ul << 12;

static const auto page_width = 64 * cell_offset + cell_margin_boost_per_row;
static const auto page_height = 64 * cell_offset + cell_margin;
static const auto page_margin = 100ul;

static const auto pages_per_row = 8ul;

static const auto page_row_width = (page_width + page_margin) * pages_per_row;
static const auto page_row_height = page_height;

static const auto map_margin = 500ul;
static const auto map_padding = cell_width;
static const auto map_width = page_row_width + 2 * map_padding;

static const auto spacer_height = 5'000ul;

/************************* /Drawing Constants ************************/

void draw_page_cells(ul x, ul y, page_table_t *pt, active_page_notifier_t *pn) {
  // TODO: remove these once in use
  (void)pt;
  (void)pn;

  static const auto zoom_threshold = 0.1f;

  if (get_zoom() < zoom_threshold)
    return;

  // TODO: mark page as active

  // TODO: use monospaced font, possibly:
  //       /usr/share/fonts/TTF/CaskaydiaMonoNerdFont-Regular.ttf

  for (size_t i = 0; i < page_byte_size; i++) {
    auto x_boost = 0ul;
    auto i_pos = i % cells_per_line;

    x_boost += i_pos / 4 * cell_margin / 2;
    x_boost += i_pos / 8 * cell_margin / 2;

    auto x_pos = cell_margin + i % cells_per_line * cell_offset + x + x_boost;
    auto y_pos = cell_margin + i / cells_per_line * cell_offset + y;

    Rectangle rec = {
        .x = (float)x_pos,
        .y = (float)y_pos,
        .width = (float)cell_width,
        .height = (float)cell_height,
    };

    if (!visible_rect(rec))
      continue;

    Rectangle shadow_rec = {
        .x = (float)(x_pos + 3),
        .y = (float)(y_pos + 3),
        .width = (float)cell_width,
        .height = (float)cell_height,
    };
    draw_rect(shadow_rec, BLACK);
    draw_rect(rec, LIGHTGRAY);

    if (get_zoom() < zoom_threshold * 3)
      continue;

    // TODO: draw cells from page data if possible, else draw placeholder cells

    // auto byte = buf[i];
    // char text[3];
    // snprintf(text, sizeof(text), "%02X", byte);

    Vector2 text_pos = {
        (float)x_pos + (float)cell_width / 2,
        (float)y_pos + (float)cell_height / 2,
    };
    draw_text_center("--", text_pos, (float)font_size, DARKGRAY);
  }
}

void draw_page(size_t page_num, page_table_t *pt, active_page_notifier_t *pn,
               ul y_cursor) {
  auto x_pos =
      page_margin + (page_num % pages_per_row) * (page_width + page_margin);
  auto y_pos = y_cursor + page_margin +
               (page_num / pages_per_row) * (page_height + page_margin);
  auto width = page_width;
  auto height = page_height;

  Rectangle rec = {
      .x = (float)x_pos,
      .y = (float)y_pos,
      .width = (float)width,
      .height = (float)height,
  };
  if (!visible_rect(rec))
    return;
  draw_rect(rec, GRAY);

  draw_page_cells(x_pos, y_pos, pt, pn);
}

ul draw_map(map_t *map, active_page_notifier_t *pn, ul y_cursor) {
  auto pages = map->len / page_byte_size;
  auto page_rows = pages / pages_per_row;
  if (pages % pages_per_row != 0) {
    page_rows++;
  }

  auto x_pos = 0;
  auto y_pos = y_cursor;
  auto width = map_width;
  auto height = page_rows * (page_row_height + page_margin) + 2 * map_padding;

  Rectangle rec = {
      .x = (float)x_pos,
      .y = (float)y_pos,
      .width = (float)width,
      .height = (float)height,
  };
  auto ret = y_cursor + height + map_margin;
  if (!visible_rect(rec))
    return ret;
  draw_rect(rec, DARKGRAY);

  for (size_t i = 0; i < pages; i++) {
    draw_page(i, map->root_pt, pn, y_cursor);
  }

  return ret;
}

ul draw_spacer(ul y_cursor) {
  auto x_pos = 0;
  auto y_pos = y_cursor;
  auto width = map_width;
  auto height = spacer_height;

  Rectangle rec = {
      .x = (float)x_pos,
      .y = (float)y_pos,
      .width = (float)width,
      .height = (float)height,
  };
  draw_rect(rec, LIGHTGRAY);

  return y_cursor + spacer_height + map_margin;
}

void draw_maps(map_registry_t *reg, active_page_notifier_t *pn) {
  pthread_mutex_lock(&reg->mu);

  auto prev = (map_t *)NULL;
  auto curr = reg->first;
  auto y_cursor = 0ul;

  while (curr != NULL) {
    // diff prev addr with curr addr and draw a spacer
    if ((prev == NULL && curr->remote_addr != 0) ||
        (prev != NULL && (prev->remote_addr + prev->len) != curr->remote_addr))
      y_cursor = draw_spacer(y_cursor);

    y_cursor = draw_map(curr, pn, y_cursor);
    prev = curr;
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);
}

static inline double now_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

static auto global_frame_count = 0ul;
static auto frame_count = 0ul;
static double frame_rate_start_time;
static double frame_rate;
static ul draw_count;

void draw_debug_info() {
  char buf[1 << 12] = "";

  auto now = now_seconds();
  auto time_delta = now - frame_rate_start_time;
  if (time_delta > 1) {
    frame_rate = (double)frame_count / time_delta;
    frame_count = 0;
    frame_rate_start_time = now;
  }

  snprintf(buf, sizeof(buf),
           "global_frame_count: %ld\n"
           "frame_rate: %f\n"
           "draw_count: %ld\n"
           "zoom: %f\n",
           global_frame_count, frame_rate, draw_count, (double)get_zoom());

  DrawText(buf, 24, 24, 20, DARKGRAY);
  DrawText(buf, 20, 20, 20, LIGHTGRAY);

  draw_count = 2;
}

void draw_loop(map_registry_t *reg, active_page_notifier_t *pn) {
  frame_rate_start_time = now_seconds();

  while (!WindowShouldClose()) {
    BeginDrawing();

    // NOTE: This translation can only be done after the first frame is drawn.
    if (global_frame_count == 1) {
      set_origin((Vector2){
          .x = ((float)GetScreenWidth() - page_row_width * get_zoom()) / 2,
          .y = 200,
      });
    }

    ClearBackground(BLACK);

    handle_input();

    draw_maps(reg, pn);

    draw_debug_info();

    EndDrawing();

    global_frame_count++;
    frame_count++;
  }
}
