#include "core.h"
#include "actor_self.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. Confirmed by
 * docs/rom_map.md as a `category_vtable` slot (`gStaticData_081756C4`,
 * type 1, slot 6) - part of this actor's per-frame dispatch table.
 *
 * Zero-fills one 0x40-byte (8bpp) tile right before BG char block 3
 * (`BG_CHAR_ADDR(3) - 0x40`..`BG_CHAR_ADDR(3)`, a blank/transparent
 * filler tile), then DMA3-fills a 0xffff halfword into BG char block 3
 * itself and runs `sub_8031604`'s VRAM fill-level meter generator (see
 * docs/rom_map.md's "procedurally-generated VRAM fill-level meter"
 * finding). While the small tracker object (`gUnknown_03001534`)'s
 * state (`gUnknown_03001538`) is non-zero: forces a BG2CNT preset
 * toggle (via `gUnknown_03001524`/`gUnknown_03001520` and
 * `sub_80312C4`), looks up a keyframe-table tilemap pointer through the
 * tracker object's own table-index (`+0xc`) and accumulator (`+8`,
 * `>>8`) fields and blits it via `sub_8030D48` (docs/rom_map.md's
 * confirmed "rectangular BG-tilemap blit routine"), sets DISPCNT's
 * bit10 (the same window/mosaic-family bit `sub_8030C98` clears), and
 * DMAs a 0x10-halfword palette strip from `gStaticData_08167AD4` into
 * BG palette bank 1 (`0x05000020`). Once there, one of two mutually
 * exclusive tails run based on the tracker's state: state 5 mirrors
 * palette index 8/0/0xf (slots `+0x10`/`+8`/`+2`/`+0x1e`) all down to
 * black; state 4 clears individual palette slots (`+0x1e`/`+2`/`+8`/
 * `+0x10`) as `gUnknown_0300153C` (a frame/flags counter) crosses four
 * successive thresholds (9, 0x31, 0x4f, 0x6d) - a fade/flash-out
 * sequence for the effect's palette strip.
 *
 * Matching notes (no register pins needed): the filler-tile clear walks
 * a plain `s32` address downward (so its loop test is the ROM's signed
 * `bge`) with the zero hoisted into a local first, the fill/copy are the
 * standard `DmaFill16`/`DmaCopy16` macros, and the palette strip is
 * `vu16` - the state-5 blackout is one chained assignment, whose
 * volatile read-backs are the ROM's `ldrh`/`strh` ladder. */
extern s32 gUnknown_03001538;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001520;
extern struct actor_self *gUnknown_03001534;
extern u32 gUnknown_0300153C;
extern u16 gStaticData_08167AD4[];

extern void sub_8031604(void);
extern void sub_8030D48(u16 *src);
extern void sub_80312C4(void);

void sub_8031504(void)
{
    s32 i;
    s32 base = 0x0600BFC0;
    u32 zero = 0;

    for (i = base + 0x3c; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)0x0600C000, 0x1000);
    sub_8031604();
    if (gUnknown_03001538 != 0) {
        struct actor_self *self;
        vu16 *pal;

        gUnknown_03001524 = 1;
        gUnknown_03001520 = 0;
        self = gUnknown_03001534;
        {
            s32 t = self->animTime >> 8;
            sub_8030D48((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
        REG_DISPCNT |= 0x400;
        sub_80312C4();
        pal = (vu16 *)0x05000020;
        DmaCopy16(3, gStaticData_08167AD4, pal, 0x20);
        if (gUnknown_03001538 == 5) {
            pal[15] = pal[1] = pal[4] = pal[8] = 0;
        } else if (gUnknown_03001538 == 4) {
            if (gUnknown_0300153C > 9)
                pal[15] = 0;
            if (gUnknown_0300153C > 0x31)
                pal[1] = 0;
            if (gUnknown_0300153C > 0x4f)
                pal[4] = 0;
            if (gUnknown_0300153C > 0x6d)
                pal[8] = 0;
        }
    }
}
