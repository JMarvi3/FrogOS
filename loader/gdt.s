	.code32
        .globl load_gdt, gdt
	.section .text
load_gdt:

	mov $(gdt_copy_end-gdt_copy), %ecx
	mov $gdt_copy, %esi
	mov $gdt, %edi
	rep movsb

        lgdt gdt_ptr
        mov $0x10, %eax
        mov %eax, %ds
        mov %eax, %es
        mov %eax, %fs
        mov %eax, %gs
        mov %eax, %ss
        ljmp $0x08,$gdt_flush2
gdt_flush2:
        ret

	.globl gdt_ptr, end_gdt
	.align 16
gdt_ptr:
        .short gdt_copy_end-gdt_copy-1
        .long  gdt

gdt_copy:
        .quad  0

	// 0x08 Kernel CS
        .short 0xFFFF
        .short 0
        .byte  0
        .byte  0x9A
        .byte  0xCF
        .byte  0

	// 0x10 Kernel DS
        .short 0xFFFF
        .short 0
        .byte  0
        .byte  0x92
        .byte  0xCF
        .byte  0
gdt_copy_end:

	.section .bss
gdt:	.zero 0x1000
