#include "core.h"
#include "gba/dma_macros.h"

/* GitHub issue #38: 0x08024590-0x08024783 (game_loop), the sound-channel-
 * handle helper family - see docs/matching/issue-38-medal-results-tally.md
 * for the original write-up describing this whole group as left raw, and
 * docs/matching/issue-38-sound-channel-family.md for this follow-up pass.
 * `sub_8024640` (the per-item driver loop) and `sub_80246D8` (the "find
 * next self->0x10==1 item" index scanner) are matched here as real C;
 * `sub_8024590` and `sub_8024708` are kept as NON_MATCHING C
 * reconstructions - each is fully understood field-by-field but hits a
 * residual gcc-2.9 register-allocation quirk in one small spot (see each
 * function's own comment below) - their real bytes stay in
 * asm/code_3_2_17_24590.s and asm/code_3_2_17_24708.s respectively. */

struct AudioContext;

struct SoundChannelItem {
    void *asset;    /* +0x00: tile/gfx asset pointer, sub_8024708 only */
    s32 field_04;     /* +0x04: sub_80010E0's "count" arg */
    s32 field_08;       /* +0x08: OR'd with -0x80 then truncated to a byte
                         * (bit 7 set), sub_8024590's sub_800132C arg */
    s32 field_0c;         /* +0x0c: sentinel -1 means "none"; else
                            * truncated to a byte and passed to
                            * sub_800132C, sub_8024640/sub_8024790 */
    u8 field_10;            /* +0x10: sub_80010E0's checkButtons arg;
                              * also sub_80246D8's scan target (==1) */
    u8 field_11;              /* +0x11: nonzero triggers a duck-out via
                                * sub_8001AC4 */
    u8 field_12;                /* +0x12: nonzero (and field_18 != 0x63)
                                  * triggers a re-arm via sub_80019A8 */
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
    s32 toggle;                          /* +0x0c: sub_8024708's VRAM-bank
                                           * toggle, alternates each call */
};

extern struct AudioContext *gUnknown_030012BC;
extern u32 sub_8001AB8(struct AudioContext *self);
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern void sub_80019A8(struct AudioContext *self, u32 id);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_80010E0(s32 count, u8 checkButtons, s32 mask);
extern void LoadTaggedAsset(void *asset, void *dest);
extern void sub_80006A8(void);
extern void *gUnknown_03001314;

/* Forward declarations: sub_8024708/sub_8024590 are NON_MATCHING here
 * (real bytes stay in asm/code_3_2_17_24590.s and
 * asm/code_3_2_17_24708.s under the default build) but sub_8024640
 * always calls them; sub_80246D8 is matched below but called by
 * sub_8024640 above it (ROM order). */
extern void sub_8024708(struct SoundChannelList *self, s32 idx);
extern void sub_8024590(struct SoundChannelList *self, s32 idx);
extern s32 sub_80246D8(struct SoundChannelList *self, s32 startIdx, u8 condFlag);

#if NON_MATCHING
/* Starts/re-selects a sound cue for `self->items[idx]` (`sub_8001B54`),
 * then either plays its secondary sfx (`field_18`) immediately if the
 * channel already reports the requested cue id playing, or busy-polls
 * `sub_8001AB8` until it does before playing it. Either way, also ORs
 * bit 7 into `field_08`'s low byte and passes it to `sub_800132C` (a
 * screen-brightness-fade start, see src/graphics/fade_util.c) - once
 * immediately if the id already matched, or before the busy-wait
 * otherwise.
 *
 * PARKED, NOT BYTE-MATCHING: every field, offset, branch and call
 * argument is confirmed against the ROM. The residual gap is in the
 * `field_08 | -0x80` computation, done twice (once per branch): the ROM
 * materializes -0x80 into one register then copies it to a second
 * register before the OR (`movs r2,#0x80; rsbs r2,r2,#0; adds r1,r2,#0;
 * orrs r0,r1`), while every C phrasing tried here (a plain expression, a
 * named local, register-pinned locals with an explicit copy step, `~0x7F`
 * instead of `-0x80`) collapses the redundant copy away, producing the
 * mask directly in its final register instead (`mov r1,#0x80; neg r1,r1;
 * orrs r0,r1` - two instructions short). See
 * docs/matching/issue-38-sound-channel-family.md. */
void sub_8024590(struct SoundChannelList *self0, s32 idx)
{
    register struct SoundChannelList *self asm("r5") = self0;
    struct AudioContext *audio = gUnknown_030012BC;
    struct SoundChannelItem *item = self->items[idx];
    register u32 playing asm("r0");

    sub_8001B54(audio, item->field_14);
    playing = sub_8001AB8(gUnknown_030012BC);
    item = self->items[idx];

    if (playing == item->field_14) {
        if (item->field_18 != 0x63) {
            PlaySfx(gUnknown_030012BC, item->field_18, 0x100);
        }
        {
            struct SoundChannelItem *item2 = self->items[idx];
            register s32 raw asm("r0") = item2->field_08;
            register s32 mask asm("r2") = -0x80;
            register s32 val asm("r1") = mask;
            register s32 combined asm("r0") = raw | val;

            sub_800132C((u8)combined, 1, 0);
        }
    } else {
        {
            register s32 mask asm("r2") = -0x80;
            register s32 val asm("r1") = mask;
            register s32 combined asm("r0") = item->field_08 | val;

            sub_800132C((u8)combined, 1, 0);
        }
        item = self->items[idx];
        if (item->field_18 != 0x63) {
            do {
                playing = sub_8001AB8(gUnknown_030012BC);
                item = self->items[idx];
            } while (playing != item->field_14);
            PlaySfx(gUnknown_030012BC, item->field_18, 0x100);
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

/* Per-frame driver loop over `self`'s item list: for each index, streams
 * the item's VRAM tile bank and refreshes its sound-channel handle
 * (`sub_8024708`/`sub_8024590`), polls input (`sub_80010E0`) to get a
 * confirm/cancel result, applies the item's duck-out (`field_11`) and
 * fade-start (`field_0c`, sentinel -1) side effects, re-arms the item's
 * cue if needed (`field_12`/`field_18`), then advances to the next
 * "still active" item via `sub_80246D8`. */
void sub_8024640(struct SoundChannelList *self0)
{
    register struct SoundChannelList *self asm("r4") = self0;
    s32 i;

    for (i = 0; i < self->count; i++) {
        u8 checkButtons;
        struct SoundChannelItem *item;

        sub_8024708(self, i);
        sub_8024590(self, i);

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
                sub_80019A8(gUnknown_030012BC, item2->field_18);
            }
        }

        i = sub_80246D8(self, i, checkButtons);
    }
}

/* Scans forward from `startIdx + 1` for the next item whose `field_10`
 * isn't 1 ("busy"), returning the index just before it (or the last
 * index reached if every remaining item is busy). Returns `startIdx`
 * unchanged if `condFlag` is set, or if `startIdx + 1` is already past
 * the list. */
s32 sub_80246D8(struct SoundChannelList *self, s32 startIdx, u8 condFlag)
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

#if NON_MATCHING
/* Toggles `self`'s VRAM-bank flip-flop (`self->toggle`) and streams
 * `self->items[idx]`'s tile asset (its `+0x200` byte offset - the
 * asset's second half) to whichever of the two OBJ tile VRAM banks the
 * new toggle state selects (`0x06000000`/`0x0600A000`), via
 * `LoadTaggedAsset`. Then rebuilds `gUnknown_03001314`'s bit 4 from the
 * toggle's low bit (same `& ~0x10 | bit`-idiom byte-shadow-update shape
 * as `sub_8024708`'s cousin in game_loop18.c, but for a different
 * global), DMA3-copies the asset's first half into `BG_PLTT` (a second,
 * independent palette-DMA-plus-DISPCNT-write path alongside the
 * already-documented `sub_8001614`/`gUnknown_03001288` one - see
 * docs/rom_map.md), and finally commits `gUnknown_03001314`'s low
 * halfword straight to `REG_DISPCNT`.
 *
 * PARKED, NOT BYTE-MATCHING: every field, offset, branch and call
 * argument is confirmed against the ROM (including the `& ~0x10`
 * negated-constant idiom, and the `self`/`asset` register pins needed to
 * reach `push {r4, r5, r6, lr}` at all). The residual gap is a single
 * branch's address computation (`asset + 0x200` inside the
 * `toggle != 0` case): the ROM computes the `0x80 << 2` scratch offset
 * into `r2` there (`movs r2,#0x80; lsls r2,r2,#2; adds r0,r6,r2`) while
 * every C phrasing tried here (plain expression, a named local, a
 * register-pinned local for either the offset or the destination
 * constant, evaluation-order swaps) puts it in `r1` instead - the
 * *other* (fallthrough) branch's identical-shaped computation already
 * matches exactly using `r1`, pointing at a per-branch scheduling
 * artifact from the ROM's original build rather than anything
 * controllable from this reconstruction's own source shape. See
 * docs/matching/issue-38-sound-channel-family.md. */
void sub_8024708(struct SoundChannelList *self0, s32 idx)
{
    register struct SoundChannelList *self asm("r5") = self0;
    struct SoundChannelItem *item = self->items[idx];
    void *asset = item->asset;
    s32 toggle = self->toggle ^ 1;

    self->toggle = toggle;

    if (toggle == 0) {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)0x06000000);
    } else {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)0x0600A000);
    }

    {
        register u8 toggleByte asm("r5") = *((u8 *)self + 0xc);
        u8 bit4 = (toggleByte & 1) << 4;
        u8 *shadow = (u8 *)&gUnknown_03001314;
        register s32 mask asm("r0") = ~0x10;
        register s32 byte asm("r2") = *shadow;
        register s32 result asm("r0");

        result = mask & byte;
        result = result | bit4;
        *shadow = result;
    }

    sub_80006A8();

    DmaSet(3, asset, (void *)0x05000000, (u32)((DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_INC | DMA_DEST_INC) << 16 | 0x100));
    REG_DISPCNT = *(u16 *)&gUnknown_03001314;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
