#include "core.h"
#include "gba/dma_macros.h"

/* GitHub issue #38: 0x08024590-0x08024783 (game_loop), the sound-channel-
 * handle helper family - see docs/matching/issue-38-medal-results-tally.md
 * for the original write-up describing this whole group as left raw, and
 * docs/matching/issue-38-sound-channel-family.md for this follow-up pass
 * (plus its own second-pass addendum for `sub_8024590`/`sub_8024708`'s
 * final disposition). `sub_8024640` (the per-item driver loop),
 * `sub_80246D8` (the "find next self->0x10==1 item" index scanner), and
 * `sub_8024708` (the VRAM-bank tile-asset streamer) are all matched here
 * as real C. `sub_8024590` is a NAKED transcription (byte-correct but not
 * real decompiled C, tracked as parked) - see its own comment below for
 * the confirmed toolchain-bug gap that forced this. */

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

/* Forward declaration: sub_8024708 is defined further down (after
 * sub_8024590/sub_8024640/sub_80246D8, matching ROM order) but
 * sub_8024640 above it calls it; sub_80246D8 is matched below but called
 * by sub_8024640 above it too (ROM order). */
extern void sub_8024708(struct SoundChannelList *self, s32 idx);
extern s32 sub_80246D8(struct SoundChannelList *self, s32 startIdx, u8 condFlag);

/* Starts/re-selects a sound cue for `self->items[idx]` (`sub_8001B54`),
 * then either plays its secondary sfx (`field_18`) immediately if the
 * channel already reports the requested cue id playing, or busy-polls
 * `sub_8001AB8` until it does before playing it. Either way, also ORs
 * bit 7 into `field_08`'s low byte and passes it to `sub_800132C` (a
 * screen-brightness-fade start, see src/graphics/fade_util.c) - once
 * immediately if the id already matched, or before the busy-wait
 * otherwise.
 *
 * NAKED, not plain C: a second pass on this function's plain-C
 * reconstruction (docs/matching/issue-38-sound-channel-family.md) closed
 * two of its three documented gaps for real - the `field_08 | -0x80`
 * redundant register-copy step (fixed with the `asm volatile("" : "=r"(v)
 * : "0"(expr))` forced-same-register-move idiom `settings_menu13.c`
 * documents) and a mismatched initial `self->items[idx]` pointer load
 * (fixed by splitting the transient first-use load, fed straight to
 * `sub_8001B54`, from the persistent `item` local the rest of the
 * function reloads - the "differently-named pointer variable avoids
 * reuse" gotcha, also from `settings_menu13.c`). The third gap resists
 * every C-level technique for a different, well-precedented reason: the
 * ROM's busy-poll loop in the `playing != item->field_14` branch needs
 * `push {r4, r5, r6, r7, lr}` (caching `&gUnknown_030012BC` in r7 and a
 * copy of the item byte-offset in r6 across the loop), but this
 * compiler's `register T x asm("r7")` never adds an inline-asm-clobbered
 * or even genuinely-written-and-read r7 to the function's own push/pop
 * list - the same confirmed, extensively-precedented toolchain gap
 * documented at length for `sub_8022D50` (game_loop40.c),
 * `LoadGraphicsPackage` (graphics_package_1e578.c), and every other
 * `asm("r7")` call-out project-wide. Transcribed straight from the
 * confirmed-correct ROM disassembly instead of re-chasing this specific
 * combination further - see docs/matching/issue-38-sound-channel-family.md. */
NAKED void sub_8024590(struct SoundChannelList *self0, s32 idx)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r0, #0\n\t"
        "ldr r6, 1f\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r2, [r5]\n\t"
        "lsl r4, r1, #2\n\t"
        "add r2, r4, r2\n\t"
        "ldr r1, [r2]\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "bl sub_8001B54\n\t"
        "ldr r0, [r6]\n\t"
        "bl sub_8001AB8\n\t"
        "ldr r1, [r5]\n\t"
        "add r1, r4, r1\n\t"
        "ldr r2, [r1]\n\t"
        "ldr r1, [r2, #0x14]\n\t"
        "cmp r0, r1\n\t"
        "bne 2f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "cmp r1, #0x63\n\t"
        "beq 3f\n\t"
        "ldr r0, [r6]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
    "3:\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #8]\n\t"
        "mov r2, #0x80\n\t"
        "neg r2, r2\n\t"
        "add r1, r2, #0\n\t"
        "orr r0, r0, r1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov r1, #1\n\t"
        "mov r2, #0\n\t"
        "bl sub_800132C\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012BC\n"
    "2:\n\t"
        "ldr r0, [r2, #8]\n\t"
        "mov r2, #0x80\n\t"
        "neg r2, r2\n\t"
        "add r1, r2, #0\n\t"
        "orr r0, r0, r1\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "mov r1, #1\n\t"
        "mov r2, #0\n\t"
        "bl sub_800132C\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x18]\n\t"
        "cmp r0, #0x63\n\t"
        "beq 4f\n\t"
        "add r7, r6, #0\n\t"
        "add r6, r4, #0\n\t"
    "5:\n\t"
        "ldr r0, [r7]\n\t"
        "bl sub_8001AB8\n\t"
        "ldr r2, [r5]\n\t"
        "add r1, r4, r2\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r1, [r1, #0x14]\n\t"
        "cmp r0, r1\n\t"
        "bne 5b\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r6, r2\n\t"
        "ldr r1, [r1]\n\t"
        "ldr r1, [r1, #0x18]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
    "4:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "6: .4byte gUnknown_030012BC\n"
    );
}

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
        register u8 *addr asm("r0");

        asm volatile("mov r2, #0x80\n\tlsl r2, r2, #2\n\tadd %0, %1, r2"
                     : "=r"(addr) : "r"(asset) : "r2");
        LoadTaggedAsset(addr, (void *)0x0600A000);
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

    DmaSet(3, asset, (void *)0x05000000, (u32)((DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_INC | DMA_DEST_INC) << 16 | 0x100));
    REG_DISPCNT = *(u16 *)&gUnknown_03001314;
}
