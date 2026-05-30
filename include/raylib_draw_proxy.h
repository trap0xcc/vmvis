#ifndef _RAYLIB_DRAW_PROXY
#define _RAYLIB_DRAW_PROXY

#include "coords.h"
#include "debug.h"
#include "space.h"

typedef struct {
  debug_info *di;
} raylib_draw_proxy;

void draw_rect(raylib_draw_proxy *rdp, rect rec, Color color, space *s);
void draw_text(raylib_draw_proxy *rdp, char *text, vec2 position,
               double font_size, double spacing, Color color, space *s);
void draw_text_center(raylib_draw_proxy *rdp, char *text, vec2 position,
                      double font_size, double spacing, Color color, space *s);

#endif
