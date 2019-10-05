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

void install_irq_handler(int irq, void (*handler)(struct regs *r));
void uninstall_irq_handler(int irq);
void *get_physaddr(void *vaddr);
#endif
