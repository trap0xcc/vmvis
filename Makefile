.PHONY: all clean

WARNINGS = -Wall -Wextra -Wconversion -Wimplicit-int-float-conversion
INCLUDES = -Iinclude
LIBRARIES = -lraylib
STANDARD = -std=c23
DEBUG = -g
OPTIMIZATIONS = -Og

BINARIES = build/main
OBJECTS = build/map.o build/window.o build/relative.o build/input.o build/draw.o

all: $(BINARIES) $(OBJECTS)

clean:
	-rm -rf build 2>/dev/null

build/map.o: src/map.c
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -c -o $@ $^

build/window.o: src/window.c
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -c -o $@ $^

build/relative.o: src/relative.c
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -c -o $@ $^

build/input.o: src/input.c
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -c -o $@ $^

build/draw.o: src/draw.c
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -c -o $@ $^

build/main: src/main.c $(OBJECTS)
	mkdir -p $(@D)
	clang $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) -o $@ $^ $(LIBRARIES)
