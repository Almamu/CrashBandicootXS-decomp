#ifndef __GFX_H__
#define __GFX_H__

/* The gfx subsystem (src/gfx/): the OAM shadow buffer, the OBJ VRAM and
 * palette caches, the small `struct actor` entity, brightness fades,
 * DISPCNT helpers, BG packages and the sprite frame cache. Every function
 * src/gfx/ defines, with the prototype of its definition, and the globals
 * and tables its files use (docs/headers_plan.md). HudPart's methods
 * (palette_cycle.cpp) are C++ only (part_list.hpp). A .c file that needs a
 * different local declaration for codegen keeps it as an asm-label alias
 * with a `codegen:` comment. */

#include "core.h"
#include "graphics_package.h"
#include "hitbox_quad.h"
#include "sprite_bank.h"

struct aabb;
struct actor;
struct dma_queue;
struct game_progress;
struct gfx_box_obj;
struct queued_oam_entry;
struct rle_frame;
struct sprite_bank_table;
struct vram_tile_block;

/* One shadow OAM entry: attributes 0-2, then the affine-parameter
 * halfword the hardware interleaves between entries. */
union oam_shadow_entry {
    u32 words[2];
    u16 attr[4]; // [3] is the affine parameter
    struct {
        u8 y;
        u8 affineMode:2; // 0 regular, 1 affine, 2 hidden, 3 affine at double size
        u8 objMode:2;    // struct oam_attrs' names
        u8 mosaic:1;
        u8 bpp:1;
        u8 shape:2;
    } attr0; // byte-wide, as HideUnusedOamEntries stores it
};

/* The OAM shadow buffer (gOamBuffer: class OamBuffer, sprite_obj.hpp,
 * whose `table` is the entries above), the palette cache (gPaletteCache:
 * PaletteCache) and the OBJ VRAM upload cursor (gObjVramCursor:
 * ObjVramCursor). No C file reads their fields: the C callers of their
 * methods (below) only pass the pointers, so the C side has the tags
 * alone. */
struct oam_shadow_buffer;
struct palette_cache;
struct vram_upload_cursor;

/* A sprite frame (sprite_bank.h's struct sprite_frame, GetSpriteFrame) as
 * DrawSpritePieces and DrawAffineSpritePieces read it: one offset and one
 * shape/size id per OBJ piece, the frame's VRAM source and its piece
 * count. codegen: the piece count is read as the `tiles` word's top byte
 * (`ldrb`), which sprite_frame's `u32 tiles` can't express. (Its
 * `struct piece_offset` was a copy of sprite_piece_pos, #656.) */
struct piece_info {
    struct sprite_piece_pos *offsets; // 0x00
    u8 *ids;                          // 0x04 - low 4 bits: shape/size index
    union {
        u32 packed; // 0x08 - low 24 bits: VRAM source offset
        struct {
            u8 src[3];
            u8 count; // 0x0B - piece count
        } b;
    } u;
};

/* `gDispcnt`, the REG_DISPCNT shadow `CommitDispcnt` commits, viewed as
 * its bitfields (field stores give the ROM's byte-wide and/or
 * sequences). */
struct dispcnt_bits {
    u16 mode:3;
    u16 cgbMode:1;
    u16 frame:1;
    u16 hblankOam:1;
    u16 objMap1D:1;
    u16 forcedBlank:1;
    u16 bg0:1;
    u16 bg1:1;
    u16 bg2:1;
    u16 bg3:1;
    u16 obj:1;
    u16 win0:1;
    u16 win1:1;
    u16 objWin:1;
};

/* A REG_DISPCNT value as a halfword or its bitfields: the menus', the
 * level select's, the language select's and the continue prompt's copy of
 * the register, which they commit whole (level_select.hpp's
 * LevelSelectDispcnt, menus.hpp's MenuDispcnt and the classes' anonymous
 * copies were merged into it, #656). In a class it takes a word: a union
 * with a struct is 4-aligned. */
union dispcnt {
    u16 raw;
    struct dispcnt_bits bits;
};

/* One hardware OAM entry as bitfields, as AddOamEntry copies it into the
 * shadow buffer: attributes 0-2, then the affine parameter. The u32
 * storage units of attributes 0-1 give the ROM's word-wide and/or
 * sequences, and the u16 units of attribute 2 mask the tile store with
 * lsl/lsr #22 as in the ROM (a u32 field loads a 0x3ff constant
 * instead). In affine mode `matrixBit3`/`matrixBit4` are the top bits
 * of the matrix number; otherwise they are the h/v flip. DrawSpritePieces
 * and DrawAffineSpritePieces build one on the stack (their local `struct
 * oam_pair`/`oam_attr01` and gfx.h's `oam_attr2` were copies, #574
 * batch 9e), as do the credits' logos; Font::DrawGlyph builds one in the
 * font's `oam_scratch`, and graphics_package.cpp's scaled sprite keeps
 * one. font_glyph.cpp's `struct glyph_oam`, credits.cpp's `struct
 * popup_oam` (byte and halfword units) and graphics_package.cpp's `struct
 * oam_attrs_u16` (halfword units) were copies too; all of their objects
 * compile to the same code with these units (#656). */
struct oam_attrs {
    u32 y:8;          // 0x00
    u32 affineMode:2; // 0x01
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9; // 0x02
    u32 matrixLo:3;
    u32 matrixBit3:1;
    u32 matrixBit4:1;
    u32 size:2;
    u16 tileNum:10; // 0x04
    u16 priority:2;
    u16 palette:4;
    u16 affineParam; // 0x06
};

/* REG_BLDCNT and REG_BLDALPHA as one word of bitfields, and REG_BLDY. */
struct blend_bits {
    u32 bg0First:1; // BLDCNT 1st target
    u32 bg1First:1;
    u32 bg2First:1;
    u32 bg3First:1;
    u32 objFirst:1;
    u32 bdFirst:1;
    u32 effect:2;
    u32 bg0Second:1; // BLDCNT 2nd target
    u32 bg1Second:1;
    u32 bg2Second:1;
    u32 bg3Second:1;
    u32 objSecond:1;
    u32 bdSecond:1;
    u32 unused_14:2; // BLDCNT bits 14-15, unused by the hardware
    u32 eva:5;       // BLDALPHA
    u32 unused_21:3; // BLDALPHA bits 5-7, unused
    u32 evb:5;
    u32 unused_29:3; // BLDALPHA bits 13-15, unused
};

union blend {
    u32 raw;
    struct blend_bits bits;
};

struct bldy {
    u32 evy:5;
    u32 unused_5:27; // BLDY bits 5-31, unused by the hardware
};

/* `gBlendRegs`, the blend register shadow SetupRoomBlend builds and
 * CommitBlendRegs (util/aabb.c) writes: BLDCNT/BLDALPHA as one word, then
 * the BLDY byte. */
struct blend_regs {
    union blend blend; // 0x00
    u8 bldy;           // 0x04
};

/* The brightness fade state (fade.c), set up by FadeBrightness. */
struct brightness_fade {
    s32 period; /* frames per step of StepBrightnessFade; -1 when idle (IsBrightnessFadeActive) */
    s32 callbackId; /* StepBrightnessFade's VBlank callback (AddVBlankCallback) */
    u8 flags;       /* FadeBrightness's flags (FADE_FLAG_*) */
};

/* FadeBrightness's flags. */
#define FADE_FLAG_WHITE 1    /* fade through white (BLDCNT lighten), not black (darken) */
#define FADE_FLAG_IN    0x80 /* count BLDY down from 0x10 (fade in), not up from 0 */

/* One node of the sprite frame cache (sprite_frame.c, sprite_arm.c): the
 * frame record and the OBJ VRAM its pixel data was DMA'd into. Nodes live
 * in a fixed pool (`gSpriteFrameCacheSpares`, seeded by
 * InitSpriteFrameCache) and move between two ring lists as they age:
 * `gSpriteFrameCacheCurrent` (this frame's in-use entries, newest at the
 * head) and `gSpriteFrameCachePrevious` (last frame's entries, oldest at
 * the tail) - see AgeSpriteFrameCache/LoadSpriteFrameTiles. */
struct sprite_frame_cache_node {
    struct sprite_frame_cache_node *next; // 0x00
    struct sprite_frame_cache_node *prev; // 0x04
    u8 *frame;                            // 0x08
    void *vramAddr;                       // 0x0C
};

/* src/gfx/bitmap_screen.c */
extern void ShowBitmapScreen(void *asset, void *palette);

/* src/gfx/display.c */
extern void SetDispcntMode(s32 val);
extern void HideBg3(void);
extern void HideBg2(void);
extern void HideBg1(void);
extern void HideBg0(void);
extern void HideObj(void);
extern void ShowBg3(void);
extern void ShowBg2(void);
extern void ShowBg1(void);
extern void ShowBg0(void);
extern void ShowObj(void);
extern void SetObjMapping2D(void);
extern void SetObjMapping1D(void);
extern void CommitDispcnt(void);

/* src/gfx/fade.c */
extern void StepBrightnessFade(void);
extern void FadeBrightness(u8 flags, s32 frameDelay, u8 sync);
extern void DarkenPalette(s32 factor);

/* src/gfx/fade_to_black.c */
extern void FadePaletteToBlack(void);
extern s32 IsBrightnessFadeActive(void);

/* src/gfx/graphics.cpp: the OAM shadow buffer */
extern s32 GetCompletionPercent(const struct game_progress *progress);
extern void SetOamAffineScales(void *table, u16 *scales, s32 count);
extern void AppendOamEntries(struct oam_shadow_buffer *self, void *entries, s32 count);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *self);
extern void RewindOamBuffer(struct oam_shadow_buffer *self);
extern void ResetOamBuffer(struct oam_shadow_buffer *self);
extern void CommitOamBuffer(struct oam_shadow_buffer *self);

/* src/gfx/graphics.cpp: the VRAM DMA queue and OBJ VRAM cursor */
extern void FlushVramDmaQueue(void);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern void FreeVramDmaQueue(void);
extern s32 AllocVramDmaQueue(void);
extern void ResetObjVram(struct vram_upload_cursor *self);

/* src/gfx/graphics.cpp: the palette cache */
extern void BindPaletteSlot(struct palette_cache *self, s32 slot, s32 index);
extern void UploadPaletteCache(struct palette_cache *self);
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);

/* src/gfx/graphics.cpp: the entity (`struct actor`, actor.h) */
extern void WorldToScreen(void *unused, s32 x, s32 y, s32 *outX, s32 *outY);
extern void WorldPosToScreen(s32 *pos, s32 *outX, s32 *outY);
extern void nullsub_12(void);
#ifndef __cplusplus
/* Entity's out-of-line SetPixelPos and SetPos; the C++ files see them
 * taking an Entity (entity.hpp). */
extern void SetEntityPixelPos(struct actor *self, s32 x, s32 y);
extern void SetEntityPos(struct actor *self, s32 x, s32 y);
#endif

/* src/gfx/graphics_package.cpp */
extern void LoadGraphicsPackage(struct bg_setup *self, const struct bg_package *pkg);
extern u16 GetBgSetupControl(struct bg_setup *self);
extern struct bg_setup *InitBgSetup(struct bg_setup *self, u32 charBlock, u32 screenBlock,
                                    u32 paletteBank, u32 priority);
extern void FitScaledSprite(struct gfx_box_obj *self, s32 width, s32 height);
extern void DrawScaledSprite(struct gfx_box_obj *self);
extern void SetScaledSpriteColor(struct gfx_box_obj *self, s32 color);
extern void SetScaledSpritePriority(struct gfx_box_obj *self, u32 priority);
extern void SetScaledSpritePos(struct gfx_box_obj *self, u32 arg1, u32 arg2);
extern void ResetScaledSpriteAttrs(struct gfx_box_obj *self);

/* src/gfx/sprite_frame.c */
extern void InitObjTileFreeList(void *base);
extern void FreeVramTileBlock(void *addr);
extern void *AllocVramTileBlock(s32 requestedSize);
extern void WalkVramTileBlocks(void);
extern s32 GetFreeVramTileBytes(void);
extern void FreeObjTileFreeList(void);
extern void QueueSpriteFrameOam(u32 attr01, u16 attr2, s32 priority);
extern void FreeSpriteFrameOamQueue(void);
extern void FlushSpriteFrameOamQueue(void);
extern void InitSpriteFrameOamQueue(void);
extern s32 LoadSpriteFrameTiles(u8 *frame);
extern void SetupSpriteFrameOam(u8 *frame, u32 attr01, u32 attr2, s32 priority);
extern void FreeSpriteFrameCache(void);
extern void AgeSpriteFrameCache(void);
extern void InitSpriteFrameCache(void);
extern u32 GetSpriteShapeSizeBits(u8 *frame);
extern void FreeCategorySpriteSheet(void);
extern void DecompressCategorySpriteSheet(const void *sheet);

/* The menus' sky background (src/data/bg_package_16c484.c): the language
 * select, level select, power dialog and save menu load it on BG0. */
extern const struct bg_package gMenuSkyBg;

/* The blend register shadow (sym_iwram.txt). */
extern struct blend_regs gBlendRegs;

/* The master sprite bank table (src/data/sprite_banks_4a5600.c,
 * sprite_bank.h): its palettes seed the palette cache. */
extern const struct sprite_bank_table gSpriteBankTable;

/* The palette cycles (PaletteCycles, part_list.hpp; sym_iwram.txt; NULL
 * outside a level). C++ only: no C file uses them. */
#ifdef __cplusplus
extern class PaletteCycles *gPaletteCycles;
#endif

/* src/iwram/iwram_data.c */
extern struct brightness_fade gBrightnessFade;
extern s32 gBrightnessFadeStep;
extern s32 gBrightnessFadeTimer;
/* The ARM sprite frame routines' hooks (iwram.h). The frame cache's
 * optional frame-source override, called by LoadSpriteFrameTiles
 * through `_call_via_r1`, and the RLE frame unpacker. */
extern s32 (*gLookupSpriteFrameCacheFunc)(u8 *frame);
extern void (*gUnpackRleSpriteFrameFunc)(u16 *dst, struct rle_frame *frame);

/* The OBJ shape/size index as width and height in pixels: as bytes for
 * DrawSpritePieces/DrawAffineSpritePieces (src/data/obj_sizes_16b2e0.c),
 * as s32s for FitScaledSprite/DrawScaledSprite (src/data/map_tables_16c5f0.c). */
extern const u8 gObjPieceHeights[12];
extern const u8 gObjPieceWidths[12];
extern const s32 gObjSizeHeights[12];
extern const s32 gObjSizeWidths[12];

/* sym_iwram.txt: the palette fade buffers (fade.c, fade_to_black.c) */
extern u16 gPaletteBackup[512];
extern u16 gPaletteFadeBuffer[512];

/* sym_iwram.txt: the VRAM DMA queue (graphics.cpp) */
extern struct dma_queue gVramDmaQueue;

/* sym_iwram.txt: the OBJ tile allocator (sprite_frame.c) */
extern struct vram_tile_block *gVramTileBlockPool;   /* pool base */
extern struct vram_tile_block gVramTileBlockList;    /* address-sorted free-block list sentinel */
extern struct vram_tile_block *gVramTileBlockRover;  /* next-fit search cursor ("rover") */
extern struct vram_tile_block *gVramTileBlockSpares; /* spare-record stack head */
/* tile-index -> pool-record-index lookup table, TOTAL_OBJ_TILE_COUNT bytes */
extern u8 *gVramTileBlockIndex;

/* sym_iwram.txt: the overflow OAM queue (sprite_frame.c) */
extern struct queued_oam_entry *gSpriteOamQueue; /* queued OAM entries, OAM_ENTRY_COUNT max */
/* queued affine (x,y) pairs, packed one s16 each into a u32, deduped */
extern s32 *gSpriteAffineQueue;
extern s32 gSpriteOamQueueCount;    /* gSpriteOamQueue count */
extern s32 gSpriteAffineQueueCount; /* gSpriteAffineQueue count */

/* sym_iwram.txt: the sprite frame cache (sprite_frame.c, sprite_arm.c) */
extern struct sprite_frame_cache_node gSpriteFrameCacheCurrent; /* "this frame" MRU list sentinel */
/* "last frame" eviction list sentinel */
extern struct sprite_frame_cache_node gSpriteFrameCachePrevious;
extern struct sprite_frame_cache_node *gSpriteFrameCacheSpares; /* spare-record stack head */
extern struct sprite_frame_cache_node *gSpriteFrameCachePool;   /* pool base */

#endif /* __GFX_H__ */
