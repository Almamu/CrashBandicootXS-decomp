#include "bg_layer.hpp"
#include "entity.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"

extern "C" {
#include "math_util.h"
#include "gfx.h"
#include "memory.h"
#include "util.h"
#include <libgcc.h>
#include "menus.h"
#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"
#include "level_state.h"
#include "player.h"
#include "level.h"
#include "globals.h"
}

/* The sprite graphics managers (the OAM shadow buffer, the VRAM DMA
 * queue, the OBJ VRAM cursor, the palette cache, the sprite bank set)
 * and the entity base class (#664, include/sprite_obj.hpp,
 * include/entity.hpp). */

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

/* Allocated capacity of gVramDmaQueue.entries. */
#define DMA_QUEUE_MAX_ENTRIES 0x300

/* The save's completion percentage: the crystals, gems, relics (a
 * sapphire counts half) and the four secret flags, out of 72. */
s32 GetCompletionPercent(const struct game_progress *self)
{
    s32 total = CountCrystals(self);
    s32 gems = CountGems(self);
    s32 sapphires = CountSapphireRelics(self);
    s32 golds = CountGoldRelics(self);
    s32 platinums = CountPlatinumRelics(self);
    u8 flags;

    total += gems;
    total += sapphires / 2;
    total += golds;
    total += platinums;
    flags = self->flags;
    total += flags >> 7;
    total += ((u32)flags << 26) >> 31;
    total += ((u32)flags << 25) >> 31;
    total += ((u32)flags << 27) >> 31;
    return __divsi3(total * 100, 0x48);
}

/* Writes `n` affine matrices' scales: each matrix's pa/pd from the next
 * two of `scales`, its pb/pc 0. */
void OamBuffer::SetAffineScales(u16 *scales, s32 n)
{
    s32 i;
    u16 zero;

    if (n <= 0)
        return;
    zero = 0;
    i = 0;
    do {
        table[i++].attr[3] = scales[0];
        table[i++].attr[3] = zero;
        table[i++].attr[3] = zero;
        table[i++].attr[3] = scales[1];
        scales += 2;
        n--;
    } while (n != 0);
}

/* Appends `n` entries (DMA). */
void OamBuffer::Append(void *entries, s32 n)
{
    if (n == 0)
        return;
    DMA3.src = (u32)entries;
    DMA3.dst = (u32)&table[count];
    DMA3.cnt = (n << 1) | ((DMA_ENABLE | DMA_32BIT) << 16);
    (void)DMA3.cnt;
    count += n;
}

/* Hides the entries from `count` on (the OBJ disable bit of attr0). */
void OamBuffer::HideUnused()
{
    s32 i = count;

    if (i > OAM_ENTRY_COUNT - 1)
        return;
    do {
        table[i].attr0.affineMode = 2;
        i++;
    } while (i <= OAM_ENTRY_COUNT - 1);
}

void OamBuffer::Rewind()
{
    count = base;
    matrixCount = 0;
}

void OamBuffer::MarkBase()
{
    base = count;
    matrixCount = 0;
}

void OamBuffer::Reset()
{
    count = 0;
    matrixCount = 0;
    MarkBase();
    Rewind();
}

/* Copies the shadow table to OAM (DMA). */
void OamBuffer::Commit()
{
    DMA3.src = (u32)table;
    DMA3.dst = OAM;
    DMA3.cnt = ((DMA_ENABLE | DMA_32BIT) << 16) | 0x100;
    (void)DMA3.cnt;
}

/* Adds one entry, keeping the affine parameter (attr[3]) of the slot it
 * goes in: that belongs to a matrix. */
void OamBuffer::Add(const void *entry)
{
    s32 n = count;

    if (n > OAM_ENTRY_COUNT - 1)
        return;
    u16 saved = table[n].attr[3];

    table[n] = *(const union oam_shadow_entry *)entry;
    table[count++].attr[3] = saved;
}

OamBuffer::~OamBuffer()
{
}

OamBuffer::OamBuffer()
{
    Reset();
}

/* Runs the queued VRAM transfers, then waits for the last one. */
void FlushVramDmaQueue(void)
{
    s32 i;

    for (i = 0; i < gVramDmaQueue.count; i++) {
        struct dma_queue_entry *entry = &gVramDmaQueue.entries[i];

        if (entry->unit == 0x20) {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
            DMA3.cnt = (entry->size >> 2) | ((DMA_ENABLE | DMA_32BIT) << 16);
        } else {
            DMA3.src = (u32)entry->src;
            DMA3.dst = (u32)entry->dest;
            DMA3.cnt = (entry->size >> 1) | ((DMA_ENABLE | DMA_16BIT) << 16);
        }
        (void)DMA3.cnt;
    }
    gVramDmaQueue.count = 0;

    while (DMA3.cnt & (DMA_ENABLE << 16)) {
    }
}

/* Queues a transfer of `size` bytes in units of `unit` bits: 0 when
 * there is nothing to send, -1 when the queue is full. */
s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit)
{
    struct dma_queue_entry *entry;

    if (size == 0)
        return 0;
    if (gVramDmaQueue.count > DMA_QUEUE_MAX_ENTRIES - 1)
        return -1;
    entry = &gVramDmaQueue.entries[gVramDmaQueue.count];
    gVramDmaQueue.count++;
    entry->dest = dest;
    entry->src = src;
    entry->size = size;
    entry->unit = unit;
    return 0;
}

void FreeVramDmaQueue(void)
{
    if (gVramDmaQueue.entries != 0) {
        mem_free((u8 *)gVramDmaQueue.entries);
        gVramDmaQueue.entries = 0;
    }
}

s32 AllocVramDmaQueue(void)
{
    gVramDmaQueue.entries = (struct dma_queue_entry *)mem_alloc(
        sizeof(struct dma_queue_entry) * DMA_QUEUE_MAX_ENTRIES, MEM_HEAP_EWRAM);
    if (gVramDmaQueue.entries == 0)
        return -1;
    gVramDmaQueue.count = 0;
    return 0;
}

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

/* Queues `size` bytes from `src` for upload (QueueVramDmaTransfer);
 * returns their first tile, -1 when VRAM is full, -2 when the queue
 * is. */
s32 ObjVramCursor::Upload(void *src, s32 size)
{
    if (offset + size <= OBJ_VRAM0_SIZE) {
        if (QueueVramDmaTransfer(src, OBJ_VRAM0 + offset, (u16)size, 0x20) == 0) {
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
    SetObjMapping1D();
    baseTile = count;
    Reset();
    Reset();
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

SpriteBankSet::~SpriteBankSet()
{
}

SpriteBankSet::SpriteBankSet()
{
}

/* Unless always active, whether its IsInsideRect a box around the camera
 * (layer 0's position less 100/60 px, 440x280, in Q8). */
u8 Entity::IsNearCamera()
{
    u8 result = (f.flags >> 4) & 1;

    if (!result) {
        struct aabb near;
        BgLayer *layer;
        s32 w = INT_TO_Q8(440);
        s32 h = INT_TO_Q8(280);

        near.w = w;
        near.h = h;
        layer = gLevelLayers->layer0;
        s32 nx = INT_TO_Q8(layer->x) - INT_TO_Q8(100);
        s32 ny = INT_TO_Q8(layer->y) - INT_TO_Q8(60);

        near.x = nx;
        near.y = ny;
        result = IsInsideRect(&near);
    }
    return result;
}

/* While in contact, its bounds (GetBounds) at its position against the
 * player's (PlayerTouchesBox): on a touch it is marked touched and tells
 * the player (HandleEvent with its kind). */
s32 Entity::CheckPlayerContact()
{
    const struct hitbox_quad *rec = GetBounds();
    struct aabb box;
    s32 bx = Q8_TO_INT(x);
    s32 offX = rec->offX;
    s32 by = Q8_TO_INT(y);
    s32 offY = rec->offY;
    u8 w = rec->w;
    u8 h = rec->h;

    SetAabbPos(&box, bx + offX, by + offY);
    SetAabbSize(&box, w, h);
    s32 inContact = (f.flags >> 2) & 1;

    if (inContact) {
        if (gPlayer->TouchesBox(&box)) {
            f.b.bit3 = 1;
            gPlayer->HandleEvent(0, kind, 0);
        }
    }
    return 0;
}

void Entity::Draw()
{
}

void Entity::Update()
{
    CheckPlayerContact();
}

/* Its size as a box around its position (halfW, halfH, rawW, rawH). */
const struct hitbox_quad *Entity::GetBounds()
{
    return (const struct hitbox_quad *)&halfW;
}

/* `w`/`h` and -w/2, -h/2 (the offsets of a centred box). */
void Entity::SetSize(s32 w, s32 h)
{
    halfW = -w / 2;
    halfH = -h / 2;
    rawW = w;
    rawH = h;
}

s32 Entity::OverlapsRect(struct aabb *)
{
    return 0;
}

u8 Entity::IsOnScreen()
{
    return 0;
}

/* Unless always active, whether its bounds (GetBounds, centred on its
 * position) are inside `box`. */
s32 Entity::IsInsideRect(struct aabb *box)
{
    u8 result = (f.flags >> 4) & 1;

    if (!result) {
        const struct hitbox_quad *rec = GetBounds();
        s32 hw = rec->w << 7;
        s32 hh = rec->h << 7;
        s32 x0 = x - hw;
        s32 y0 = y - hh;
        s32 x1 = x + hw;
        s32 y1 = y + hh;
        s32 inside = 0;

        if (x0 > box->x && x1 < box->x + box->w && y0 > box->y && y1 < box->y + box->h)
            inside = 1;
        result = inside;
    }
    return result;
}

/* (x, y) relative to the camera (layer 0's position, sign-extended from
 * its low 24 bits). `unused` is not read. */
void WorldToScreen(void *unused, s32 x, s32 y, s32 *outX, s32 *outY)
{
    BgLayer *layer = gLevelLayers->layer0;
    s32 dx = (layer->x << 8) >> 8;
    s32 dy = (layer->y << 8) >> 8;

    *outX = x - dx;
    *outY = y - dy;
}

/* A Q8 position (rounded) relative to the camera, in pixels. */
void WorldPosToScreen(s32 *pos, s32 *outX, s32 *outY)
{
    BgLayer *layer;
    s32 x = pos[0];
    s32 y = pos[1];

    if (x & 0x80)
        x += 0x80;
    if (y & 0x80)
        y += 0x80;
    layer = gLevelLayers->layer0;
    s32 cx = INT_TO_Q8(layer->x);
    s32 cy = INT_TO_Q8(layer->y);

    *outX = Q8_TO_INT(x - cx);
    *outY = Q8_TO_INT(y - cy);
}

/* UNUSED - empty, no caller in the ROM. */
void nullsub_12(void)
{
}

/* The constructor, inline here only: Create inlines it, and the other
 * classes' constructors call it (InitEntity). Its out-of-line copy is the
 * first of the inline methods' at the end of the file. */
inline Entity::Entity()
{
    Reset();
}

/* A new entity at the spawn's position. */
Entity *Entity::Create(u16 id, u16 x, u16 y, u16)
{
    Entity *e = new Entity;

    e->id = id;
    e->x = INT_TO_Q8((s32)x);
    e->y = INT_TO_Q8((s32)y);
    return e;
}

s32 Entity::GetClassId()
{
    return 0;
}

/* Out of contact, no longer touched or always active, the flag-1 bit and
 * gone cleared, contact enabled; a 1x1 size. */
void Entity::Reset()
{
    f.b.unk_1 = 0;
    f.b.visible = 1;
    f.b.gone = 0;
    f.b.bit3 = 0;
    f.b.active = 0;
    halfW = 0;
    halfH = 0;
    rawW = 1;
    rawH = 1;
}
