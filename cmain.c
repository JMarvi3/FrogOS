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
	init_kbd();
	print_info();
	printf("%p\n",get_physaddr((void *)0xE00B8000));
	printf("%p\n",get_physaddr((void *)(~0xFFF)));
	printf("%p\n",get_physaddr((void *)(0xFFC00000)));
	printf("%p\n",get_physaddr((void *)(0xFFF80000)));
	printf("%p\n",get_physaddr((void *)(0xB8000)));
	puts("Done.\n");
	for(;;) {
		while(!kbd_isempty()) {
			unsigned int c=kbd_dequeue();
			switch(c&0x7f) {
				case KEY_PAUSE:
					puts("Pause\n");
					break;
				case KEY_PRTSC:
					puts("Print Screen\n");
					break;
				case 'h':
					puts("Halting:");
					return;
					break;
				case 'z':
					puts("Divide by zero:");
					int a=1/0;
					break;
				case 't':
					printf("%lld\n",pit_counter);
					break;
				case 's':
					__asm__("pushl %ebx; \
					pushl %eax; \
					mov $0xBEEF, %eax; \
					mov $0xDEAD, %ebx; \
					int $48; \
					popl %eax; \
					popl %ebx");
					break;
				case 'r':
					reboot();
					break;
				default:
					if (!(c&0x80)) putch(c&0xff);
					break;
			}	
		}
		__asm__ __volatile__ ("hlt");
	}
}
