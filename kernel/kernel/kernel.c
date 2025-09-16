#include <stdint.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	// uint64_t kstack_top = (uint64_t)(uintptr_t)stack_top;
	// uint64_t kdfstack_top = (uint64_t)(uintptr_t)df_stack_top;
	// gdt_install(kstack_top, kdfstack_top);
	terminal_initialize();
	gdt_init();
	printf("Hello, kernel World!\n");
}
