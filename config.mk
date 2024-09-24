LDFLAGS = -T src/arch/$(ARCH)/linker.ld \
					-nostdlib \
					-static