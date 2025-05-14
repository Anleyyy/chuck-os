global loader                   ; the entry symbol for ELF
extern print
extern unicorn



KERNEL_STACK_SIZE equ 4096
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                ; (magic number + checksum + flags should equal 0)
BLACK equ 0x0
WHITE equ 0xE

section .bss
align 4096
global kernel_page_directory
kernel_page_directory:
    resb 4096       ; 1024 * 4

page_table_0:
    resb 4096       ; 1024 * 4

page_table_1:
    resb 4096

kernel_stack:
    resb KERNEL_STACK_SIZE


section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text:                  ; start of the text (code) section



loader:                         ; the loader label (defined as entry point in linker script)
    cli

     ; Initialiser le Page Table 0
    mov eax, 0
    mov edi, page_table_0
    mov ecx, 1024
.fill_pt:
    mov dword [edi], eax           ; Adresse physique
    or dword [edi], 0x3            ; Present + Read/Write
    add eax, 0x1000                ; prochaine page (4KB)
    add edi, 4
    loop .fill_pt

    ; Initialiser le Page Directory
    mov eax, page_table_0
    or eax, 0x3                    ; Present + RW
    mov [kernel_page_directory], eax     ; Entrée 0 → page_table_0

    mov eax, page_table_1
    or eax, 0x3                    ; Present + RW
    mov [kernel_page_directory + 4], eax     ; Entrée 1 → page_table_1

    ; Charger CR3 avec adresse du Page Directory
    mov eax, kernel_page_directory
    mov cr3, eax

    ; Activer le Paging (bit PG dans CR0)
    mov eax, cr0
    or eax, 0x80000000             ; Set PG (bit 31)
    mov cr0, eax

    ; Activer le stack
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    ; Affiche un caractère
    push byte BLACK
    push byte WHITE
    call unicorn


.halt:
    hlt
    jmp .halt
