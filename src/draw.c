#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "raylib.h"

#include "active_page_notifier.h"
#include "coords.h"
#include "draw.h"
#include "lang.h"
#include "raylib_draw_proxy.h"
#include "time_util.h"

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
                     space *s, raylib_draw_proxy *rdp) {
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
    draw_rect(rdp, shadow_rec, BLACK, s);
    draw_rect(rdp, rec, LIGHTGRAY, s);

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
    draw_text_center(rdp, "--", text_pos, (double)FONT_SIZE, 1, DARKGRAY, s);
  }
}

void draw_page(size_t page_num, page_table *pt, active_page_notifier *pn,
               ul y_cursor, space *s, raylib_draw_proxy *rdp) {
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
  draw_rect(rdp, rec, GRAY, s);

  draw_page_cells(x_pos, y_pos, pt, pn, s, rdp);
}

ul draw_map(map *map, page_table *pt, active_page_notifier *pn, ul y_cursor,
            space *s, raylib_draw_proxy *rdp) {
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
  draw_rect(rdp, rec, DARKGRAY, s);

  for (size_t i = 0; i < pages; i++) {
    draw_page(i, pt, pn, y_cursor, s, rdp);
  }

  return ret;
}

ul draw_spacer(ul y_cursor, space *s, raylib_draw_proxy *rdp) {
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
  draw_rect(rdp, rec, LIGHTGRAY, s);

  return y_cursor + SPACER_HEIGHT + MAP_MARGIN;
}

typedef struct {
  map *prev;
  ul y_cursor;

  page_table *pt;
  active_page_notifier *pn;
  space *s;
  raylib_draw_proxy *rdp;
} _draw_map_visitor_userdata;

void _draw_map_visitor(map *map, void *userdata) {
  _draw_map_visitor_userdata *ud = userdata;

  if ((ud->prev == nullptr && map->remote_addr != 0) ||
      (ud->prev != nullptr &&
       (ud->prev->remote_addr + ud->prev->len) != map->remote_addr))
    ud->y_cursor = draw_spacer(ud->y_cursor, ud->s, ud->rdp);

  ud->y_cursor = draw_map(map, ud->pt, ud->pn, ud->y_cursor, ud->s, ud->rdp);

  ud->prev = map;
}

void draw_maps(map_registry *reg, page_table *pt, active_page_notifier *pn,
               space *s, raylib_draw_proxy *rdp) {
  _draw_map_visitor_userdata ud = {
      .pt = pt,
      .pn = pn,
      .s = s,
      .rdp = rdp,
  };
  registry_visit(reg, _draw_map_visitor, &ud);
}

void draw_debug_info(debug_info *di, space *s, raylib_draw_proxy *rdp) {
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
           "zoom: %f\n"
           "origin: (%f, %f)\n",
           di->global_frame_count, di->frame_rate, di->draw_count, s->zoom,
           s->origin.x, s->origin.y);

  draw_text(rdp, buf, (vec2){24, 24}, 20, 1, DARKGRAY, nullptr);
  draw_text(rdp, buf, (vec2){20, 20}, 20, 1, LIGHTGRAY, nullptr);
}
