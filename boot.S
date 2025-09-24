ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0:step2

step2:
    cli
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1             ; this ORs eax thus enabling protected mode and the long jump to load32
    mov cr0, eax
    jmp CODE_SEG:load32     ; actual final step to switch to protected mode

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:       ; CS SHOULD POINT TO THIS
    dw 0xffff   ; Segment limit bits 0-15
    dw 0        ; Base bits 0-15
    db 0        ; Base bits 16-23 
    db 0x9a     ; Access byte
    db 11001111b; High 4 bit flags and low 4 bit flags
    db 0        ; Base bits 24-31

; offset 0x10
gdt_data:
    dw 0xffff   ; Segment limit bits 0-15
    dw 0        ; Base bits 0-15
    db 0        ; Base bits 16-23 
    db 0x92     ; Access byte
    db 11001111b; High 4 bit flags and low 4 bit flags
    db 0        ; Base bits 24-31

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x000200000
    mov esp, ebp
    
times 510 -($-$$) db 0
dw 0xAA55
