#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/io.h>
#include <stdio.h>

void kernel_main() {
    terminal_initialize();
    printf("hello world!\nnext line\n\n\n\n\n\nnext\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nagain");
    idt_init();
    outb(0x60, 0xff);
}