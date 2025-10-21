#include "stdlib.h"

#if defined(__is_libk)
#include <stdio.h>
#include <string.h>

#include <kernel/config.h>
#include <kernel/heap.h>

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init() {
  const int total_table_entries =
      SAP_OS_HEAP_SIZE_BYTES / SAP_OS_HEAP_BLOCK_SIZE;
  kernel_heap_table.entries =
      (HEAP_BLOCK_TABLE_ENTRY *)(SAP_OS_HEAP_TABLE_ADDRESS);
  kernel_heap_table.total = total_table_entries;
  void *end = (void *)(SAP_OS_HEAP_ADDRESS + SAP_OS_HEAP_SIZE_BYTES);
  int res = heap_create(&kernel_heap, (void *)(SAP_OS_HEAP_ADDRESS), end,
                        &kernel_heap_table);
  if (res < 0) {
    printf("Failed to create heap.\n");
    abort();
  }
}

void *malloc(size_t size) { return heap_malloc(&kernel_heap, size); }

void free(void *mem) { heap_free(&kernel_heap, mem); }

void *calloc(size_t size) {
  void *mem = malloc(size);
  if (!mem)
    return NULL;
  memset(mem, 0x00, size);
  return mem;
}
#endif
