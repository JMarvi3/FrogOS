#ifndef _SYSTEM_H
#define _SYSTEM_H

#define enable() __asm__ __volatile__ ("sti");
#define disable() __asm__ __volatile__ ("cli");

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

typedef struct irq_handler_struct {
	void (*handler)(struct regs *r, void *data);
	void *data;
	struct irq_handler_struct *next;
} irq_handler_t;

void install_irq_handler(int irq, irq_handler_t *handler);
void uninstall_irq_handler(int irq, irq_handler_t *handler);
void *get_physaddr(void *vaddr);
void init_kbd();
#endif
