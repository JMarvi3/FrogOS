#include <stdio.h>
#include <stddef.h>

void dump(void *addr, size_t n)
{
	size_t idx, i;
	for(idx=0;idx<n;idx+=0x10) {
		printf("0x%p: ",idx);
		for(i=0;(i<0x10)&&((idx+i)<n);++i) printf(" %02x",*(unsigned char*)(addr+idx+i));
		printf("\n");
	}
}
