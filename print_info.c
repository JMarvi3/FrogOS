#include <stdio.h>
#include <multiboot2.h>

extern unsigned long multiboot_info;
extern unsigned long kernel_start, kernel_end;
void print_info()
{
	printf("0x%p - 0x%p\n",&kernel_start,&kernel_end);
	void *mi=&multiboot_info;
	printf("0x%p\n",mi);
	unsigned long idx=8, size=*(unsigned long*)mi;
	while(idx<size) {
		unsigned long tag_type=*(unsigned long *)(mi+idx);
		unsigned long tag_size=*(unsigned long *)(mi+idx+4);
		printf("idx: 0x%lx type: %ld, size: 0x%lx\n",idx,tag_type,tag_size);
		if(tag_type==MULTIBOOT_TAG_TYPE_MMAP) {
		  struct multiboot_tag_mmap *mmap=(struct multiboot_tag_mmap *)(mi+idx);
		  struct multiboot_mmap_entry *mmap_entry;
		  unsigned long i;
		  for(i=16;i<mmap->size;i+=mmap->entry_size) {
		    mmap_entry = (struct multiboot_mmap_entry *)(mi+idx+i);
		    printf("0x%llx 0x%llx %ld\n",
			mmap_entry->addr, mmap_entry->len, mmap_entry->type);
	 	  }
		}
		idx+=((tag_size+7)&~7);
	}
}
