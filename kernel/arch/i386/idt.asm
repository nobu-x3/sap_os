section .asm

extern int21_handler
extern no_interrupt_handler

global int21h
global idt_load
global no_interrupt

idt_load:
    cli
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8]
    lidt [ebx]
    pop ebp
    sti
    ret

int21h:
    cli
    pushad
    call int21_handler
    popad
    sti
    iret

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret