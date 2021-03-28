CC = gcc
CFLAGS = -Wall -g -MD

all: solve_square

solve_square: main.o SolveSquare.o input.o
	$(CC) -o $@ $^ -lm

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf solve_square *.d *.o

-include *.d