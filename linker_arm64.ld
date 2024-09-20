ENTRY(_start)

SECTIONS
{
    . = 0x40000000;

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

    . = ALIGN(16);
    . = . + 0x1000;
    stack_top = .;

    /DISCARD/ : {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
}