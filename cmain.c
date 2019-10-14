#include <stdio.h>
#include <conio.h>
#include <port.h>
#include <stdint.h>
#include <system.h>
#include <unistd.h>
#include <memory.h>
#include <net.h>
#include <ipv4.h>
#include <string.h>

extern void gdt_flush();
extern void set_irqs();
void reboot()
{

	printf("Rebooting\n");
	outportb(0x64, 0xfe);
}

void irq48_handler(struct regs *r, void *data)
{
	printf("SYSCALL: %x%x\n",r->ebx,r->eax);
}

irq_handler_t irq48_h={irq48_handler,0,0};

extern void set_pit();
extern unsigned long long pit_counter;
extern void print_info();
extern void pci_init();
extern void print_mem();

uint8_t ip[] = {192,168,201,77};
uint8_t netmask[] = {255,255,255,0};

void cmain()
{
	disable();
	cls();
	gdt_flush();
	init_mem();
	set_irqs();
	enable();
	set_pit();
	install_irq_handler(48-32, &irq48_h);
	init_kbd();
	pci_init();
	in_addr ipv4_addr, ipv4_netmask;
	memcpy(&ipv4_addr.s_addr,ip,4);
	memcpy(&ipv4_netmask.s_addr,netmask,4);
	net_ifconfig(0,ipv4_addr,ipv4_netmask);
//	print_info();
/*
	printf("%p\n",get_physaddr((void *)0xE00B8000));
	printf("%p\n",get_physaddr((void *)(~0xFFF)));
	printf("%p\n",get_physaddr((void *)(0xFFC00000)));
	printf("%p\n",get_physaddr((void *)(0xFFF80000)));
	printf("%p\n",get_physaddr((void *)(0xB8000)));
*/
	puts("Done.\n");
	for(;;) {
		net_process();
		while(!kbd_isempty()) {
			unsigned int c=kbd_dequeue();
			switch(c&0x7f) {
				case 'r':
					reboot();
					break;
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
				case 'm':
					print_mem();
					break;
				case 'n':
					net_print_stats();
					break;
				case 'p':
					print_info();
					break;
				default:
					if (!(c&0x80)) putch(c&0xff);
					break;
			}	
		}
		__asm__ __volatile__ ("hlt");
//		__asm__("int $(32+9)");
	}
}
