CC = gcc
CFLAGS = -Wall -O2
	
all: vasim

vasim: vasim.c
	$(CC) $(CFLAGS) -o $@ vasim.c

clean:
	rm -f vasim
	rm -f tests/*_actual* tests/*_diff*


