CFLAGS=-g -Wall -Wextra
LIBS=-lm

oppenheimer-simulator : src/main.o src/config.o
	$(CC) -o $@ $^ $(LIBS)

config-test : src/config.c src/config-test.c
	$(CC) -Wall -Wextra -g -o $@ $^ $(LIBS)
