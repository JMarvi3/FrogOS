	.code32
.macro INTERRUPT id
#.globl irq\id
irq\id: cli
	push $0
	push $\id
	jmp irq_stub
.endm

.macro INTERRUPT_WITH_ERROR id
.globl irq\id
irq\id: cli
	push $\id
	jmp irq_stub
.endm

	.section .text
	.global install_irqs

install_irqs:
	xor %eax, %eax
	mov $irqs, %esi
	mov $idt, %edi
loop:
	movl (%esi, %ecx, 4), %ebx
	movl %ebx, (%edi, %ecx, 8)
	movl $0x0008, 2(%edi, %ecx, 8)
	movl $0x8e00, 4(%edi, %ecx, 8)
	movl 2(%esi, %ecx, 4), %ebx
	movl %ebx, 6(%edi, %ecx, 8)
	inc %ecx
	cmpl $32, %ecx
	jnz loop
	lidt idt_ptr
	ret

INTERRUPT 0
INTERRUPT 1
INTERRUPT 2
INTERRUPT 3
INTERRUPT 4
INTERRUPT 5
INTERRUPT 6
INTERRUPT 7
INTERRUPT_WITH_ERROR 8
INTERRUPT 9
INTERRUPT_WITH_ERROR 10
INTERRUPT_WITH_ERROR 11
INTERRUPT_WITH_ERROR 12
INTERRUPT_WITH_ERROR 13
INTERRUPT_WITH_ERROR 14
INTERRUPT 15
INTERRUPT 16
INTERRUPT 17
INTERRUPT 18
INTERRUPT 19
INTERRUPT 20
INTERRUPT 21
INTERRUPT 22
INTERRUPT 23
INTERRUPT 24
INTERRUPT 25
INTERRUPT 26
INTERRUPT 27
INTERRUPT 28
INTERRUPT 29
INTERRUPT 30
INTERRUPT 31

#define VIDEO 0xB8000
irq_stub:
	movl $0x0f4e, VIDEO+0x10
	movl $0x0f6f, VIDEO+0x12
	movl $0x0f2d, VIDEO+0x14
	pop %esi
	pop %edi
	movl $0x0f30, %eax
	addl %esi, %eax
	cmpb $0x39, %al
	jle good
	addl $0x27, %eax
good:
	movl %eax, VIDEO+0x16
stub_loop:
	hlt
	jmp stub_loop

irqs:	.4byte irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9
	.4byte irq10, irq11, irq12, irq13, irq14, irq15, irq16, irq17, irq18, irq19
	.4byte irq20, irq21, irq22, irq23, irq24, irq25, irq26, irq27, irq28, irq29
	.4byte irq30, irq31

	.globl idt, idt_ptr
idt_ptr: .short 2047
	.long idt
	.section .bss
idt:	.zero 2048

