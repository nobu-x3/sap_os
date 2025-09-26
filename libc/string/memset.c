#include <string.h>

void* memset(void* bufptr, int value, size_t size) {
	char* buf = (char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (char) value;
	return bufptr;
}
