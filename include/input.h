#ifndef _INPUT_H
#define _INPUT_H

#include "coords.h"
#include "space.h"

typedef struct {
  vec2 pressed_pos;
  vec2 pressed_origin;
} input_state;

void handle_input(space *s, input_state *is);

#endif
