#ifndef __VRAM_POOL_H__
#define __VRAM_POOL_H__

/* A bump/rollback cursor pair over a fixed OBJ_VRAM0_SIZE-byte staging
 * window used to upload freshly-decompressed tile data - see
 * src/graphics/graphics.c for the functions operating on it.
 * `field_04` is the "next write offset" cursor, bumped forward by
 * every reserve; `field_00` is a separately-settable checkpoint the
 * caller can copy `field_04` into/out of - a manual commit/rollback
 * pair, though only one caller of either has been found so far.
 * `field_08` is a slot count, used only by the constructor to seed
 * both cursors past already-reserved space. */
struct vram_upload_cursor {
    u32 field_00;
    u32 field_04;
    s32 field_08;
};

/* A 16-slot raw-asset cache: `slots[i]` is a generic TILE_SIZE_4BPP-byte
 * (32-byte) buffer, DMA'd out either as one OBJ tile (into OBJ_VRAM0)
 * or as one 16-color OBJ palette bank (into OBJ_PLTT) depending on
 * which function the caller invokes - see src/graphics/graphics.c for
 * the functions operating on it. `records` points at a ROM array of
 * `count` 32-byte source records (the "logical" assets); `remap[id]`
 * gives the slot currently holding record `id`'s data, or 0xFF if not
 * cached. `reserved[slot]`/`pending[slot]` are two free-list flag
 * arrays - see sub_8006DF8 (allocate-or-reuse-cached) and
 * sub_8006D68/sub_8006D84 (pin/unpin a slot against reuse). */
struct tile_asset_cache {
    u16 count;                    // 0x00
    u8 pad_02[2];
    const u8 *records;             // 0x04
    u8 *remap;                      // 0x08
    u8 reserved[16];                 // 0x0C
    u8 pending[16];                   // 0x1C
    u8 slots[16][TILE_SIZE_4BPP];      // 0x2C
    u8 dirty;                           // 0x22C
    u8 pad_22d[3];
};
COMPILE_TIME_ASSERT(sizeof(struct tile_asset_cache) == 0x230);

#endif /* __VRAM_POOL_H__ */
