#include "core.h"
#include "gfx.h"
#include "memory.h"
#include "vram_pool.h"
#include "actor.h"
#include "vtable.h"
#include "aabb.h"
#include "util.h"
#include <libgcc.h>
#include "menus.h"
#include "player.h"

struct dma_queue_entry {
    void *dest;
    void *src;
    u16 size;
    u16 unit;
};

struct dma_queue {
    struct dma_queue_entry *entries;
    s32 count;
};

#define DMA3 (*(struct dma_regs *)REG_ADDR_DMA3SAD)
#define QUEUE_COUNT (((volatile struct dma_queue *)&gVramDmaQueue)->count)
/* Allocated capacity of gVramDmaQueue.entries. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

/* The register pins below (and in several functions further down) match
 * the ROM's own register allocation exactly - required for a byte-exact
 * build, not stylistic. See docs/matching.md, "Matching decompilation"
 * for why plain C alone doesn't reproduce them. */
s32 GetCompletionPercent(void *arg0)
{
    register void *self asm("r6");
    register s32 total asm("r4");
    register s32 b asm("r9");
    register s32 c asm("r5");
    register s32 d asm("r8");
    s32 e;
    u8 flags;

    self = arg0;
    total = CountCrystals(self);
    b = CountGems(self);
    c = CountSapphireRelics(self);
    d = CountGoldRelics(self);
    e = CountPlatinumRelics(self);
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
    return __divsi3(total * 100, 0x48);
}

void SetOamAffineScales(void *arg0, u16 *arg1, s32 arg2)
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

void AppendOamEntries(struct oam_shadow_buffer *arg0, void *arg1, s32 arg2)
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
void HideUnusedOamEntries(struct oam_shadow_buffer *arg0)
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

void RewindOamBuffer(struct oam_shadow_buffer *arg0)
{
    arg0->count = arg0->base;
    arg0->matrixCount = 0;
}

void MarkOamBufferBase(struct oam_shadow_buffer *arg0)
{
    arg0->base = arg0->count;
    arg0->matrixCount = 0;
}

void ResetOamBuffer(struct oam_shadow_buffer *arg0)
{
    arg0->count = 0;
    arg0->matrixCount = 0;
    MarkOamBufferBase(arg0);
    RewindOamBuffer(arg0);
}

void CommitOamBuffer(struct oam_shadow_buffer *arg0)
{
    DMA3.src = (u8 *)arg0 + 0xC;
    DMA3.dst = (void *)OAM;
    DMA3.cnt = ((DMA_ENABLE | DMA_32BIT) << 16) | 0x100;
    (void)DMA3.cnt;
}

/* Inserts one record (arg1[0]/arg1[1]) into the shadow OAM table at the
 * current count, preserving the padding halfword at +0x12 that overlaps
 * the tail of arg1[1] on real hardware (see docs/matching.md). */
void AddOamEntry(struct oam_shadow_buffer *arg0, const void *entry)
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
    /* `n1`/`addr2` are `arg0` moved on by `count` entries, so their
     * `table[0]` is entry `count`. */
    n1 = (s32)arg0 + (n1 << 3);
    saved = ((struct oam_shadow_buffer *)n1)->table[0].attr[3];
    v0 = ((const u32 *)entry)[0];
    v1 = ((const u32 *)entry)[1];
    ((struct oam_shadow_buffer *)n1)->table[0].words[0] = v0;
    ((struct oam_shadow_buffer *)n1)->table[0].words[1] = v1;
    n2 = *(vs32 *)arg0;
    addr2 = (s32)arg0 + (n2 << 3);
    ((struct oam_shadow_buffer *)addr2)->table[0].attr[3] = saved;
    n2++;
    *(s32 *)arg0 = n2;
}

void DestroyOamBuffer(struct oam_shadow_buffer *arg0, u32 arg1)
{
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}

struct oam_shadow_buffer *InitOamBuffer(struct oam_shadow_buffer *arg0)
{
    ResetOamBuffer(arg0);
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
        entry = &gVramDmaQueue.entries[i];
        if (entry->unit == 0x20) {
            DMA3.src = entry->src;
            DMA3.dst = entry->dest;
            raw = entry->size;
            shifted = raw >> 2;
            shifted |= (DMA_ENABLE | DMA_32BIT) << 16;
        } else {
            DMA3.src = entry->src;
            DMA3.dst = entry->dest;
            raw = entry->size;
            shifted = raw >> 1;
            shifted |= (DMA_ENABLE | DMA_16BIT) << 16;
        }
        DMA3.cnt = shifted;
        (void)DMA3.cnt;
    }
    gVramDmaQueue.count = 0;

    while (DMA3.cnt & (DMA_ENABLE << 16)) {
    }
}

s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3)
{
    struct dma_queue_entry *entry;

    if (arg2 == 0) {
        return 0;
    }
    if (gVramDmaQueue.count > DMA_QUEUE_MAX_ENTRIES - 1) {
        return -1;
    }
    entry = &gVramDmaQueue.entries[gVramDmaQueue.count];
    gVramDmaQueue.count++;
    entry->dest = arg1;
    entry->src = arg0;
    entry->size = arg2;
    entry->unit = arg3;
    return 0;
}

void FreeVramDmaQueue(void)
{
    if (gVramDmaQueue.entries != NULL) {
        mem_free((u8 *)gVramDmaQueue.entries);
        gVramDmaQueue.entries = NULL;
    }
}

s32 AllocVramDmaQueue(void)
{
    gVramDmaQueue.entries = (struct dma_queue_entry *)mem_alloc(
        sizeof(struct dma_queue_entry) * DMA_QUEUE_MAX_ENTRIES, MEM_HEAP_EWRAM);
    if (gVramDmaQueue.entries == NULL) {
        return -1;
    }
    gVramDmaQueue.count = 0;
    return 0;
}

void RewindObjVram(struct vram_upload_cursor *self)
{
    self->offset = self->mark;
}

void MarkObjVram(struct vram_upload_cursor *self)
{
    self->mark = self->offset;
}

/* No callers anywhere in the codebase - genuinely unreachable, matched
 * anyway to keep the ROM's byte layout intact (see docs/decomp_dev.md's
 * mem_collect entry for the established pattern). */
s32 GetObjVramFreeBytes(struct vram_upload_cursor *self)
{
    return OBJ_VRAM0_SIZE - self->offset;
}

s32 GetObjVramTile(struct vram_upload_cursor *self)
{
    return self->offset >> 5;
}

void ResetObjVram(struct vram_upload_cursor *self)
{
    self->offset = self->mark = self->baseTile << 5;
}

s32 ReserveObjVram(struct vram_upload_cursor *self, s32 size)
{
    s32 result;

    if (self->offset + size <= OBJ_VRAM0_SIZE) {
        result = GetObjVramTile(self);
        self->offset += size;
        return result;
    }
    return -1;
}

s32 UploadObjVram(struct vram_upload_cursor *self, void *src, s32 size)
{
    s32 result;

    if (self->offset + size <= OBJ_VRAM0_SIZE) {
        if (QueueVramDmaTransfer(src, OBJ_VRAM0 + self->offset, (u16)size, 0x20) == 0) {
            result = GetObjVramTile(self);
            self->offset += size;
            return result;
        }
        return -2;
    }
    return -1;
}

void DestroyObjVramCursor(struct vram_upload_cursor *self, u32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

struct vram_upload_cursor *InitObjVramCursor(struct vram_upload_cursor *self, s32 count)
{
    SetObjMapping1D();
    self->baseTile = count;
    ResetObjVram(self);
    ResetObjVram(self);
    return self;
}

/* `(u8 *)self + 0x2c` (offsetof(slots)) plus a separate `+= slot << 5`
 * step, instead of `self->slots[slot]` directly - the plain field
 * access compiles to a different instruction order/operand choice
 * than the ROM here (tried, rebuilt, confirmed different - see
 * docs/matching.md, "Matching decompilation"). */
void LoadPaletteSlot(struct palette_cache *self, s32 slot, s32 recordId)
{
    const u8 *src;
    u8 *dst;

    self->slotOf[recordId] = slot;
    self->dirty = 1;
    src = self->palettes;
    dst = (u8 *)self + 0x2c;
    src += recordId << 5;
    dst += slot << 5;
    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void BindPaletteSlot(struct palette_cache *self, s32 slot, s32 index)
{
    self->slotOf[index] = slot;
    self->isFree[slot] = 0;
}

s32 ClaimPaletteSlot(struct palette_cache *self, s32 index)
{
    if (self->isFree[index] == 0) {
        return 0;
    }
    self->isFree[index] = 0;
    return 1;
}

/* The inline asm pins the ROM's exact `add r0, r1, r0` (slot-then-base)
 * operand order for this address computation - a plain C `base[slot]`
 * or `slot + base` expression both lowered to the opposite operand
 * order regardless of how the addition was phrased (see docs/matching.md,
 * "Matching decompilation"). */
void UnlockPalette(struct palette_cache *self, s32 index)
{
    u8 *base;
    s32 slot;
    u8 *addr;

    if (self->slotOf[index] != 0xFF) {
        base = self->locked;
        slot = self->slotOf[index];
        asm volatile("add %0, %1, %2" : "=r"(addr) : "r"(slot), "r"(base));
        *addr = 0;
    }
}

void LockPalette(struct palette_cache *self, s32 index)
{
    u8 *base;
    s32 slot;
    u8 *addr;

    if (self->slotOf[index] != 0xFF) {
        base = self->locked;
        slot = self->slotOf[index];
        asm volatile("add %0, %1, %2" : "=r"(addr) : "r"(slot), "r"(base));
        *addr = 1;
    }
}

void UploadPaletteSlot(struct palette_cache *self, s32 index)
{
    DMA3.src = self->slots[index];
    DMA3.dst = OBJ_PLTT + (index << 5);
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void UploadPaletteCache(struct palette_cache *self)
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
u8 GetPaletteSlot(struct palette_cache *self, s32 recordId)
{
    register struct palette_cache *pSelf asm("r2") = self;
    register s32 pRecordId asm("r5") = recordId;
    register u8 *remap asm("r0") = pSelf->slotOf;
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
    reservedBase = pSelf->isFree;
    for (; i <= 15; i++) {
        if (reservedBase[i] != 0) {
            reservedBase[i] = 0;
            pSelf->slotOf[pRecordId] = i;
            src = pSelf->palettes;
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
s32 FreePaletteSlot(struct palette_cache *self, s32 slot)
{
    s32 i;
    s32 result;

    if (self->locked[slot] == 0) {
        self->isFree[slot] = 1;
        for (i = 0; i < self->count; i++) {
            if (self->slotOf[i] == slot) {
                self->slotOf[i] = 0xFF;
            }
        }
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void FreeUnlockedPaletteSlots(struct palette_cache *self)
{
    s32 slot;
    s32 i;

    for (slot = 0; slot <= 15; slot++) {
        if (self->locked[slot] == 0) {
            self->isFree[slot] = 1;
            for (i = 0; i < self->count; i++) {
                if (self->slotOf[i] == slot) {
                    self->slotOf[i] = 0xFF;
                }
            }
        }
    }
}

void SetPaletteCacheSource(struct palette_cache *self, u16 count, const u8 *records)
{
    s32 i;

    if (self->slotOf != NULL) {
        OperatorDelete(self->slotOf);
    }
    self->slotOf = NULL;
    self->count = 0;
    self->palettes = NULL;
    for (i = 0; i <= 15; i++) {
        self->isFree[i] = 1;
        self->locked[i] = 0;
    }
    self->count = count;
    self->palettes = records;
    self->slotOf = (u8 *)OperatorNewArray(self->count);
    for (i = 0; i < self->count; i++) {
        self->slotOf[i] = 0xFF;
    }
}

void ClearPaletteCache(struct palette_cache *self)
{
    s32 i;

    if (self->slotOf != NULL) {
        OperatorDelete(self->slotOf);
    }
    self->slotOf = NULL;
    self->count = 0;
    self->palettes = NULL;
    for (i = 0; i <= 15; i++) {
        self->isFree[i] = 1;
        self->locked[i] = 0;
    }
}

void DestroyPaletteCache(struct palette_cache *self, u32 flags)
{
    ClearPaletteCache(self);
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* The inline asm pins the ROM's `add r1, r0, r3` (self-plus-constant,
 * not in-place) operand order/register choice - see docs/matching.md,
 * "Matching decompilation". */
struct palette_cache *InitPaletteCache(struct palette_cache *self)
{
    register s32 offset asm("r3");
    register u8 *dirtyAddr asm("r1");

    self->count = 0;
    self->slotOf = NULL;
    self->palettes = NULL;
    offset = 0x8b << 2;
    asm volatile("add %0, %1, %2" : "=r"(dirtyAddr) : "r"(self), "r"(offset));
    *dirtyAddr = 0;
    return self;
}

void DestroySpriteBankSet(void *arg0, u32 arg1)
{
    if (arg1 & 1) {
        OperatorDelete(arg0);
    }
}

void nullsub_1(void)
{
}
asm(".align 2, 0");

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void *gLevelLayers;

/* `self` uses the shared `struct actor` layout (see actor.h) - the
 * "field_18 -> {s16 offset; ...; void *text}" convention docs/rom_map.md
 * documents is `table` (the per-category table's own internal shape
 * isn't known yet, so a dynamic offset into it stays raw pointer math).
 *
 * `pSelf` is pinned to r2: this function makes a call, and plain C
 * phrasing left `self` in r3 instead of the ROM's r2 (tried, rebuilt,
 * confirmed different - see docs/matching.md, "Matching decompilation"). */
u8 IsEntityNearCamera(struct actor *self)
{
    register struct actor *pSelf asm("r2") = self;
    s32 buf[4];
    void *table;
    void *subObj;
    s32 a, b, c, d;
    u8 flag;

    flag = (pSelf->flags >> 4) & 1;
    if (!flag) {
        a = 0xdc << 9;
        b = 0x8c << 9;
        buf[2] = a;
        buf[3] = b;

        subObj = *(void **)((u8 *)gLevelLayers + 0x10);
        c = (*(s32 *)subObj << 8) + (s32)0xFFFF9C00;
        d = (*(s32 *)((u8 *)subObj + 4) << 8) + (s32)0xFFFFC400;
        buf[0] = c;
        buf[1] = d;

        table = (u8 *)pSelf->table + 0x40;
        flag = (u8)_call_via_r2((u8 *)pSelf + *(s16 *)table, buf, *(void **)((u8 *)table + 4));
    }
    return flag;
}

extern void *_call_via_r1(void *arg0, void *arg1);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern struct actor *gPlayer;

/* `self` uses the shared `struct actor` layout (see actor.h) - same
 * precedent as IsEntityNearCamera above.
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
s32 CheckEntityPlayerContact(struct actor *self)
{
    struct vtable_slot *table;
    struct hitbox_quad *rec;
    s32 x, rx;
    s32 y, ry;
    u8 rw, rh;
    struct aabb buf;
    void *table2;
    void *addr;
    u8 field0a;
    s32 flagTest;

    table = self->table;
    rec = _call_via_r1((u8 *)self + table[2].delta, table[2].fn);

    x = self->x >> 8;
    rx = rec->offX;
    y = self->y >> 8;
    ry = rec->offY;
    rw = rec->w;
    rh = rec->h;
    x += rx;
    y += ry;
    SetAabbPos(&buf, x, y);
    SetAabbSize(&buf, rw, rh);

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
        if (PlayerTouchesBox(gPlayer, &buf)) {
            asm volatile(
                "mov r0, #8\n\t"
                "ldrb r2, [%0, #0xc]\n\t"
                "orr r0, r0, r2\n\t"
                "strb r0, [%0, #0xc]"
                :
                : "r"(self)
                : "r0", "r2", "memory");

            table2 = (u8 *)gPlayer->table + 0x68;
            addr = (u8 *)gPlayer + *(s16 *)table2;
            field0a = self->field_0A;
            {
                register void *deadRead asm("r4") = *(void *volatile *)((u8 *)table2 + 4);
                (void)deadRead;
            }
            _call_via_r4(addr, 0, field0a, 0);
        }
    }
    return 0;
}

/* gEntityVtable slot 4, the entity base class's draw: empty (the sprite
 * classes override it with DrawSpriteObj). */
void DrawEntity(void)
{
}
asm(".align 2, 0");

void UpdateEntity(struct actor *self)
{
    void *table = self->table;
    _call_via_r1((u8 *)self + *(s16 *)((u8 *)table + 8), *(void **)((u8 *)table + 0xc));
}

void *GetEntityBounds(struct actor *self)
{
    return &self->halfW;
}

/* `w`/`h` are stored both as the raw byte and as a halved-and-negated
 * s16 - the negate-then-divide-by-2 idiom below is C's `(-w) / 2`,
 * matched by the truncating-toward-zero integer division the ROM
 * itself performs (see docs/matching.md, "Matching decompilation"). */
void SetEntitySize(struct actor *self, s32 w, s32 h)
{
    self->halfW = -w / 2;
    self->halfH = -h / 2;
    self->rawW = w;
    self->rawH = h;
}

s32 EntityOverlapsRect(void)
{
    return 0;
}

s32 IsEntityOnScreen(void)
{
    return 0;
}

/* `self` uses the shared `struct actor` layout (see actor.h), same
 * precedent as IsEntityNearCamera/CheckEntityPlayerContact above. `box` is a plain
 * {s32 x0, y0, w, h} AABB rect - only seen at this one call site so
 * far, so it's not (yet) worth a named struct of its own. Both
 * `self` and `box` are pinned - matching the ROM's exact register
 * dance required it (see the inline asm block below), and a plain-C
 * parameter reload picked different registers once anything else in
 * this function was pinned. This function's body is 94 bytes (not
 * 4-aligned) and is the last thing in this translation unit right
 * now, so the trailing `asm(".align 2, 0")` below is required to get
 * the ROM's zero-fill instead of `as`'s default NOP pad - see
 * docs/matching.md, "A gotcha worth knowing". */
s32 IsEntityInsideRect(struct actor *self, struct aabb *box)
{
    register struct actor *pSelf asm("r5") = self;
    register struct aabb *pBox asm("r6") = box;
    struct vtable_slot *table;
    void *rec;
    u8 flag;
    s32 result;

    flag = (pSelf->flags >> 4) & 1;
    if (!flag) {
        table = pSelf->table;
        rec = _call_via_r1((u8 *)pSelf + table[2].delta, table[2].fn);
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
            boxX0 = pBox->x;
            if (minXR4 > boxX0) {
                s32 boxX1 = boxX0 + pBox->w;
                if (maxXR1 < boxX1) {
                    register s32 boxY0 asm("r2") = pBox->y;
                    if (minYR5 > boxY0) {
                        register s32 boxY1 asm("r0") = boxY0 + pBox->h;
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
 * incoming value is ever read. `gLevelLayers`'s sub-object here
 * is the same one IsEntityNearCamera reads, but as two raw s32 fields
 * (dx/dy) sign-extended from their low 24 bits, not the record table
 * IsEntityNearCamera uses - a different part of the same object. */
void WorldToScreen(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4)
{
    void *subObj;
    s32 dx, dy;

    subObj = *(void **)((u8 *)gLevelLayers + 0x10);
    dx = (*(s32 *)subObj << 8) >> 8;
    dy = (*(s32 *)((u8 *)subObj + 4) << 8) >> 8;
    *arg3 = arg1 - dx;
    *arg4 = arg2 - dy;
}

void WorldPosToScreen(s32 *arg0, s32 *arg1, s32 *arg2)
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
    subObj = *(void **)((u8 *)gLevelLayers + 0x10);
    subX = *(s32 *)subObj << 8;
    subY = *(s32 *)((u8 *)subObj + 4) << 8;
    *arg1 = (x - subX) >> 8;
    *arg2 = (y - subY) >> 8;
}

void nullsub_12(void)
{
}
asm(".align 2, 0");

extern u8 gEntityVtable[];

struct actor *CreateEntity(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *obj;

    obj = OperatorNew(sizeof(struct actor));
    obj->table = gEntityVtable;
    ResetEntity(obj);
    obj->field_08 = arg0;
    obj->x = (s32)arg1 << 8;
    obj->y = (s32)arg2 << 8;
    return obj;
}

s32 GetEntityClassId(void)
{
    return 0;
}

/* Clears self's bit1/bit0/bit3/bit4, sets bit2 - the actor-init step
 * called from CreateEntity. 44-byte body isn't 4-aligned, so the
 * trailing asm(".align 2, 0") is required (see the first entry in
 * docs/matching.md). */
void ResetEntity(struct actor *self)
{
    /* `result` and `tmp` are pinned so the running result stays in
     * the constant's own register (r1) rather than the freshly-loaded
     * byte's (r2), matching the ROM's exact register dance - plain C
     * naturally accumulates into the loaded-byte's register instead
     * (see docs/matching.md, "Matching decompilation"). */
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = ~2;
    tmp = self->flags;
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
    /* Stored through `*(T *)&self->field` casts rather than plain
     * member stores: those shift the zero-constant's materialization
     * earlier and into a different register (r2 instead of reusing r1
     * right after the strb), a real regression, not just a style
     * difference (see docs/matching.md, "Matching decompilation"). */
    *(u8 *)&self->flags = result;
    *(u16 *)&self->halfW = 0;
    *(u16 *)&self->halfH = 0;
    *(u8 *)&self->rawW = 1;
    *(u8 *)&self->rawH = 1;
}
asm(".align 2, 0");

struct actor *InitEntity(struct actor *self)
{
    self->table = gEntityVtable;
    ResetEntity(self);
    return self;
}

/* `result`/`tmp` pinned so the running result stays in the constant's
 * own register (r1) rather than the freshly-loaded byte's (r2) -
 * same pattern as ResetEntity above (see docs/matching.md, "Matching
 * decompilation"). */
void ClearEntityAlwaysActive(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = -17;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive above. */
void SetEntityAlwaysActive(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = 16;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}
asm(".align 2, 0");

u8 IsEntityAlwaysActive(struct actor *self)
{
    return (self->flags >> 4) & 1;
}
asm(".align 2, 0");

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive above. */
void ClearEntityTouched(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = -9;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched above. */
void SetEntityTouched(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = 8;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}
asm(".align 2, 0");

u8 IsEntityTouched(struct actor *self)
{
    return (self->flags >> 3) & 1;
}
asm(".align 2, 0");

/* Register pins force the ROM's exact register dance: gcc otherwise
 * doesn't move `self` to r1 at all (it can ldrb directly through r0),
 * and separately computes the AND into r1 instead of the constant's
 * own r0 - see docs/matching.md, "Matching decompilation". */
u8 IsEntityGone(struct actor *self)
{
    register struct actor *pSelf asm("r1") = self;
    register u8 flags asm("r1");
    register s32 result asm("r0") = 1;

    flags = pSelf->flags;
    result = result & flags;
    return result;
}
asm(".align 2, 0");

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched above. */
void ClearEntityGone(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = -2;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

extern void *gEntityFlags;

/* Always sets self->flags bit0; if self->field_08 (an id) isn't the
 * sentinel 0xFFFF, also sets bit `field_08 & 0x1F` of a 32-bit-word
 * bitmap at `*gEntityFlags + 0x108`, word-indexed by
 * `field_08 >> 5` - looks like "mark this object's slot as active" in
 * some external allocation-tracking table. Several register-pinned
 * blocks below reproduce the ROM's exact instruction order/register
 * choices - see the inline comment on each, and docs/matching.md,
 * "Matching decompilation" for the general techniques. */
void MarkEntityGone(struct actor *self)
{
    register struct actor *pSelf asm("r1") = self;
    register u16 id asm("r4");

    {
        register s32 result asm("r0");
        register s32 tmp asm("r2");

        result = 1;
        tmp = pSelf->flags;
        result |= tmp;
        pSelf->flags = result;
    }
    {
        s32 cmpVal = 0xFFFF;
        id = pSelf->field_08;
        if (id != cmpVal) {
            s32 rawId = pSelf->field_08;
            void *base = gEntityFlags;
            /* `word` is pinned to r0 and reused as the running
             * accumulator for the rest of the block (word<<5, then
             * rawId-word) - a fresh local for the subtraction's
             * result computed into a different register than the
             * ROM's own reuse of r0. The initial asm copy-then-shift
             * pair forces the ROM's extra `add`/`asr` instead of
             * gcc's one-instruction shift straight out of `rawId`'s
             * register. */
            register s32 word asm("r0");
            s32 wordOffset;

            asm volatile("add %0, %1, #0\n\tasr %0, %0, #5" : "=r"(word) : "r"(rawId));
            wordOffset = word << 2;

            base = (u8 *)base + 0x108;
            base = (u8 *)base + wordOffset;
            word = word << 5;
            word = rawId - word;
            *(u32 *)base |= 1 << word;
        }
    }
}

u8 IsEntityContactEnabled(struct actor *self)
{
    return (self->flags >> 2) & 1;
}
asm(".align 2, 0");

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone above. */
void DisableEntityContact(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = -5;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone/DisableEntityContact above. */
void EnableEntityContact(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = 4;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}
asm(".align 2, 0");

u8 GetEntityFlag1(struct actor *self)
{
    return (self->flags >> 1) & 1;
}
asm(".align 2, 0");

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone/DisableEntityContact/
 * EnableEntityContact above. */
void ClearEntityFlag1(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = -3;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone/DisableEntityContact/
 * EnableEntityContact/ClearEntityFlag1 above. */
void SetEntityFlag1(struct actor *self)
{
    register s32 result asm("r1");
    register s32 tmp asm("r2");

    result = 2;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}
asm(".align 2, 0");

s32 GetEntityPixelY(struct actor *self)
{
    return self->y >> 8;
}

s32 GetEntityPixelX(struct actor *self)
{
    return self->x >> 8;
}

s32 GetEntityY(struct actor *self)
{
    return self->y;
}

s32 GetEntityX(struct actor *self)
{
    return self->x;
}

void SetEntityPixelPos(struct actor *self, s32 arg1, s32 arg2)
{
    self->x = arg1 << 8;
    self->y = arg2 << 8;
}
asm(".align 2, 0");

void SetEntityPixelPosVec(struct actor *self, s32 *arg1)
{
    SetEntityPixelPos(self, arg1[0], arg1[1]);
}

void SetEntityPos(struct actor *self, s32 arg1, s32 arg2)
{
    self->x = arg1;
    self->y = arg2;
}
asm(".align 2, 0");

void SetEntityPosVec(struct actor *self, s32 *arg1)
{
    SetEntityPos(self, arg1[0], arg1[1]);
}

void SetEntityKind(struct actor *self, u8 arg1)
{
    self->field_0A = arg1;
}

u8 GetEntityKind(struct actor *self)
{
    return self->field_0A;
}

u16 GetEntityId(struct actor *self)
{
    return self->field_08;
}

void DestroyEntity(struct actor *self, u32 arg1)
{
    self->table = gEntityVtable;
    if (arg1 & 1) {
        OperatorDelete(self);
    }
}
