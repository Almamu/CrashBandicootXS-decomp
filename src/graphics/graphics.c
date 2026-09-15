#include "core.h"
#include "memory.h"
#include "vram_pool.h"

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

s32 AllocVramDmaQueue(void)
{
    gUnknown_03001290.entries = (struct dma_queue_entry *)mem_alloc(
        sizeof(struct dma_queue_entry) * DMA_QUEUE_MAX_ENTRIES, MEM_HEAP_EWRAM);
    if (gUnknown_03001290.entries == NULL) {
        return -1;
    }
    gUnknown_03001290.count = 0;
    return 0;
}

extern void sub_8001604(void);
extern void *sub_8026EC0(u32 size);

void sub_8006C28(struct vram_upload_cursor *self)
{
    self->field_04 = self->field_00;
}

void sub_8006C30(struct vram_upload_cursor *self)
{
    self->field_00 = self->field_04;
}

/* No callers anywhere in the codebase - genuinely unreachable, matched
 * anyway to keep the ROM's byte layout intact (see docs/decomp_dev.md's
 * mem_collect entry for the established pattern). */
s32 sub_8006C38(struct vram_upload_cursor *self)
{
    return OBJ_VRAM0_SIZE - self->field_04;
}

s32 sub_8006C44(struct vram_upload_cursor *self)
{
    return self->field_04 >> 5;
}

void sub_8006C4C(struct vram_upload_cursor *self)
{
    self->field_04 = self->field_00 = self->field_08 << 5;
}

s32 sub_8006C58(struct vram_upload_cursor *self, s32 size)
{
    s32 result;

    if (self->field_04 + size <= OBJ_VRAM0_SIZE) {
        result = sub_8006C44(self);
        self->field_04 += size;
        return result;
    }
    return -1;
}

s32 sub_8006C84(struct vram_upload_cursor *self, void *src, s32 size)
{
    s32 result;

    if (self->field_04 + size <= OBJ_VRAM0_SIZE) {
        if (QueueVramDmaTransfer(src, OBJ_VRAM0 + self->field_04, (u16)size, 0x20) == 0) {
            result = sub_8006C44(self);
            self->field_04 += size;
            return result;
        }
        return -2;
    }
    return -1;
}

void sub_8006CD0(struct vram_upload_cursor *self, u32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

struct vram_upload_cursor *sub_8006CE8(struct vram_upload_cursor *self, s32 count)
{
    sub_8001604();
    self->field_08 = count;
    sub_8006C4C(self);
    sub_8006C4C(self);
    return self;
}

/* `(u8 *)self + 0x2c` (offsetof(slots)) plus a separate `+= slot << 5`
 * step, instead of `self->slots[slot]` directly - the plain field
 * access compiles to a different instruction order/operand choice
 * than the ROM here (tried, rebuilt, confirmed different - see
 * docs/matching.md, "Matching decompilation"). */
void sub_8006D08(struct tile_asset_cache *self, s32 slot, s32 recordId)
{
    const u8 *src;
    u8 *dst;

    self->remap[recordId] = slot;
    self->dirty = 1;
    src = self->records;
    dst = (u8 *)self + 0x2c;
    src += recordId << 5;
    dst += slot << 5;
    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void sub_8006D40(struct tile_asset_cache *self, s32 slot, s32 index)
{
    self->remap[index] = slot;
    self->reserved[slot] = 0;
}

s32 sub_8006D50(struct tile_asset_cache *self, s32 index)
{
    if (self->reserved[index] == 0) {
        return 0;
    }
    self->reserved[index] = 0;
    return 1;
}

/* The inline asm pins the ROM's exact `add r0, r1, r0` (slot-then-base)
 * operand order for this address computation - a plain C `base[slot]`
 * or `slot + base` expression both lowered to the opposite operand
 * order regardless of how the addition was phrased (see docs/matching.md,
 * "Matching decompilation"). */
void sub_8006D68(struct tile_asset_cache *self, s32 index)
{
    u8 *base;
    s32 slot;
    u8 *addr;

    if (self->remap[index] != 0xFF) {
        base = self->pending;
        slot = self->remap[index];
        asm volatile("add %0, %1, %2" : "=r"(addr) : "r"(slot), "r"(base));
        *addr = 0;
    }
}

void sub_8006D84(struct tile_asset_cache *self, s32 index)
{
    u8 *base;
    s32 slot;
    u8 *addr;

    if (self->remap[index] != 0xFF) {
        base = self->pending;
        slot = self->remap[index];
        asm volatile("add %0, %1, %2" : "=r"(addr) : "r"(slot), "r"(base));
        *addr = 1;
    }
}

void sub_8006DA0(struct tile_asset_cache *self, s32 index)
{
    DMA3.src = self->slots[index];
    DMA3.dst = OBJ_PLTT + (index << 5);
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void sub_8006DC8(struct tile_asset_cache *self)
{
    if (self->dirty) {
        DMA3.src = self->slots;
        DMA3.dst = OBJ_PLTT;
        DMA3.cnt = (DMA_ENABLE << 16) | 0x100;
        (void)DMA3.cnt;
    }
}

/* Explicit register pins for `self`/`recordId`: this function makes no
 * calls, so gcc is otherwise free to pick any registers for them and
 * lands on a different (equally valid) allocation than the ROM's own -
 * pinned to match byte-exactly (see docs/matching.md, "Matching
 * decompilation"). */
u8 sub_8006DF8(struct tile_asset_cache *self, s32 recordId)
{
    register struct tile_asset_cache *pSelf asm("r2") = self;
    register s32 pRecordId asm("r5") = recordId;
    register u8 *remap asm("r0") = pSelf->remap;
    register u8 *addr asm("r1");
    u8 slot;
    s32 i;
    const u8 *src;
    u8 *dst;
    u8 *reservedBase;
    register s32 shiftedId asm("r0");

    asm volatile("add %0, %1, %2" : "=r"(addr) : "r"(remap), "r"(pRecordId));
    slot = *addr;

    if (slot != 0xFF) {
        return slot;
    }
    pSelf->dirty = 1;
    i = 0;
    reservedBase = pSelf->reserved;
    for (; i <= 15; i++) {
        if (reservedBase[i] != 0) {
            reservedBase[i] = 0;
            pSelf->remap[pRecordId] = i;
            src = pSelf->records;
            dst = (u8 *)pSelf + 0x2c;
            shiftedId = pRecordId << 5;
            src += shiftedId;
            dst += i << 5;
            DMA3.src = src;
            DMA3.dst = dst;
            DMA3.cnt = (DMA_ENABLE << 16) | 16;
            (void)DMA3.cnt;
            return (u8)i;
        }
    }
    return 0;
}

/* No callers anywhere in the codebase - genuinely unreachable, matched
 * anyway to keep the ROM's byte layout intact (see docs/decomp_dev.md's
 * mem_collect entry for the established pattern). */
s32 sub_8006E64(struct tile_asset_cache *self, s32 slot)
{
    s32 i;
    s32 result;

    if (self->pending[slot] == 0) {
        self->reserved[slot] = 1;
        for (i = 0; i < self->count; i++) {
            if (self->remap[i] == slot) {
                self->remap[i] = 0xFF;
            }
        }
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void sub_8006EA8(struct tile_asset_cache *self)
{
    s32 slot;
    s32 i;

    for (slot = 0; slot <= 15; slot++) {
        if (self->pending[slot] == 0) {
            self->reserved[slot] = 1;
            for (i = 0; i < self->count; i++) {
                if (self->remap[i] == slot) {
                    self->remap[i] = 0xFF;
                }
            }
        }
    }
}

void sub_8006EF0(struct tile_asset_cache *self, u16 count, const u8 *records)
{
    s32 i;

    if (self->remap != NULL) {
        sub_8026ED0(self->remap);
    }
    self->remap = NULL;
    self->count = 0;
    self->records = NULL;
    for (i = 0; i <= 15; i++) {
        self->reserved[i] = 1;
        self->pending[i] = 0;
    }
    self->count = count;
    self->records = records;
    self->remap = (u8 *)sub_8026EC0(self->count);
    for (i = 0; i < self->count; i++) {
        self->remap[i] = 0xFF;
    }
}

void sub_8006F5C(struct tile_asset_cache *self)
{
    s32 i;

    if (self->remap != NULL) {
        sub_8026ED0(self->remap);
    }
    self->remap = NULL;
    self->count = 0;
    self->records = NULL;
    for (i = 0; i <= 15; i++) {
        self->reserved[i] = 1;
        self->pending[i] = 0;
    }
}

void sub_8006F94(struct tile_asset_cache *self, u32 flags)
{
    sub_8006F5C(self);
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* The inline asm pins the ROM's `add r1, r0, r3` (self-plus-constant,
 * not in-place) operand order/register choice - see docs/matching.md,
 * "Matching decompilation". */
void sub_8006FB4(struct tile_asset_cache *self)
{
    register s32 offset asm("r3");
    register u8 *dirtyAddr asm("r1");

    self->count = 0;
    self->remap = NULL;
    self->records = NULL;
    offset = 0x8b << 2;
    asm volatile("add %0, %1, %2" : "=r"(dirtyAddr) : "r"(self), "r"(offset));
    *dirtyAddr = 0;
}

void sub_8006FC8(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        sub_8026ED0(arg0);
    }
}

void nullsub_1(void)
{
}
asm(".align 2, 0");

extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void *gUnknown_03001308;

/* `self`'s own layout isn't tied to a named struct yet - raw offsets,
 * matching the many similar actor-zone functions docs/rom_map.md
 * documents this session using the same "field+0x18 -> {s16 offset;
 * ...; void *text}" convention.
 *
 * `pSelf` is pinned to r2: this function makes a call, and plain C
 * phrasing left `self` in r3 instead of the ROM's r2 (tried, rebuilt,
 * confirmed different - see docs/matching.md, "Matching decompilation"). */
u8 sub_8006FE4(void *self)
{
    register void *pSelf asm("r2") = self;
    s32 buf[4];
    void *subObj;
    void *table;
    s32 a, b, c, d;
    u8 flag;

    flag = (*((u8 *)pSelf + 0xc) >> 4) & 1;
    if (!flag) {
        a = 0xdc << 9;
        b = 0x8c << 9;
        buf[2] = a;
        buf[3] = b;

        subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
        c = (*(s32 *)subObj << 8) + (s32)0xFFFF9C00;
        d = (*(s32 *)((u8 *)subObj + 4) << 8) + (s32)0xFFFFC400;
        buf[0] = c;
        buf[1] = d;

        table = *(void **)((u8 *)pSelf + 0x18);
        table = (u8 *)table + 0x40;
        flag = (u8)sub_803AD80((u8 *)pSelf + *(s16 *)table, buf, *(void **)((u8 *)table + 4));
    }
    return flag;
}

extern void *sub_803AD7C(void *arg0, void *arg1);
extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_800B37C(void *arg0, void *buf);
extern void sub_803AD88(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void *gUnknown_030012D8;

/* `self` uses the same actor-zone raw-offset layout as sub_8006FE4
 * above (field_0c flags byte, field_18 table pointer) - still no named
 * struct, per the same precedent.
 *
 * Two of the flag-byte tests below use inline asm rather than plain C
 * (`(byte >> N) & 1`, `byte | const`): gcc's register choice for the
 * intermediate/result value flipped from the ROM's own pick (which
 * register survives vs. which is scratch) no matter how the C was
 * rephrased or split into locals - tried and rebuilt several ways, see
 * docs/matching.md, "Matching decompilation". The `deadRead` r4 pin
 * mirrors a load the ROM performs but never uses (dead code in the
 * original too, apparently a field access whose result just goes
 * unused at this call site) - kept to match the byte count exactly. */
s32 sub_8007048(void *self)
{
    void *table;
    void *rec;
    s32 x, rx;
    s32 y, ry;
    u8 rw, rh;
    s32 buf[4];
    void *table2;
    void *addr;
    u8 field0a;
    s32 flagTest;

    table = *(void **)((u8 *)self + 0x18);
    rec = sub_803AD7C((u8 *)self + *(s16 *)((u8 *)table + 0x10), *(void **)((u8 *)table + 0x14));

    x = *(s32 *)self >> 8;
    rx = *(s16 *)((u8 *)rec + 0);
    y = *(s32 *)((u8 *)self + 4) >> 8;
    ry = *(s16 *)((u8 *)rec + 2);
    rw = *((u8 *)rec + 4);
    rh = *((u8 *)rec + 5);
    x += rx;
    y += ry;
    sub_803AFE4(buf, x, y);
    sub_803AFDC(buf, rw, rh);

    {
        register s32 flagTestR0 asm("r0");
        asm volatile(
            "ldrb r1, [%1, #0xc]\n\t"
            "lsr %0, r1, #2\n\t"
            "mov r1, #1\n\t"
            "and %0, %0, r1"
            : "=r"(flagTestR0)
            : "r"(self)
            : "r1");
        flagTest = flagTestR0;
    }
    if (flagTest) {
        if (sub_800B37C(gUnknown_030012D8, buf)) {
            asm volatile(
                "mov r0, #8\n\t"
                "ldrb r2, [%0, #0xc]\n\t"
                "orr r0, r0, r2\n\t"
                "strb r0, [%0, #0xc]"
                :
                : "r"(self)
                : "r0", "r2", "memory");

            table2 = *(void **)((u8 *)gUnknown_030012D8 + 0x18);
            table2 = (u8 *)table2 + 0x68;
            addr = (u8 *)gUnknown_030012D8 + *(s16 *)table2;
            field0a = *((u8 *)self + 0xa);
            {
                register void *deadRead asm("r4") = *(void *volatile *)((u8 *)table2 + 4);
                (void)deadRead;
            }
            sub_803AD88(addr, 0, field0a, 0);
        }
    }
    return 0;
}

void nullsub_11(void)
{
}
asm(".align 2, 0");

void sub_80070D4(void *self)
{
    void *table = *(void **)((u8 *)self + 0x18);
    sub_803AD7C((u8 *)self + *(s16 *)((u8 *)table + 8), *(void **)((u8 *)table + 0xc));
}

void *sub_80070E8(void *self)
{
    return (u8 *)self + 0x10;
}

/* `w`/`h` are stored both as the raw byte and as a halved-and-negated
 * s16 - the negate-then-divide-by-2 idiom below is C's `(-w) / 2`,
 * matched by the truncating-toward-zero integer division the ROM
 * itself performs (see docs/matching.md, "Matching decompilation"). */
void sub_80070EC(void *self, s32 w, s32 h)
{
    *(s16 *)((u8 *)self + 0x10) = -w / 2;
    *(s16 *)((u8 *)self + 0x12) = -h / 2;
    *((u8 *)self + 0x14) = w;
    *((u8 *)self + 0x15) = h;
}

s32 sub_800710C(void)
{
    return 0;
}

s32 sub_8007110(void)
{
    return 0;
}

/* Same raw self layout/field+0x18 table convention as sub_8006FE4 and
 * sub_8007048 above. Both `self` and `box` are pinned - matching the
 * ROM's exact register dance required it (see the inline asm block
 * below), and a plain-C parameter reload picked different registers
 * once anything else in this function was pinned. This function's
 * body is 94 bytes (not 4-aligned) and is the last thing in this
 * translation unit right now, so the trailing `asm(".align 2, 0")`
 * below is required to get the ROM's zero-fill instead of `as`'s
 * default NOP pad - see docs/matching.md, "A gotcha worth knowing". */
s32 sub_8007114(void *self, void *box)
{
    register void *pSelf asm("r5") = self;
    register void *pBox asm("r6") = box;
    void *table;
    void *rec;
    u8 flag;
    s32 result;

    flag = (*((u8 *)pSelf + 0xc) >> 4) & 1;
    if (!flag) {
        table = *(void **)((u8 *)pSelf + 0x18);
        rec = sub_803AD7C((u8 *)pSelf + *(s16 *)((u8 *)table + 0x10), *(void **)((u8 *)table + 0x14));
        {
            register void *recR0 asm("r0") = rec;
            register s32 minXR4 asm("r4");
            register s32 maxXR1 asm("r1");
            register s32 minYR5 asm("r5");
            register s32 maxYR3 asm("r3");
            s32 boxX0;
            /* r7 is never usable for an explicit register-variable pin
             * in this toolchain (the compiler drops it from the
             * prologue's push list regardless - see
             * matching_decomp_register_pinning memory), so `result`
             * below is left as an ordinary unpinned local and happens
             * to land in r7 on its own, matching the ROM. The rest of
             * this block reproduces the ROM's own register dance:
             * gcc otherwise computes rec[4]<<7/rec[5]<<7 in place
             * (same register as the byte load) rather than moving the
             * shifted result to a free register the way the ROM does
             * - tried several C-level rephrasings with no effect, see
             * docs/matching.md, "Matching decompilation". */
            asm volatile(
                "ldrb r1, [%4, #4]\n\t"
                "lsl r2, r1, #7\n\t"
                "ldrb r0, [%4, #5]\n\t"
                "lsl r3, r0, #7\n\t"
                "ldr r1, [%5]\n\t"
                "sub %0, r1, r2\n\t"
                "ldr r0, [%5, #4]\n\t"
                "sub %2, r0, r3\n\t"
                "add %1, r1, r2\n\t"
                "add %3, r0, r3"
                : "=r"(minXR4), "=r"(maxXR1), "=r"(minYR5), "=r"(maxYR3)
                : "r"(recR0), "r"(pSelf)
                : "r0", "r1", "r2");
            result = 0;
            boxX0 = *(s32 *)pBox;
            if (minXR4 > boxX0) {
                s32 boxX1 = boxX0 + *(s32 *)((u8 *)pBox + 8);
                if (maxXR1 < boxX1) {
                    register s32 boxY0 asm("r2") = *(s32 *)((u8 *)pBox + 4);
                    if (minYR5 > boxY0) {
                        register s32 boxY1 asm("r0") = boxY0 + *(s32 *)((u8 *)pBox + 0xc);
                        if (maxYR3 < boxY1) {
                            result = 1;
                        }
                    }
                }
            }
        }
        flag = result;
    }
    return flag;
}
asm(".align 2, 0");

/* `arg0` is unused by the ROM - overwritten as scratch before its
 * incoming value is ever read. `gUnknown_03001308`'s sub-object here
 * is the same one sub_8006FE4 reads, but as two raw s32 fields
 * (dx/dy) sign-extended from their low 24 bits, not the record table
 * sub_8006FE4 uses - a different part of the same object. */
void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4)
{
    void *subObj;
    s32 dx, dy;

    subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
    dx = (*(s32 *)subObj << 8) >> 8;
    dy = (*(s32 *)((u8 *)subObj + 4) << 8) >> 8;
    *arg3 = arg1 - dx;
    *arg4 = arg2 - dy;
}

void sub_800719C(void *arg0, s32 *arg1, s32 *arg2)
{
    void *subObj;
    s32 x, y;
    s32 subX, subY;

    x = *(s32 *)arg0;
    y = *(s32 *)((u8 *)arg0 + 4);
    if (x & 0x80) {
        x += 0x80;
    }
    if (y & 0x80) {
        y += 0x80;
    }
    subObj = *(void **)((u8 *)gUnknown_03001308 + 0x10);
    subX = *(s32 *)subObj << 8;
    subY = *(s32 *)((u8 *)subObj + 4) << 8;
    *arg1 = (x - subX) >> 8;
    *arg2 = (y - subY) >> 8;
}

void nullsub_12(void)
{
}
asm(".align 2, 0");

extern void *sub_8026EDC(s32 size);
extern void sub_8007230(void *self);
extern u8 gStaticData_087E3BEC[];

void *sub_80071E4(u16 arg0, u16 arg1, u16 arg2)
{
    void *obj;

    obj = sub_8026EDC(0x1c);
    *(void **)((u8 *)obj + 0x18) = gStaticData_087E3BEC;
    sub_8007230(obj);
    *(u16 *)((u8 *)obj + 8) = arg0;
    *(s32 *)obj = (s32)arg1 << 8;
    *(s32 *)((u8 *)obj + 4) = (s32)arg2 << 8;
    return obj;
}

s32 sub_800722C(void)
{
    return 0;
}

/* Clears self's bit1/bit0/bit3/bit4, sets bit2 - the actor-init step
 * called from sub_80071E4. 44-byte body isn't 4-aligned, so the
 * trailing asm(".align 2, 0") is required (see the first entry in
 * docs/matching.md). */
void sub_8007230(void *self)
{
    /* `result` and `tmp` are pinned so the running result stays in
     * the constant's own register (r1) rather than the freshly-loaded
     * byte's (r2), matching the ROM's exact register dance - plain C
     * naturally accumulates into the loaded-byte's register instead
     * (see docs/matching.md, "Matching decompilation"). */
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = ~2;
    tmp = *((u8 *)self + 0xc);
    result &= tmp;
    tmp = 4;
    result |= tmp;
    /* The `tmp = -2` reload below is real, matching the ROM's own
     * fresh mov+neg - gcc otherwise derives -2 as "4 - 6" reusing the
     * OR step's leftover register value (cheaper, but not what the
     * ROM does) no matter how the C is phrased, so it's pinned via
     * inline asm to force the fresh load. */
    asm volatile("mov %0, #2\n\tneg %0, %0" : "=r"(tmp));
    result &= tmp;
    tmp -= 7;
    result &= tmp;
    tmp -= 8;
    result &= tmp;
    *((u8 *)self + 0xc) = result;
    *(u16 *)((u8 *)self + 0x10) = 0;
    *(u16 *)((u8 *)self + 0x12) = 0;
    *((u8 *)self + 0x14) = 1;
    *((u8 *)self + 0x15) = 1;
}
asm(".align 2, 0");

void *sub_800725C(void *self)
{
    *(void **)((u8 *)self + 0x18) = gStaticData_087E3BEC;
    sub_8007230(self);
    return self;
}
