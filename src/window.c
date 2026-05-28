#include "raylib.h"

void create_window() {
  static const auto init_width = 1024;
  static const auto init_height = 768;

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(init_width, init_height, "Virtual Memory Visualizer");
  SetTargetFPS(60);
}

void destroy_window() { CloseWindow(); }
