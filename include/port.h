#ifndef _PORT_H
#define _PORT_H

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

#endif
