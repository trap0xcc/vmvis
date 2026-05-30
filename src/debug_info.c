#include <stdio.h>

#include "debug_info.h"

void debug_info_str(char *buf, size_t len, debug_info *di, space *s) {
  snprintf(buf, len,
           "global_frame_count: %'ld\n"
           "frame_rate: %f\n"
           "draw_count: %'ld\n"
           "map_reg_len: %'ld\n"
           "map_reg_byte_size: %'ld\n"
           "map_reg_page_size: %'ld\n"
           "zoom: %f\n"
           "origin: (%f, %f)\n",
           di->global_frame_count, di->frame_rate, di->draw_count,
           di->map_reg_len, di->map_reg_byte_size, di->map_reg_page_size,
           s->zoom, s->origin.x, s->origin.y);
}
