ENTRY(_start)

SECTIONS
{
    . = 0x40000000;  /* Load kernel at 1G */

    .text : ALIGN(8) {
        KEEP(*(.text.boot))  /* Ensure the boot code is not discarded */
        *(.text)
    }

    .rodata : ALIGN(8) {
        *(.rodata)
    }

    .data : ALIGN(8) {
        *(.data)
    }

    .bss : ALIGN(8) {
        bss_begin = .;
        *(.bss)
        bss_end = .;
    }

    /* Define the stack */
    . = ALIGN(16);
    .stack (NOLOAD) : {
        stack_bottom = .;
        . += 0x1000;  /* Reserve 4 KB for stack */
        stack_top = .;
    }

    PROVIDE(debug_stack_top = stack_top);

    /DISCARD/ : {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
}