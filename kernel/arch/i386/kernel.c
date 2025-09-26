#include "idt.h"
#include "io.h"
#include "tty.h"
#include <stdio.h>

void kernel_main() {
  terminal_initialize();
  printf("hello world!\nnext "
         "again\n");
  idt_init();
}