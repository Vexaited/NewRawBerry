OUTPUT_FORMAT("elf64-x86-64")
ENTRY(main)

SECTIONS
{
    . = 0x400000;
    
    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata) 
    }

    .data : {
        *(.data)  
    }

    .bss : {
        *(.bss)
    }

    .debug : {
        *(.debug.*)
    }

    /DISCARD/ : {
        *(.comment)
        *(.note)
    }
}