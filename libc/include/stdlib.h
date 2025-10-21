#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__)) void abort(void);

__attribute__((__noreturn__)) void panic(const char *msg);

#if defined(__is_libk) || defined(__is_kernel)
void kheap_init();
#endif

void *malloc(size_t size);
void free(void *mem);
void *calloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif
