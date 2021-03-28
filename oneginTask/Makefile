CC = gcc
CFLAGS = -Wall -g -MD

all: onegin

onegin: onegin.o main.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf onegin *.d *.o

-include *.d