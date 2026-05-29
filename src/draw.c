#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "raylib.h"

#include "coords.h"
#include "draw.h"
#include "lang.h"
#include "page_table.h"
#include "time_util.h"

static ul draw_count;

void draw_rect(rect rec, Color color, space *s) {
  draw_count++;
  DrawRectangleRec(rect_to_raylib(rect_apply_space(rec, s)), color);
}

void draw_text_center(char *text, vec2 position, double font_size, Color color,
                      space *s) {
  position = vec_apply_space(position, s);
  font_size = double_apply_space(font_size, s);
  auto font = GetFontDefault();
  double spacing = s->zoom;
  auto text_size = raylib_to_vec2(
      MeasureTextEx(font, text, (float)font_size, (float)spacing));
  position.x -= text_size.x / 2;
  position.y -= text_size.y / 2 - 1;
  draw_count++;
  DrawTextEx(font, text, vec2_to_raylib(position), (float)font_size,
             (float)spacing, color);
}

bool directional_overlap_1d(double a_a, double a_b, double b_a, double b_b) {
  return !(a_b < b_a || a_a > b_b);
}

bool visible_rect(rect relative_rect, space *s) {
  auto screen_rect = rect_apply_space(relative_rect, s);

  return directional_overlap_1d(screen_rect.y,
                                screen_rect.y + screen_rect.height, 0,
                                (double)GetScreenHeight()) &&
         directional_overlap_1d(screen_rect.x,
                                screen_rect.x + screen_rect.width, 0,
                                (double)GetScreenWidth());
}

void draw_page_cells(ul x, ul y, page_table *pt, active_page_notifier *pn,
                     space *s) {
  // TODO: remove these once in use
  (void)pt;
  (void)pn;

  static const auto zoom_threshold = 0.1;

  if (s->zoom < zoom_threshold)
    return;

  // TODO: mark page as active

  // TODO: use monospaced font, possibly:
  //       /usr/share/fonts/TTF/CaskaydiaMonoNerdFont-Regular.ttf

  for (size_t i = 0; i < PAGE_BYTE_SIZE; i++) {
    auto x_boost = 0ul;
    auto i_pos = i % CELLS_PER_LINE;

    x_boost += i_pos / 4 * CELL_MARGIN / 2;
    x_boost += i_pos / 8 * CELL_MARGIN / 2;

    auto x_pos = CELL_MARGIN + i % CELLS_PER_LINE * CELL_OFFSET + x + x_boost;
    auto y_pos = CELL_MARGIN + i / CELLS_PER_LINE * CELL_OFFSET + y;

    rect rec = {
        .x = (double)x_pos,
        .y = (double)y_pos,
        .width = (double)CELL_WIDTH,
        .height = (double)CELL_HEIGHT,
    };

    if (!visible_rect(rec, s))
      continue;

    rect shadow_rec = {
        .x = (double)(x_pos + 3),
        .y = (double)(y_pos + 3),
        .width = (double)CELL_WIDTH,
        .height = (double)CELL_HEIGHT,
    };
    draw_rect(shadow_rec, BLACK, s);
    draw_rect(rec, LIGHTGRAY, s);

    if (s->zoom < zoom_threshold * 3)
      continue;

    // TODO: draw cells from page data if possible, else draw placeholder cells

    // auto byte = buf[i];
    // char text[3];
    // snprintf(text, sizeof(text), "%02X", byte);

    vec2 text_pos = {
        (double)x_pos + (double)CELL_WIDTH / 2,
        (double)y_pos + (double)CELL_HEIGHT / 2,
    };
    draw_text_center("--", text_pos, (double)FONT_SIZE, DARKGRAY, s);
  }
}

void draw_page(size_t page_num, page_table *pt, active_page_notifier *pn,
               ul y_cursor, space *s) {
  auto x_pos =
      PAGE_MARGIN + (page_num % PAGES_PER_ROW) * (PAGE_WIDTH + PAGE_MARGIN);
  auto y_pos = y_cursor + PAGE_MARGIN +
               (page_num / PAGES_PER_ROW) * (PAGE_HEIGHT + PAGE_MARGIN);
  auto width = PAGE_WIDTH;
  auto height = PAGE_HEIGHT;

  rect rec = {
      .x = (double)x_pos,
      .y = (double)y_pos,
      .width = (double)width,
      .height = (double)height,
  };
  if (!visible_rect(rec, s))
    return;
  draw_rect(rec, GRAY, s);

  draw_page_cells(x_pos, y_pos, pt, pn, s);
}

ul draw_map(map *map, active_page_notifier *pn, ul y_cursor, space *s) {
  auto pages = map->len / PAGE_BYTE_SIZE;
  auto page_rows = pages / PAGES_PER_ROW;
  if (pages % PAGES_PER_ROW != 0) {
    page_rows++;
  }

  auto x_pos = 0;
  auto y_pos = y_cursor;
  auto width = MAP_WIDTH;
  auto height = page_rows * (PAGE_ROW_HEIGHT + PAGE_MARGIN) + 2 * MAP_PADDING;

  rect rec = {
      .x = (double)x_pos,
      .y = (double)y_pos,
      .width = (double)width,
      .height = (double)height,
  };
  auto ret = y_cursor + height + MAP_MARGIN;
  if (!visible_rect(rec, s))
    return ret;
  draw_rect(rec, DARKGRAY, s);

  for (size_t i = 0; i < pages; i++) {
    draw_page(i, map->root_pt, pn, y_cursor, s);
  }

  return ret;
}

ul draw_spacer(ul y_cursor, space *s) {
  auto x_pos = 0;
  auto y_pos = y_cursor;
  auto width = MAP_WIDTH;
  auto height = SPACER_HEIGHT;

  rect rec = {
      .x = (double)x_pos,
      .y = (double)y_pos,
      .width = (double)width,
      .height = (double)height,
  };
  draw_rect(rec, LIGHTGRAY, s);

  return y_cursor + SPACER_HEIGHT + MAP_MARGIN;
}

void draw_maps(map_registry *reg, active_page_notifier *pn, space *s) {
  // TODO: refactor this to not take the lock in this module
  pthread_mutex_lock(&reg->mu);

  auto prev = (map *)nullptr;
  auto curr = reg->first;
  auto y_cursor = 0ul;

  while (curr != nullptr) {
    // diff prev addr with curr addr and draw a spacer
    if ((prev == nullptr && curr->remote_addr != 0) ||
        (prev != nullptr &&
         (prev->remote_addr + prev->len) != curr->remote_addr))
      y_cursor = draw_spacer(y_cursor, s);

    y_cursor = draw_map(curr, pn, y_cursor, s);
    prev = curr;
    curr = curr->next;
  }

  pthread_mutex_unlock(&reg->mu);
}

void draw_debug_info(debug_info *di, space *s) {
  char buf[1 << 12] = "";

  auto now = now_seconds();
  auto time_delta = now - di->frame_rate_start_time;
  if (time_delta > 1) {
    di->frame_rate = (double)di->frame_count / time_delta;
    di->frame_count = 0;
    di->frame_rate_start_time = now;
  }

  snprintf(buf, sizeof(buf),
           "global_frame_count: %ld\n"
           "frame_rate: %f\n"
           "draw_count: %ld\n"
           "zoom: %f\n",
           di->global_frame_count, di->frame_rate, draw_count, (double)s->zoom);
  // TODO: pull out draw counts into debug info
  // TODO: report origin as well

  DrawText(buf, 24, 24, 20, DARKGRAY);
  DrawText(buf, 20, 20, 20, LIGHTGRAY);

  draw_count = 2;
}
