#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void kheap_init();
void *kmalloc(size_t size);
void kfree(void *mem);
void *kcalloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif
