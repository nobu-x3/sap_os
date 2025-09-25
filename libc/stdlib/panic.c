#include <stdlib.h>
#include <stdio.h>

__attribute__((__noreturn__))
void panic(const char* msg) {
    #if defined(__is_libk)
	// TODO: Add proper kernel panic.
	printf("kernel panic: %s\n", msg);
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("panic: %s\n", msg);
#endif
	while (1) { }
	__builtin_unreachable();
}