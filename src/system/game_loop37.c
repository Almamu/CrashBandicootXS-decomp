#include "core.h"
#include "gba/dma_macros.h"

/* GitHub issue #38: 0x08024590-0x08024783 (game_loop), the sound-channel-
 * handle helper family - see docs/matching/issue-38-medal-results-tally.md
 * and docs/matching/issue-38-sound-channel-family.md. All four functions
 * are real C, built with old_agbcc - see
 * docs/matching/game-loop-old-agbcc.md. */

struct AudioContext;

struct SoundChannelItem {
    void *asset;    /* +0x00: tile/gfx asset pointer, ShowSlidePicture only */
    s32 field_04;     /* +0x04: sub_80010E0's "count" arg */
    s32 field_08;       /* +0x08: OR'd with -0x80 then truncated to a byte
                         * (bit 7 set), BeginSlide's sub_800132C arg */
    s32 field_0c;         /* +0x0c: sentinel -1 means "none"; else
                            * truncated to a byte and passed to
                            * sub_800132C, RunSlideshow/EndSlide */
    u8 field_10;            /* +0x10: sub_80010E0's checkButtons arg;
                              * also SkipSlides's scan target (==1) */
    u8 field_11;              /* +0x11: nonzero triggers a duck-out via
                                * sub_8001AC4 */
    u8 field_12;                /* +0x12: nonzero (and field_18 != 0x63)
                                  * triggers a re-arm via StopSfx */
    u8 unused_13;
    u32 field_14;                  /* +0x14: sound cue id, sub_8001B54/
                                     * sub_8001AB8 */
    u32 field_18;                    /* +0x18: secondary sfx id passed to
                                       * PlaySfx; sentinel 0x63 (99) means
                                       * "no sfx" */
};

struct SoundChannelList {
    struct SoundChannelItem **items; /* +0x00 */
    s32 count;                        /* +0x04 */
    u8 unused_08[4];
    s32 toggle;                          /* +0x0c: ShowSlidePicture's VRAM-bank
                                           * toggle, alternates each call */
};

extern struct AudioContext *gUnknown_030012BC;
extern u32 sub_8001AB8(struct AudioContext *self);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern void StopSfx(struct AudioContext *self, u32 id);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_80010E0(s32 count, u8 checkButtons, s32 mask);
extern void LoadTaggedAsset(void *asset, void *dest);
extern void sub_80006A8(void);
extern void *gUnknown_03001314;

/* Forward declaration: ShowSlidePicture is defined further down (after
 * BeginSlide/RunSlideshow/SkipSlides, matching ROM order) but
 * RunSlideshow above it calls it; SkipSlides is matched below but called
 * by RunSlideshow above it too (ROM order). */
extern void ShowSlidePicture(struct SoundChannelList *self, s32 idx);
extern s32 SkipSlides(struct SoundChannelList *self, s32 startIdx, u8 condFlag);

/* Starts sound cue `items[idx]->field_14` on the audio context. If the
 * channel already reports that cue, plays the item's secondary sfx
 * (unless it is the 0x63 "none" sentinel) and then starts the item's
 * fade; otherwise starts the fade first, then busy-waits for the cue
 * before playing the sfx. */
void BeginSlide(struct SoundChannelList *self, s32 idx)
{
    struct SoundChannelItem *item;

    sub_8001B54(gUnknown_030012BC, self->items[idx]->field_14);
    if (sub_8001AB8(gUnknown_030012BC) == (item = self->items[idx])->field_14)
    {
        if (item->field_18 != 0x63)
            PlaySfx(gUnknown_030012BC, item->field_18, 0x100);
        sub_800132C(self->items[idx]->field_08 | -0x80, 1, 0);
    }
    else
    {
        sub_800132C(item->field_08 | -0x80, 1, 0);
        if (self->items[idx]->field_18 != 0x63)
        {
            while (sub_8001AB8(gUnknown_030012BC) != self->items[idx]->field_14)
                ;
            PlaySfx(gUnknown_030012BC, self->items[idx]->field_18, 0x100);
        }
    }
}

/* Per-frame driver loop over `self`'s item list: for each index, streams
 * the item's VRAM tile bank and refreshes its sound-channel handle
 * (`ShowSlidePicture`/`BeginSlide`), polls input (`sub_80010E0`) to get a
 * confirm/cancel result, applies the item's duck-out (`field_11`) and
 * fade-start (`field_0c`, sentinel -1) side effects, re-arms the item's
 * cue if needed (`field_12`/`field_18`), then advances to the next
 * "still active" item via `SkipSlides`.
 *
 * UNUSED - no caller anywhere in the ROM (checked src/, asm/ and every
 * Thumb `bl` and aligned word of baserom.gba for its address). It plays
 * a slide list without text; the cutscenes use RunCutscenePlayer
 * (game_loop57.c), the same loop with the text pages added. */
void RunSlideshow(struct SoundChannelList *self0)
{
    struct SoundChannelList *self = self0;
    s32 i;

    for (i = 0; i < self->count; i++) {
        u8 checkButtons;
        struct SoundChannelItem *item;

        ShowSlidePicture(self, i);
        BeginSlide(self, i);

        item = self->items[i];
        checkButtons = (u8)sub_80010E0(item->field_04, item->field_10, 8);

        if (self->items[i]->field_11 != 0) {
            sub_8001AC4(gUnknown_030012BC, 0);
        }

        {
            s32 v = self->items[i]->field_0c;

            if (v != -1) {
                sub_800132C((u8)v, 1, 0);
            }
        }

        {
            struct SoundChannelItem *item2 = self->items[i];

            if (item2->field_12 != 0 && item2->field_18 != 0x63) {
                StopSfx(gUnknown_030012BC, item2->field_18);
            }
        }

        i = SkipSlides(self, i, checkButtons);
    }
}

/* Scans forward from `startIdx + 1` for the next item whose `field_10`
 * isn't 1 ("busy"), returning the index just before it (or the last
 * index reached if every remaining item is busy). Returns `startIdx`
 * unchanged if `condFlag` is set, or if `startIdx + 1` is already past
 * the list. */
s32 SkipSlides(struct SoundChannelList *self, s32 startIdx, u8 condFlag)
{
    s32 cur = startIdx;
    s32 next;
    s32 count;
    struct SoundChannelItem **items;

    if (condFlag) {
        return cur;
    }

    next = cur + 1;
    count = self->count;
    if (next >= count) {
        return cur;
    }
    items = self->items;

    while (items[cur + 1]->field_10 == 1) {
        cur = next;
        next = cur + 1;
        if (next >= count) {
            return cur;
        }
    }
    return cur;
}

/* Toggles `self`'s VRAM-bank flip-flop (`self->toggle`) and streams
 * `self->items[idx]`'s tile asset (its `+0x200` byte offset - the
 * asset's second half) to whichever of the two OBJ tile VRAM banks the
 * new toggle state selects (`0x06000000`/`0x0600A000`), via
 * `LoadTaggedAsset`. Then rebuilds `gUnknown_03001314`'s bit 4 from the
 * toggle's low bit (same `& ~0x10 | bit`-idiom byte-shadow-update shape
 * as `ShowSlidePicture`'s cousin in game_loop18.c, but for a different
 * global), DMA3-copies the asset's first half into `BG_PLTT` (a second,
 * independent palette-DMA-plus-DISPCNT-write path alongside the
 * already-documented `sub_8001614`/`gUnknown_03001288` one - see
 * docs/rom_map.md), and finally commits `gUnknown_03001314`'s low
 * halfword straight to `REG_DISPCNT`.
 *
 * Matched, but only after two more register-pinning/ordering gotchas on
 * top of the `self`/`asset` pins the first pass already found:
 * - The `toggle != 0` branch's `asset + 0x200` scratch-offset computation
 *   needs to land in r2 (not gcc's own natural choice of r1, which
 *   happens to already match the *other*, `toggle == 0` branch's
 *   identical-shaped computation) - an `asm volatile("mov r2, #0x80\n\t
 *   lsl r2, r2, #2\n\tadd %0, %1, r2" : "=r"(addr) : "r"(asset) : "r2")`
 *   anchor (the same "hardcode the scratch register, let the output land
 *   wherever" idiom `hud_icon_widget_8a78.c` uses) forces it.
 * - The `gUnknown_03001314` shadow-byte rebuild needed its own two-part
 *   fix: gcc's front end always schedules the `& ~0x10` mask/byte-read
 *   pair *before* the toggle-bit `& 1 << 4` shift-and-mask when both are
 *   written as independent statements (this reconstruction's first
 *   attempt), where the ROM computes the shifted toggle bit first; an
 *   `asm volatile("" ::: "memory")` ordering barrier right after it
 *   fixes that. But the barrier alone widens `bit4`'s tracked value range
 *   just enough that the final `orr` gets an extra defensive
 *   `lsl #24; lsr #24` truncation pair the ROM doesn't have - avoided by
 *   writing the AND as `bit4 & toggleByte` (not `toggleByte & bit4`),
 *   which happens to pick the same destination register (r1, not r5) the
 *   ROM's own `ands r1, r5` uses. See
 * docs/matching/issue-38-sound-channel-family.md. */
void ShowSlidePicture(struct SoundChannelList *self0, s32 idx)
{
    register struct SoundChannelList *self asm("r5") = self0;
    struct SoundChannelItem *item = self->items[idx];
    void *asset = item->asset;
    s32 toggle = self->toggle ^ 1;

    self->toggle = toggle;

    if (toggle == 0) {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)VRAM);
    } else {
        register u8 *addr asm("r0");

        asm volatile("mov r2, #0x80\n\tlsl r2, r2, #2\n\tadd %0, %1, r2"
                     : "=r"(addr) : "r"(asset) : "r2");
        LoadTaggedAsset(addr, (void *)(VRAM + 0xA000));
    }

    {
        u8 *shadow = (u8 *)&gUnknown_03001314;
        {
            register s32 bit4 asm("r1") = 1;
            register s32 toggleByte asm("r5");
            register s32 mask asm("r0");
            register s32 byte asm("r2");
            register s32 result asm("r0");

            toggleByte = *((u8 *)self + 0xc);
            bit4 = (bit4 & toggleByte) << 4;
            asm volatile("" ::: "memory");
            mask = ~0x10;
            byte = *shadow;
            result = mask & byte;
            result = result | bit4;
            *shadow = result;
        }
    }

    sub_80006A8();

    DmaSet(3, asset, (void *)PLTT, (u32)((DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_INC | DMA_DEST_INC) << 16 | 0x100));
    REG_DISPCNT = *(u16 *)&gUnknown_03001314;
}
