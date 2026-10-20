LIBS=-lm $$(pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer)
CFLAGS=-g -std=c11 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined $(LIBS)

.PHONY: all clean

all: bin/oppenheimer-simulator bin/config-test

clean:
	$(RM) bin/* build/*

bin/oppenheimer-simulator : build/main.o build/config.o
	mkdir -p bin
	$(CC) -o $@ $^ $(CFLAGS)

bin/config-test : build/config.o src/config-test.c
	mkdir -p bin
	$(CC) -o $@ $^ $(CFLAGS)

build/config.o: src/config.c src/config.h
build/gui.o: src/gui.c src/gui.h

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@
