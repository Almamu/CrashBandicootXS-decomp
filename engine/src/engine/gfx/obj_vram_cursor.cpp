/* The OBJ VRAM upload cursor (ObjVramCursor, gObjVramCursor;
 * engine/include/game/entities/sprite_obj.hpp). Split from gfx/graphics.cpp (#767), same flags
 * (old_agbcc). */

#include "engine/sys/display.hpp"
#include "engine/sys/vram_dma_queue.hpp"
#include "game/entities/sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Gfx::ObjVramCursor;
using Engine::Sys::Display;
using Engine::Sys::VramDmaQueue;

void ObjVramCursor::Rewind()
{
    offset = mark;
}

void ObjVramCursor::Mark()
{
    mark = offset;
}

/* UNUSED - no caller in the ROM. */
s32 ObjVramCursor::GetFreeBytes()
{
    return OBJ_VRAM0_SIZE - offset;
}

s32 ObjVramCursor::GetTile()
{
    return offset >> 5;
}

void ObjVramCursor::Reset()
{
    offset = mark = baseTile << 5;
}

/* Reserves `size` bytes; returns their first tile, or -1. */
s32 ObjVramCursor::Reserve(s32 size)
{
    if (offset + size <= OBJ_VRAM0_SIZE) {
        s32 tile = GetTile();

        offset += size;
        return tile;
    }
    return -1;
}

/* Queues `size` bytes from `src` for upload (VramDmaQueue::Add);
 * returns their first tile, -1 when VRAM is full, -2 when the queue
 * is. */
s32 ObjVramCursor::Upload(const void *src, s32 size)
{
    if (offset + size <= OBJ_VRAM0_SIZE) {
        if (VramDmaQueue::Add(src, OBJ_VRAM0 + offset, (u16)size, 0x20) == 0) {
            s32 tile = GetTile();

            offset += size;
            return tile;
        }
        return -2;
    }
    return -1;
}

ObjVramCursor::~ObjVramCursor()
{
}

ObjVramCursor::ObjVramCursor(s32 count)
{
    Display::SetObjMapping1D();
    baseTile = count;
    Reset();
    Reset();
}
