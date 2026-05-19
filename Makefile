.PHONY: all clean

all: build/draw_ui

clean:
	-rm -rf build 2>/dev/null

build/%: src/%.c
	mkdir -p $(@D)
	clang -lraylib -std=gnu23 -g -Og -o $@ $^
