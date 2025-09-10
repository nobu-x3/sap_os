.set ALIGN,     1<<0 /* alight loaded modules on page boundaries*/
.set MEMINFO,   1<<1 /*provide mem map*/
.set FLAGS,     ALIGN|MEMINFO
.set MAGIC,     0x1BADB002 /* to let bootloader find the header*/
.set CHECKSUM,  -(MAGIC+FLAGS)

/* Header that marks program as kernel, docs in multiboot std.
    Bootloader searches first 8 KiB of the kernel file aligned 32.*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* We have to provide our own esp. This allocates 16384 bytes for the stack
    by creating a symbol at stack_bottom, allocating said no of bytes and then symbol at stack_top.
    Grows downwards on x86. It's in it's own section, so marking it no-bits makes the kernel
    file smaller (not uninit stack.) Must be 16-bye aligned or UB on x86.*/
.section .bss
.align  16
stack_bottom:
    .skip 16384
stack_top:

/* Linker script specifies _start as entry point to kernel, so
    bootloader will jump here one kernel is loaded. */
.section .text
.global _start
.type _start,   @function
_start:
    // setup stack
    mov $stack_top, %esp
    /* Good place to initialize processor state. The processor
        is not fully initialized yet - floating point insts and
        inst set extensions are not initialized yet. GDT and paging
        should be enabled here. C++ global ctors and exceptions
        require runtime support as well. */

    /* Entering high-level kernel here. ABI requires 16-byte aligned stack at this time.
        16-byte aligned originally, ret ptr is 4 bytes (multiple of 16), so we're good. */
    call kernel_main

    /* If there's nothing to do, put the computer is infinite loop by
    1. Disable interrupts with cli. (technically turned off by bootloader but we might set it in kernel_main)
    2. Wait for next interrupt with hlt
    3. Jmp to hlt if ever wakes up cuz non-maskable interrupt or system mgmt mode.*/
    cli
1:  hlt
    jmp 1b

.size _start, . - _start // Useful for debugging/stacktrace
