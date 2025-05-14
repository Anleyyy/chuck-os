ENTRY(loader)

SECTIONS {
    /* L'adresse physique initiale (GRUB charge à 1MB) */
    . = 1M;

    /* L'en-tête Multiboot doit être tout au début du fichier */
    .multiboot ALIGN(4K) : {
        KEEP(*(.multiboot))
    }

    /* On saute à l'adresse virtuelle désirée pour le kernel */

    .text ALIGN(4K) : {
        *(.text*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    .data ALIGN(4K) : {
        *(.data)
    }

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }
}


