/* The OBJ palette cache (PaletteCache, gPaletteCache;
 * include/sprite_obj.hpp). Split from gfx/graphics.cpp (#767), same flags
 * (old_agbcc). */

#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "globals.h"
}

/* Loads palette `id` into bank `slot` (DMA into `slots`). */
void PaletteCache::LoadSlot(s32 slot, s32 id)
{
    const u8 *src;
    u8 *dst;

    slotOf[id] = slot;
    dirty = 1;
    src = palettes;
    dst = slots[0];
    src += id << 5;
    dst += slot << 5;
    DMA3.src = (u32)src;
    DMA3.dst = (u32)dst;
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void PaletteCache::BindSlot(s32 slot, s32 id)
{
    slotOf[id] = slot;
    isFree[slot] = 0;
}

s32 PaletteCache::ClaimSlot(s32 slot)
{
    if (isFree[slot] == 0)
        return 0;
    isFree[slot] = 0;
    return 1;
}

void PaletteCache::Unlock(s32 id)
{
    if (slotOf[id] != 0xFF)
        locked[slotOf[id]] = 0;
}

void PaletteCache::Lock(s32 id)
{
    if (slotOf[id] != 0xFF)
        locked[slotOf[id]] = 1;
}

void PaletteCache::UploadSlot(s32 slot)
{
    DMA3.src = (u32)slots[slot];
    DMA3.dst = OBJ_PLTT + (slot << 5);
    DMA3.cnt = (DMA_ENABLE << 16) | 16;
    (void)DMA3.cnt;
}

void PaletteCache::Upload()
{
    if (dirty) {
        DMA3.src = (u32)slots;
        DMA3.dst = OBJ_PLTT;
        DMA3.cnt = (DMA_ENABLE << 16) | 0x100;
        (void)DMA3.cnt;
    }
}

/* The bank palette `id` is in; if none, it is loaded into the first free
 * bank (0 when there is none). */
u8 PaletteCache::GetSlot(s32 id)
{
    s32 i;

    if (slotOf[id] != 0xFF)
        return slotOf[id];
    dirty = 1;
    for (i = 0; i <= 15; i++) {
        if (isFree[i] != 0) {
            const u8 *src;
            u8 *dst;

            isFree[i] = 0;
            slotOf[id] = i;
            src = palettes;
            dst = slots[0];
            src += id << 5;
            dst += i << 5;
            DMA3.src = (u32)src;
            DMA3.dst = (u32)dst;
            DMA3.cnt = (DMA_ENABLE << 16) | 16;
            (void)DMA3.cnt;
            return i;
        }
    }
    return 0;
}

/* UNUSED - no caller in the ROM. Frees bank `slot` unless it is locked. */
s32 PaletteCache::FreeSlot(s32 slot)
{
    s32 i;
    s32 result;

    if (locked[slot] == 0) {
        isFree[slot] = 1;
        for (i = 0; i < count; i++) {
            if (slotOf[i] == slot)
                slotOf[i] = 0xFF;
        }
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void PaletteCache::FreeUnlockedSlots()
{
    s32 slot;
    s32 i;

    for (slot = 0; slot <= 15; slot++) {
        if (locked[slot] == 0) {
            isFree[slot] = 1;
            for (i = 0; i < count; i++) {
                if (slotOf[i] == slot)
                    slotOf[i] = 0xFF;
            }
        }
    }
}

/* Clears the cache and sets its palettes. */
void PaletteCache::SetSource(u16 n, const u8 *records)
{
    s32 i;

    if (slotOf != 0)
        delete slotOf;
    slotOf = 0;
    count = 0;
    palettes = 0;
    for (i = 0; i <= 15; i++) {
        isFree[i] = 1;
        locked[i] = 0;
    }
    count = n;
    palettes = records;
    slotOf = new u8[count];
    for (i = 0; i < count; i++)
        slotOf[i] = 0xFF;
}

void PaletteCache::Clear()
{
    s32 i;

    if (slotOf != 0)
        delete slotOf;
    slotOf = 0;
    count = 0;
    palettes = 0;
    for (i = 0; i <= 15; i++) {
        isFree[i] = 1;
        locked[i] = 0;
    }
}

PaletteCache::~PaletteCache()
{
    Clear();
}

PaletteCache::PaletteCache()
{
    count = 0;
    slotOf = 0;
    palettes = 0;
    dirty = 0;
}
