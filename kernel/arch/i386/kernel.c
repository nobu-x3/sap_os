#include "idt.h"
#include "io.h"
#include "kheap.h"
#include "paging.h"
#include "tty.h"
#include <stdio.h>

static struct paging_4gb_chunk *kernel_chunk = NULL;

void kernel_main() {
  terminal_initialize();
  printf("hello world!\nnext "
         "again\n");
  kheap_init();
  idt_init();
  kernel_chunk = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT |
                                PAGING_ACCESS_FROM_ALL);
  uint32_t *kernel_directory = paging_4gb_chunk_get_directory(kernel_chunk);
  paging_switch(kernel_directory);
  enable_paging();
  printf("allocated\n");
  enable_interrupts();
}
