CPPFLAGS=-I../include
objects=boot.o irq.o enable_lmode.o gdt.o
.SILENT: clean $(objects) loader

loader: boot.o irq.o enable_lmode.o gdt.o
	$(LD) --build-id=none -n -z max-page-size=4096 $^ -o $@
	readelf -s loader >symbols
	strip loader
	cp loader ../boot

rebuild: clean loader

clean:
	rm -f loader *.o symbols
