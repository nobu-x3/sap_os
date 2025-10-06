section .asm

extern int21_handler
extern no_interrupt_handler

global enable_interrupts
global disable_interrupts

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret

global int21h
global idt_load
global no_interrupt

idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8]
    lidt [ebx]
    pop ebp
    ret

int21h:
    pushad
    call int21_handler
    popad
    iret

no_interrupt:
    pushad
    call no_interrupt_handler
    popad
    iret