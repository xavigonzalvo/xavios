	MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
	MEMINFO  equ  1 << 1            ; provide memory map
	FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
	MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
	CHECKSUM equ -(MAGIC + FLAGS)

	section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

	section .bss
	align 16
stack_bottom:
	resb 16384 		; 16 KiB
stack_top:

	section .text
	global start:function (start.end - start)

start:
	mov esp, stack_top
  push ebx ; Push multiboot pointer onto the stack
	push eax ; Push magic number onto the stack
	extern kernel_main
	call kernel_main

	cli

.hang:	hlt
	jmp .hang
.end:
