[BITS 32]
global _start
extern kernel_main

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

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ;; ===== full PIC remap (master 0x20, slave 0x28) and unmask keyboard IRQ1 =====
    cli                     ; disable interrupts while we reprogram PICs

    mov al, 0x11            ; ICW1: start init (edge triggered, cascade, expect ICW4)
    out 0x20, al            ; send to master PIC command port (0x20)
    out 0xA0, al            ; send to slave PIC command port  (0xA0)

    mov al, 0x20            ; ICW2: master vector offset = 0x20
    out 0x21, al            ; master PIC data port
    mov al, 0x28            ; ICW2: slave  vector offset = 0x28
    out 0xA1, al            ; slave PIC data port

    mov al, 0x04            ; ICW3: tell master there is a slave at IRQ2 (bitmask 0000 0100)
    out 0x21, al
    mov al, 0x02            ; ICW3: tell slave its cascade identity (2)
    out 0xA1, al

    mov al, 0x01            ; ICW4: 8086/88 mode
    out 0x21, al
    out 0xA1, al

    ; restore/adjust masks: clear keyboard mask (IRQ1) on master PIC
    in  al, 0x21            ; read current master mask from port 0x21
    and al, 0xFD            ; clear bit 1 (mask &= ~(1<<1)) -> unmask IRQ1
    out 0x21, al

    ; optionally ensure slave mask is unchanged (or adjust if needed)
    in  al, 0xA1
    out 0xA1, al
    ; ================================================================================
    
    call kernel_main

    jmp $

times 512-($-$$) db 0