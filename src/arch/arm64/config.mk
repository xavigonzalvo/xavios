# For 64-bit ARM
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
AS = aarch64-linux-gnu-as
OBJCOPY = aarch64-linux-gnu-objcopy
CFLAGS = -ffreestanding \
         -nostdlib \
				 -nostartfiles \
				 -mcpu=cortex-a53 \
				 -march=armv8-a \
				 -Wall \
				 -Wextra \
				 -O2 \
				 -I.
ASFLAGS = -mcpu=cortex-a53 \
					-march=armv8-a