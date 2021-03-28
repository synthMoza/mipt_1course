CC = gcc
CFLAGS = -Wall -g -MD

all: akinator

akinator: akinator.o stack.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf akinator *.d *.o

-include *.d