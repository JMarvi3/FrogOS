#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
size_t strlen(char *s);
void *memset(void *s, unsigned char c, size_t n);
void *memsetw(void *s, unsigned short c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif
