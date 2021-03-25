CPPFLAGS=-I../include
CFLAGS=-fno-pic -O3 -g -fno-builtin -fno-stack-protector -Wall -Wextra -Werror
ASFLAGS=-g
objects=boot.o irq.o enable_lmode.o gdt.o serial.o
.SILENT: clean $(objects) loader

loader: boot.o irq.o enable_lmode.o gdt.o serial.o # ssfn.o fonts/font.o
	$(LD) -g --build-id=none -n -z max-page-size=4096 $^ -o $@
	readelf -s loader >symbols
#	strip loader
	cp loader ../boot

rebuild: clean loader

clean:
	rm -f loader *.o symbols
