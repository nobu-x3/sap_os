DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
# INCLUDES= -I./src
ARCHDIR=arch/$(HOSTARCH)
BUILDDIR=../build/kernel
BINDIR=../bin

FLAGS=-g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinclude

OBJS=$(BUILDDIR)/kernel.o \
$(BUILDDIR)/kernel.asm.o

BINOBJS=$(BUILDDIR)/boot.bin \
$(BUILDDIR)/kernel.bin

.PHONY: all bin clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: bin

bin: $(OBJS) $(BUILDDIR)/kernel.bin $(BUILDDIR)/boot.bin
	rm -rf $(BUILDDIR)/sap_os.bin
	dd if=$(BUILDDIR)/boot.bin >> $(BUILDDIR)/sap_os.bin
	dd if=$(BUILDDIR)/kernel.bin >> $(BUILDDIR)/sap_os.bin
	dd if=/dev/zero bs=512 count=100 >> $(BUILDDIR)/sap_os.bin

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(FLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(FLAGS)

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: bin
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(BUILDDIR)/sap_os.bin $(DESTDIR)$(BOOTDIR)

$(BUILDDIR)/kernel.bin: $(OBJS)
	echo "kernel.bin"
	$(LD) -g -relocatable $(OBJS) -o $(BUILDDIR)/kernelfull.o
	$(CC) $(FLAGS) -T $(ARCHDIR)/linker.ld -o $(BUILDDIR)/kernel.bin $(BUILDDIR)/kernelfull.o

$(BUILDDIR)/boot.bin:
	$(AS) -f bin $(ARCHDIR)/boot.asm -o $(BUILDDIR)/boot.bin

$(BUILDDIR)/kernel.asm.o: $(ARCHDIR)/kernel.asm
	$(AS) -f elf -g $(ARCHDIR)/kernel.asm -o $(BUILDDIR)/kernel.asm.o

$(BUILDDIR)/kernel.o : $(ARCHDIR)/kernel.c
	$(CC) $(INCLUDES) $(FLAGS) -std=gnu99 -c $(ARCHDIR)/kernel.c -o $(BUILDDIR)/kernel.o

clean:
	rm -rf $(BINDIR)/kernel.bin
	rm -rf $(BUILDDIR)/