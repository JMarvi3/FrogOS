CFLAGS=-Iinclude -nostdinc -fno-builtin -fno-stack-protector -O3
kernel: cmain.o boot.o lib/libc.a irq_asm.o irq.o gdt.o pit.o paging.o keyboard.o pci.o pcnet32.o  print_info.o memory.o net.o ether.o arp.o ipv4.o icmp.o ld.script boot/generic.grub
	ld -T ld.script -o kernel -Llib *.o -l:libc.a  -L/usr/lib/gcc/i686-linux-gnu/5 -lgcc
	cp kernel boot
lib/libc.a: lib/*.c
	$(MAKE) -C lib
boot.o: boot.S
	$(CC) -Iinclude -c boot.S
boot/generic.grub:
	$(MAKE) -C boot
clean:
	$(MAKE) -C lib clean
	$(MAKE) -C boot clean
	rm -f *.o kernel boot/kernel
