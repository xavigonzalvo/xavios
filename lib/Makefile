GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fleading-underscore -I../include/
ASPARAMS = -f elf32
LDPARAMS = -melf_i386

.cpp.o: ../include/string.h
	gcc $(GCCPARAMS) -c -o $*.o $<

OBJS = string.o ctype.o errno.o

lib.o: $(OBJS)
	ld $(LDPARAMS) -r -o $@ $(OBJS)

clean:
	rm -f *.o

all: lib.o

