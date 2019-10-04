#include <system.h>
#include <port.h>
#include <conio.h>

unsigned long long pit_counter=0;
void pit_handler(struct regs *r) {
	++pit_counter;
//	if(pit_counter%100==0) putch('.');
}

void set_pit()
{
	disable();
	install_irq_handler(0, pit_handler);
	outportb(0x42, 0b00110100);
	outportb(0x40, 0x9c);
	outportb(0x40, 0x2e);
	unsigned char mask = inportb(0x21);
	outportb(0x21, mask & ~1);
	enable();
}

