#ifndef _STDIO_H
#define _STDIO_H

char *sprintf(char *str, char *fmt, ...);
void kprintf(char *fmt, ...);

#define printf printf_
int printf_(const char* format, ...);

#define sprintf sprintf_
int sprintf_(char* buffer, const char* format, ...);

#endif
