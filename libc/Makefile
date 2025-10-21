DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O0 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -Wall
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

ARCHDIR=arch/$(HOSTARCH)

BUILDDIR=../build
$(shell mkdir -p $(BUILDDIR))

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

SRCS := \
    stdio/printf.c \
    stdio/putchar.c \
    stdio/puts.c \
    stdlib/abort.c \
    stdlib/memory.c \
    ssp/ssp.c \
    string/memcmp.c \
    string/memcpy.c \
    string/memmove.c \
    string/memset.c \
    string/strlen.c \

SRCDIRS := $(sort $(dir $(SRCS)))
VPATH := $(patsubst %/,%,$(SRCDIRS))

BINARIES=$(BUILDDIR)/libk.a

OBJS := $(patsubst %.c,$(BUILDDIR)/%.o,$(notdir $(SRCS)))
LIBK_OBJS := $(OBJS:.o=.libk.o)

FREEOBJS := $(OBJS)
HOSTEDOBJS := $(ARCH_HOSTEDOBJS)
ALL_OBJS := $(FREEOBJS) $(HOSTEDOBJS)

$(shell mkdir -p $(BUILDDIR))

.PHONY: all clean install install-headers install-libs

all: $(BINARIES)

$(BINARIES): $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@)

$(BUILDDIR)/%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@)

$(BUILDDIR)/%.libk.o: %.c
	@mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@)

$(BUILDDIR)/%.libk.o: %.S
	@mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$@)

clean:
	@rm -f $(BINARIES) $(BUILDDIR)/*.a
	@rm -f $(OBJS) $(LIBK_OBJS) $(BUILDDIR)*.o $(BUILDDIR)/*/*.o $(BUILDDIR)/*/*/*.o
	@rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) $(BUILDDIR)/*.d $(BUILDDIR)/*/*.d $(BUILDDIR)/*/*/*.d

install: install-headers install-libs

install-headers:
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)

FREEOBJS := $(OBJS)
LIBK_OBJS := $(LIBK_OBJS)
OBJS := $(ALL_OBJS)
