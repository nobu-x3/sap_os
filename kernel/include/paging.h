#ifndef PAGING_H
#define PAGING_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef SAP_OS_64

// clang-format off
#define PAGING_CACHE_DISABLED               0b00010000
#define PAGING_WRITE_THROUGH                0b00001000
#define PAGING_ACCESS_FROM_ALL              0b00000100
#define PAGING_IS_WRITABLE                  0b00000010
#define PAGING_IS_PRESENT                   0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE      1024
#define PAGING_PAGE_SIZE                    4096
// clang-format on

void enable_paging();

struct paging_4gb_chunk {
  uint32_t *directory_entry;
};

struct paging_4gb_chunk *paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t *directory);

uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk);
#endif

#ifdef __cplusplus
}
#endif

#endif
