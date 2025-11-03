#ifndef STRING_H
#define STRING_H
#include <stdbool.h>

int strlen(const char *s);
int strnlen(const char *s, int max);
bool isdigit(char c);
int tonumericdigit(char c);

#endif
