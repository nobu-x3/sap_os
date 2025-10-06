#include "idt.h"
#include "io.h"
#include "kheap.h"
#include "tty.h"
#include <stdio.h>

void kernel_main() {
  terminal_initialize();
  printf("hello world!\nnext "
         "again\n");
  kheap_init();
  idt_init();
  enable_interrupts();
}