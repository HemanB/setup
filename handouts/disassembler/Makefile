all: facile

facile: facile.o asm.o dis.o machine.o
	gcc -g -o $@ facile.o asm.o dis.o machine.o

facile.o: facile.c
	gcc -g -c $?

asm.o: asm.c 
	gcc -g -c $?

dis.o: dis.c 
	gcc -g -c $?

machine.o: machine.c 
	gcc -g -c $?

clean:
	rm -rf *o a.out *.dSYM
	rm -rf __pycache__/

scratch: clean
	rm -f facile
