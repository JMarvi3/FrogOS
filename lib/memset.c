#include <string.h>

void *memset(void *s, unsigned char c, size_t n)
{
	__asm__ ("cld; rep stosb"::"c"(n),"a"(c),"D"(s));
        return s;
}

void *memsetw(void *s, unsigned short c, size_t n)
{
	__asm__ ("cld; rep stosw"::"c"(n),"a"(c),"D"(s));
        return s;
}

