LIBS+=\
	-lm \
	$$(pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer) \
	-fsanitize=address -fsanitize=undefined
CFLAGS+=-g -std=c11 -Wall -Wextra -Wpedantic

.PHONY: all clean

all: bin/oppenheimer-simulator bin/config-test

clean:
	$(RM) bin/* build/*

bin/oppenheimer-simulator : build/main.o build/config.o build/gui.o
	mkdir -p bin
	$(CC) -o $@ $^ $(LIBS)

bin/config-test : build/config.o src/config-test.c
	mkdir -p bin
	$(CC) -o $@ $^ $(LIBS)

build/config.o: src/config.c src/config.h
build/gui.o: src/gui.c src/gui.h

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@
