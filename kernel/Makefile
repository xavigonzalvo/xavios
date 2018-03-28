GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fleading-underscore -I../include/ -fpermissive
ASPARAMS = --32
LDPARAMS = -melf_i386

.cpp.o:
	gcc $(GCCPARAMS) -c -o $*.o $<

.s.o:
	as $(ASPARAMS) -o $@ $<

OBJS	= kernel.o printk.o vsprintf.o tty_io.o sched.o panic.o console.o serial.o keyboard.o rs_io.o

all: mykernel.o

mykernel.o: $(OBJS)
	ld $(LDPARAMS) -r -o $@ $(OBJS)

clean:
	rm -f *.o
