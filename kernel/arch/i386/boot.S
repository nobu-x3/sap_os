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
    ; equation SEGMENT*16 + offset
    ; 0x00 * 16 + offset, so only offset
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
    or eax, 0x1                     ; this ORs eax thus enabling protected mode and the long jump to load32
    mov cr0, eax
    jmp CODE_SEG:load32             ; actual final step to switch to protected mode

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:               ; CS SHOULD POINT TO THIS
    dw 0xffff           ; Segment limit bits 0-15
    dw 0                ; Base bits 0-15
    db 0                ; Base bits 16-23 
    db 0x9a             ; Access byte
    db 11001111b        ; High 4 bit flags and low 4 bit flags
    db 0                ; Base bits 24-31

; offset 0x10
gdt_data:
    dw 0xffff           ; Segment limit bits 0-15
    dw 0                ; Base bits 0-15
    db 0                ; Base bits 16-23 
    db 0x92             ; Access byte
    db 11001111b        ; High 4 bit flags and low 4 bit flags
    db 0                ; Base bits 24-31

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32: 
    ; load kernel
    ; lba sector 0 os bootloader, 1 is the second sector
    mov eax, 1
    mov ecx, 100            ; 51200 bytes loaded
    mov edi, 0x0100000      ; the address in memory to load the sectors into
    call ata_lba_read
    ; Now we jump to where we loaded our kernel, this executing kernel.asm
    ; CODE_SEG ensures CS becomes the code selector specified in GDT.
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax            ; backup logical block address
    shr eax, 24             ; we'll be sending 8 highest bits of LDA to hard disk controller
    or eax, 0xE0            ; select master drive
    mov dx, 0x1F6
    out dx, al

    ; send total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al

    ; send more bits of the lda
    mov eax, ebx            ; restore backup
    mov dx, 0x1F3
    out dx, al

    ; send next 8 bits
    mov dx, 0x1F4
    mov eax, ebx
    shr eax, 8
    out dx, al

    ; send upper 16 bits
    mov dx, 0x1F5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

.next_sector:
    push ecx                ; save loop counter

.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    ; need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw               ; this performs a repeated word io block input from port specified in dx the number of times specified in ecx. 
                            ; It decrements ecx. Loads the sectors to address stored in edi.
    pop ecx                 ; restore old loop counter
    loop .next_sector
    ret


times 510 -($-$$) db 0
dw 0xAA55
