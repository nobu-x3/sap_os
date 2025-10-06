#include "heap.h"
#include "config.h"
#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static int heap_validate_table(void *heap_start, void *heap_end,
                               struct heap_table *table) {
  int res = 0;
  size_t table_size = (size_t)(heap_end - heap_start);
  size_t total_blocks = table_size / SAP_OS_HEAP_BLOCK_SIZE;
  if (table->total != total_blocks) {
    res = -EINVARG;
    goto out;
  }
out:
  return res;
}

static bool heap_validate_alignment(void *ptr) {
  return ((unsigned int)ptr % SAP_OS_HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap *heap, void *heap_start, void *heap_end,
                struct heap_table *table) {
  int res = 0;
  if (!heap_validate_alignment(heap_start) ||
      !heap_validate_alignment(heap_end)) {
    res = -EINVARG;
    goto out;
  }
  memset(heap, 0, sizeof(struct heap));
  heap->start_address = heap_start;
  heap->table = table;
  res = heap_validate_table(heap_start, heap_end, table);
  if (res < 0)
    goto out;
  size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
  memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);
out:
  return res;
}

static size_t heap_align_value_to_upper(size_t val) {
  if ((val % SAP_OS_HEAP_BLOCK_SIZE) == 0)
    return val;
  val = (val - (val % SAP_OS_HEAP_BLOCK_SIZE));
  val += SAP_OS_HEAP_BLOCK_SIZE;
  return val;
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry) {
  return entry & 0x0f;
}

// search heap for the first contiguous free block sequence that is at least
// total_blocks size and return it's index or -ENOMEM
static int heap_get_start_block(struct heap *heap, uint32_t total_blocks) {
  int seq_ind = -1;
  int free_blocks = 0;
  struct heap_table *table = heap->table;
  for (size_t i = 0; i < table->total; ++i) {
    if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
      seq_ind = -1;
      free_blocks = 0;
      continue;
    }
    if (seq_ind == -1) {
      seq_ind = i;
    }
    ++free_blocks;
    if (free_blocks == total_blocks) {
      break;
    }
  }
  if (seq_ind == -1) {
    seq_ind = -ENOMEM;
    goto out;
  }
out:
  return seq_ind;
}

void *heap_block_to_abs_address(struct heap *heap, int block_id) {
  return heap->start_address + (block_id * SAP_OS_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(struct heap *heap, int start_block_id,
                            int blocks_num) {
  const uint32_t end_block_id = (start_block_id + blocks_num) - 1;
  HEAP_BLOCK_TABLE_ENTRY entry =
      HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
  if (blocks_num > 1)
    entry |= HEAP_BLOCK_HAS_NEXT;
  for (size_t i = start_block_id; i < end_block_id; ++i) {
    heap->table->entries[i] = entry;
    entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    if (i != end_block_id - 1)
      entry |= HEAP_BLOCK_HAS_NEXT;
  }
}

void* heap_malloc_blocks(struct heap *heap, uint32_t total_blocks) {
  void *address = NULL;
  int start_block_id = heap_get_start_block(heap, total_blocks);
  if (start_block_id < 0)
    goto out;
  address = heap_block_to_abs_address(heap, start_block_id);
  heap_mark_blocks_taken(heap, start_block_id, total_blocks);
out:
  return address;
}

void heap_mark_blocks_free(struct heap *heap, uint32_t starting_block_id) {
  struct heap_table *table = heap->table;
  for (size_t i = starting_block_id; i < table->total; ++i) {
    HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
    table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
    if (!(entry & HEAP_BLOCK_HAS_NEXT))
      break;
  }
}

int heap_address_to_block(struct heap *heap, void *address) {
  return (int)((address - heap->start_address) / SAP_OS_HEAP_BLOCK_SIZE);
}

void *heap_malloc(struct heap *heap, size_t size) {
  size_t aligned_size = heap_align_value_to_upper(size);
  uint32_t total_blocks = aligned_size / SAP_OS_HEAP_BLOCK_SIZE;
  return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap *heap, void *ptr) {
  int block_id = heap_address_to_block(heap, ptr);
  heap_mark_blocks_free(heap, block_id);
}