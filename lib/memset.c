#include <string.h>

void *memset(void *s, int c, size_t n)
{
	unsigned char *str = (unsigned char *)s;
	while(n--) str[n]=c;
	return s;
}

void *memsetw(void *s, int c, size_t n)
{
	unsigned short *str = (unsigned short *)s;
	while(n--) str[n]=c;
	return s;
}
