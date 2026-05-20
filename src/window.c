#include "raylib.h"

void create_window() {
  static const auto init_width = 1024;
  static const auto init_height = 768;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(init_width, init_height, "test");
  SetTargetFPS(60);
}

void destroy_window() { CloseWindow(); }
