CC=i686-linux-gnu-gcc
CFLAGS=-I../include -nostdinc -fno-builtin -std=c99 -fno-stack-protector -O3 -fno-pic
libc.a: *.c
	rm -f libc.a
	$(CC) $(CFLAGS) -c *.c
	ar rcs libc.a *.o
clean:
	rm -f *.o libc.a
