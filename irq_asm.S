
.macro INTERRUPT id
	.globl irq\id
irq\id :
	cli
	push $0
	push $\id
	jmp irq_stub
.endm
.macro INTERRUPT_WITH_ERROR id
	.globl irq\id
irq\id :
	cli
	push $\id
	jmp irq_stub
.endm

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
INTERRUPT 32
INTERRUPT 33
INTERRUPT 34
INTERRUPT 35
INTERRUPT 36
INTERRUPT 37
INTERRUPT 38
INTERRUPT 39
INTERRUPT 40
INTERRUPT 41
INTERRUPT 42
INTERRUPT 43
INTERRUPT 44
INTERRUPT 45
INTERRUPT 46
INTERRUPT 47
INTERRUPT 48

	.global idt
	.comm idt, 2048

	.global idt_ptr
idt_ptr:
	.short 2047
	.long  idt

	.extern irq_handler

irq_stub:
	pushal
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %esp, %eax
	push %eax
	mov $irq_handler, %eax
	call *%eax
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
	add $8, %esp
	iret

	.globl disable, enable
disable:
	cli
	ret

enable:
	sti
	ret
	
	.globl idt_load
idt_load:
	lidt idt_ptr
	ret
