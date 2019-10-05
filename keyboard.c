#include <system.h>
#include <port.h>
#include <stdio.h>
#include <conio.h>
#include <kbd_scancode.h>
#define KBD_QUEUE_SIZE 128
unsigned int kbd_buffer[KBD_QUEUE_SIZE];
int kbd_size=0, kbd_head=0, kbd_tail=0;

int kbd_isempty()
{
	return (kbd_size==0);	
}

void kbd_enqueue(unsigned int c)
{
	if(kbd_size == KBD_QUEUE_SIZE) return;
	kbd_buffer[kbd_tail++] = c;
	if(kbd_tail == KBD_QUEUE_SIZE) kbd_tail = 0;
	++kbd_size;
}

unsigned int kbd_dequeue()
{
	if(kbd_size == 0) return 0;
	disable();
	unsigned int c = kbd_buffer[kbd_head++];
	if(kbd_head == KBD_QUEUE_SIZE) kbd_head=0;
	--kbd_size;
	enable();
	return c;
}

unsigned int kbd_state=0;
unsigned int kbd_shiftstate=0;
#define KBD_PRESS 0x80;
#define ENQUEUE(c) kbd_enqueue(kbd_shiftstate<<8|c)

void decode(unsigned char c) 
{
	if((kbd_state==0&&c==0xE0)||(kbd_state==0&&c==0xE1)||
		(kbd_state==0xE1&&c==0x1D)||(kbd_state==0x1D&&c==0x45)||
		(kbd_state==0x2E1&&c==0x9D)) {
		 	kbd_state=c;
	} else if (kbd_state==0x45 && c==0xE1) {
		 kbd_state=0x2E1;
	} else if (kbd_state==0x9D && c==0xC5)  {
		ENQUEUE(KEY_PAUSE); kbd_state=0;
	} else if (kbd_state==0xE02A && c==0xE0) {
		kbd_state=0x2AE0;
	} else if (kbd_state==0x2AE0 && c==0x37) {
		ENQUEUE(KEY_PRTSC); kbd_state=0;
	} else if (kbd_state==0xE0 && c==0x2A) {
		kbd_state=0xE02A;
	} else if (kbd_state==0xE0) {
		switch(c) {
		  case 0x1D: //right control pressed
			kbd_shiftstate|=KBD_CTRL; break;
		  case 0x38: //right alt pressed
			kbd_shiftstate|=KBD_ALT; break;
		  case 0x47:
			ENQUEUE(KEY_HOME); break;
		  case 0x48:
			ENQUEUE(KEY_UP); break;
		  case 0x49:
			ENQUEUE(KEY_PGUP); break;
		  case 0x4B:
			ENQUEUE(KEY_LEFT); break;
		  case 0x4D:
			ENQUEUE(KEY_RIGHT); break;
		  case 0x4F:
			ENQUEUE(KEY_END); break;
		  case 0x50:
			ENQUEUE(KEY_DOWN); break;
		  case 0x51:
			ENQUEUE(KEY_PGDOWN); break;
		  case 0x52:
			kbd_shiftstate^=KBD_INSERT; break;
		  case 0x53:
			ENQUEUE(KEY_DELETE); break;
		  case 0x5B:
			ENQUEUE(KEY_GUI); break;
		  case 0x5C:
			ENQUEUE(KEY_RGHTGUI); break;
		  case 0x5D:
			ENQUEUE(KEY_APPS); break;
		  case 0x9D: //right control released
			kbd_shiftstate&=~KBD_CTRL; break;
		  case 0xB8: //right alt released
			kbd_shiftstate&=~KBD_ALT; break;
		}
		kbd_state=0;
	} else if (kbd_state==0) {
		if(!(c&0x80)) { // make
		  unsigned char decoded;
		  if(kbd_shiftstate&KBD_SHIFT) decoded=shift_scancodes[c];
		  else decoded=scancodes[c];
		  switch(decoded) {
			case KEY_LEFTCTRL:
				kbd_shiftstate|=KBD_CTRL;
				break;
			case KEY_CAPSLOCK:
				kbd_shiftstate^=KBD_SHIFT;
				break;
			case KEY_LEFTSHFT:
			case KEY_RGHTSHFT:
				kbd_shiftstate|=KBD_SHIFT;
				break;
			case KEY_LEFTALT:
				kbd_shiftstate|=KBD_ALT;
				break;
			case 0:
				break;
			default:
				ENQUEUE(decoded);
				break;
		  }
		} else { // break
		  switch(scancodes[c&0x7F]) {
			case KEY_LEFTCTRL:
				kbd_shiftstate&=~KBD_CTRL;
				break;
			case KEY_LEFTSHFT:
			case KEY_RGHTSHFT:
				kbd_shiftstate&=~KBD_SHIFT;
				break;
			case KEY_LEFTALT:
				kbd_shiftstate&=~KBD_ALT;
				break;
		  }
		}
		kbd_state=0;
	} else { // ERROR
		kbd_state=0;
		decode(c);
	}
}

void kbd_handler(struct regs *r)
{
	decode(inportb(0x60));
}



void init_kbd()
{
	disable();
	install_irq_handler(1, kbd_handler);
	unsigned char mask = inportb(0x21);
	outportb(0x21, mask & ~2);
	enable();
}
