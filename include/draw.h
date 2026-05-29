
#ifndef _DRAW_H
#define _DRAW_H

#include "active_page_notifier.h"
#include "debug.h"
#include "map.h"

void draw_loop(map_registry *reg, active_page_notifier *pn);
void draw_debug_info(debug_info *di);

/************************* Drawing Constants *************************/

// TODO: upper case these
static const auto cell_width = 50ul;
static const auto cell_height = cell_width;
static const auto cell_margin = 15ul;
static const auto cell_offset = cell_width + cell_margin;
static const auto cells_per_line = 64ul;
static const auto font_size = cell_height / 8 * 5;

static const auto cell_margin_boost_per_row =
    ((cells_per_line / 4) * cell_margin / 2) +
    ((cells_per_line / 8) * cell_margin / 2);

static const auto page_byte_size = 1ul << 12;

static const auto page_width = 64 * cell_offset + cell_margin_boost_per_row;
static const auto page_height = 64 * cell_offset + cell_margin;
static const auto page_margin = 100ul;

static const auto pages_per_row = 8ul;

static const auto page_row_width = (page_width + page_margin) * pages_per_row;
static const auto page_row_height = page_height;

static const auto map_margin = 500ul;
static const auto map_padding = cell_width;
static const auto map_width = page_row_width + 2 * map_padding;

static const auto spacer_height = 5'000ul;

/************************* /Drawing Constants ************************/

#endif
