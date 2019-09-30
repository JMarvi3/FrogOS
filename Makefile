CFLAGS=-Iinclude -nostdinc -fno-builtin
kernel: cmain.o boot.o lib/libc.a
	ld -T ld.script -o kernel -Llib boot.o cmain.o -l:libc.a  -L/usr/lib/gcc/i586-linux-gnu/4.9 -lgcc
	cp kernel /srv/tftp/xinu.boot
lib/libc.a: lib/*.c
	$(MAKE) -C lib
boot.o: boot.S
	$(CC) -Iinclude -c boot.S
clean:
	$(MAKE) -C lib clean
	rm -f *.o kernel
