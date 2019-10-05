#include <conio.h>

unsigned char scancodes[0x59] =
  {  0, 27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
   'q','w','e','r','t','y','u','i','o','p','[',']','\n',KEY_LEFTCTRL,'a','s',
   'd','f','g','h','j','k','l',';', 39,'`',KEY_LEFTSHFT,'|','z','x','c','v',
   'b','n','m',',','.','/',KEY_RGHTSHFT,'*',KEY_LEFTALT,' ',KEY_CAPSLOCK,KEY_F1,
	KEY_F2,KEY_F3,KEY_F4,KEY_F5,
   KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_NUMLOCK,KEY_SCRLLOCK,'7','8','9','-',
	'4','5','6','+','1',
   '2','3','0','.',0,0,0,KEY_F11,KEY_F12 };
unsigned char shift_scancodes[0x59] =
  {  0, 27,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
   'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',KEY_LEFTCTRL,'A','S',
   'D','F','G','H','J','K','L',':','"','~',KEY_LEFTSHFT,'\\','Z','X','C','V',
   'B','N','M','<','>','?',KEY_RGHTSHFT,'*',KEY_LEFTALT,' ',KEY_CAPSLOCK,KEY_F1,
	KEY_F2,KEY_F3,KEY_F4,KEY_F5,
   KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_NUMLOCK,KEY_SCRLLOCK,'7','8','9','-',
	'4','5','6','+','1',
   '2','3','0','.',0,0,0,KEY_F11,KEY_F12 };
