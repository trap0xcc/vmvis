.PHONY: all clean run

CC := clang
DEBUG := -g
WARNINGS := -Wall -Wextra -Wconversion -Wimplicit-int-float-conversion
WARNINGS += -Wpedantic -Wshadow -Wstrict-prototypes -Wdouble-promotion
WARNINGS += -Wreturn-stack-address -Werror
INCLUDES := -Iinclude
STANDARD := -std=c23
DEPENDENCIES := -MMD -MP
OPTIMIZATIONS := -O0

SAN ?= none
SANS := -fsanitize=undefined -fno-omit-frame-pointer
ifeq ($(SAN),asan)
	SANS += -fsanitize=address
else
	SANS += -fsanitize=thread
endif

CFLAGS := $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) $(DEPENDENCIES) $(SANS)

LDLIBS := -lraylib
LDFLAGS := -fuse-ld=lld $(SANS)

BINARIES := build/vmvis
OBJECTS := $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))

all: $(BINARIES)

clean:
	rm -rf build

run: build/vmvis
	TSAN_OPTIONS="suppressions=tsan_ignore.txt" $<

build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/vmvis: $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(OBJECTS:.o=.d)
