#include "core.h"
#include "memory.h"

struct dma_queue_entry {
    void *field_00;
    void *field_04;
    u16 field_08;
    u16 field_0A;
};

struct dma_queue {
    struct dma_queue_entry *entries;
    s32 count;
};

extern struct dma_queue gUnknown_03001290;
#define DMA3 (*(struct dma_regs *)REG_ADDR_DMA3SAD)
#define QUEUE_COUNT (((volatile struct dma_queue *)&gUnknown_03001290)->count)
/* Allocated capacity of gUnknown_03001290.entries. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

extern s32 sub_800695C(void *arg0);
extern s32 sub_80068CC(void *arg0);
extern s32 sub_8006864(void *arg0);
extern s32 sub_8006820(void *arg0);
extern s32 sub_80067EC(void *arg0);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);

/* The register pins below (and in several functions further down) match
 * the ROM's own register allocation exactly - required for a byte-exact
 * build, not stylistic. See docs/matching.md, "Matching decompilation"
 * for why plain C alone doesn't reproduce them. */
s32 sub_800697C(void *arg0)
{
    register void *self asm("r6");
    register s32 total asm("r4");
    register s32 b asm("r9");
    register s32 c asm("r5");
    register s32 d asm("r8");
    s32 e;
    u8 flags;

    self = arg0;
    total = sub_800695C(self);
    b = sub_80068CC(self);
    c = sub_8006864(self);
    d = sub_8006820(self);
    e = sub_80067EC(self);
    total += b;
    c = (c + (s32)((u32)c >> 31)) >> 1;
    total += c;
    total += d;
    total += e;
    flags = *((u8 *)self + 2);
    total += flags >> 7;
    total += ((u32)flags << 26) >> 31;
    total += ((u32)flags << 25) >> 31;
    total += ((u32)flags << 27) >> 31;
    return sub_803ADB4(total * 100, 0x48);
}

void sub_80069E8(void *arg0, u16 *arg1, s32 arg2)
{
    u8 *entry;
    u16 zero;

    if (arg2 <= 0) {
        return;
    }
    zero = 0;
    entry = (u8 *)arg0;
    do {
        *(u16 *)(entry + 0x12) = arg1[0];
        entry += 8;
        *(u16 *)(entry + 0x12) = zero;
        entry += 8;
        *(u16 *)(entry + 0x12) = zero;
        entry += 8;
        *(u16 *)(entry + 0x12) = arg1[1];
        entry += 8;
        arg1 += 2;
        arg2--;
    } while (arg2 != 0);
}

/* Manages a shadow copy of a chunk of the 128-entry hardware OAM table:
 * a count of active entries, two unidentified fields, then the
 * 1024-byte shadow table itself (128 entries * 8 bytes) starting right
 * after. Functions below that need volatile or register-pinned access
 * to `count`/the table still use raw pointer casts on purpose (see
 * docs/matching.md, "Matching decompilation") - this type exists so
 * call sites can be typed meaningfully instead of passing `void *`. */
struct oam_shadow_buffer {
    s32 count;
    s32 field_04;
    s32 field_08;
    u8 table[0x400];
};
COMPILE_TIME_ASSERT(sizeof(struct oam_shadow_buffer) == 0x40C);

void sub_8006A14(struct oam_shadow_buffer *arg0, void *arg1, s32 arg2)
{
    if (arg2 == 0) {
        return;
    }
    DMA3.src = arg1;
    DMA3.dst = (u8 *)arg0 + ((*(s32 *)arg0 << 3) + 0xC);
    DMA3.cnt = (arg2 << 1) | ((DMA_ENABLE | DMA_32BIT) << 16);
    (void)DMA3.cnt;
    *(s32 *)arg0 = *(s32 *)arg0 + arg2;
}

/* The two inline-asm `add`s below anchor operations gcc would otherwise
 * reorder or canonicalize differently than the ROM (see docs/matching.md,
 * "Matching decompilation") - not obfuscation, just pinning byte-exact
 * order. */
void sub_8006A48(struct oam_shadow_buffer *arg0)
{
    register u8 *self asm("r1");
    register s32 i asm("r2");
    register s32 bit asm("r3");
    register s32 mask asm("r4");
    register u8 loaded asm("r5");
    register s32 result asm("r0");

    self = (u8 *)arg0;
    i = *(s32 *)self;
    if (i > OAM_ENTRY_COUNT - 1) {
        return;
    }
    mask = ~3;
    bit = 2;
    result = (i << 3) + 0xD;
    asm volatile("add %0, %1, %0" : "+r"(self) : "r"(result));
    do {
        asm volatile("add %0, %1, #0" : "=r"(result) : "r"(mask));
        loaded = *self;
        result &= loaded;
        result |= bit;
        *self = result;
        self += 8;
        i++;
    } while (i <= OAM_ENTRY_COUNT - 1);
}

void sub_8006A78(struct oam_shadow_buffer *arg0)
{
    arg0->count = arg0->field_04;
    arg0->field_08 = 0;
}

void sub_8006A84(struct oam_shadow_buffer *arg0)
{
    arg0->field_04 = arg0->count;
    arg0->field_08 = 0;
}

void sub_8006A90(struct oam_shadow_buffer *arg0)
{
    arg0->count = 0;
    arg0->field_08 = 0;
    sub_8006A84(arg0);
    sub_8006A78(arg0);
}

void sub_8006AAC(struct oam_shadow_buffer *arg0)
{
    DMA3.src = (u8 *)arg0 + 0xC;
    DMA3.dst = (void *)OAM;
    DMA3.cnt = ((DMA_ENABLE | DMA_32BIT) << 16) | 0x100;
    (void)DMA3.cnt;
}

/* Inserts one record (arg1[0]/arg1[1]) into the shadow OAM table at the
 * current count, preserving the padding halfword at +0x12 that overlaps
 * the tail of arg1[1] on real hardware (see docs/matching.md). */
void sub_8006AC8(struct oam_shadow_buffer *arg0, u32 *arg1)
{
    register s32 n1 asm("r2");
    register u16 saved asm("r3");
    register s32 n2 asm("r1");
    register s32 addr2 asm("r0");
    u32 v0;
    u32 v1;

    n1 = *(vs32 *)arg0;
    if (n1 > OAM_ENTRY_COUNT - 1) {
        return;
    }
    n1 = (s32)arg0 + (n1 << 3);
    saved = *(u16 *)(n1 + 0x12);
    v0 = arg1[0];
    v1 = arg1[1];
    *(u32 *)(n1 + 0xC) = v0;
    *(u32 *)(n1 + 0x10) = v1;
    n2 = *(vs32 *)arg0;
    addr2 = (s32)arg0 + (n2 << 3);
    *(u16 *)(addr2 + 0x12) = saved;
    n2++;
    *(s32 *)arg0 = n2;
}

extern void sub_8026ED0(void *arg0);

void sub_8006AF4(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

struct oam_shadow_buffer *sub_8006B0C(struct oam_shadow_buffer *arg0)
{
    sub_8006A90(arg0);
    return arg0;
}

void FlushVramDmaQueue(void)
{
    struct dma_queue_entry *entry;
    s32 i; /* QUEUE_COUNT must be re-read each iteration - see its
            * definition above - and `raw`/`shifted` are pinned to match
            * the ROM's register choice for the size-field load+shift
            * (docs/matching.md, "Matching decompilation"). */
    register u16 raw asm("r1");
    register u32 shifted asm("r0");

    for (i = 0; i < QUEUE_COUNT; i++) {
        entry = &gUnknown_03001290.entries[i];
        if (entry->field_0A == 0x20) {
            DMA3.src = entry->field_04;
            DMA3.dst = entry->field_00;
            raw = entry->field_08;
            shifted = raw >> 2;
            shifted |= (DMA_ENABLE | DMA_32BIT) << 16;
        } else {
            DMA3.src = entry->field_04;
            DMA3.dst = entry->field_00;
            raw = entry->field_08;
            shifted = raw >> 1;
            shifted |= (DMA_ENABLE | DMA_16BIT) << 16;
        }
        DMA3.cnt = shifted;
        (void)DMA3.cnt;
    }
    gUnknown_03001290.count = 0;

    while (DMA3.cnt & (DMA_ENABLE << 16)) {
    }
}

s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3)
{
    struct dma_queue_entry *entry;

    if (arg2 == 0) {
        return 0;
    }
    if (gUnknown_03001290.count > DMA_QUEUE_MAX_ENTRIES - 1) {
        return -1;
    }
    entry = &gUnknown_03001290.entries[gUnknown_03001290.count];
    gUnknown_03001290.count++;
    entry->field_00 = arg1;
    entry->field_04 = arg0;
    entry->field_08 = arg2;
    entry->field_0A = arg3;
    return 0;
}

void FreeVramDmaQueue(void)
{
    if (gUnknown_03001290.entries != NULL) {
        mem_free((u8 *)gUnknown_03001290.entries);
        gUnknown_03001290.entries = NULL;
    }
}
