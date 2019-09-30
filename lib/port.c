#include <port.h>

unsigned char inportb(unsigned short port)
{
  unsigned char r;
  __asm__ __volatile__ ("inb %1, %0": "=a" (r) : "dN" (port));
  return r;
}

void outportb(unsigned short port, unsigned char data)
{
  __asm__ __volatile__ ("outb %1, %0": : "dN" (port), "a" (data));
}
