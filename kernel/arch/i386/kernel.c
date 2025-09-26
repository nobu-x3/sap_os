#include <kernel/tty.h>
#include <kernel/idt.h>
#include <stdio.h>

void test(int one, int zero) {
    printf("%d", one / zero);
}

void kernel_main() {
    terminal_initialize();
    printf("hello world!\nnext line\n\n\n\n\n\nnext\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nagain");
    idt_init();
    test(1, 0);
}