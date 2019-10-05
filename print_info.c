#include <multiboot.h>
#include <stdio.h>
extern int gdt_ptr, end_gdt;
extern unsigned long multiboot_info;
void print_info()
{
	unsigned long addr=multiboot_info;
	printf(" multiboot_info: 0x%lx\n",addr);
        // http://stanislavs.org/helppc/bios_data_area.html
        multiboot_info_t *mi=(multiboot_info_t *)addr;
        printf("flags: 0x%x\n",mi->flags);
        if(mi->flags & MULTIBOOT_INFO_MEMORY)
          printf("mem: 0x%x, 0x%x\n",mi->mem_lower,mi->mem_upper);
	printf("%s\n",mi->boot_loader_name);
        if(mi->flags & MULTIBOOT_INFO_MEM_MAP)
        {
          unsigned long long total_mem=0;
          unsigned long index=0;
          while(index<mi->mmap_length) {
            multiboot_memory_map_t *mmap=(multiboot_memory_map_t *)(mi->mmap_addr+index);
	    if(mmap->type==1)
            printf("mmap: %d 0x%llx 0x%llx (%lld) %d\n",mmap->size, mmap->addr, mmap->len, mmap->len, mmap->type);
            if(mmap->type==1) total_mem+=mmap->len;
            index += mmap->size+4;
          }
          printf("%lld\n",total_mem);
        }
	if(mi->flags & MULTIBOOT_INFO_ELF_SHDR) {
	  multiboot_elf_section_header_table_t *elf_sec=&(mi->u.elf_sec);
	  printf("elf_section: num: %ld, size: 0x%lx\n   addr:0x%lx shndx:0x%lx\n",
		elf_sec->num, elf_sec->size, elf_sec->addr, elf_sec->shndx);
	}

}
