.global _start
.section .text
.align 4  // 16-byte alignment for .text section (instructions)
_start:
    // Initialize stack pointer
    ldr x0, =stack_top
    mov sp, x0

    // Initialize UART for logging
    bl uart_init
    bl kernel_main
hang:
    b hang

.section .bss
.align 4  // Ensure the stack is aligned on a 16-byte boundary
stack:
    .space 4096    // Reserve 4 KB for the stack

.section .data
.align 4  // Ensure stack_top is aligned to 16 bytes
stack_top:
    .quad stack + 4096
