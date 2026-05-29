#define _DEFAULT_SOURCE
#include <unistd.h>

#include <raylib.h>

#include "app.h"
#include "debug.h"
#include "draw.h"
#include "input.h"
#include "relative.h"
#include "time_util.h"

void app_loop(map_registry *reg, active_page_notifier *pn) {
  debug_info di = {.frame_rate_start_time = now_seconds()};

  while (!WindowShouldClose()) {
    BeginDrawing();

    // NOTE: This translation can only be done after the first frame is drawn
    // since the screen width takes a while to be computed.
    if (di.global_frame_count == 1) {
      set_origin((Vector2){
          .x = ((float)GetScreenWidth() - page_row_width * get_zoom()) / 2,
          .y = 200,
      });
    }

    ClearBackground(BLACK);

    handle_input();

    draw_maps(reg, pn);

    draw_debug_info(&di);

    EndDrawing();

    di.global_frame_count++;
    di.frame_count++;
  }
}
