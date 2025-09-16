    .text
    .code64
    .global gdt_load_and_reload_segments
/* void gdt_load_and_reload_segments(uint64_t gdtr_ptr, uint16_t code_sel, uint16_t data_sel, uint16_t tss_sel) */
gdt_load_and_reload_segments:
    /* SysV: rdi = gdtr_ptr, rsi = code_selector, rdx = data_selector, rcx = tss_selector */

    /* Load GDTR (gdtr struct at RDI contains 2-byte limit and 8-byte base) */
    movq    %rdi, %rax
    lidt    (%rax)

    /* Load DS/ES/SS from data_selector (in rdx) */
    movw    %dx, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %ss

    /* Far return trick to reload CS: push selector then RIP, then lretq */
    pushq   %rsi            /* push code selector (low 16 bits used) */
    lea     1f(%rip), %rax
    pushq   %rax
    lretq
1:
    /* Load TSS selector (16-bit) via ltrw */
    movw    %cx, %ax
    ltrw    %ax

    ret
