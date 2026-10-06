#include "core.h"
#include "match.h"
#include "gfx.h"
#include "memory.h"
#include "level.h"

/* GitHub issue #43: BG layer 0 of the level-layers singleton
 * (`level_layers.c`) and the VRAM tile-slot pool it owns.
 *
 * Layer 0 is a BG-scroll layer (`InitBgLayer`) extended with a
 * `struct tile_slot_pool` at `+0x5C`: `InitPooledBgLayer` constructs it
 * (base constructor, own method table `gPooledBgLayerVtable`, sets
 * `+0x34` bit 7 and clears bits 2-3, allocates the pool), `DestroyPooledBgLayer`
 * destroys it (frees the pool, restores the base table
 * `gBgLayerVtable`, chains to the base destructor `DestroyBgLayerBase`).
 * `GetPooledBgLayerPriority` reads `+0x34` bits 0-1 (the BG priority).
 *
 * The pool maps up to 0x2000 source tiles onto 0x200 reference-counted
 * VRAM tile slots:
 * - `ResetTileSlotPool` - reset: every slot free, every source tile
 *   non-resident, all counts zero.
 * - `AcquireTileSlot(pool, tile)` - acquire: if source tile `tile & 0x3FFF`
 *   isn't resident, pops a free slot and queues a 64-byte (8bpp tile)
 *   VRAM DMA of it via `UploadTileSlot`; bumps the slot's count and returns
 *   a BG map entry: the slot number with `tile`'s top two bits moved to
 *   bits 10-11 (the map entry's flip bits). Upper bits of the returned
 *   entry are never set.
 * - `ReleaseTileSlot(pool, tile)` - release: drops the count and returns the
 *   slot to the free stack when it reaches zero.
 * - `SetTileSlotPoolSource(pool, charBase, src)` - sets the VRAM destination to
 *   character base block `charBase` and the source tile data.
 *
 * Matching notes: the free-stack push and the slot-table accessors are
 * small `static inline` helpers - that is what makes the ROM recompute
 * `pool + 0x4808`/`pool + 0x408` instead of reusing one address
 * register (plain inline code CSEs them). The tile reference and the
 * returned map entry are 4-byte unions of a `u16` and a `u32` bitfield
 * struct, reproducing the ROM's register-held `& 0xFFFF0000 | tile`
 * and `lsl #18/lsr #18`, `lsl #16/lsr #30` field extraction. The
 * refcount updates use an r1-pinned temp, `InitPooledBgLayer`'s `+0x34`
 * update is a narrow inline-asm block (same case as `InitBgLayer`), and
 * `AcquireTileSlot` needs two more (constant-before-load for the residency
 * test, and its refcount update) - see the comments there. See
 * docs/matching/archive/issue-43-level-layers.md.
 *
 * Real bytes formerly the tail of `asm/code_3_2_17_25fc8.s` (that file
 * now ends at `nullsub_26`). */

struct tile_slot_pool {
    u32 vramBase;            // 0x0000
    u32 srcBase;             // 0x0004
    u16 refCount[0x200];     // 0x0008
    u16 slotForTile[0x2000]; // 0x0408 - TILE_SLOT_NONE when not resident
    u16 freeSlots[0x200];    // 0x4408
    s32 freeTop;             // 0x4808
};

#define TILE_SLOT_NONE 0x200

union tile_ref {
    u16 raw;
    struct {
        u32 id:14;
        u32 flip:2;
    } bits;
};

union bg_entry {
    u16 raw;
    struct {
        u32 tile:10;
        u32 flip:2;
        u32 palette:4;
    } bits;
};

struct pooled_layer {
    u8 unk_00[0x30]; // 0x00 - BG-scroll-layer base (InitBgLayer)
    void *vtable;    // 0x30
    u8 priority:2;   // 0x34 - the BGnCNT shadow (struct bg_scroll_layer.cnt)
    u8 charBase:2;
    u8 bits4_6:3;
    u8 colors256:1;
    u8 unk_35[0x27];             // 0x35
    struct tile_slot_pool *pool; // 0x5C
};

static inline void PushFreeSlot(struct tile_slot_pool *pool, s32 slot)
{
    pool->freeSlots[--pool->freeTop] = slot;
}

static inline void ClearTileSlot(struct tile_slot_pool *pool, s32 id)
{
    pool->slotForTile[id] = TILE_SLOT_NONE;
}

static inline u16 PopFreeSlot(struct tile_slot_pool *pool)
{
    return pool->freeSlots[pool->freeTop++];
}

static inline void SetTileSlot(struct tile_slot_pool *pool, s32 id, u16 slot)
{
    pool->slotForTile[id] = slot;
}

static inline u16 GetTileSlot(struct tile_slot_pool *pool, s32 id)
{
    return pool->slotForTile[id];
}

void DestroyPooledBgLayer(struct pooled_layer *self, u32 flags)
{
    self->vtable = (void *)gPooledBgLayerVtable;
    if (self->pool != NULL)
        OperatorDelete(self->pool);
    self->vtable = (void *)gBgLayerVtable;
    DestroyBgLayerBase(self, flags);
}

struct pooled_layer *InitPooledBgLayer(struct pooled_layer *self, s32 bgIndex)
{
    InitBgLayer(self, bgIndex);
    self->vtable = (void *)gPooledBgLayerVtable;
    {
        /* colors256 = 1, charBase = 0. This compiler folds both masks to
         * immediates; the ROM keeps the `& 0x7f` and derives `-0xd` from
         * the `0x80` register (`subs #0x8d`) - same class as
         * InitBgLayer's +0x34/+0x35 updates (bg_layer_init.c). */
        MATCH_HOLD_REG(u8 *, bits, r2) = (u8 *)self + 0x34;
        // clang-format off
        asm volatile(
            "mov r1, #0x80\n\t"
            "mov r0, #0x7f\n\t"
            "ldrb r3, [%0]\n\t"
            "and r0, r0, r3\n\t"
            "orr r0, r0, r1\n\t"
            "sub r1, #0x8d\n\t"
            "and r0, r0, r1\n\t"
            "strb r0, [%0]\n\t"
            : : "r"(bits) : "r0", "r1", "r3", "memory");
        // clang-format on
    }
    self->pool = OperatorNew(sizeof(struct tile_slot_pool));
    return self;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * method tables). Layer 0's BG priority, BGnCNT bits 0-1. */
u32 GetPooledBgLayerPriority(struct pooled_layer *self)
{
    return self->priority;
}

void ResetTileSlotPool(struct tile_slot_pool *pool)
{
    s32 i;

    pool->freeTop = TILE_SLOT_NONE;
    for (i = 0; i < 0x200; i++)
        PushFreeSlot(pool, i);
    for (i = 0; i < 0x2000; i++)
        pool->slotForTile[i] = TILE_SLOT_NONE;
    for (i = 0; i < 0x200; i++)
        pool->refCount[i] = 0;
}

u16 AcquireTileSlot(struct tile_slot_pool *pool, u16 tile)
{
    union tile_ref ref;
    union bg_entry out;
    u16 slot;

    ref.raw = tile;
    {
        s32 id = ref.bits.id;
        MATCH_HOLD_REG(u32, cur, r0);
        MATCH_HOLD_REG(s32, none, r1);

        /* The ROM materializes 0x200 before loading the entry; gcc always
         * loads a compare's memory operand first. The "m" operand keeps
         * the address computation (and its ordering) in the compiler's
         * hands - only the constant and the load are fixed here. */
        // clang-format off
        asm("mov %1, #0x80\n\tlsl %1, %1, #2\n\tldrh %0, %2"
            : "=r"(cur), "=&r"(none)
            : "m"(pool->slotForTile[id]));
        // clang-format on
        if (cur != none)
            slot = GetTileSlot(pool, id);
        else {
            slot = PopFreeSlot(pool);
            SetTileSlot(pool, id, slot);
            UploadTileSlot(pool, id, slot);
        }
    }
    {
        /* A plain `pool->refCount[slot]++` swaps r0/r1 against the ROM, and
         * pinning the count to r1 instead swaps r4/r5 for the whole
         * function; fixing just the three-instruction update closes both. */
        u32 c;

        asm("ldrh %0, %1\n\tadd %0, #1\n\tstrh %0, %1" : "=&l"(c), "+m"(pool->refCount[slot]));
    }
    out.raw = slot;
    out.bits.flip = ref.bits.flip;
    return out.raw;
}

void ReleaseTileSlot(struct tile_slot_pool *pool, u32 tile)
{
    s32 id = tile & 0x3FFF;
    u16 slot = pool->slotForTile[id];
    MATCH_HOLD_REG(u16, count, r1);

    count = pool->refCount[slot] - 1;
    pool->refCount[slot] = count;
    if (count == 0) {
        PushFreeSlot(pool, slot);
        ClearTileSlot(pool, id);
    }
}

void UploadTileSlot(struct tile_slot_pool *pool, s32 tileId, s32 slot)
{
    QueueVramDmaTransfer((void *)(pool->srcBase + tileId * 64),
                         (void *)(pool->vramBase + slot * 64), 0x40, 0x10);
}

void SetTileSlotPoolSource(struct tile_slot_pool *pool, s32 charBase, u32 src)
{
    pool->vramBase = VRAM + (charBase << 14);
    pool->srcBase = src;
}

asm(".align 2, 0");
