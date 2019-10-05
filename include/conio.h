#ifndef _CONIO_H
#define _CONIO_H

void cls();
void putch(unsigned char c);
void puts(unsigned char *s);

int kbd_isempty();
unsigned int kbd_dequeue();

#endif
