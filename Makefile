ARCH ?= arm64

# For 64-bit ARM
CC_arm64 = aarch64-linux-gnu-gcc
LD_arm64 = aarch64-linux-gnu-ld
AS_arm64 = aarch64-linux-gnu-as
OBJCOPY_arm64 = aarch64-linux-gnu-objcopy
CFLAGS_arm64 = -ffreestanding \
               -nostdlib \
               -nostartfiles \
               -mcpu=cortex-a53 \
               -march=armv8-a \
               -Wall \
               -Wextra \
               -O2 \
               -I.
ASFLAGS_arm64 = -mcpu=cortex-a53 \
                -march=armv8-a
LDFLAGS_arm64 = -T linker_$(ARCH).ld \
                -nostdlib \
                -static

# For 64-bit x86.
# TODO.

# Assign variables based on the architecture selected
CC = $(CC_$(ARCH))
LD = $(LD_$(ARCH))
AS = $(AS_$(ARCH))
OBJCOPY = $(OBJCOPY_$(ARCH))
CFLAGS = $(CFLAGS_$(ARCH))
ASFLAGS = $(ASFLAGS_$(ARCH))
LDFLAGS = $(LDFLAGS_$(ARCH))

boot.o: boot_$(ARCH)/boot.s
	$(AS) $(ASFLAGS) boot_$(ARCH)/boot.s -o boot.o

uart.o: kernel_$(ARCH)/uart.c
	$(CC) $(CFLAGS) -c kernel_$(ARCH)/uart.c -o uart.o

# custom_printf.o: kernel_$(ARCH)/custom_printf.c
# 	$(CC) $(CFLAGS) -c kernel_$(ARCH)/custom_printf.c -o custom_printf.o

kernel.o: kernel_$(ARCH)/kernel.c
	$(CC) $(CFLAGS) -c kernel_$(ARCH)/kernel.c -o kernel.o

kernel.elf: boot.o uart.o kernel.o
	$(LD) $(LDFLAGS) boot.o uart.o kernel.o -o kernel.elf

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

# This is WIP.

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

# Show build information
info:
  @echo "Building for architecture: $(ARCH)"
  @echo "Using compiler: $(CC)"
  @echo "Using assembler: $(AS)"
  @echo "Using linker: $(LD)"
  @echo "Using flags: $(CFLAGS)"

clean:
	rm -rf boot_$(ARCH)/*.o kernel_$(ARCH)/*.o iso/* *.bin *.iso *.elf *.o *.out
