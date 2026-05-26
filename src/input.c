#include <stdlib.h>

#include "raylib.h"

#include "relative.h"

void handle_input_scroll() {
  static const auto zoom_factor = 0.1f;

  auto pos = GetMousePosition();
  auto i_pos = screen_vec_to_relative(pos);

  auto val = GetMouseWheelMove();
  if (val != 0) {
    auto direction = (val < 0) ? 1 - zoom_factor : 1 + zoom_factor;
    set_zoom(get_zoom() * direction);

    auto new_pos = relative_vec_to_screen(i_pos);

    auto origin = get_origin();
    set_origin((Vector2){
        origin.x - new_pos.x + pos.x,
        origin.y - new_pos.y + pos.y,
    });
  }
}

void handle_input_pan() {
  // TODO: fix minor glitch when translating and zooming at the same time
  static Vector2 pressed_pos;
  static Vector2 pressed_origin;

  auto mouse_pos = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    pressed_pos = mouse_pos;
    pressed_origin = get_origin();
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    set_origin((Vector2){
        pressed_origin.x + mouse_pos.x - pressed_pos.x,
        pressed_origin.y + mouse_pos.y - pressed_pos.y,
    });
  }
}

void handle_input_key_press() {
  int key;
  while ((key = GetKeyPressed()) != 0) {
    switch (key) {
      // default:
      //   printf("UNKNOWN KEY PRESSED: '%c', (%x)", key, key);
    case KEY_Q:
      exit(0);
      break;
    }
  }
}

void handle_input() {
  handle_input_scroll();
  handle_input_pan();
  handle_input_key_press();
}
