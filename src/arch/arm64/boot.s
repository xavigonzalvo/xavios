.global _start
.section .text
.align 4  // 16-byte alignment for .text section (instructions)

_start:
    mrs x0, CurrentEL          /* Read the Current Exception Level */
    lsr x0, x0, #2             /* Extract the EL bits (bits [3:2]) */
    bl log_el

    /* Check and switch to EL1 if necessary */
    cmp x0, #3
    b.eq switch_to_el1

    cmp x0, #2
    b.eq switch_to_el1

    /* Already in EL1, continue */
    b el1_main

switch_to_el1:
    msr daifset, #0xf          /* Disable interrupts */
    mov x0, #(1 << 2)          /* Lower EL to EL1, enable SP_EL1 */
    msr spsr_el3, x0
    adr x0, el1_main           /* Set return address to el1_main */
    msr elr_el3, x0
    eret                       /* Return to EL1 */

el1_main:
    // Initialize stack pointer
    ldr x0, =stack_top
    mov sp, x0

    /* Clear BSS */
    ldr x1, =bss_begin
    ldr x2, =bss_end
zero_bss:
    cmp x1, x2
    b.ge bss_cleared
    str xzr, [x1], #8
    b zero_bss

bss_cleared:
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
