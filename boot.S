/* boot.S - bootstrap the kernel */
/* Copyright (C) 1999, 2001, 2010  Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define ASM_FILE        1
#include <multiboot.h>

#define STACK_SIZE                      0x0200

#define MULTIBOOT_HEADER_FLAGS MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
        
        .section .start

        .globl  start
start:
        jmp     multiboot_entry

        /* Align 32 bits boundary. */
        .align  4
        
        /* Multiboot header. */
multiboot_header:
        /* magic */
        .long   MULTIBOOT_HEADER_MAGIC
        /* flags */
        .long   MULTIBOOT_HEADER_FLAGS
        /* checksum */
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	.globl multiboot_info
multiboot_info:
	.long 0

multiboot_entry:
        /* Initialize the stack pointer. */
        /*movl    $(temp_stack + STACK_SIZE), %esp */

	movl	%ebx, multiboot_info
	cmpl	$MULTIBOOT_BOOTLOADER_MAGIC, %eax
	jne	exit

	.set offset, 0xE0000000
	movl $(page_table0 - offset + 0x3), page_directory - offset
	movl $(page_table0 - offset + 0x3), page_directory - offset + 0x380*4
	movl $(page_table1 - offset + 0x3), page_directory - offset + 0x3ff*4
	
	movl	$(page_table0 - offset), %edi
	movl	$1024, %ecx
	movl	$3, %esi
1:	movl	%esi, %edx
	movl	%edx, (%edi)
	addl	$4096, %esi
	addl	$4, %edi
	dec	%ecx
	jnz	1b


	movl	$(page_table0 - offset + 0x3), page_table1 - offset
	movl	$(page_table0 - offset + 0x3), page_table1 - offset + 0x380*4
	movl	$(page_directory - offset + 0x3), page_table1 - offset + 0x3ff*4
	
	movl	$(page_directory - offset + 0x3), %ecx
	movl	%ecx, %cr3	

	movl	%cr0, %ecx
	orl	$0x80000000, %ecx
	movl	%ecx, %cr0

	lea	2f, %ecx
	jmp 	*%ecx

2:
	movl 	$0, page_directory
	movl	$0, page_table1
	invlpg  0x0
	/* movl	%cr3, %ecx
	movl	%ecx, %cr3 */
	
	movl	$(stack + 0x10000), %esp

        /* Reset EFLAGS. */
        pushl   $0
        popf

        call    cmain

exit:
	cli
loop:   hlt
        jmp     loop

temp_stack:
	.zero STACK_SIZE


	.bss
	.globl page_directory, page_table0, page_table1
	.align 4096
page_directory:
	.zero 0x1000
page_table0:
	.zero 0x1000
page_table1:
	.zero 0x1000
stack:
	.zero 0x10000

