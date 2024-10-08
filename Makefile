include config.mk

ARCH ?= arm64

-include src/arch/$(ARCH)/config.mk

CFLAGS += -Isrc/include -Isrc/arch/$(ARCH)

# Add architecture-specific defines
ifeq ($(ARCH),arm64)
    CFLAGS += -D__aarch64__
else ifeq ($(ARCH),x86_64)
    CFLAGS += -D__x86_64__
else
    $(error Unsupported architecture: $(ARCH))
endif

# Paths
SRC_KERNEL := src/kernel
SRC_ARCH := src/arch/$(ARCH)
KERNEL_SRCS := $(wildcard $(SRC_KERNEL)/*.c)
ARCH_SRCS := $(wildcard $(SRC_ARCH)/*.c)

KERNEL_OBJS := $(patsubst $(SRC_KERNEL)/%.c, $(SRC_KERNEL)/%.o, $(wildcard $(SRC_KERNEL)/*.c))
ARCH_OBJS := $(patsubst $(SRC_ARCH)/%.c, $(SRC_ARCH)/%.o, $(wildcard $(SRC_ARCH)/*.c))

OBJS := $(KERNEL_OBJS) $(ARCH_OBJS) src/arch/$(ARCH)/boot.o

# For each .c file, generate a .d file to track dependencies
DEPFILES := $(KERNEL_SRCS:.c=.d) $(ARCH_SRCS:.c=.d)

all: kernel.bin

src/arch/$(ARCH)/boot.o: src/arch/$(ARCH)/boot.s
	$(AS) $(ASFLAGS) -o $@ $<

# src/arch/$(ARCH)/exception_vectors.o: src/arch/$(ARCH)/exception_vectors.s
# 	$(AS) $(ASFLAGS) -o $@ $<

# Manually track the linking order for specific files
OBJS_ORDERED := src/arch/$(ARCH)/boot.o src/arch/$(ARCH)/uart.o src/kernel/printk.o src/kernel/kernel.o

# Final linking step
kernel.elf: $(OBJS_ORDERED) $(filter-out $(OBJS_ORDERED), $(OBJS))
	$(LD) $(LDFLAGS) -o $@ $^

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

# Rule to compile .c to .o and generate .d dependency files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files (if they exist)
-include $(DEPFILES)

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

clean:
	rm -f $(OBJS) $(DEPFILES) src/arch/$(ARCH)/*.o kernel.elf kernel.bin src/kernel/*.o src/kernel/*.d
