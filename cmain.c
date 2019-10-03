#include <multiboot.h>
#include <stdio.h>
#include <conio.h>
#include <port.h>
#include <stdint.h>
#include <system.h>

extern void gdt_flush();
extern void set_irqs();
void reboot()
{

	printf("Rebooting\n");
	outportb(0x64, 0xfe);
}

void cmain(unsigned long magic, unsigned long addr)
{
	if(magic!=MULTIBOOT_BOOTLOADER_MAGIC) return;
	disable();
	cls();
	gdt_flush();
	set_irqs();
	enable();
	printf("Done.\n");
	int a=1/0;
	//__asm__("int $0x00");
	for(;;) {
		while(inportb(0x64)&1==0);
		while(inportb(0x64)&1==1) {
			unsigned char c=inportb(0x60);
			if(c==0x93) {
				reboot();
			} else if(c==0xa3) {
				printf("Halting.");
				return;
			} else {
				printf("0x%x ",(int)c);
			}
		}
	}
}
