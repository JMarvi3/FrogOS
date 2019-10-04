CFLAGS=-I../include -nostdinc -fno-builtin -std=c99 -fno-stack-protector -O3
libc.a: *.c
	rm -f libc.a
	$(CC) $(CFLAGS) -c *.c
	ar rcs libc.a *.o
clean:
	rm -f *.o libc.a
