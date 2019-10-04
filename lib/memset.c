#include <string.h>

void *memset(void *s, int c, size_t n)
{
	unsigned char *str, *end=s+n;
	for(str=s; str<end; ++str) *str=c;
	return s;
}

void *memsetw(void *s, int c, size_t n)
{
	unsigned short *str, *end=((unsigned short *)s+n);
	for(str=s; str<end; ++str) *str=c;
	return s;
}
