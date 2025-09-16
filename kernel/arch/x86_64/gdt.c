#include <kernel/gdt.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

/* GDT table (will be filled at runtime) */
static uint64_t gdt_table[GDT_ENTRIES] __attribute__((aligned(0x100)));

/* GDT descriptor for LGDT */
struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint64_t base;
};

/* TSS structure for x86_64 minimal fields */
struct __attribute__((packed)) tss_t {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
};

static struct tss_t tss __attribute__((aligned(16)));

/* helper: create a normal 8-byte GDT entry */
static inline uint64_t gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    uint64_t entry = 0;
    entry  = (limit & 0xFFFFULL);
    entry |= ((uint64_t)(base & 0xFFFFULL)) << 16;
    entry |= ((uint64_t)((base >> 16) & 0xFFULL)) << 32;
    entry |= ((uint64_t)access) << 40;
    entry |= ((uint64_t)((limit >> 16) & 0xFULL)) << 48;
    entry |= ((uint64_t)(flags & 0xFULL)) << 52;
    entry |= ((uint64_t)((base >> 24) & 0xFFULL)) << 56;
    return entry;
}

/* helper: create the 16-byte TSS descriptor stored as two 8-byte slots */
static inline void gdt_set_tss_descriptor(int idx, uint64_t base, uint32_t limit) {
    /* first 8 bytes */
    uint64_t low = 0;
    low  = (limit & 0xFFFFULL);
    low |= (base & 0xFFFFFFULL) << 16;
    low |= (uint64_t)0x89 << 40; /* present=1, DPL=00, Type=1001 (available 64-bit TSS) */
    low |= ((uint64_t)((limit >> 16) & 0xFULL)) << 48;
    /* flags: set G=1 (granularity=4KiB) and zero for others (AVL=0, L=0, D/B=0) */
    low |= ((uint64_t)(0x8 & 0xF)) << 52; /* 1000b -> G=1 */
    low |= ((base >> 24) & 0xFFULL) << 56;

    /* second 8 bytes: high base */
    uint64_t high = 0;
    high = (base >> 32) & 0xFFFFFFFFULL;
    /* the rest reserved = 0 */
    gdt_table[idx]     = low;
    gdt_table[idx + 1] = high;
}

/* assembly helper to actually load GDT, reload segment regs and LTR */
extern void gdt_load_and_reload_segments(uint64_t gdtr_base, uint16_t code_selector, uint16_t data_selector, uint16_t tss_selector);

void gdt_install(uint64_t kernel_stack_top, uint64_t df_stack_top) {
    memset(gdt_table, 0, sizeof(gdt_table));
    /* Null descriptor at 0 left as zero */

    /* Access and flags:
       Access: 0x9A -> present=1, DPL=0, S=1 (code/data), Type=0b1010 (executable, readable)
       Flags:  0xA  -> G=1 (4KiB gran), D/B=0, L=1 (64-bit), AVL=0  -> 1010b = 0xA
    */
    gdt_table[1] = gdt_entry(0, 0, 0x9A, 0xA); /* kernel code64 */
    gdt_table[2] = gdt_entry(0, 0, 0x92, 0xC); /* kernel data (0x92 access: data, writable). flags 0xC -> G=1, D/B=1, L=0 */
    /* TODO: add user-space segments*/

    /* Setup TSS: zero it and program RSP0 and IST1 (we use ist1 for double fault) */
    memset(&tss, 0, sizeof(tss));
    tss.rsp0 = kernel_stack_top;   /* kernel stack used on ring3->ring0 */
    tss.ist1 = df_stack_top;       /* IST1 -> double fault stack pointer (high address) */

    /* TSS descriptor occupies entries 5 and 6 (index 5) */
    uint64_t tss_base = (uint64_t)(uintptr_t)&tss;
    uint32_t tss_limit = sizeof(tss) - 1;
    gdt_set_tss_descriptor(5, tss_base, tss_limit);

    /* Load GDTR and reload segments via assembly stub */
    struct gdtr gdtr;
    gdtr.limit = sizeof(gdt_table) - 1;
    gdtr.base  = (uint64_t)(uintptr_t)&gdt_table;

    /* code selector = index 1 -> selector value = 1<<3 = 0x08
       data selector = index 2 -> 2<<3 = 0x10
       tss selector  = index 5 -> 5<<3 = 0x28
    */
    gdt_load_and_reload_segments((uint64_t)(uintptr_t)&gdtr, 0x08, 0x10, 0x28);
}
