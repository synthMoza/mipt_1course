CC = gcc
CFLAGS = -Wall -g -MD

all: stack

stack: stack.o main.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf stack *.d *.o

-include *.d