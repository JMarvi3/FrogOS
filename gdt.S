        .globl gdt_flush
gdt_flush:
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
gdt_ptr:
        .short end_gdt-gdt-1
        .long  gdt

gdt:
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

	// 0x18 User CS 
        .short 0xFFFF
        .short 0
        .byte  0
        .byte  0xFA
        .byte  0xCF
        .byte  0
        .short 0xFFFF

	// 0x20 User DS
        .short 0
        .byte  0
        .byte  0xF2
        .byte  0xCF
        .byte  0
end_gdt:
