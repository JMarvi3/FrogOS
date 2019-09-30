#include <multiboot.h>
#include <stdio.h>
#include <conio.h>

extern unsigned long _start;
extern unsigned long _end;
extern unsigned long stack;
void cmain(unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mi;
	cls();
	if(magic!=MULTIBOOT_BOOTLOADER_MAGIC) return;
	printf("0x%lx - 0x%lx\n",_start,_end);
	printf("stack: 0x%lx  multiboot_info: 0x%lx\n",stack,addr);
	mi=(multiboot_info_t *)addr;
	printf("flags: 0x%x\n",mi->flags);
	if(mi->flags & MULTIBOOT_INFO_CMDLINE) 
	  printf("cmdline: %s\n",(char *)(mi->cmdline));
	if(mi->flags & MULTIBOOT_INFO_MEMORY) 
	  printf("mem: 0x%x, 0x%x\n",mi->mem_lower,mi->mem_upper);
	if(mi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
	  printf("bootloader: %s\n",mi->boot_loader_name);
	if(mi->flags & MULTIBOOT_INFO_MEM_MAP)
	{
	  unsigned long index=0;
	  while(index<mi->mmap_length) {
	    multiboot_memory_map_t *mmap=(multiboot_memory_map_t *)(mi->mmap_addr+index);
	      if(mmap->type==1)
	    printf("mmap: %d 0x%llx 0x%llx (%lld) %d\n",mmap->size, mmap->addr, mmap->len, mmap->len, mmap->type);
	    index += mmap->size+4;
 	  }
	}
}
