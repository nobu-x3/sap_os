ORG 0
BITS 16

start:
    cli
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    sti

    mov ax, 0x7c0
    mov word [ss:0x02], ax ; the logic here is that addr = inter_index * sizeof(addr) and this is the offset
    mov ax, div_zero_print
    mov word [ss:0x00], ax

    mov ax, 0
    div ax
    jmp $

div_zero_print:
    mov si, div_zero_msg
    call print
    iret

print:
    mov bx, 0
    .loop:
        lodsb
        cmp al, 0
        je .done
        call print_char
        jmp .loop
    .done:
        ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

div_zero_msg: db "Divide by zero error!", 0
times 510 -($-$$) db 0
dw 0xAA55
