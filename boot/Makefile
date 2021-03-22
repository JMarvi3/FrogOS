CC=i686-linux-gnu-gcc
CFLAGS=-static -I../include -nostdlib

generic.grub: pre_grub.cfg
#	grub-mkimage -O i386-pc-pxe -o generic.grub --prefix='(pxe)' tftp multiboot2 boot ls pxe pxechain help http configfile echo -c pre_grub.cfg
	grub-mkimage -O i386-pc-pxe -o generic.grub --prefix="(http,192.168.1.202:8000)" pxe pxechain cat multiboot multiboot2 boot ls help http configfile echo -c pre_grub.cfg

clean:
	rm -f loader
	rm -f generic.grub
