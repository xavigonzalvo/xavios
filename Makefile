# GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
GCCPARAMS = -m32 -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector
ASPARAMS = -f elf32
LDPARAMS = -melf_i386

boot/boot.o: boot/boot.s
	nasm $(ASPARAMS) -o $@ $<

kernel/kernel.o: kernel/kernel.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

kernel/sprintf.o: kernel/sprintf.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

mykernel.bin: linker.ld boot/boot.o kernel/sprintf.o kernel/kernel.o
	ld $(LDPARAMS) -T $< -o $@ boot/boot.o kernel/sprintf.o kernel/kernel.o

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
	rm -rf boot/*.o kernel/*.o *.bin *.iso
