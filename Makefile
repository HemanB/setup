all: hello

hello: hello.c
	gcc -o $@ $<

clean:
	rm -f hello
	rm -f tests/*_diff* tests/*_actual*
	rm -rf __pycache__/
