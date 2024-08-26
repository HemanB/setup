CC = gcc
CFLAGS = -Wall -O2

PROGS = sumarrayrows\
	sumarraycols\

all: casim $(PROGS)

casim: casim.c
	$(CC) $(CFLAGS) -o $@ casim.c -lm

clean:
	rm -f casim sumarrayrows sumarraycols
	rm -f tests/*_actual* tests/*_diff*
	rm -rf __pycache__/


