CC = gcc
CFLAGS = -Wall -g

PROGS = counter array

all: $(PROGS)

counter: counter.c mythreads.h .thread_util.o
	$(CC) $(CFLAGS) -o $@  $< .thread_util.o -lpthread

array: array.c mythreads.h .thread_util.o
	$(CC) $(CFLAGS) -o $@  $< .thread_util.o -lpthread

clean:
	rm -f $(PROGS)


