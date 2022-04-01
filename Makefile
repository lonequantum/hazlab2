hazlab2: clean
	c99 -o hazlab2 *.c -D_POSIX_C_SOURCE=200809L

release:
	c99 -o hazlab2 -O3 *.c -DNDEBUG -D_POSIX_C_SOURCE=200809L
	strip hazlab2

clean:
	rm -f hazlab2
