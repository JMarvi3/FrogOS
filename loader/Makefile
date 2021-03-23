#CC=i686-linux-gnu-gcc
CFLAGS=-static -I../include -nostdlib

loader: boot.S ld.script irq.S enable_lmode.S gdt.S
	$(CC) $(CFLAGS) -m32 -T ld.script -o loader boot.S irq.S enable_lmode.S gdt.S -fno-pic -O3
	cp loader ../boot

clean:
	rm -f loader
