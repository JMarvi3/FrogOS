CC=i686-linux-gnu-gcc
CFLAGS=-static -I../include -nostdlib

loader: boot.S ld.script irq.S
	$(CC) $(CFLAGS) -T ld.script -o loader boot.S irq.S -fno-pic -O3
	cp loader ../boot

clean:
	rm -f loader
