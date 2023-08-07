# GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
GCCPARAMS = -m32 -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-stack-protector
ASPARAMS = -f elf32
LDPARAMS = -melf_i386

boot/boot.o: boot/boot.s
	nasm $(ASPARAMS) -o $@ $<

kernel/kernel.o: kernel/kernel.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

kernel/terminal.o: kernel/terminal.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

kernel/sprintf.o: kernel/sprintf.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

kernel/printf.o: kernel/printf.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

mykernel.bin: linker.ld boot/boot.o kernel/terminal.o kernel/printf.o kernel/sprintf.o kernel/kernel.o
	ld $(LDPARAMS) -T $< -o $@ boot/boot.o kernel/terminal.o kernel/printf.o kernel/sprintf.o kernel/kernel.o

intermediate.iso: mykernel.bin
	# Create directory for kernel
	mkdir -p intermediate/boot

	# Copy the kernel binary
	cp mykernel.bin intermediate/boot/mykernel.bin

	# Create the ISO without GRUB
	mkisofs -o intermediate.iso intermediate

	# Cleanup the temporary directory
	rm -rf intermediate

mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	# cp intermediate.iso iso/boot/intermediate.iso
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
	rm -rf boot/*.o kernel/*.o iso/* *.bin *.iso
