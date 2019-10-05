#ifndef _PORT_H
#define _PORT_H

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

static inline void io_wait(void)
{
    __asm__ __volatile__ ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif
