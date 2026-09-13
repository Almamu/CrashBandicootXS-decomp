#include "core.h"

/* Sits right after sub_8001254 (ROM 0x08001254, in src/line_util2.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

extern void sub_8000670(s32 arg0);
extern s32 gUnknown_030007F8;
extern s32 gUnknown_030007F4;

struct unk_030007E8 {
    s32 field_0;
    s32 field_4;
    u8 field_8;
};

extern struct unk_030007E8 gUnknown_030007E8;

/* A per-frame screen-brightness fade tick: every `gUnknown_030007E8`.
 * `field_0` frames, writes the next brightness step to `REG_BLDY`,
 * counting either up or down depending on `field_8`'s top bit (fading
 * in vs. out). After 17 steps (a full fade), resets both counters,
 * briefly disables interrupts (`REG_IME`) while resetting `field_0` to
 * `-1` and calling `sub_8000670` with `field_4` (presumably to kick off
 * whatever comes after the fade), then re-enables interrupts.
 * `mask`/`flag8` are pinned to r0/r1 to match the ROM's exact register
 * choice for the `& 0x80` check - the natural (unpinned) allocation
 * puts the loaded byte in r0 and the constant in r1 instead, one
 * register off. */
void sub_80012AC(void)
{
    s32 counter;

    counter = gUnknown_030007F8 + 1;
    gUnknown_030007F8 = counter;
    if (counter == gUnknown_030007E8.field_0) {
        s32 val;
        register u8 flag8 asm("r1");
        register s32 mask asm("r0");

        gUnknown_030007F8 = 0;
        mask = 0x80;
        flag8 = gUnknown_030007E8.field_8;
        if (mask & flag8) {
            REG_BLDY = 16 - gUnknown_030007F4;
        } else {
            REG_BLDY = gUnknown_030007F4;
        }
        val = gUnknown_030007F4 + 1;
        gUnknown_030007F4 = val;
        if (val == 0x11) {
            gUnknown_030007F4 = 0;
            gUnknown_030007F8 = 0;
            REG_IME = 0;
            gUnknown_030007E8.field_0 = -1;
            sub_8000670(gUnknown_030007E8.field_4);
            REG_IME = 1;
        }
    }
}

extern s32 sub_8000680(void *callback);
extern void sub_80006A8(void);

/* Starts a screen-brightness fade: `flags` bit 0 selects the blend
 * target (`REG_BLDCNT`, `0xBF` vs `0xFF`), bit 7 selects
 * direction (fade in from `0x10` vs fade out from `0`); `frameDelay`
 * (clamped to at least 1) is how many frames each of the 17 steps
 * takes. Refuses to start (silently) if a fade is already running -
 * `gUnknown_030007E8.field_0` is the sentinel `-1` only when idle,
 * checked via the classic `(~x + 1) | ~x < 0` "x != -1" bit-trick
 * rather than a plain comparison (matching the ROM's exact `mvn; neg;
 * orr; cmp` sequence - a direct `!= -1` compiles to a shorter
 * load-constant-and-compare instead). If `sync` is nonzero, registers
 * `sub_80012AC` as a periodic callback (via `sub_8000680`) to drive the
 * fade one step per call and returns immediately; otherwise it blocks
 * here, looping through all 17 steps itself and busy-waiting
 * `frameDelay` VBlanks between each via `sub_80006A8`. */
void sub_800132C(u8 flags, s32 frameDelay, u8 sync)
{
    {
        s32 f = gUnknown_030007E8.field_0;
        s32 notf = ~f;
        s32 t = -notf;
        t |= notf;
        if (t < 0) {
            return;
        }
    }

    if (frameDelay <= 0) {
        frameDelay = 1;
    }

    if (flags & 1) {
        REG_BLDCNT = 0xBF;
    } else {
        REG_BLDCNT = 0xFF;
    }

    if (sync != 0) {
        u8 dirBit = flags & 0x80;
        if (dirBit != 0) {
            REG_BLDY = 0x10;
        } else {
            REG_BLDY = dirBit;
        }
        REG_IME = 0;
        gUnknown_030007E8.field_8 = flags;
        gUnknown_030007E8.field_0 = frameDelay;
        gUnknown_030007E8.field_4 = sub_8000680(sub_80012AC);
        REG_IME = 1;
    } else {
        s32 i = 0;
        register s32 dirBit8 asm("r8");
        dirBit8 = flags & 0x80;
        do {
            s32 next;
            if (dirBit8 != 0) {
                REG_BLDY = 0x10 - i;
            } else {
                REG_BLDY = i;
            }
            next = i + 1;
            if (frameDelay > 0) {
                s32 k = frameDelay;
                do {
                    sub_80006A8();
                    k--;
                } while (k != 0);
            }
            i = next;
        } while (i <= 0x10);
    }
}
