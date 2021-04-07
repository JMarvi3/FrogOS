#.set GB, 16
GB = 16

	.code32
	.section .text

	.global p2, p3, p4, enable_longmode
enable_longmode:
	movl $p3, %eax
	orl $3, %eax
	movl %eax, p4

create_p3:
	mov $0x1000, %ebx
	mov $p2, %eax
	orl $3, %eax
	mov $p3, %edi
	xor %ecx, %ecx
p3_loop:
	mov %eax, (%edi, %ecx, 8)
	add %ebx, %eax
	inc %ecx
	cmp $GB, %ecx
	jnz p3_loop

create_p2:
	movl $p2, %edi
	mov $0x83, %eax
	mov $1<<21, %ebx // 2MB
	xor %ecx, %ecx
p2_loop:
	movl %eax, (%edi, %ecx, 8) // %edi + 8 * %ecx
	add %ebx, %eax
	inc %ecx
	cmpl $512*GB, %ecx
	jne p2_loop


	mov %cr0, %eax
	btr $31, %eax	// PG = 0
	mov %eax, %cr0


	movl $p4, %eax	// Load PML4
	mov %eax, %cr3


	mov %cr4, %eax
	bts $5, %eax	// Enable PAR
	mov %eax, %cr4


efer:
	movl $0xC0000080, %ecx
	rdmsr
	bts $8, %eax	// LME = 1
	wrmsr


	mov %cr0, %eax
	bts $31, %eax  // PG = 1
	mov %eax, %cr0

	mov $(gdt64_copy_end-gdt64_copy), %ecx
	mov $gdt64_copy, %esi
	mov $gdt, %edi
	rep movsb
	lgdt gdt64_ptr
	
	ret

gdt64_ptr:
	.word 0x17
	.4byte gdt, 0

gdt64_copy:
	.quad 0
	.quad 1<<43 | 1<<44 | 1<<47 | 1<<53	// CODE, PRESENT, LONGMODE
	.quad 1<<44 | 1<<47 | 1<<53		// DATA, PRESENT, LONGMOE
gdt64_copy_end:

	.section .bss
.align 0x1000
p4:	.zero 0x1000
p3:	.zero GB*0x1000
p2:	.zero 0x1000

