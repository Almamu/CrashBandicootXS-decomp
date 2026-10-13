#ifndef __GFX_H__
#define __GFX_H__

/* The gfx subsystem (src/gfx/): the OAM shadow buffer, the OBJ VRAM and
 * palette caches, the entity (class Entity, entity.hpp), brightness fades,
 * DISPCNT helpers, BG packages and the sprite frame cache. The C linkage
 * functions src/gfx/ defines, with the prototype of their definition, and
 * the globals and tables its files use (docs/headers.md); the
 * classes' methods are declared in their C++ headers (sprite_obj.hpp,
 * entity.hpp). HudPart's methods
 * (palette_cycle.cpp) are C++ only (part_list.hpp). A .c file that needs a
 * different local declaration for codegen keeps it as an asm-label alias
 * with a `codegen:` comment. */

#include "core.h"
#include "hitbox_quad.h"
#include "sprite_bank.h"

/* Palette RAM as colours, as the REG_* macros give the I/O registers:
 * BG palette `n` and OBJ palette `n` (16 colours each, the 256-colour
 * palettes starting at palette 0), and the backdrop colour (BG colour 0).
 * BG_PLTT/OBJ_PLTT stay addresses for the DMA registers. */
#define BG_PALETTE(n)  ((u16 *)(BG_PLTT + (n) * PALETTE_SIZE_16))
#define OBJ_PALETTE(n) ((u16 *)(OBJ_PLTT + (n) * PALETTE_SIZE_16))
#define BACKDROP_COLOR (*(vu16 *)PLTT)

struct aabb;
struct dma_queue;
struct rle_frame;
struct sprite_bank_table;

/* attr0 bits 8-9, the OBJ's affine mode (`affineMode` in the shadow
 * entry and in struct oam_attrs; #843, docs/flags/oam.md): the GBA
 * hardware's rotation/scaling flag (bit 8), and bit 9, which is double
 * size with it and OBJ disable without it. */
#define OAM_AFFINE_MODE_NORMAL 0 // a regular OBJ: attr1 bits 12-13 are the h/v flip
#define OAM_AFFINE_MODE_AFFINE 1 // rotated/scaled by the affine matrix attr1 bits 9-13 pick
#define OAM_AFFINE_MODE_HIDDEN 2 // not displayed (OamBuffer::HideUnused)
#define OAM_AFFINE_MODE_DOUBLE 3 // affine, drawn in a box twice the OBJ's size

/* attr0 bits 10-11, the OBJ mode (`objMode`; Sprite's `mirrorBits.gfxMode`,
 * which DrawSpritePieces copies into it; docs/flags/oam.md). Hardware
 * values. The game sets only the first two. */
#define OAM_OBJ_MODE_NORMAL           0
#define OAM_OBJ_MODE_SEMI_TRANSPARENT 1 // always alpha-blended, as a first target
#define OAM_OBJ_MODE_WINDOW           2 // the OBJ window's mask, not drawn
#define OAM_OBJ_MODE_UNK3             3 // prohibited on the GBA

/* One shadow OAM entry: attributes 0-2, then the affine-parameter
 * halfword the hardware interleaves between entries. */
union oam_shadow_entry {
    u32 words[2];
    u16 attr[4]; // [3] is the affine parameter
    struct {
        u8 y;
        u8 affineMode:2; // OAM_AFFINE_MODE_*
        u8 objMode:2;    // OAM_OBJ_MODE_*
        u8 mosaic:1;
        u8 bpp:1;
        u8 shape:2;
    } attr0; // byte-wide, as HideUnusedOamEntries stores it
};

/* The OAM shadow buffer (gOamBuffer: class OamBuffer, sprite_obj.hpp,
 * whose `table` is the entries above), the palette cache (gPaletteCache:
 * PaletteCache) and the OBJ VRAM upload cursor (gObjVramCursor:
 * ObjVramCursor) are C++ classes with no C view. */

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

/* REG_DISPCNT as its bitfields (field stores give the ROM's byte-wide
 * and/or sequences): Display::dispcnt (gDispcnt, display.hpp), the shadow
 * that Display::CommitDispcnt commits. */
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
 * font's `oam_scratch`, and scaled_sprite.cpp's scaled sprite keeps
 * one. font_glyph.cpp's `struct glyph_oam`, credits.cpp's `struct
 * popup_oam` (byte and halfword units) and graphics_package.cpp's `struct
 * oam_attrs_u16` (halfword units) were copies too; all of their objects
 * compile to the same code with these units (#656). */
struct oam_attrs {
    u32 y:8;          // 0x00
    u32 affineMode:2; // 0x01 - OAM_AFFINE_MODE_*
    u32 objMode:2;    // OAM_OBJ_MODE_*
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
    u32:2;     // BLDCNT bits 14-15, unused by the hardware
    u32 eva:5; // BLDALPHA
    u32:3;     // BLDALPHA bits 5-7, unused
    u32 evb:5;
    // BLDALPHA bits 13-15 are unused
};

union blend {
    u32 raw;
    struct blend_bits bits;
};

struct bldy {
    u32 evy:5; // bits 5-31 are unused by the hardware
};

/* Display::blendRegs (gBlendRegs, display.hpp), the blend register shadow
 * SetupRoomBlend builds and Display::CommitBlendRegs writes: BLDCNT/BLDALPHA as one word, then
 * the BLDY byte. */
struct blend_regs {
    union blend blend; // 0x00
    u8 bldy;           // 0x04
};

/* The brightness fade state (fade.cpp), set up by FadeBrightness. */
struct brightness_fade {
    s32 period; /* frames per step of StepBrightnessFade; -1 when idle (IsBrightnessFadeActive) */
    s32 callbackId; /* StepBrightnessFade's VBlank callback (AddVBlankCallback) */
    u8 flags;       /* FadeBrightness's flags (FADE_FLAG_*) */
};

/* FadeBrightness's flags. */
#define FADE_FLAG_WHITE 1    /* fade through white (BLDCNT lighten), not black (darken) */
#define FADE_FLAG_IN    0x80 /* count BLDY down from 0x10 (fade in), not up from 0 */

/* One node of the sprite frame cache (SpriteFrameCache,
 * sprite_frame_cache.hpp): the frame record and the OBJ VRAM its pixel
 * data was DMA'd into. Nodes live in a fixed pool (`pool`, its unused ones
 * on the `spares` stack) and move between the two ring lists `current`
 * and `previous` as they age. */
struct sprite_frame_cache_node {
    struct sprite_frame_cache_node *next; // 0x00
    struct sprite_frame_cache_node *prev; // 0x04
    u8 *frame;                            // 0x08
    void *vramAddr;                       // 0x0C
};

/* src/gfx/bitmap_screen.cpp */
extern void ShowBitmapScreen(void *asset, void *palette);

/* src/gfx/fade.cpp */
extern void StepBrightnessFade(void);
extern void FadeBrightness(u8 flags, s32 frameDelay, u8 sync);
extern void DarkenPalette(s32 factor);

/* src/gfx/fade_to_black.cpp */
extern void FadePaletteToBlack(void);
extern bool IsBrightnessFadeActive(void);

/* src/gfx/vram_dma_queue.cpp */
extern void FlushVramDmaQueue(void);
extern s32 QueueVramDmaTransfer(const void *src, void *dest, u16 size, u16 unit);
extern void FreeVramDmaQueue(void);
extern s32 AllocVramDmaQueue(void);

/* src/objects/entity.cpp: the world-to-screen helpers between Entity's methods */
extern void WorldToScreen(void *unused, s32 x, s32 y, s32 *outX, s32 *outY);
extern void WorldPosToScreen(s32 *pos, s32 *outX, s32 *outY);
extern void nullsub_12(void);

/* src/gfx/sprite_frame.cpp (the OBJ tile allocator, the OAM queue, the
 * frame cache and the category sprite sheet: obj_tile_allocator.hpp,
 * sprite_oam_queue.hpp, sprite_frame_cache.hpp, category_sprite_sheet.hpp) */
extern void SetupSpriteFrameOam(u8 *frame, u32 attr01, u32 attr2, s32 priority);
extern u32 GetSpriteShapeSizeBits(u8 *frame);

/* The menus' sky background (src/data/bg_package_16c484.cpp): the language
 * select, level select, power dialog and save menu load it on BG0. Its
 * type is graphics_package.hpp's. */
struct bg_package;
extern const struct bg_package gMenuSkyBg;

/* The master sprite bank table (src/data/sprite_banks_4a5600.cpp,
 * sprite_bank.h): its palettes seed the palette cache. */
extern const struct sprite_bank_table gSpriteBankTable;

/* The palette cycles (PaletteCycles, part_list.hpp; sym_iwram.txt; NULL
 * outside a level). C++ only: no C file uses them. */
#ifdef __cplusplus
extern class PaletteCycles *gPaletteCycles;
#endif

/* src/iwram/iwram_data.cpp */
extern struct brightness_fade gBrightnessFade;
extern s32 gBrightnessFadeStep;
extern s32 gBrightnessFadeTimer;
/* The ARM sprite frame routines' hooks (iwram.h). The frame cache's
 * lookup (SpriteFrameCache::Lookup), called through the pointer by
 * SpriteFrameCache::LoadTiles, and the RLE frame unpacker. */
extern s32 (*gLookupSpriteFrameCacheFunc)(u8 *frame);
extern void (*gUnpackRleSpriteFrameFunc)(u16 *dst, struct rle_frame *frame);

/* The OBJ shape/size index as width and height in pixels: as bytes for
 * DrawSpritePieces/DrawAffineSpritePieces (src/data/obj_sizes_16b2e0.cpp),
 * as s32s for FitScaledSprite/DrawScaledSprite (src/data/map_tables_16c5f0.cpp). */
extern const u8 gObjPieceHeights[12];
extern const u8 gObjPieceWidths[12];
extern const s32 gObjSizeHeights[12];
extern const s32 gObjSizeWidths[12];

/* sym_iwram.txt: the palette fade buffers (fade.cpp, fade_to_black.cpp) */
extern u16 gPaletteBackup[512];
extern u16 gPaletteFadeBuffer[512];

/* sym_iwram.txt: the VRAM DMA queue (vram_dma_queue.cpp) */
extern struct dma_queue gVramDmaQueue;

#endif /* __GFX_H__ */
