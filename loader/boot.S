#define ASM_FILE 1
#include <multiboot.h>
#define VIDEO 0xB8000


	.section .text
	.code32
multiboot_header:
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_MEMORY_INFO
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_MEMORY_INFO)
	.long 0,0,0,0,0
	.long 0,0,0,0


	.global start
start:

	cmpl $MULTIBOOT_BOOTLOADER_MAGIC, %eax
	jnz exit
	movl $stack_top, %esp


	call load_gdt
	call install_irqs
	call enable_longmode

/*
	movl %ebx, %esi
	movl $multiboot_info, %edi
	movl $(128/4), %ecx
	rep movsd

	testl $(1<<6), (%ebx)
	jz no_mmap
	movl 44(%ebx), %ecx
	mov %ecx, mmap_len
	movl 48(%ebx), %esi
	movl $mmap_info, %edi
	rep movsb

no_mmap:
	testl $(1<<3), (%ebx)
	jz no_mods
	movl 20(%ebx), %ecx
	cmpl $0, %ecx
	je no_mods
	movl %ecx, mods_count
	shl $2, %ecx
	movl 24(%ebx), %esi
	movl $mods_info, %edi
	rep movsd

no_mods:
	testl $(1<<2), (%ebx)
	jz exit
	mov 16(%ebx), %esi
	mov $cmd_line, %edi
cmd_line_loop:
	movb (%esi), %al
	inc %esi
	movb %al, (%edi)
	inc %edi
	cmpb $0, %al
	jne cmd_line_loop

	pushl %ebx
	call test
*/
	ljmp $0x08,$long

	.code64
long:
	mov $output, %esi
	mov $VIDEO, %edi
	mov $0x0f00, %eax
loop1:
	movb (%esi), %al
	cmpb $0, %al
	jz exit
	mov %eax, (%edi)
	inc %esi
	add $2, %edi
	jmp loop1
exit:
loop:
	hlt
	jmp loop


output:
	.asciz "FrogOS 64"
	.globl mmap_len, mods_count, multiboot_info, mods_info, mmap_info
	.globl cmd_line, test_var

	.section .bss
	.align 0x1000
mmap_len:
	.word 0
mods_count:
	.word 0
test_var:
	.word 0
	.align 0x100
multiboot_info:
	.zero 0x100
mods_info:
	.zero 0x100
mmap_info:
	.zero 0x200
cmd_line:
	.zero 0x100
stack:
	.zero 0x1000
stack_top:
	.word 0

