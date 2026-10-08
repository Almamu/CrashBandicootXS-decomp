#ifndef __GFX_H__
#define __GFX_H__

/* The gfx subsystem (src/gfx/): the OAM shadow buffer, the OBJ VRAM and
 * palette caches, the small `struct actor` entity, brightness fades,
 * DISPCNT helpers, BG packages and the sprite frame cache. Every function
 * src/gfx/ defines, with the prototype of its definition, and the globals
 * and tables its files use (docs/headers_plan.md). `DrawHudPart`/
 * `InitHudPart` (palette_cycle.cpp) are in hud.h. A .c file that needs a
 * different local declaration for codegen keeps it as an asm-label alias
 * with a `codegen:` comment. */

#include "core.h"
#include "graphics_package.h"
#include "vram_pool.h"
#include "hitbox_quad.h"

struct aabb;
struct actor;
struct affine_part;
struct dma_queue;
struct gfx_box_obj;
struct oam_part;
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

/* Manages a shadow copy of a chunk of the 128-entry hardware OAM table:
 * a count of active entries, the `base` count RewindOamBuffer goes
 * back to (entries kept from frame to frame, set by MarkOamBufferBase),
 * the number of affine matrices handed out this frame, then the
 * 1024-byte shadow table itself (128 entries * 8 bytes) starting right
 * after. Affine matrix `m`'s pa/pb/pc/pd are the affine parameters of
 * entries 4m..4m+3 (`table[4 * m + n].attr[3]`). The C view of
 * sprite_obj.hpp's OamBuffer (graphics.cpp). */
struct oam_shadow_buffer {
    s32 count;
    s32 base;
    s32 matrixCount;
    union oam_shadow_entry table[0x80];
};
COMPILE_TIME_ASSERT(gfx_h, sizeof(struct oam_shadow_buffer) == 0x40C);

/* A sprite frame (GetSpriteFrame) as DrawSpritePieces and
 * DrawAffineSpritePieces read it: one offset and one shape/size id per
 * OBJ piece, the frame's VRAM source and its piece count. */
struct piece_offset {
    s16 x;
    s16 y;
};

struct piece_info {
    struct piece_offset *offsets; // 0x00
    u8 *ids;                      // 0x04 - low 4 bits: shape/size index
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

/* One hardware OAM entry as bitfields, as AddOamEntry copies it into the
 * shadow buffer: attributes 0-2, then the affine parameter. The u32
 * storage units of attributes 0-1 give the ROM's word-wide and/or
 * sequences, and the u16 units of attribute 2 mask the tile store with
 * lsl/lsr #22 as in the ROM (a u32 field loads a 0x3ff constant
 * instead). In affine mode `matrixBit3`/`matrixBit4` are the top bits
 * of the matrix number; otherwise they are the h/v flip. DrawSpritePieces
 * and DrawAffineSpritePieces build one on the stack (their local `struct
 * oam_pair`/`oam_attr01` and gfx.h's `oam_attr2` were copies, #574
 * batch 9e). */
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
    u32 unk_14:2;
    u32 eva:5; // BLDALPHA
    u32 unk_21:3;
    u32 evb:5;
    u32 unk_29:3;
};

union blend {
    u32 raw;
    struct blend_bits bits;
};

struct bldy {
    u32 evy:5;
    u32 unk_5:27;
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

/* Up to three palette colour cycles, at `gPaletteCycles`
 * (`OperatorNew(0x48)`, matching this struct's size). run_room.c
 * adds them with `targets` = BG palette RAM and `lists` = the palette
 * indices to cycle; every `periods[i]` = 60 / rate frames,
 * `TickPaletteCycles` shifts the colours at those indices by one
 * place. (docs/rom_map.md's "fx" investigation first read the pair as a
 * particle/projectile-trajectory queue and its `rate` argument as an
 * angle; `__divsi3` is plain division.)
 *
 * Reading both functions in full (docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md's "Third pass" section) settled
 * the remaining fields: each of the 3 slots pairs a `targets`/`lists`
 * pointer pair with a `periods`/`counts` scalar pair, and `TickPaletteCycles`
 * (the per-frame consumer) rotates `targets[i]` by one position, once
 * every `periods[i]` frames (`gRoomFrameCount % periods[i] == 0`),
 * walking the permutation order given by `lists[i]` - forwards or
 * backwards depending on `direction`. `AddPaletteCycle` (the producer) only
 * ever appends at `count` (no wraparound seen in either function - the
 * caller resets the queue via `ClearPaletteCycles`/`InitPaletteCycles` between
 * bursts rather than this pair enforcing the 3-slot cap itself). The C
 * view of PaletteCycles (include/part_list.hpp), which checks the size. */
struct palette_cycler {
    u8 active;        /* +0x00 */
    u8 unknown_01[3]; /* +0x01 */
    s32 fields_e[3];  /* +0x04 - only ever written (to 0) by
                       * AddPaletteCycle; never read by either function
                       * matched here. Purpose unconfirmed. */
    u16 *targets[3];  /* +0x10 - array TickPaletteCycles rotates. */
    u16 *lists[3];    /* +0x1c - permutation order (as u16 indices
                       * into `targets[i]`), `counts[i]` long. */
    s32 periods[3];   /* +0x28 - AddPaletteCycle sets this from
                       * __divsi3(0x3C, rate); TickPaletteCycles
                       * rotates slot i once every `periods[i]`
                       * frames. */
    s32 counts[3];    /* +0x34 - `lists[i]`'s element count. */
    s32 count;        /* +0x40 - number of active slots (0-3). */
    u8 direction;     /* +0x44 - 0/1 selects which end of
                       * `lists[i]` the rotation starts from. */
    u8 unknown_45[3];
};
COMPILE_TIME_ASSERT(gfx_h, sizeof(struct palette_cycler) == 0x48);

/* src/gfx/affine_sprite_pieces.c */
extern void DrawAffineSpritePieces(void *unused, struct affine_part *part, s32 *pos);

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
extern s32 GetCompletionPercent(void *progress);
extern void SetOamAffineScales(void *table, u16 *scales, s32 count);
extern void AppendOamEntries(struct oam_shadow_buffer *self, void *entries, s32 count);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *self);
extern void RewindOamBuffer(struct oam_shadow_buffer *self);
extern void MarkOamBufferBase(struct oam_shadow_buffer *self);
extern void ResetOamBuffer(struct oam_shadow_buffer *self);
extern void CommitOamBuffer(struct oam_shadow_buffer *self);
extern void AddOamEntry(struct oam_shadow_buffer *self, const void *entry);
extern void DestroyOamBuffer(struct oam_shadow_buffer *self, u32 flags);

/* src/gfx/graphics.cpp: the VRAM DMA queue and OBJ VRAM cursor */
extern void FlushVramDmaQueue(void);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern void FreeVramDmaQueue(void);
extern s32 AllocVramDmaQueue(void);
extern void RewindObjVram(struct vram_upload_cursor *self);
extern void MarkObjVram(struct vram_upload_cursor *self);
extern s32 GetObjVramFreeBytes(struct vram_upload_cursor *self);
extern s32 GetObjVramTile(struct vram_upload_cursor *self);
extern void ResetObjVram(struct vram_upload_cursor *self);
extern s32 ReserveObjVram(struct vram_upload_cursor *self, s32 size);
extern s32 UploadObjVram(struct vram_upload_cursor *self, void *src, s32 size);
extern void DestroyObjVramCursor(struct vram_upload_cursor *self, u32 flags);

/* src/gfx/graphics.cpp: the palette cache */
extern void LoadPaletteSlot(struct palette_cache *self, s32 slot, s32 recordId);
extern void BindPaletteSlot(struct palette_cache *self, s32 slot, s32 index);
extern s32 ClaimPaletteSlot(struct palette_cache *self, s32 index);
extern void UnlockPalette(struct palette_cache *self, s32 index);
extern void LockPalette(struct palette_cache *self, s32 index);
extern void UploadPaletteSlot(struct palette_cache *self, s32 index);
extern void UploadPaletteCache(struct palette_cache *self);
extern u8 GetPaletteSlot(struct palette_cache *self, s32 recordId);
extern s32 FreePaletteSlot(struct palette_cache *self, s32 slot);
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern void SetPaletteCacheSource(struct palette_cache *self, u16 count, const u8 *records);
extern void ClearPaletteCache(struct palette_cache *self);
extern void DestroyPaletteCache(struct palette_cache *self, u32 flags);
extern struct palette_cache *InitPaletteCache(struct palette_cache *self);
extern void DestroySpriteBankSet(void *self, u32 flags);

/* src/gfx/graphics.cpp: the entity (`struct actor`, actor.h) */
extern u8 IsEntityNearCamera(struct actor *self);
extern s32 CheckEntityPlayerContact(struct actor *self);
extern void DrawEntity(void);
extern void UpdateEntity(struct actor *self);
extern void *GetEntityBounds(struct actor *self);
extern void SetEntitySize(struct actor *self, s32 w, s32 h);
extern s32 EntityOverlapsRect(void);
extern s32 IsEntityOnScreen(void);
extern s32 IsEntityInsideRect(struct actor *self, struct aabb *box);
extern void WorldToScreen(void *unused, s32 x, s32 y, s32 *outX, s32 *outY);
extern void WorldPosToScreen(s32 *pos, s32 *outX, s32 *outY);
extern void nullsub_12(void);
extern s32 GetEntityClassId(void);
extern void ResetEntity(struct actor *self);
extern void ClearEntityAlwaysActive(struct actor *self);
extern void SetEntityAlwaysActive(struct actor *self);
extern u8 IsEntityAlwaysActive(struct actor *self);
extern void ClearEntityTouched(struct actor *self);
extern void SetEntityTouched(struct actor *self);
extern u8 IsEntityTouched(struct actor *self);
extern u8 IsEntityGone(struct actor *self);
extern void ClearEntityGone(struct actor *self);
extern void MarkEntityGone(struct actor *self);
extern u8 IsEntityContactEnabled(struct actor *self);
extern void DisableEntityContact(struct actor *self);
extern void EnableEntityContact(struct actor *self);
extern u8 GetEntityFlag1(struct actor *self);
extern void ClearEntityFlag1(struct actor *self);
extern void SetEntityFlag1(struct actor *self);
extern s32 GetEntityPixelY(struct actor *self);
extern s32 GetEntityPixelX(struct actor *self);
extern s32 GetEntityY(struct actor *self);
extern s32 GetEntityX(struct actor *self);
extern void SetEntityPixelPos(struct actor *self, s32 x, s32 y);
extern void SetEntityPixelPosVec(struct actor *self, s32 *pos);
extern void SetEntityPos(struct actor *self, s32 x, s32 y);
extern void SetEntityPosVec(struct actor *self, s32 *pos);
extern void SetEntityKind(struct actor *self, u8 kind);
extern u8 GetEntityKind(struct actor *self);
extern u16 GetEntityId(struct actor *self);
extern void DestroyEntity(struct actor *self, u32 flags);

/* src/gfx/graphics_package.c */
extern void LoadGraphicsPackage(struct bg_setup *self, const struct bg_package *pkg);
extern u16 GetBgSetupControl(struct bg_setup *self);
extern struct bg_setup *InitBgSetup(struct bg_setup *self, u32 charBlock, u32 screenBlock,
                                    u32 paletteBank, u32 priority);
extern void FitScaledSprite(struct gfx_box_obj *self, s32 width, s32 height);
extern void DrawScaledSprite(struct gfx_box_obj *self);
extern void SetScaledSpriteColor(u8 *self, s32 arg1);
extern void SetScaledSpritePriority(u8 *self, u32 arg1);
extern void SetScaledSpritePos(struct gfx_box_obj *self, u32 arg1, u32 arg2);
extern void ResetScaledSpriteAttrs(u8 *self);

/* src/gfx/palette_cycle.cpp */
extern void TickPaletteCycles(struct palette_cycler *self);
extern void AddPaletteCycle(struct palette_cycler *self, u16 *targets, u16 *lists, s32 rate,
                            s32 listCount, u8 direction);
extern void ClearPaletteCycles(struct palette_cycler *self);
extern void DestroyPaletteCycles(struct palette_cycler *self, s32 flags);
extern void DestroyHudPart(struct actor *part, u32 flags);

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

/* src/gfx/sprite_pieces.c */
extern void DrawSpritePieces(void *unused, struct oam_part *part, s32 *pos);

/* The menus' sky background (src/data/bg_package_16c484.c): the language
 * select, level select, power dialog and save menu load it on BG0. */
extern const struct bg_package gMenuSkyBg;

/* The blend register shadow (sym_iwram.txt). */
extern struct blend_regs gBlendRegs;

/* The master sprite bank table (src/data/sprite_banks_4a5600.c,
 * sprite_bank.h): its palettes seed the palette cache. */
extern const struct sprite_bank_table gSpriteBankTable;

/* The palette cycler instance (sym_iwram.txt; NULL outside a level). */
extern struct palette_cycler *gPaletteCycles;

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
