CFLAGS=-g -Wall -Wextra
LIBS=-lm

oppenheimer-simulator : src/main.o src/config.o
	$(CC) -o $@ $^ $(LIBS)
