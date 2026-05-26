.PHONY: all clean

CC := clang
DEBUG := -g
WARNINGS := -Wall -Wextra -Wconversion -Wimplicit-int-float-conversion
WARNINGS += -Wpedantic -Wshadow -Wstrict-prototypes -Wdouble-promotion
INCLUDES := -Iinclude
STANDARD := -std=c23
DEPENDENCIES := -MMD -MP
OPTIMIZATIONS := -O0
CFLAGS := $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) $(DEPENDENCIES)

LDLIBS := -lraylib
LDFLAGS := -fuse-ld=lld

BINARIES := build/vmvis
OBJECTS := $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))

all: $(BINARIES)

clean:
	rm -rf build

build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/vmvis: $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(OBJECTS:.o=.d)
