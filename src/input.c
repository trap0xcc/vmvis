#include <stdlib.h>

#include "coords.h"
#include "raylib.h"

#include "space.h"

void handle_input_scroll(space *s) {
  static const auto zoom_factor = 0.1;

  auto pos = raylib_to_vec2(GetMousePosition());
  auto i_pos = vec_undo_space(pos, s);

  auto val = GetMouseWheelMove();
  if (val != 0) {
    s->zoom *= (val < 0) ? 1 - zoom_factor : 1 + zoom_factor;

    auto new_pos = vec_apply_space(i_pos, s);

    s->origin.x += -new_pos.x + pos.x;
    s->origin.y += -new_pos.y + pos.y;
  }
}

void handle_input_pan(space *s) {
  // TODO: fix minor glitch when translating and zooming at the same time
  // TODO: remove static state
  static vec2 pressed_pos;
  static vec2 pressed_origin;

  auto mouse_pos = raylib_to_vec2(GetMousePosition());
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    pressed_pos = mouse_pos;
    pressed_origin = s->origin;
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    s->origin.x = pressed_origin.x + mouse_pos.x - pressed_pos.x;
    s->origin.y = pressed_origin.y + mouse_pos.y - pressed_pos.y;
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

void handle_input(space *s) {
  handle_input_scroll(s);
  handle_input_pan(s);
  handle_input_key_press();
}
