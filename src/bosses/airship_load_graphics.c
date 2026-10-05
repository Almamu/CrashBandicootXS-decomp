#include "core.h"
#include "actor_self.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. Confirmed by
 * docs/rom_map.md as a `category_vtable` slot (`gActorCategoryVtables`,
 * type 1, slot 6) - part of this actor's per-frame dispatch table.
 *
 * Zero-fills one 0x40-byte (8bpp) tile right before BG char block 3
 * (`BG_CHAR_ADDR(3) - 0x40`..`BG_CHAR_ADDR(3)`, a blank/transparent
 * filler tile), then DMA3-fills a 0xffff halfword into BG char block 3
 * itself and runs `ConvertAirshipTiles`'s VRAM fill-level meter generator (see
 * docs/rom_map.md's "procedurally-generated VRAM fill-level meter"
 * finding). While the small tracker object (`gAirship`)'s
 * state (`gAirshipState`) is non-zero: forces a BG2CNT preset
 * toggle (via `gAirshipBg2PageFlip`/`gAirshipBg2Page` and
 * `UpdateAirshipBg2`), looks up a keyframe-table tilemap pointer through the
 * tracker object's own table-index (`+0xc`) and accumulator (`+8`,
 * `>>8`) fields and blits it via `DrawAirshipMap` (docs/rom_map.md's
 * confirmed "rectangular BG-tilemap blit routine"), sets DISPCNT's
 * bit10 (the same window/mosaic-family bit `AirshipStateFall` clears), and
 * DMAs a 0x10-halfword palette strip from `gAirshipPalette` into
 * BG palette bank 1 (`0x05000020`). Once there, one of two mutually
 * exclusive tails run based on the tracker's state: state 5 mirrors
 * palette index 8/0/0xf (slots `+0x10`/`+8`/`+2`/`+0x1e`) all down to
 * black; state 4 clears individual palette slots (`+0x1e`/`+2`/`+8`/
 * `+0x10`) as `gAirshipStateTimer` (a frame/flags counter) crosses four
 * successive thresholds (9, 0x31, 0x4f, 0x6d) - a fade/flash-out
 * sequence for the effect's palette strip.
 *
 * Matching notes (no register pins needed): the filler-tile clear walks
 * a plain `s32` address downward (so its loop test is the ROM's signed
 * `bge`) with the zero hoisted into a local first, the fill/copy are the
 * standard `DmaFill16`/`DmaCopy16` macros, and the palette strip is
 * `vu16` - the state-5 blackout is one chained assignment, whose
 * volatile read-backs are the ROM's `ldrh`/`strh` ladder. */
extern s32 gAirshipState;
extern u8 gAirshipBg2PageFlip;
extern s32 gAirshipBg2Page;
extern struct actor_self *gAirship;
extern u32 gAirshipStateTimer;
extern u16 gAirshipPalette[];

extern void ConvertAirshipTiles(void);
extern void DrawAirshipMap(u16 *src);
extern void UpdateAirshipBg2(void);

void LoadAirshipGraphics(void)
{
    s32 i;
    s32 base = VRAM + 0xBFC0;
    u32 zero = 0;

    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)(VRAM + 0xC000), 0x1000);
    ConvertAirshipTiles();
    if (gAirshipState != 0) {
        struct actor_self *self;
        vu16 *pal;

        gAirshipBg2PageFlip = 1;
        gAirshipBg2Page = 0;
        self = gAirship;
        {
            s32 t = self->animTime >> 8;
            DrawAirshipMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
        REG_DISPCNT |= 0x400;
        UpdateAirshipBg2();
        pal = (vu16 *)(PLTT + 0x20);
        DmaCopy16(3, gAirshipPalette, pal, 0x20);
        if (gAirshipState == 5) {
            pal[15] = pal[1] = pal[4] = pal[8] = 0;
        } else if (gAirshipState == 4) {
            if (gAirshipStateTimer > 9)
                pal[15] = 0;
            if (gAirshipStateTimer > 0x31)
                pal[1] = 0;
            if (gAirshipStateTimer > 0x4f)
                pal[4] = 0;
            if (gAirshipStateTimer > 0x6d)
                pal[8] = 0;
        }
    }
}
