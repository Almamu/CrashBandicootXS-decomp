#include "core.h"
#include "bitmap_font.h"
#include "vram_pool.h"

/* Same continue prompt ("fade overlay") self object as `actor_part87.c` (`InitContinuePrompt`) -
 * redeclared locally here per this project's minimal-local-type
 * convention for a type already anchored in another translation unit
 * (see e.g. settings_menu10.c's own `struct sub_8006700_actor`
 * comment). Only the fields this file actually touches are named. */
struct continue_prompt {
    u8 unused_00[0xc];
    u16 dispcnt; /* 0x0c */
    u8 unused_0e[2];
    u32 unused_10;      /* 0x10 */
    u8 unused_14[4];
    struct bitmap_font *icons; /* 0x18 */
};

extern struct vram_upload_cursor *gObjVramCursor;
extern void ResetObjVram(struct vram_upload_cursor *self);
extern struct bitmap_font *gSmallFont;
extern void *_call_via_r1(void *arg0, void *arg1);
extern s32 ReserveObjVram(struct vram_upload_cursor *self, s32 size);
extern void MarkObjVram(struct vram_upload_cursor *self);
extern struct palette_cache *gPaletteCache;
extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern s32 ClaimPaletteSlot(struct palette_cache *self, s32 index);
extern void UploadPaletteCache(struct palette_cache *self);
extern struct oam_shadow_buffer *gOamBuffer;
extern void ResetOamBuffer(struct oam_shadow_buffer *arg0);
extern void HideUnusedOamEntries(struct oam_shadow_buffer *arg0);
extern void WaitForVBlank(void);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern u16 gStaticData_0817C512[];
extern u16 gStaticData_0817C532[];
extern u16 gStaticData_0817C552[];
extern u16 gStaticData_0817C572[];

/* The other half of the continue prompt's setup, called from
 * `InitContinuePrompt` (actor_part87.c): flushes the shared VRAM upload cursor
 * twice, hooks `self->icons` up to the global text icon manager
 * (`gSmallFont`), fires its 7th OAM trampoline slot, clears its
 * `marginX` and re-derives the cursor's limit from `tileCount`, resets
 * the shared tile cache and pins its first four slots, seeds those four
 * slots with fixed 32-byte tile patterns from ROM data, flushes the
 * cache, sets the overlay's DISPCNT "OBJ enable" bit, and flushes the
 * OAM shadow buffer.
 *
 * Built with old_agbcc. The seeding loop is indexed through `i`: gcc
 * strength-reduces every access into its own pointer but keeps `i` as
 * the up-counting trip counter (r7) the ROM has. Written with explicit
 * pointer increments, `i` has nothing left to do but count, and gcc
 * reverses it into a down-counter. */
void InitContinuePromptGraphics(struct continue_prompt *self)
{
    struct bitmap_font *icons;
    struct palette_cache *cache;
    s32 i;

    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);

    icons = gSmallFont;
    self->icons = icons;
    icons->tileBase = 0;
    {
        u8 *rec = (u8 *)icons->record + 0x40;
        _call_via_r1((u8 *)icons + *(s16 *)rec, *(void **)(rec + 4));
    }
    self->icons->marginX = 0;
    ReserveObjVram(gObjVramCursor, self->icons->tileCount << 5);
    MarkObjVram(gObjVramCursor);

    FreeUnlockedPaletteSlots(gPaletteCache);
    ClaimPaletteSlot(gPaletteCache, 0);
    ClaimPaletteSlot(gPaletteCache, 1);
    ClaimPaletteSlot(gPaletteCache, 2);
    ClaimPaletteSlot(gPaletteCache, 3);

    cache = gPaletteCache;
    {
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gStaticData_0817C512[i];
            destA[i + 0x10] = gStaticData_0817C532[i];
            destB[i] = gStaticData_0817C552[i];
            destB[i + 0x10] = gStaticData_0817C572[i];
        }
    }
    UploadPaletteCache(gPaletteCache);

    ((u8 *)&self->dispcnt)[1] |= 0x10;

    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
}
