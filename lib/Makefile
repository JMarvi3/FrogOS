CFLAGS=-I../include -nostdinc -fno-builtin -std=c99 -fno-stack-protector
libc.a: *.c
	rm -f libc.a
	$(CC) $(CFLAGS) -c *.c
	$(CC) $(CFLAGS) -std=c99 -c printf.c
	ar rcs libc.a *.o
clean:
	rm -f *.o libc.a
