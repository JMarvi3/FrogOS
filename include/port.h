#ifndef _PORT_H
#define _PORT_H

static inline unsigned char inportb(unsigned short port)
{
  unsigned char r;
  __asm__ __volatile__ ("inb %1, %0": "=a" (r) : "dN" (port));
  return r;
}

static inline void outportb(unsigned short port, unsigned char data)
{
  __asm__ __volatile__ ("outb %1, %0": : "dN" (port), "a" (data));
}

static inline void outportl(unsigned short port, unsigned long data)
{
  __asm__ __volatile__ ("outl %1, %0": : "dN" (port), "a" (data));
}

static inline unsigned long inportl(unsigned short port)
{
  unsigned long r;
  __asm__ __volatile__ ("inl %1, %0": "=a" (r) : "dN" (port));
  return r;
}

static inline void outportw(unsigned short port, unsigned short data)
{
  __asm__ __volatile__ ("outw %1, %0": : "dN" (port), "a" (data));
}

static inline unsigned short inportw(unsigned short port)
{
  unsigned short r;
  __asm__ __volatile__ ("inw %1, %0": "=a" (r) : "dN" (port));
  return r;
}

static inline void io_wait(void)
{
    __asm__ __volatile__ ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif
