GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fleading-underscore -Iinclude/
ASPARAMS = -f elf32
LDPARAMS = -melf_i386

boot/boot.o: boot/boot.s
	nasm $(ASPARAMS) -o $@ $<

boot/head.o: boot/head.s
	as --32 -o $@ $<

kernel/mykernel.o:
	(cd kernel; make)

lib/lib.o:
	(cd lib; make)

LIBS = kernel/mykernel.o lib/lib.o

mykernel.bin: linker.ld boot/boot.o boot/head.o $(LIBS)
	ld $(LDPARAMS) -T $< -o $@ boot/boot.o boot/head.o $(LIBS)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

clean:
	rm -rf boot/*.o kernel/*.o lib/*.o *.bin *.iso
