#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

void init_mem();
void *alloc(uint32_t size, uint16_t align);

typedef struct memhdr {
	uint32_t size;
	struct memhdr *next;
} memhdr;


#endif
