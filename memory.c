#include <stdint.h>
#include <memory.h>
#include <stdio.h>

memhdr *high_mem;

uint32_t round_up(uint32_t addr, uint16_t size)
{
	return (addr+(size-1))&~(size-1);
}

void print_mem()
{
	memhdr *mem=high_mem;
	uint32_t total_mem=0;
	while(mem) {
		printf("mem: %p %p %p\n",mem,mem->size,mem->next);
		total_mem+=mem->size;
		mem = mem->next;
        }
	printf("mem: total: %p\n",total_mem);
}

uint32_t kernel_end;
void init_mem()
{
	high_mem=(memhdr *)round_up((uint32_t)&kernel_end,16);
	high_mem->size=0xC0C00000-(uint32_t)high_mem;
	high_mem->next=0;
}

void *alloc(uint32_t size, uint16_t align)
{
	size=round_up(size,16);
	memhdr *mem=high_mem;
	while(mem) {
		if(mem->size >= size) {
			uint32_t lost=round_up((uint32_t)mem,align)-(uint32_t)mem;
			if((mem->size-lost)>size) {
				if(lost>=16) {
				   memhdr *new_mem=(memhdr *)((uint32_t)mem+lost+size);
				   new_mem->next=mem->next;
				   new_mem->size=mem->size-(lost+size);
				   mem->size=lost;
				   mem->next=new_mem;
				   return (void *)((uint32_t)mem+lost);
				} else {
				   memhdr *new_mem=(memhdr *)((uint32_t)mem+size);
				   new_mem->next=mem->next;
				   new_mem->size=mem->size-size;
				   void *ret=(void *)high_mem;
				   high_mem=new_mem;
				   return ret;
				}
			}
		}
		mem=mem->next;
	};
	return 0;
}

