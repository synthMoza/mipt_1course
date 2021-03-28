CC = gcc
CFLAGS = -Wall -g -MD

all: list

list: list.o main.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf list *.d *.o

-include *.d