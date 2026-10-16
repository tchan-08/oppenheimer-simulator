CFLAGS=-g -Wall -Wextra
LIBS=-lm

oppenheimer-simulator : src/main.o
	$(CC) -o $@ $^ $(LIBS)
