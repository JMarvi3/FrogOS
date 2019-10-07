#include <system.h>
#include <port.h>
#include <conio.h>
#include <memory.h>

unsigned long long pit_counter=0;
void pit_handler(struct regs *r, void *data) {
	++pit_counter;
}

void set_pit()
{
	irq_handler_t *handler=alloc(sizeof(irq_handler_t),1);
	if(handler) {
		handler->handler=pit_handler;
		handler->next=0;
		handler->data=0;
		install_irq_handler(0,handler);
		disable();
		outportb(0x42, 0b00110100);
		io_wait();
		outportb(0x40, 0x9c);
		io_wait();
		outportb(0x40, 0x2e);
		io_wait();
		unsigned char mask = inportb(0x21);
		io_wait();
		outportb(0x21, mask & ~1);
		io_wait();
		enable();
	}
}

