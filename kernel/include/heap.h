#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_table {
  HEAP_BLOCK_TABLE_ENTRY *entries;
  size_t total;
};

struct heap {
  struct heap_table *table;
  void *start_address;
};

/**
 * @brief:              Creates a heap at the given memory range [ptr, end)
 *                      using the provided heap table.
 * @param: heap         heap to be created.
 * @param: heap_start   pointer to the start of the heap data.
 * @param: heap_end     pointer to the end of the heap data.
 * @param: table        pointer to the heap table that describes which blocks of
 *                      data are free.
 * @returns:            0 if success, -EINVARG otherwise.
 */
int heap_create(struct heap *heap, void *heap_start, void *heap_end,
                struct heap_table *table);
void *heap_malloc(struct heap *heap, size_t size);
void heap_free(struct heap *heap, void *ptr);

#ifdef __cplusplus
}
#endif
#endif