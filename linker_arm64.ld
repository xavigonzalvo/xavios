ENTRY(_start)

SECTIONS
{
    . = 0x40000000;

    .text : {
        *(.text.boot)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        bss_begin = .;
        *(.bss)
        bss_end = .;
    }

    . = ALIGN(8);
    . = . + 0x1000;
    stack_top = .;
}
