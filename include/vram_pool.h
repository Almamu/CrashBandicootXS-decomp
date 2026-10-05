#ifndef __VRAM_POOL_H__
#define __VRAM_POOL_H__

/* gObjVramCursor: a bump allocator over OBJ tile VRAM (OBJ_VRAM0,
 * OBJ_VRAM0_SIZE bytes) for tile data uploaded through the VRAM DMA
 * queue - see src/gfx/graphics.c (InitObjVramCursor and the
 * *ObjVram functions). `offset` is the next free byte, bumped by
 * ReserveObjVram/UploadObjVram; `mark` is a checkpoint MarkObjVram
 * saves and RewindObjVram restores. `baseTile` is the number of tiles
 * kept below the allocator; ResetObjVram starts both cursors there. */
struct vram_upload_cursor {
    u32 mark;
    u32 offset;
    s32 baseTile;
};

/* gPaletteCache: maps a ROM table of `count` 16-colour OBJ palettes
 * (`palettes`, 32 bytes each - the sprite-bank table's 125 palettes)
 * onto the 16 OBJ palette banks. `slotOf[id]` is the bank palette `id`
 * is loaded in, or 0xFF; `slots` holds each bank's colours, uploaded
 * to OBJ_PLTT by UploadPaletteSlot/UploadPaletteCache. `isFree[bank]`
 * marks a bank available to GetPaletteSlot; `locked[bank]` keeps a bank
 * from being reclaimed by FreeUnlockedPaletteSlots (LockPalette/
 * UnlockPalette). See src/gfx/graphics.c. */
struct palette_cache {
    u16 count;                    // 0x00
    u8 pad_02[2];
    const u8 *palettes;             // 0x04
    u8 *slotOf;                      // 0x08
    u8 isFree[16];                 // 0x0C
    u8 locked[16];                   // 0x1C
    u8 slots[16][TILE_SIZE_4BPP];      // 0x2C
    u8 dirty;                           // 0x22C
    u8 pad_22d[3];
};
COMPILE_TIME_ASSERT(vram_pool_h, sizeof(struct palette_cache) == 0x230);

#endif /* __VRAM_POOL_H__ */
