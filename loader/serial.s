	.text
	.global serial_init
	.code32
port=0x3f8
.macro out_p value, offset
	mov $\value, %al
	mov $port+\offset, %dx
	outb %al, (%dx)
.endm
.macro in_p offset
	mov $port+\offset, %dx
	inb (%dx)
.endm

serial_init:
	out_p 0x00, 1
	out_p 0x80, 3
	out_p 0x03, 0
	out_p 0x00, 1
	out_p 0x03, 3
	out_p 0xc7, 2
	out_p 0x0b, 4
	out_p 0x1e, 4
	out_p 0xae, 0

serial_test:
	in_p 0
	cmpb $0xae, %al
	je serial_good
	mov $1, %eax
	ret

serial_good:
	out_p 0x0f, 4
	xor %eax, %eax
	ret	
