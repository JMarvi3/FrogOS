#ifndef _CONIO_H
#define _CONIO_H

void cls();
void putch(unsigned char c);
void puts(unsigned char *s);

int kbd_isempty();
unsigned int kbd_dequeue();

#define KBD_SHIFT 0x1
#define KBD_CTRL  0x2
#define KBD_ALT   0x4
#define KBD_INSERT 0x8
#define KEY_PAUSE 0x80
#define KEY_PRTSC 0x81
#define KEY_LEFTCTRL 0x82
#define KEY_LEFTSHFT 0x83
#define KEY_RGHTSHFT 0x84
#define KEY_LEFTALT  0x85
#define KEY_CAPSLOCK 0x86
#define KEY_F1	     0x87
#define KEY_F2	     0x88
#define KEY_F3	     0x89
#define KEY_F4	     0x8A
#define KEY_F5	     0x8B
#define KEY_F6	     0x8C
#define KEY_F7	     0x8D
#define KEY_F8	     0x8E
#define KEY_F9	     0x8F
#define KEY_F10	     0x90
#define KEY_F11	     0x91
#define KEY_F12	     0x92
#define KEY_NUMLOCK  0x93
#define KEY_SCRLLOCK 0x94
#define KEY_HOME     0x95
#define KEY_UP	     0x96
#define KEY_PGUP     0x97
#define KEY_LEFT     0x98
#define KEY_RIGHT    0x99
#define KEY_DOWN     0x9A
#define KEY_PGDOWN   0x9B
#define KEY_DELETE   0x9C
#define KEY_GUI	     0x9D
#define KEY_RGHTGUI  0x9E
#define KEY_APPS     0x9F
#define KEY_END      0xA0

#endif
