#include <multiboot.h>
#include <stdio.h>
#include <conio.h>
#include <port.h>
#include <stdint.h>
#include <system.h>
#include <unistd.h>

extern void gdt_flush();
extern void set_irqs();
void reboot()
{

	printf("Rebooting\n");
	outportb(0x64, 0xfe);
}

void irq48_handler(struct regs *r)
{
	printf("SYSCALL: %x%x\n",r->ebx,r->eax);
}

extern void set_pit();
extern unsigned long long pit_counter;
extern void print_info();

void cmain()
{
	disable();
	cls();
	gdt_flush();
	set_irqs();
	enable();
	set_pit();
	install_irq_handler(48-32, irq48_handler);
	print_info();
	puts("Done.\n");
	for(;;) {
		__asm__ __volatile__ ("hlt");
//		usleep(1000);
//		putch('.');
		while(inportb(0x64)&1==1) {
			unsigned char c=inportb(0x60);
			if(c==0x93) { // r
				reboot();
			} else if(c==0xa3) { // h
				printf("Halting:");
				return;
			} else if(c==0xac) { // z
				printf("Dividing by zero:");
				int a=1/0;
			} else if(c==0x94) { // t
				unsigned long long counter=pit_counter;
				printf("%lld\n",counter);
			} else if(c==0x9f) { // s
				__asm__("pushl %ebx; \
					pushl %eax; \
					mov $0xBEEF, %eax; \
					mov $0xDEAD, %ebx; \
					int $48; \
					popl %eax; \
					popl %ebx");
			} else {
				printf("0x%x ",(int)c);
			}
		}
	}
}
