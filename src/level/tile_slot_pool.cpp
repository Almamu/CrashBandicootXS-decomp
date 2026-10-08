#include "bg_layer.hpp"

extern "C" {
#include "gfx.h"
#include "memory.h"
}

/* GitHub issue #43: BG layer 0 of the level-layers singleton
 * (`level_layers.cpp`) and the VRAM tile-slot pool it owns.
 *
 * Layer 0 is a PooledBgLayer (include/bg_layer.hpp): a BgLayer
 * extended with a TileSlotPool at `+0x5C`. Its constructor sets the
 * 256-colour bit and char base 0 and allocates the pool, its destructor
 * frees it and then expands BgLayer's inline one. The destructor is the
 * class's key method: g++ emits gPooledBgLayerVtable here. `GetPriority`
 * reads the BG priority.
 *
 * The pool (TileSlotPool, include/bg_layer.hpp; a class since #752) maps
 * up to 0x2000 source tiles onto 0x200 reference-counted VRAM tile slots:
 * - `Reset` (ResetTileSlotPool) - every slot free, every source tile
 *   non-resident, all counts zero.
 * - `Acquire(tile)` (AcquireTileSlot) - if source tile `tile & 0x3FFF`
 *   isn't resident, pops a free slot and queues a 64-byte (8bpp tile)
 *   VRAM DMA of it via `Upload`; bumps the slot's count and returns a BG
 *   map entry: the slot number with `tile`'s top two bits moved to bits
 *   10-11 (the map entry's flip bits). Upper bits of the returned entry
 *   are never set.
 * - `Release(tile)` (ReleaseTileSlot) - drops the count and returns the
 *   slot to the free stack when it reaches zero.
 * - `SetSource(charBase, src)` (SetTileSlotPoolSource) - sets the VRAM
 *   destination to character base block `charBase` and the source tile
 *   data.
 *
 * Matching notes: the free-stack push and the slot-table accessors are
 * small inline methods - that is what makes the ROM recompute
 * `this + 0x4808`/`this + 0x408` instead of reusing one address register
 * (plain inline code CSEs them). The tile reference and the returned map
 * entry are 4-byte unions of a `u16` and a `u32` bitfield struct,
 * reproducing the ROM's register-held `& 0xFFFF0000 | tile` and
 * `lsl #18/lsr #18`, `lsl #16/lsr #30` field extraction. Built with
 * old_agbcp (the Makefile's OLD_AGBCC_OBJS). As C it needed an asm block
 * for the constructor's BGnCNT update, two in `AcquireTileSlot` and a pin
 * in `ReleaseTileSlot`; the C++ needs none. See
 * docs/matching/archive/issue-43-level-layers.md.
 *
 * Real bytes formerly the tail of `asm/code_3_2_17_25fc8.s` (that file
 * now ends at `nullsub_26`). */

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

PooledBgLayer::~PooledBgLayer()
{
    if (pool != NULL)
        delete pool;
}

/* Through BgLayer's inline setters, each field store is a general insert
 * (the field cleared, then the value ORed in) even with a constant: the
 * ROM's `& 0x7f` before the `| 0x80`. */
PooledBgLayer::PooledBgLayer(s32 bgIndex) : BgLayer(bgIndex)
{
    SetColors256(1);
    SetCharBase(0);
    pool = new TileSlotPool;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * method tables). Layer 0's BG priority, BGnCNT bits 0-1. */
u32 PooledBgLayer::GetPriority()
{
    return cnt.bits.priority;
}

void TileSlotPool::Reset()
{
    s32 i;

    freeTop = TILE_SLOT_NONE;
    for (i = 0; i < 0x200; i++)
        PushFreeSlot(i);
    for (i = 0; i < 0x2000; i++)
        slotForTile[i] = TILE_SLOT_NONE;
    for (i = 0; i < 0x200; i++)
        refCount[i] = 0;
}

u16 TileSlotPool::Acquire(u16 tile)
{
    union tile_ref ref;
    union bg_entry out;
    u16 slot;

    ref.raw = tile;
    {
        s32 id = ref.bits.id;

        if (slotForTile[id] != TILE_SLOT_NONE)
            slot = GetSlot(id);
        else {
            slot = PopFreeSlot();
            SetSlot(id, slot);
            Upload(id, slot);
        }
    }
    refCount[slot]++;
    out.raw = slot;
    out.bits.flip = ref.bits.flip;
    return out.raw;
}

void TileSlotPool::Release(u32 tile)
{
    s32 id = tile & 0x3FFF;
    u16 slot = slotForTile[id];

    if (--refCount[slot] == 0) {
        PushFreeSlot(slot);
        ClearSlot(id);
    }
}

void TileSlotPool::Upload(s32 tileId, s32 slot)
{
    QueueVramDmaTransfer((void *)(srcBase + tileId * 64), (void *)(vramBase + slot * 64), 0x40,
                         0x10);
}

void TileSlotPool::SetSource(s32 charBase, u32 src)
{
    vramBase = VRAM + (charBase << 14);
    srcBase = src;
}
