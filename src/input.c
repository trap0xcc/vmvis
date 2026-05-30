#include <stdlib.h>

#include "input.h"
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

void handle_input_pan(space *s, input_state *is) {
  // TODO: fix minor glitch when translating and zooming at the same time

  auto mouse_pos = raylib_to_vec2(GetMousePosition());
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    is->pressed_pos = mouse_pos;
    is->pressed_origin = s->origin;
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    s->origin.x = is->pressed_origin.x + mouse_pos.x - is->pressed_pos.x;
    s->origin.y = is->pressed_origin.y + mouse_pos.y - is->pressed_pos.y;
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

void handle_input(space *s, input_state *is) {
  handle_input_scroll(s);
  handle_input_pan(s, is);
  handle_input_key_press();
}
