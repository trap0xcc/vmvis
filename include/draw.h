
#ifndef _DRAW_H
#define _DRAW_H

#include "active_page_notifier.h"
#include "debug_info.h"
#include "map_registry.h"
#include "page_table.h"
#include "raylib_draw_proxy.h"
#include "space.h"

/************************* Drawing Constants *************************/

static const auto CELL_WIDTH = 50ul;
static const auto CELL_HEIGHT = CELL_WIDTH;
static const auto CELL_MARGIN = 15ul;
static const auto CELL_OFFSET = CELL_WIDTH + CELL_MARGIN;
static const auto CELLS_PER_LINE = 64ul;
static const auto FONT_SIZE = CELL_HEIGHT / 8 * 5;

static const auto _CELL_MARGIN_BOOST_PER_ROW =
    ((CELLS_PER_LINE / 4) * CELL_MARGIN / 2) +
    ((CELLS_PER_LINE / 8) * CELL_MARGIN / 2);

static const auto PAGE_BYTE_SIZE = 1ul << 12;

static const auto PAGE_WIDTH = 64 * CELL_OFFSET + _CELL_MARGIN_BOOST_PER_ROW;
static const auto PAGE_HEIGHT = 64 * CELL_OFFSET + CELL_MARGIN;
static const auto PAGE_MARGIN = 100ul;

static const auto PAGES_PER_ROW = 8ul;

static const auto PAGE_ROW_WIDTH = (PAGE_WIDTH + PAGE_MARGIN) * PAGES_PER_ROW;
static const auto PAGE_ROW_HEIGHT = PAGE_HEIGHT;

static const auto MAP_MARGIN = 500ul;
static const auto MAP_PADDING = CELL_WIDTH;
static const auto MAP_WIDTH = PAGE_ROW_WIDTH + 2 * MAP_PADDING;

static const auto SPACER_HEIGHT = 5'000ul;

/************************* /Drawing Constants ************************/

void draw_maps(map_registry *reg, page_table *pt, active_page_notifier *pn,
               space *s, raylib_draw_proxy *rdp);
void draw_debug_info(debug_info *di, space *s, raylib_draw_proxy *rdp);

#endif
