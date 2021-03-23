
	.section .text

	.global p2, p3, p4, enable_longmode
enable_longmode:

	mov $24, %ecx
	mov $gdt_copy, %esi
	mov $gdt, %edi
	rep movsb

	movl $p3, %eax
	orl $3, %eax
	movl %eax, p4

	movl $p2, %eax
	orl $3, %eax
	movl %eax, p3

	movl $p2, %edi
	xor %ecx, %ecx
create_p2:
	movl $0x200000, %eax // 2MB
	mul %ecx
	or $0x83, %eax	// 0x80 = Huge page
	movl %eax, (%edi, %ecx, 8) // %edi + 8 * %ecx
	inc %ecx
	cmpl $512, %ecx
	jne create_p2


	movl $p4, %eax	// Load PML4
	mov %eax, %cr3


	mov %cr4, %eax
	or $1<<5, %eax	// Enable PAE
	mov %eax, %cr4


efer:
	movl $0xC0000080, %ecx
	rdmsr
//	xor %ecx, %ecx
//	div %ecx
	or $1<<8, %eax	// LME = 1
	wrmsr


	mov %cr0, %eax
	or $1<<31, %eax  // PG = 1
	mov %eax, %cr0

	lgdt gdt_ptr
	ret

gdt_ptr:
	.word 0x17
	.4byte gdt, 0

gdt_copy:
	.quad 0
	.quad 1<<43 | 1<<44 | 1<<47 | 1<<53	// CODE, PRESENT, LONGMODE
	.quad 1<<44 | 1<<47 | 1<<53		// DATA, PRESENT, LONGMOE

	.section .bss
.align 0x1000
p4:	.zero 0x1000
p3:	.zero 0x1000
p2:	.zero 0x1000
gdt:	.zero 0x1000

