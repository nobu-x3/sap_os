#include "string.h"

int strlen(const char *s) {
  int len = 0;
  while (*s != 0) {
    ++len;
    ++s;
  }
  return len;
}

int strnlen(const char *s, int max) {
  int i = 0;
  for (i = 0; i < max; ++i) {
    if (s[i] == 0)
      break;
  }
  return i;
}

bool isdigit(char c) { return c >= 48 && c <= 57; }

int tonumericdigit(char c) { return c - 48; }
