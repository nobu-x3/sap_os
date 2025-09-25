#include <kernel/tty.h>
#include <stdio.h>

void kernel_main() {
    terminal_initialize();
    printf("hello world!\nnext line");
}