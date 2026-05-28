.PHONY: all clean run run.asan run.tsan run.ubsan

CC := clang
DEBUG := -g
WARNINGS := -Wall -Wextra -Wconversion -Wimplicit-int-float-conversion
WARNINGS += -Wpedantic -Wshadow -Wstrict-prototypes -Wdouble-promotion
WARNINGS += -Wreturn-stack-address -Werror
INCLUDES := -Iinclude
STANDARD := -std=c23
DEPENDENCIES := -MMD -MP
OPTIMIZATIONS := -O0
OTHER := -fno-omit-frame-pointer
SAN ?= none
SANS :=
ifeq ($(SAN),asan)
	SANS += -fsanitize=address
	SANS += -fsanitize=undefined
else ifeq ($(SAN),tsan)
	SANS += -fsanitize=thread
	SANS += -fsanitize=undefined
else ifeq ($(SAN),ubsan)
	SANS += -fsanitize=undefined
endif
CFLAGS := $(WARNINGS) $(INCLUDES) $(STANDARD) $(DEBUG) $(OPTIMIZATIONS) $(DEPENDENCIES) $(OTHER) $(SANS)

LDLIBS := -lraylib
LDFLAGS := -fuse-ld=lld $(SANS)

BINARIES := build/vmvis
OBJECTS := $(patsubst src/%.c,build/%.o,$(wildcard src/*.c))

all: $(BINARIES)

clean:
	rm -rf build

run: clean build/vmvis
	build/vmvis

run.tsan: clean
	SAN=tsan make build/vmvis
	TSAN_OPTIONS="suppressions=tsan_ignore.txt" build/vmvis

run.asan: clean
	SAN=asan make build/vmvis
	build/vmvis

run.ubsan: clean
	SAN=ubsan make build/vmvis
	build/vmvis

build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/vmvis: $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(OBJECTS:.o=.d)
