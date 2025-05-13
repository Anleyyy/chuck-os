global loader                   ; the entry symbol for ELF
extern print
extern unicorn



KERNEL_STACK_SIZE equ 4096
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                ; (magic number + checksum + flags should equal 0)
BLACK equ 0x0
WHITE equ 0xF

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE


section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text:                  ; start of the text (code) section



loader:                         ; the loader label (defined as entry point in linker script)
    mov eax, 0xCAFEBABE         ; place the number 0xCAFEBABE in the register eax
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    push byte BLACK
    push byte WHITE
    call unicorn
.loop:
    jmp .loop                   ; loop forever
