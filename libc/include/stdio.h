#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF (-1)

int printf(const char *__restrict, ...);
int putchar(int);
int puts(const char *);

#endif

#ifdef __cplusplus
}
#endif