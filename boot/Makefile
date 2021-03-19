generic.grub:
	grub-mkimage -O i386-pc-pxe -o generic.grub --prefix='(pxe)' tftp multiboot2 boot ls pxe pxechain help http configfile echo -c pre_grub.cfg
clean:
	rm -f generic.grub
