.global _start
.section .text
_start:
    // Initialize stack pointer
    ldr x0, =stack_top
    mov sp, x0
    bl kernel_main
hang:
    b hang

.section .bss
stack:
    .space 4096

.section .data
stack_top:
    .quad stack + 4096
