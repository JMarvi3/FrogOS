#include <stdio.h>
extern int _start;
extern int _end;
extern int stack;
void print_info(unsigned long addr)
{
	printf("0x%p - 0x%p\n",&_start,&_end);
        printf("stack: 0x%p  multiboot_info: 0x%lx\n",&stack,addr);
        unsigned short *ebda=(unsigned short *)0x40E;
        printf("EBDA: 0x%x\n",(*ebda)<<4);
        // http://stanislavs.org/helppc/bios_data_area.html
        printf("Video mode: %d, cols: %d\n",(unsigned int)*(unsigned char *)0x449,
                (unsigned int)*(unsigned short *)0x44a);
        multiboot_info_t *mi=(multiboot_info_t *)addr;
        printf("flags: 0x%x\n",mi->flags);
        if(mi->flags & MULTIBOOT_INFO_CMDLINE)
          printf("cmdline: %s\n",(char *)(mi->cmdline));
        if(mi->flags & MULTIBOOT_INFO_MEMORY)
          printf("mem: 0x%x, 0x%x\n",mi->mem_lower,mi->mem_upper);
        if(mi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
          printf("bootloader: %s\n",mi->boot_loader_name);
        if(mi->flags & MULTIBOOT_INFO_MEM_MAP)
        {
          unsigned long long total_mem=0;
          unsigned long index=0;
          while(index<mi->mmap_length) {
            multiboot_memory_map_t *mmap=(multiboot_memory_map_t *)(mi->mmap_addr+index);
            printf("mmap: %d 0x%llx 0x%llx (%lld) %d\n",mmap->size, mmap->addr, mmap->len, mmap->len, mmap->type);
            if(mmap->type==1) total_mem+=mmap->len;
            index += mmap->size+4;
          }
          printf("%lld\n",total_mem);
        }

}
