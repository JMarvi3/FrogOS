CC=i686-linux-gnu-gcc
CFLAGS=-static -I../include -nostdlib
other_mods=video_fb videoinfo videotest vga vbe all_video
mods=pxe pxechain http cat multiboot multiboot2 boot ls help configfile echo $(other_mods)
generic.grub: pre_grub.cfg Makefile
#	grub-mkimage -O i386-pc-pxe -o generic.grub --prefix='(pxe)' tftp multiboot2 boot ls pxe pxechain help http configfile echo -c pre_grub.cfg
	grub-mkimage -O i386-pc-pxe -o generic.grub --prefix="(http,192.168.1.202:8000)" $(mods) -c pre_grub.cfg

clean:
	rm -f loader
	rm -f generic.grub
