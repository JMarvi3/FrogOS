#include <conio.h>
#include <stdio.h>
void *get_physaddr(void *vaddr)
{
	unsigned long pdidx = (unsigned long)vaddr>>22;
	unsigned long ptidx = ((unsigned long)vaddr>>12) & 0x3FF;

	unsigned long *pd = (unsigned long *)0xFFFFF000;
	if ((pd[pdidx]&1)==0) return 0;
	unsigned long *pt = (unsigned long *)0xFFC00000 + (0x400*pdidx);
	if ((pt[ptidx]&1)==0) return 0;
	return (void *)((pt[ptidx] & ~0xFFF) +
			((unsigned long)vaddr &0xFFF));
}
