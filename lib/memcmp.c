#include <string.h>
#include <stdint.h>

int memcmp(void *s1, void *s2, size_t n)
{
	register uint8_t ret;
	__asm__("cld; cmp %%ecx,%%ecx; repe cmpsb; setz %0":"=r"(ret):
		"c"(n),"S"(s1),"D"(s2));
	return ret;

}
