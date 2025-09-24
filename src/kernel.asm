[BITS 32]
global _start

CODE_SEG equ 0x08       ; code_seg entry in GDT
DATA_SEG equ 0x10       ; data_seg entry in GDT

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    jmp $