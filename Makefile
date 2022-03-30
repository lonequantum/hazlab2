hazlab2:
	c99 -o hazlab2 *.c -std=c99 -D_POSIX_C_SOURCE=200809L

release:
	c99 -o hazlab2 -O3 *.c -DNDEBUG -std=c99 -D_POSIX_C_SOURCE=200809L
	strip hazlab2

clean:
	rm hazlab2
