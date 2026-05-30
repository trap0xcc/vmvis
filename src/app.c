#define _DEFAULT_SOURCE
#include <unistd.h>

#include <raylib.h>

#include "app.h"
#include "debug_info.h"
#include "draw.h"
#include "input.h"
#include "page_table.h"
#include "raylib_draw_proxy.h"
#include "space.h"
#include "time_util.h"

void app_loop(map_registry *reg, active_page_notifier *pn) {
  space s = space_relative_default;

  debug_info di = {.frame_rate_start_time = now_seconds()};
  raylib_draw_proxy rdp = {.di = &di};

  page_table pt = {};
  input_state is = {};

  while (!WindowShouldClose()) {
    BeginDrawing();

    // NOTE: This translation can only be done after the first frame is drawn
    // since the screen width takes a while to be computed.
    if (di.global_frame_count == 1) {
      s.origin.x = (GetScreenWidth() - PAGE_ROW_WIDTH * s.zoom) / 2;
      s.origin.y = 200;
    }

    ClearBackground(BLACK);

    handle_input(&s, &is);

    draw_maps(reg, &pt, pn, &s, &rdp);
    draw_debug_info(&di, &s, &rdp);

    EndDrawing();

    di.global_frame_count++;
    di.frame_count++;
    auto stats = registry_stats(reg);
    di.map_reg_byte_size = stats.byte_size;
    di.map_reg_len = stats.len;
    di.map_reg_page_size = stats.page_size;
    di.draw_count = 0;
  }
}
