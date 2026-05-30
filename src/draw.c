#define _DEFAULT_SOURCE
#include <libgen.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
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

bool visible_rect_const_size_right_aligned(rect rect, vec2 offset, space *s) {
  auto screen_rect = rect_apply_space(rect, s);
  screen_rect.width = rect.width;
  screen_rect.height = rect.height;
  screen_rect.x -= screen_rect.width;
  screen_rect.x += offset.x;
  screen_rect.y += offset.y;

  return directional_overlap_1d(screen_rect.y,
                                screen_rect.y + screen_rect.height, 0,
                                (double)GetScreenHeight()) &&
         directional_overlap_1d(screen_rect.x,
                                screen_rect.x + screen_rect.width, 0,
                                (double)GetScreenWidth());
}

void draw_page_cells(ul x, ul y, page_table *pt, space *s,
                     raylib_draw_proxy *rdp, page_ids *page_ids,
                     page_id page_id) {
  // TODO: remove these once in use
  (void)pt;

  static const auto zoom_threshold = 0.1;

  if (s->zoom < zoom_threshold)
    return;

  page_ids_add(page_ids, page_id);

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

void draw_page(size_t page_num, page_table *pt, page_ids *page_ids,
               page_id page_id, ul y_cursor, space *s, raylib_draw_proxy *rdp,
               ul pages_per_row) {
  auto x_pos =
      PAGE_MARGIN + (page_num % pages_per_row) * (PAGE_WIDTH + PAGE_MARGIN);
  auto y_pos = y_cursor + PAGE_MARGIN +
               (page_num / pages_per_row) * (PAGE_HEIGHT + PAGE_MARGIN);
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

  draw_page_cells(x_pos, y_pos, pt, s, rdp, page_ids, page_id);
}

ul draw_map(map *map, page_table *pt, active_page_notifier *apn, ul y_cursor,
            space *s, raylib_draw_proxy *rdp) {
  auto pages = map->len / PAGE_BYTE_SIZE;
  auto pages_per_row = PAGES_PER_ROW;
  if (pages > 1 << 10)
    pages_per_row = (ul)sqrt((double)pages);
  auto page_rows = pages / pages_per_row;
  if (pages % pages_per_row != 0)
    page_rows++;

  auto x_pos = 0;
  auto y_pos = y_cursor;
  // TODO: clean this up once you make map sizing more dynamic vs consts
  auto width = MAP_WIDTH / PAGES_PER_ROW * pages_per_row;
  auto height = page_rows * (PAGE_ROW_HEIGHT + PAGE_MARGIN) + 2 * MAP_PADDING;

  // draw map info panel
  {
    static const auto INFO_PANEL_COLOR = LIGHTGRAY;
    static const auto INFO_PANEL_BGCOLOR = BLACK;
    static const auto INFO_PANEL_TEXT_SPACING = 2;
    static const auto INFO_PANEL_TEXT_PADDING = 10;
    static const auto INFO_PANEL_TEXT_SIZE = 20;

    rect rect = {
        .x = (double)x_pos,
        .y = (double)y_pos,
        .width = (double)400,
        .height = (double)200,
    };
    auto rect_offset = (vec2){
        .x = -(double)INFO_PANEL_TEXT_PADDING / 2,
        .y = -(double)INFO_PANEL_TEXT_PADDING / 2,
    };
    if (!visible_rect_const_size_right_aligned(rect, rect_offset, s))
      goto draw_map_rect;
    draw_rect_const_size_right_aligned(rdp, rect, rect_offset,
                                       INFO_PANEL_BGCOLOR, s);

    auto name = (map->path && strlen(map->path)) ? map->path : nullptr;
    auto position = (vec2){
        .x = (double)x_pos,
        .y = (double)y_pos,
    };
    auto offset = (vec2){
        .x = -INFO_PANEL_TEXT_PADDING,
    };
    if (name) {
      draw_text_const_size_right_aligned(
          rdp, name, position, offset, INFO_PANEL_TEXT_SIZE,
          INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);
      if (s->zoom < 0.01)
        goto draw_map_rect;
    }

    char buf[1 << 8] = {};
    snprintf(buf, sizeof(buf), "0x%lx-0x%lx", map->start, map->end);
    if (name)
      offset.y += INFO_PANEL_TEXT_SIZE;
    draw_text_const_size_right_aligned(
        rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
        INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);
    if (!name && s->zoom < 0.01)
      goto draw_map_rect;

    snprintf(buf, sizeof(buf), "len: 0x%lx", map->len);
    offset.y += INFO_PANEL_TEXT_SIZE;
    draw_text_const_size_right_aligned(
        rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
        INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);

    if (map->offset) {
      snprintf(buf, sizeof(buf), "offset: 0x%lx", map->offset);
      offset.y += INFO_PANEL_TEXT_SIZE;
      draw_text_const_size_right_aligned(
          rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
          INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);
    }

    snprintf(buf, sizeof(buf), "pages: %'ld", map->pages);
    offset.y += INFO_PANEL_TEXT_SIZE;
    draw_text_const_size_right_aligned(
        rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
        INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);

    auto read_str = (map->prot & PROT_READ) ? "READ | " : "";
    auto write_str = (map->prot & PROT_WRITE) ? "WRITE | " : "";
    auto exec_str = (map->prot & PROT_EXEC) ? "EXEC | " : "";
    auto shared_str = (map->flags & MAP_SHARED) ? "SHARED | " : "";
    auto private_str = (map->flags & MAP_PRIVATE) ? "PRIVATE | " : "";
    snprintf(buf, sizeof(buf), "%s%s%s%s%s", read_str, write_str, exec_str,
             shared_str, private_str);
    auto len = strlen(buf);
    if (len) {
      buf[len - 3] = '\0';
      offset.y += INFO_PANEL_TEXT_SIZE;
      draw_text_const_size_right_aligned(
          rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
          INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);
    }

    if (map->inode) {
      snprintf(buf, sizeof(buf), "dev: %2d:%2d", map->dev_major,
               map->dev_minor);
      offset.y += INFO_PANEL_TEXT_SIZE;
      draw_text_const_size_right_aligned(
          rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
          INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);

      snprintf(buf, sizeof(buf), "inode: %'d", map->inode);
      offset.y += INFO_PANEL_TEXT_SIZE;
      draw_text_const_size_right_aligned(
          rdp, buf, position, offset, INFO_PANEL_TEXT_SIZE,
          INFO_PANEL_TEXT_SPACING, INFO_PANEL_COLOR, s);
    }
  }

draw_map_rect:
  // draw map rect
  {
    rect rect = {
        .x = (double)x_pos,
        .y = (double)y_pos,
        .width = (double)width,
        .height = (double)height,
    };
    // if map isn't visible, stop all work
    if (!visible_rect(rect, s))
      goto ret;
    draw_rect(rdp, rect, DARKGRAY, s);
  }

  {
    // skip drawing pages if we are zoomed out
    if (s->zoom < 0.002)
      goto ret;

    // draw pages
    auto p_ids = calloc(1, sizeof(page_ids));
    for (size_t i = 0; i < pages; i++)
      draw_page(i, pt, p_ids, page_id_from_map(map, i), y_cursor, s, rdp,
                pages_per_row);
    active_page_notifier_notify(apn, p_ids);
  }

ret:
  return y_cursor + height + MAP_MARGIN;
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
  active_page_notifier *apn;
  space *s;
  raylib_draw_proxy *rdp;
} _draw_map_visitor_userdata;

void _draw_map_visitor(map *map, void *userdata) {
  _draw_map_visitor_userdata *ud = userdata;

  if ((ud->prev == nullptr && map->start != 0) ||
      (ud->prev != nullptr && ud->prev->end != map->start))
    ud->y_cursor = draw_spacer(ud->y_cursor, ud->s, ud->rdp);

  ud->y_cursor = draw_map(map, ud->pt, ud->apn, ud->y_cursor, ud->s, ud->rdp);

  ud->prev = map;
}

void draw_maps(map_registry *reg, page_table *pt, active_page_notifier *apn,
               space *s, raylib_draw_proxy *rdp) {
  _draw_map_visitor_userdata ud = {
      .pt = pt,
      .apn = apn,
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

  debug_info_str(buf, sizeof(buf), di, s);

  draw_text(rdp, buf, (vec2){24, 24}, 20, 1, DARKGRAY, nullptr);
  draw_text(rdp, buf, (vec2){20, 20}, 20, 1, LIGHTGRAY, nullptr);
}
