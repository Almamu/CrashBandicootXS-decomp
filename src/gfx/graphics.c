#include "core.h"
#include "math_util.h"
#include "match.h"
#include "gfx.h"
#include "memory.h"
#include "vram_pool.h"
#include "actor.h"
#include "vtable.h"
#include "aabb.h"
#include "util.h"
#include <libgcc.h>
#include "menus.h"
#include "level_menu.h"
#include "player.h"
#include "level.h"
#include "globals.h"

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

#define QUEUE_COUNT (((volatile struct dma_queue *)&gVramDmaQueue)->count)
/* Allocated capacity of gVramDmaQueue.entries. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

/* The register pins below (and in several functions further down) match
 * the ROM's own register allocation exactly - required for a byte-exact
 * build, not stylistic. See docs/matching.md, "Matching decompilation"
 * for why plain C alone doesn't reproduce them. */
s32 GetCompletionPercent(void *arg0)
{
    MATCH_HOLD_REG(struct menu_save *, self, r6);
    MATCH_HOLD_REG(s32, total, r4);
    MATCH_HOLD_REG(s32, b, r9);
    MATCH_HOLD_REG(s32, c, r5);
    MATCH_HOLD_REG(s32, d, r8);
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
    flags = self->flags;
    total += flags >> 7;
    total += ((u32)flags << 26) >> 31;
    total += ((u32)flags << 25) >> 31;
    total += ((u32)flags << 27) >> 31;
    return __divsi3(total * 100, 0x48);
}

/* Moves an OAM buffer view on by one shadow entry, so its `table[0]` is
 * the next entry. */
#define NEXT_OAM_ENTRY(buf) ((struct oam_shadow_buffer *)((union oam_shadow_entry *)(buf) + 1))

/* Writes `arg2` affine matrices' scales into the shadow OAM: each
 * matrix's pa/pd from the next two of `arg1`, its pb/pc 0. The ROM keeps
 * the buffer pointer and slides it one entry per store, always writing
 * through `table[0]` (the fixed #0x12 offset), so `view` does the same:
 * a plain entry pointer starts 0xC further on. */
void SetOamAffineScales(void *arg0, u16 *arg1, s32 arg2)
{
    struct oam_shadow_buffer *view;
    u16 zero;

    if (arg2 <= 0) {
        return;
    }
    zero = 0;
    view = arg0;
    do {
        view->table[0].attr[3] = arg1[0];
        view = NEXT_OAM_ENTRY(view);
        view->table[0].attr[3] = zero;
        view = NEXT_OAM_ENTRY(view);
        view->table[0].attr[3] = zero;
        view = NEXT_OAM_ENTRY(view);
        view->table[0].attr[3] = arg1[1];
        view = NEXT_OAM_ENTRY(view);
        arg1 += 2;
        arg2--;
    } while (arg2 != 0);
}

void AppendOamEntries(struct oam_shadow_buffer *arg0, void *arg1, s32 arg2)
{
    if (arg2 == 0) {
        return;
    }
    DMA3.src = (u32)arg1;
    DMA3.dst = (u32)((u8 *)arg0 + ((*(s32 *)arg0 << 3) + 0xC));
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
    MATCH_HOLD_REG(u8 *, self, r1);
    MATCH_HOLD_REG(s32, i, r2);
    MATCH_HOLD_REG(s32, bit, r3);
    MATCH_HOLD_REG(s32, mask, r4);
    MATCH_HOLD_REG(u8, loaded, r5);
    MATCH_HOLD_REG(s32, result, r0);

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
    DMA3.src = (u32)((u8 *)arg0 + 0xC);
    DMA3.dst = OAM;
    DMA3.cnt = ((DMA_ENABLE | DMA_32BIT) << 16) | 0x100;
    (void)DMA3.cnt;
}

/* Inserts one record (arg1[0]/arg1[1]) into the shadow OAM table at the
 * current count, preserving the padding halfword at +0x12 that overlaps
 * the tail of arg1[1] on real hardware (see docs/matching.md). */
void AddOamEntry(struct oam_shadow_buffer *arg0, const void *entry)
{
    MATCH_HOLD_REG(s32, n1, r2);
    MATCH_HOLD_REG(u16, saved, r3);
    MATCH_HOLD_REG(s32, n2, r1);
    MATCH_HOLD_REG(s32, addr2, r0);
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
    v0 = ((const union oam_shadow_entry *)entry)->words[0];
    v1 = ((const union oam_shadow_entry *)entry)->words[1];
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
    MATCH_HOLD_REG(u16, raw, r1);
    MATCH_HOLD_REG(u32, shifted, r0);

    for (i = 0; i < QUEUE_COUNT; i++) {
        entry = &gVramDmaQueue.entries[i];
        if (entry->unit == 0x20) {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
            raw = entry->size;
            shifted = raw >> 2;
            shifted |= (DMA_ENABLE | DMA_32BIT) << 16;
        } else {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
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
    DMA3.src = (u32)src;
    DMA3.dst = (u32)dst;
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
    DMA3.src = (u32)self->slots[index];
    DMA3.dst = OBJ_PLTT + (index << 5);
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void UploadPaletteCache(struct palette_cache *self)
{
    if (self->dirty) {
        DMA3.src = (u32)self->slots;
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
    MATCH_HOLD_REG(struct palette_cache *, pSelf, r2) = self;
    MATCH_HOLD_REG(s32, pRecordId, r5) = recordId;
    MATCH_HOLD_REG(u8 *, remap, r0) = pSelf->slotOf;
    MATCH_HOLD_REG(u8 *, addr, r1);
    u8 slot;
    s32 i;
    const u8 *src;
    u8 *dst;
    u8 *reservedBase;
    MATCH_HOLD_REG(s32, shiftedId, r0);

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
            DMA3.src = (u32)src;
            DMA3.dst = (u32)dst;
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
    MATCH_HOLD_REG(s32, offset, r3);
    MATCH_HOLD_REG(u8 *, dirtyAddr, r1);

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

/* The sprite-bank set's constructor (`gSpriteBankSet`, a 4-byte object
 * InitLevelState allocates, then points at gSpriteBankTable): empty, the
 * `this` pointer passes through in r0. DestroySpriteBankSet above is its
 * destructor. */
void InitSpriteBankSet(void)
{
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* `self` uses the shared `struct actor` layout (see actor.h); `table`
 * is its method table, called through slot 8 with a box around the
 * camera (`buf`, Q8: layer 0's scroll moved back 100/60 px, then
 * 440x280 px).
 *
 * `pSelf` is pinned to r2: this function makes a call, and plain C
 * phrasing left `self` in r3 instead of the ROM's r2 (tried, rebuilt,
 * confirmed different - see docs/matching.md, "Matching decompilation"). */
u8 IsEntityNearCamera(struct actor *self)
{
    MATCH_HOLD_REG(struct actor *, pSelf, r2) = self;
    s32 buf[4];
    struct actor_method *method;
    struct bg_scroll_layer *layer;
    s32 a, b, c, d;
    u8 flag;

    flag = (pSelf->flags >> 4) & 1;
    if (!flag) {
        a = 0xdc << 9;
        b = 0x8c << 9;
        buf[2] = a;
        buf[3] = b;

        layer = gLevelLayers->layer0;
        c = INT_TO_Q8(layer->x) + (s32)0xFFFF9C00;
        d = INT_TO_Q8(layer->y) + (s32)0xFFFFC400;
        buf[0] = c;
        buf[1] = d;

        method = (struct actor_method *)pSelf->table + 8; /* slot 8 */
        flag = (u8)_call_via_r2((u8 *)pSelf + method->thisOffset, buf, method->fn);
    }
    return flag;
}

extern void *_call_via_r1(void *arg0, void *arg1);
extern void _call_via_r4(void *arg0, s32 arg1, s32 arg2, s32 arg3);

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
    const struct actor_method *table2;
    void *addr;
    u8 field0a;
    s32 flagTest;

    table = self->table;
    rec = _call_via_r1((u8 *)self + table[2].delta, table[2].fn);

    x = Q8_TO_INT(self->x);
    rx = rec->offX;
    y = Q8_TO_INT(self->y);
    ry = rec->offY;
    rw = rec->w;
    rh = rec->h;
    x += rx;
    y += ry;
    SetAabbPos(&buf, x, y);
    SetAabbSize(&buf, rw, rh);

    {
        MATCH_HOLD_REG(s32, flagTestR0, r0);
        // clang-format off
        asm volatile(
            "ldrb r1, [%1, #0xc]\n\t"
            "lsr %0, r1, #2\n\t"
            "mov r1, #1\n\t"
            "and %0, %0, r1"
            : "=r"(flagTestR0)
            : "r"(self)
            : "r1");
        // clang-format on
        flagTest = flagTestR0;
    }
    if (flagTest) {
        if (PlayerTouchesBox(gPlayer, &buf)) {
            // clang-format off
            asm volatile(
                "mov r0, #8\n\t"
                "ldrb r2, [%0, #0xc]\n\t"
                "orr r0, r0, r2\n\t"
                "strb r0, [%0, #0xc]"
                :
                : "r"(self)
                : "r0", "r2", "memory");
            // clang-format on

            table2 = &gPlayer->vtable->handleEvent;
            addr = (u8 *)gPlayer + table2->thisOffset;
            field0a = self->kind;
            {
                MATCH_HOLD_REG(void *, deadRead, r4) = *(void *const volatile *)&table2->fn;
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

void UpdateEntity(struct actor *self)
{
    struct actor_method *method = (struct actor_method *)self->table + 1; /* slot 1 */
    _call_via_r1((u8 *)self + method->thisOffset, method->fn);
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
 * this function was pinned. */
s32 IsEntityInsideRect(struct actor *self, struct aabb *box)
{
    MATCH_HOLD_REG(struct actor *, pSelf, r5) = self;
    MATCH_HOLD_REG(struct aabb *, pBox, r6) = box;
    struct vtable_slot *table;
    void *rec;
    u8 flag;
    s32 result;

    flag = (pSelf->flags >> 4) & 1;
    if (!flag) {
        table = pSelf->table;
        rec = _call_via_r1((u8 *)pSelf + table[2].delta, table[2].fn);
        {
            MATCH_HOLD_REG(void *, recR0, r0) = rec;
            MATCH_HOLD_REG(s32, minXR4, r4);
            MATCH_HOLD_REG(s32, maxXR1, r1);
            MATCH_HOLD_REG(s32, minYR5, r5);
            MATCH_HOLD_REG(s32, maxYR3, r3);
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
            // clang-format off
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
            // clang-format on
            result = 0;
            boxX0 = pBox->x;
            if (minXR4 > boxX0) {
                s32 boxX1 = boxX0 + pBox->w;
                if (maxXR1 < boxX1) {
                    MATCH_HOLD_REG(s32, boxY0, r2) = pBox->y;
                    if (minYR5 > boxY0) {
                        MATCH_HOLD_REG(s32, boxY1, r0) = boxY0 + pBox->h;
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

/* `arg0` is unused by the ROM - overwritten as scratch before its
 * incoming value is ever read. The camera position is layer 0's scroll
 * (x/y), sign-extended from its low 24 bits. */
void WorldToScreen(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4)
{
    struct bg_scroll_layer *layer;
    s32 dx, dy;

    layer = gLevelLayers->layer0;
    dx = (layer->x << 8) >> 8;
    dy = (layer->y << 8) >> 8;
    *arg3 = arg1 - dx;
    *arg4 = arg2 - dy;
}

void WorldPosToScreen(s32 *arg0, s32 *arg1, s32 *arg2)
{
    struct bg_scroll_layer *layer;
    s32 x, y;
    s32 subX, subY;

    x = arg0[0];
    y = arg0[1];
    if (x & 0x80) {
        x += 0x80;
    }
    if (y & 0x80) {
        y += 0x80;
    }
    layer = gLevelLayers->layer0;
    subX = INT_TO_Q8(layer->x);
    subY = INT_TO_Q8(layer->y);
    *arg1 = Q8_TO_INT(x - subX);
    *arg2 = Q8_TO_INT(y - subY);
}

void nullsub_12(void)
{
}

struct actor *CreateEntity(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *obj;

    obj = OperatorNew(sizeof(struct actor));
    obj->table = (void *)gEntityVtable;
    ResetEntity(obj);
    obj->id = arg0;
    obj->x = INT_TO_Q8((s32)arg1);
    obj->y = INT_TO_Q8((s32)arg2);
    return obj;
}

s32 GetEntityClassId(void)
{
    return 0;
}

/* Clears self's bit1/bit0/bit3/bit4, sets bit2 - the actor-init step
 * called from CreateEntity. */
void ResetEntity(struct actor *self)
{
    /* `result` and `tmp` are pinned so the running result stays in
     * the constant's own register (r1) rather than the freshly-loaded
     * byte's (r2), matching the ROM's exact register dance - plain C
     * naturally accumulates into the loaded-byte's register instead
     * (see docs/matching.md, "Matching decompilation"). */
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

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

struct actor *InitEntity(struct actor *self)
{
    self->table = (void *)gEntityVtable;
    ResetEntity(self);
    return self;
}

/* `result`/`tmp` pinned so the running result stays in the constant's
 * own register (r1) rather than the freshly-loaded byte's (r2) -
 * same pattern as ResetEntity above (see docs/matching.md, "Matching
 * decompilation"). */
void ClearEntityAlwaysActive(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = -17;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive above. */
void SetEntityAlwaysActive(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = 16;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}

u8 IsEntityAlwaysActive(struct actor *self)
{
    return (self->flags >> 4) & 1;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive above. */
void ClearEntityTouched(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = -9;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched above. */
void SetEntityTouched(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = 8;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}

u8 IsEntityTouched(struct actor *self)
{
    return (self->flags >> 3) & 1;
}

/* Register pins force the ROM's exact register dance: gcc otherwise
 * doesn't move `self` to r1 at all (it can ldrb directly through r0),
 * and separately computes the AND into r1 instead of the constant's
 * own r0 - see docs/matching.md, "Matching decompilation". */
u8 IsEntityGone(struct actor *self)
{
    MATCH_HOLD_REG(struct actor *, pSelf, r1) = self;
    MATCH_HOLD_REG(u8, flags, r1);
    MATCH_HOLD_REG(s32, result, r0) = 1;

    flags = pSelf->flags;
    result = result & flags;
    return result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched above. */
void ClearEntityGone(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = -2;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

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
    MATCH_HOLD_REG(struct actor *, pSelf, r1) = self;
    MATCH_HOLD_REG(u16, id, r4);

    {
        MATCH_HOLD_REG(s32, result, r0);
        MATCH_HOLD_REG(s32, tmp, r2);

        result = 1;
        tmp = pSelf->flags;
        result |= tmp;
        pSelf->flags = result;
    }
    {
        s32 cmpVal = 0xFFFF;
        id = pSelf->id;
        if (id != cmpVal) {
            s32 rawId = pSelf->id;
            void *base = gEntityFlags;
            /* `word` is pinned to r0 and reused as the running
             * accumulator for the rest of the block (word<<5, then
             * rawId-word) - a fresh local for the subtraction's
             * result computed into a different register than the
             * ROM's own reuse of r0. The initial asm copy-then-shift
             * pair forces the ROM's extra `add`/`asr` instead of
             * gcc's one-instruction shift straight out of `rawId`'s
             * register. */
            MATCH_HOLD_REG(s32, word, r0);
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

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone above. */
void DisableEntityContact(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = -5;
    tmp = self->flags;
    result &= tmp;
    self->flags = result;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone/DisableEntityContact above. */
void EnableEntityContact(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = 4;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}

u8 GetEntityFlag1(struct actor *self)
{
    return (self->flags >> 1) & 1;
}

/* Same accumulator-register pattern as ResetEntity/ClearEntityAlwaysActive/
 * SetEntityAlwaysActive/ClearEntityTouched/SetEntityTouched/ClearEntityGone/DisableEntityContact/
 * EnableEntityContact above. */
void ClearEntityFlag1(struct actor *self)
{
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

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
    MATCH_HOLD_REG(s32, result, r1);
    MATCH_HOLD_REG(s32, tmp, r2);

    result = 2;
    tmp = self->flags;
    result |= tmp;
    self->flags = result;
}

s32 GetEntityPixelY(struct actor *self)
{
    return Q8_TO_INT(self->y);
}

s32 GetEntityPixelX(struct actor *self)
{
    return Q8_TO_INT(self->x);
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
    self->x = INT_TO_Q8(arg1);
    self->y = INT_TO_Q8(arg2);
}

void SetEntityPixelPosVec(struct actor *self, s32 *arg1)
{
    SetEntityPixelPos(self, arg1[0], arg1[1]);
}

void SetEntityPos(struct actor *self, s32 arg1, s32 arg2)
{
    self->x = arg1;
    self->y = arg2;
}

void SetEntityPosVec(struct actor *self, s32 *arg1)
{
    SetEntityPos(self, arg1[0], arg1[1]);
}

void SetEntityKind(struct actor *self, u8 arg1)
{
    self->kind = arg1;
}

u8 GetEntityKind(struct actor *self)
{
    return self->kind;
}

u16 GetEntityId(struct actor *self)
{
    return self->id;
}

void DestroyEntity(struct actor *self, u32 arg1)
{
    self->table = (void *)gEntityVtable;
    if (arg1 & 1) {
        OperatorDelete(self);
    }
}
