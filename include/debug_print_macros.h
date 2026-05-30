#ifndef _DEBUG_PRINT_MACROS_H
#define _DEBUG_PRINT_MACROS_H

#include <stdio.h>

#define DEFINE_PRINTER_HEADER(Type) void Type##_print(const Type *v);

#define DEFINE_PRINTER(Type, FIELDS)                                           \
  void Type##_print(const Type *v) {                                           \
    printf(#Type " {\n");                                                      \
    FIELDS(PRINT_FIELD, v)                                                     \
    printf("}\n");                                                             \
  }

#define PRINT_FIELD(name, type, fmt, obj)                                      \
  printf("  " #name " = " fmt "\n", (type)((obj)->name));

#endif
