
#include "raylib.h"

Vector2 get_origin();
void set_origin(Vector2 origin);
float get_zoom();
void set_zoom(float zoom);

Vector2 relative_vec_to_screen(Vector2 vec);
Vector2 screen_vec_to_relative(Vector2 vec);
Rectangle relative_rect_to_screen(Rectangle rect);
float relative_font_size_to_screen(float font_size);
