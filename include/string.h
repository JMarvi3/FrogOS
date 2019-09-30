#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
size_t strlen(char *s);
void *memset(void *s, int c, size_t n);
void *memsetw(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif
