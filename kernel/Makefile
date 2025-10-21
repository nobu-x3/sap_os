DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

ARCHDIR := arch/$(HOSTARCH)
BUILDDIR := ../build
$(shell mkdir -p $(BUILDDIR))

DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS  := -g -O0 -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostartfiles -Wall -Iinclude
CPPFLAGS:= -D__is_kernel -Iinclude
LDFLAGS := -T $(ARCHDIR)/linker.ld
LDELFFLAGS := -T $(ARCHDIR)/linker-elf.ld

KERNEL_SRCS := kernel.c tty.c idt.c	heap.c kheap.c paging.c
KERNEL_ASMS := kernel.asm io.asm idt.asm paging.asm
KERNEL_OBJS := $(patsubst %.c,$(BUILDDIR)/%.o,$(KERNEL_SRCS)) \
               $(patsubst %.asm,$(BUILDDIR)/%.asm.o,$(KERNEL_ASMS))

.PHONY: all clean install install-headers install-kernel
all: bin

bin: $(KERNEL_OBJS) $(BUILDDIR)/kernel.bin $(BUILDDIR)/boot.bin
	@mkdir -p $(BUILDDIR)
	@rm -f $(BUILDDIR)/sap_os.bin
	dd if=$(BUILDDIR)/boot.bin  of=$(BUILDDIR)/sap_os.bin bs=1 conv=notrunc 2>/dev/null || true
	dd if=$(BUILDDIR)/kernel.bin >> $(BUILDDIR)/sap_os.bin
	dd if=/dev/zero bs=512 count=100 >> $(BUILDDIR)/sap_os.bin

# build kernel.bin: first create a relocatable kernelfull.o, then final link with linker script
$(BUILDDIR)/kernel.bin: $(KERNEL_OBJS) $(BUILDDIR)/libk.a
	@mkdir -p $(BUILDDIR)
	$(LD) -g -relocatable $(KERNEL_OBJS) -o $(BUILDDIR)/kernelfull.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -nostdlib -nodefaultlibs $(LDFLAGS) \
	    -o $@ $(BUILDDIR)/kernelfull.o $(BUILDDIR)/libk.a -lgcc
	$(CC) $(CFLAGS) $(CPPFLAGS) -nostdlib -nodefaultlibs $(LDELFFLAGS) \
	    -o $(BUILDDIR)/kernelfull-elf.o $(BUILDDIR)/kernelfull.o $(BUILDDIR)/libk.a -lgcc

$(BUILDDIR)/boot.bin:
	@mkdir -p $(BUILDDIR)
	$(AS) -f bin -g $(ARCHDIR)/boot.asm -o $@

$(BUILDDIR)/%.o: $(ARCHDIR)/%.c $(BUILDDIR)/libk.a
	@mkdir -p $(BUILDDIR)
	$(CC) -std=gnu99 $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.asm.o: $(ARCHDIR)/%.asm
	@mkdir -p $(BUILDDIR)
	$(AS) -f elf -g $< -o $@

# archive: libk.a is assumed to be built in ../build by libc Makefile
# ensure it exists as a prereq for kernel.bin (so make builds libc first)
$(BUILDDIR)/libk.a:
	@echo "libk.a should be built by libc before kernel. If not, run make in libc/ first."
	@test -f $@ || (echo "ERROR: $@ missing" && false)

install: install-headers install-kernel

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)/kernel
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/kernel/.

install-kernel: bin
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(BUILDDIR)/sap_os.bin $(DESTDIR)$(BOOTDIR)

clean:
	@rm -f $(BUILDDIR)/sap_os.bin $(BUILDDIR)/kernel.bin $(BUILDDIR)/kernelfull.o $(BUILDDIR)/boot.bin
	@rm -f $(KERNEL_OBJS)
