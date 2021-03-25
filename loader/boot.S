#define ASM_FILE 1
#include <multiboot.h>
#define VIDEO 0xB8000


	.section .text
	.code32
#define MULTIBOOT_FLAGS MULTIBOOT_MEMORY_INFO + MULTIBOOT_VIDEO_MODE
multiboot_header:
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_FLAGS
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAGS)
	.long 0,0,0,0,0
//	.long 0,1024,768,32
	.long 0,640,480,16

	.global _start
_start:

	cmpl $MULTIBOOT_BOOTLOADER_MAGIC, %eax
	jnz exit
	mov $stack_top, %esp
	mov %ebx, multiboot_info


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
pre_long:
	ljmp $0x08,$long

	.code64
long:
/*
	mov $output, %esi
	mov $VIDEO, %edi
	mov $0x0f00, %eax
loop1:
	movb (%esi), %al
	cmpb $0, %al
	jz done_output
	mov %eax, (%edi)
	inc %esi
	add $2, %edi
	jmp loop1
done_output:
*/

	call serial_init
	test %eax, %eax
	jne no_serial

	rdtsc
	call out_word

	mov multiboot_info, %eax
	mov (%eax), %eax
	call out_word

	xor %ecx, %ecx
	mov multiboot_info, %edi
loop3:
	mov 88(%edi, %ecx, 4), %eax
	call out_word
	inc %ecx
	cmp $6, %ecx
	jnz loop3

no_serial:
/*
	call init_ssfn
	mov $output, %rdi
	call puts_ssfn
*/

draw:
	mov $multiboot_info, %rax
	mov (%rax), %rax
	mov 88(%rax), %rdi
	xor %rcx, %rcx
	mov 96(%rax), %ecx
	xor %rbx, %rbx
	mov 104(%rax), %ebx
	imul %rbx, %rcx
	mov $0x50000, %rcx
	mov 0x0f, %al
	rep stosb
	mov $0x05, %edx


exit:
loop:
//	hlt
	jmp loop

out_word:
	mov $0x3f8, %dx
	push %rax
	mov $0x04, %al
	out %al, (%dx)
	pop %rax
	out %al, (%dx)
	shr $8, %eax
	out %al, (%dx)
	shr $8, %eax
	out %al, (%dx)
	shr $8, %eax
	out %al, (%dx)
	ret

	

output:
	.asciz "FrogOS 64bit Loader"


framebuffer:
	.quad 0
framebuffer_pitch:
	.quad 0	

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
	.quad 0
magic:
	.quad 0
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

