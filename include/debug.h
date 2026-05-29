
#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#include "map.h"
#include "raylib.h"

#define DEFINE_PRINTER_HEADER(Type) void Type##_print(const Type *v);

DEFINE_PRINTER_HEADER(Rectangle)

void register_test_maps(map_registry *reg);

#endif
