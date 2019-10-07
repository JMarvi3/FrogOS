#include <conio.h>
#include <port.h>
#include <string.h>
#include <system.h>
#include <stdio.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq16();
extern void irq17();
extern void irq18();
extern void irq19();
extern void irq20();
extern void irq21();
extern void irq22();
extern void irq23();
extern void irq24();
extern void irq25();
extern void irq26();
extern void irq27();
extern void irq28();
extern void irq29();
extern void irq30();
extern void irq31();
extern void irq32();
extern void irq33();
extern void irq34();
extern void irq35();
extern void irq36();
extern void irq37();
extern void irq38();
extern void irq39();
extern void irq40();
extern void irq41();
extern void irq42();
extern void irq43();
extern void irq44();
extern void irq45();
extern void irq46();
extern void irq47();
extern void irq48();

unsigned char *exception_messages[] = 
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overlow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void irq9_handler(struct regs *r, void *data) { puts("irq9"); }
irq_handler_t irq9_h={irq9_handler, 0, 0};
void irqa_handler(struct regs *r, void *data) { puts("irqa"); }
irq_handler_t irqa_h={irqa_handler, 0, 0};
void irqb_handler(struct regs *r, void *data) { puts("irqb"); }
irq_handler_t irqb_h={irqb_handler, 0, 0};
irq_handler_t *irq_handlers[] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  &irq9_h, &irqa_h, &irqb_h, 0, 0, 0, 0, 0, 0
};

void install_irq_handler(int irq, irq_handler_t *handler)
{
	disable();
	handler->next=irq_handlers[irq];
	irq_handlers[irq] = handler;
	enable();
}
void uninstall_irq_handler(int irq, irq_handler_t *handler)
{
	disable();
	if(irq_handlers[irq]==handler) {
		irq_handlers[irq]=handler->next;
	} else {
		irq_handler_t *ptr=irq_handlers[irq];
		while(ptr) {
			if(ptr->next==handler) {
				ptr->next=handler->next;
				break;
			}
			ptr=ptr->next;
		}
	}
	enable();
}

void irq_handler(struct regs *r)
{
     if(r->int_no == 14) {
	puts(exception_messages[r->int_no]);
	unsigned long cr2;
	__asm__ __volatile__ ("mov %%cr2, %0":"=r"(cr2));
	printf(" error: %d, addr: 0x%x, eip: 0x%x\n",r->err_code,cr2,r->eip);
        printf("eax: 0x%x\n",r->eax);
     	for(;;) __asm__("hlt");
     } else if(r->int_no < 32) {
	puts(exception_messages[r->int_no]);
//	printf("%s\nException. System Halted!\n",exception_messages[r->int_no]);
     	for(;;) __asm__("hlt");
     } else {
	irq_handler_t *ptr=irq_handlers[r->int_no-32];
	while(ptr) {
		ptr->handler(r,ptr->data);
		ptr=ptr->next;
	}
	if(r->int_no >= 40)
        	outportb(0xA0,0x20);
	outportb(0x20,0x20);
     }
}

typedef struct idt_entry {
	unsigned short base_lo;
	unsigned short sel;
	unsigned char always0;
	unsigned char flags;
	unsigned short base_hi;
} __attribute__((packed)) idt_entry;

extern idt_entry idt[];

void idt_set_gate(unsigned int num, void func(), unsigned short sel, unsigned char flags)
{
	idt[num].base_lo = (unsigned long)func&0xffff;
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
	idt[num].base_hi = ((unsigned long)func>>16)&0xffff;
}

void idt_load();

void set_irqs()
{
memset(idt, 0, 2048);
idt_set_gate(0, irq0, 0x08, 0x8E);
idt_set_gate(1, irq1, 0x08, 0x8E);
idt_set_gate(2, irq2, 0x08, 0x8E);
idt_set_gate(3, irq3, 0x08, 0x8E);
idt_set_gate(4, irq4, 0x08, 0x8E);
idt_set_gate(5, irq5, 0x08, 0x8E);
idt_set_gate(6, irq6, 0x08, 0x8E);
idt_set_gate(7, irq7, 0x08, 0x8E);
idt_set_gate(8, irq8, 0x08, 0x8E);
idt_set_gate(9, irq9, 0x08, 0x8E);
idt_set_gate(10, irq10, 0x08, 0x8E);
idt_set_gate(11, irq11, 0x08, 0x8E);
idt_set_gate(12, irq12, 0x08, 0x8E);
idt_set_gate(13, irq13, 0x08, 0x8E);
idt_set_gate(14, irq14, 0x08, 0x8E);
idt_set_gate(15, irq15, 0x08, 0x8E);
idt_set_gate(16, irq16, 0x08, 0x8E);
idt_set_gate(17, irq17, 0x08, 0x8E);
idt_set_gate(18, irq18, 0x08, 0x8E);
idt_set_gate(19, irq19, 0x08, 0x8E);
idt_set_gate(20, irq20, 0x08, 0x8E);
idt_set_gate(21, irq21, 0x08, 0x8E);
idt_set_gate(22, irq22, 0x08, 0x8E);
idt_set_gate(23, irq23, 0x08, 0x8E);
idt_set_gate(24, irq24, 0x08, 0x8E);
idt_set_gate(25, irq25, 0x08, 0x8E);
idt_set_gate(26, irq26, 0x08, 0x8E);
idt_set_gate(27, irq27, 0x08, 0x8E);
idt_set_gate(28, irq28, 0x08, 0x8E);
idt_set_gate(29, irq29, 0x08, 0x8E);
idt_set_gate(30, irq30, 0x08, 0x8E);
idt_set_gate(31, irq31, 0x08, 0x8E);
idt_set_gate(32, irq32, 0x08, 0x8E);
idt_set_gate(33, irq33, 0x08, 0x8E);
idt_set_gate(34, irq34, 0x08, 0x8E);
idt_set_gate(35, irq35, 0x08, 0x8E);
idt_set_gate(36, irq36, 0x08, 0x8E);
idt_set_gate(37, irq37, 0x08, 0x8E);
idt_set_gate(38, irq38, 0x08, 0x8E);
idt_set_gate(39, irq39, 0x08, 0x8E);
idt_set_gate(40, irq40, 0x08, 0x8E);
idt_set_gate(41, irq41, 0x08, 0x8E);
idt_set_gate(42, irq42, 0x08, 0x8E);
idt_set_gate(43, irq43, 0x08, 0x8E);
idt_set_gate(44, irq44, 0x08, 0x8E);
idt_set_gate(45, irq45, 0x08, 0x8E);
idt_set_gate(46, irq46, 0x08, 0x8E);
idt_set_gate(47, irq47, 0x08, 0x8E);
idt_set_gate(48, irq48, 0x08, 0x8E);
idt_load();
outportb(0x20,0x11);
io_wait();
outportb(0xA0,0x11);
io_wait();
outportb(0x21,0x20);
io_wait();
outportb(0xA1,0x28);
io_wait();
outportb(0x21,0x04);
io_wait();
outportb(0xA1,0x02);
io_wait();
outportb(0x21,0x01);
io_wait();
outportb(0xA1,0x01);
io_wait();
// mask out all interrupts
outportb(0x21,0xff);
io_wait();
outportb(0xA1,0xff);
io_wait();
}
