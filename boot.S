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
        
        .text

        .globl  start, _start
start:
_start:
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
        movl    $(temp_stack + STACK_SIZE), %esp

        /* Reset EFLAGS. */
        pushl   $0
        popf

	movl	%ebx, multiboot_info
	cmpl	$MULTIBOOT_BOOTLOADER_MAGIC, %eax
	jne	exit

        /* Now enter the C main function... */
        call    cmain

exit:
	cli
loop:   hlt
        jmp     loop

	.comm  temp_stack, STACK_SIZE
