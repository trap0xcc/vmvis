
#include "raylib.h"

Vector2 get_origin();
void set_origin(Vector2 origin);
float get_zoom();
void set_zoom(float zoom);

Vector2 relative_vec(Vector2 vec);
Vector2 inverse_relative_vec(Vector2 vec);
Rectangle relative_rect(Rectangle rect);
float relative_font_size(float font_size);
