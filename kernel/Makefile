DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

ARCHDIR := arch/$(HOSTARCH)
BUILDDIR := ../build
BINDIR := ../bin
$(shell mkdir -p $(BUILDDIR))

DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS  := -g -O0 -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostartfiles -Wall
CPPFLAGS:= -D__is_kernel
LDFLAGS := -T $(ARCHDIR)/linker.ld
LDELFFLAGS := -T $(ARCHDIR)/linker-elf.ld

FILES = $(BUILDDIR)/kernel.asm.o $(BUILDDIR)/kernel.o $(BUILDDIR)/idt.asm.o $(BUILDDIR)/idt.o $(BUILDDIR)/memory.o $(BUILDDIR)/io.asm.o $(BUILDDIR)/heap.o $(BUILDDIR)/kheap.o $(BUILDDIR)/paging.o $(BUILDDIR)/paging.asm.o $(BUILDDIR)/disk.o $(BUILDDIR)/string.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: $(BINDIR)/boot.bin $(BINDIR)/kernel.bin
	rm -rf $(BINDIR)/sap_os.bin
	dd if=$(BINDIR)/boot.bin >> $(BINDIR)/sap_os.bin
	dd if=$(BINDIR)/kernel.bin >> $(BINDIR)/sap_os.bin
	dd if=/dev/zero bs=512 count=100 >> $(BINDIR)/sap_os.bin

$(BINDIR)/kernel.bin: $(FILES)
	$(LD) -g -relocatable $(FILES) -o $(BUILDDIR)/kernelfull.o
	$(CC) $(FLAGS) -T ./src/linker.ld -o $(BINDIR)/kernel.bin -ffreestanding -O0 -nostdlib $(BUILDDIR)/kernelfull.o

$(BINDIR)/boot.bin: ./src/boot/boot.asm
	$(AS) -f bin ./src/boot/boot.asm -o $(BINDIR)/boot.bin

$(BUILDDIR)/kernel.asm.o: ./src/kernel.asm
	$(AS) -f elf -g ./src/kernel.asm -o $(BUILDDIR)/kernel.asm.o

$(BUILDDIR)/kernel.o: ./src/kernel.c
	$(CC) $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o $(BUILDDIR)/kernel.o

$(BUILDDIR)/idt.asm.o: ./src/idt/idt.asm
	$(AS) -f elf -g ./src/idt/idt.asm -o $(BUILDDIR)/idt.asm.o

$(BUILDDIR)/idt.o: ./src/idt/idt.c
	$(CC) $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o $(BUILDDIR)/idt.o

$(BUILDDIR)/memory.o: ./src/memory/memory.c
	$(CC) $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o $(BUILDDIR)/memory.o

$(BUILDDIR)/io.asm.o: ./src/io/io.asm
	$(AS) -f elf -g ./src/io/io.asm -o $(BUILDDIR)/io.asm.o

$(BUILDDIR)/heap.o: ./src/memory/heap/heap.c
	$(CC) $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o $(BUILDDIR)/heap.o

$(BUILDDIR)/kheap.o: ./src/memory/heap/kheap.c
	$(CC) $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o $(BUILDDIR)/kheap.o

$(BUILDDIR)/paging.o: ./src/memory/paging/paging.c
	$(CC) $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o $(BUILDDIR)/paging.o

$(BUILDDIR)/paging.asm.o: ./src/memory/paging/paging.asm
	$(AS) -f elf -g ./src/memory/paging/paging.asm -o $(BUILDDIR)/paging.asm.o

$(BUILDDIR)/disk.o: ./src/disk/disk.c
	$(CC) $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o $(BUILDDIR)/disk.o

$(BUILDDIR)/string.o: ./src/string/string.c
	$(CC) $(INCLUDES) -I./src/string $(FLAGS) -std=gnu99 -c ./src/string/string.c -o $(BUILDDIR)/string.o

clean:
	rm -rf $(BINDIR)/boot.bin
	rm -rf $(BINDIR)/kernel.bin
	rm -rf $(BINDIR)/sap_os.bin
	rm -rf ${FILES}
	rm -rf $(BUILDDIR)/kernelfull.o
