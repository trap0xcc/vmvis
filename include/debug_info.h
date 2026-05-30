#ifndef _DEBUG_INFO_H
#define _DEBUG_INFO_H

#include <unistd.h>

#include "lang.h"
#include "space.h"

typedef struct {
  ul global_frame_count;
  ul frame_count;
  double frame_rate_start_time;
  double frame_rate;
  ul draw_count;
  ul total_map_size;
} debug_info;

void debug_info_str(char *buf, size_t len, debug_info *di, space *s);

#endif
