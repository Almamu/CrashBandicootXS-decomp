#include "core.h"

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-54-actor-d3a8.md.
 * Eases `self`'s cached position (`self+0x1c`/`0x20`/`0x24`, the same
 * fields `InitActorPart` caches its `b`/`c`/`d` constructor arguments
 * into, per actor_part50.c) toward a caller-supplied target, with the
 * exact target/mode selected by `self+0x28` ("state"):
 *   - state 0: eases toward `posX`/`posY` offset by a per-frame-counter
 *     (`self+0x44`) lookup into `gStaticData_0816A820` (two different
 *     index strides for the X/Y offsets, producing a scatter/orbit-style
 *     curve), and toward `posZ-0x200`.
 *   - state 1: snaps (no easing) directly to `posX` for the X axis, to
 *     `posY` plus a different table-driven offset for Y, and to
 *     `posZ+0x200` for Z.
 *   - any other state: eases toward `posX`/`posY` directly (no table
 *     offset), and toward `posZ+0x200`.
 * "Easing" is a round-toward-zero divide (by 16 for X/Y, by 4 for Z) of
 * the remaining delta, added back onto the cached position - plain
 * integer division reproduces the ROM's own rsb/lsr/add/asr rounding
 * idiom here (see `sub_80070EC` in docs/matching.md).
 *
 * Semantics, register choices and every individual instruction body are
 * confirmed correct; the residual gap is the prologue's argument-
 * register-copy order (`mov ip, r2` before `adds r7, r3, #0` in the ROM)
 * - this compiler always emits the `r7` copy first regardless of C
 * statement order, pin declaration order, or forcing both copies into a
 * single inline-asm block (which fixes the order but then reintroduces
 * a second gap: any explicit `register T x asm("r7")` pin gets
 * clobbered by unrelated scratch constant loads later in the same
 * function, confirmed the same categorical `r7`-pin limitation already
 * documented for `sub_8007DBC` in actor_part2.c and
 * matching_decomp_register_pinning memory point 10). Parked rather than
 * keep chasing this specific compiler quirk. */
void sub_802D3A8(void *selfArg, s32 posX, s32 posY, s32 posZ)
{
    u8 *self = selfArg;
    s32 state = *(s32 *)(self + 0x28);

    if (state == 0) {
        extern s16 gStaticData_0816A820[];
        s32 counter = *(s32 *)(self + 0x44);
        s32 offX = gStaticData_0816A820[(counter << 2) & 0xff];
        s32 targetX = posX + (offX * 24) - 0x1000;
        s32 offY = gStaticData_0816A820[(counter << 1) & 0xff];
        s32 targetY = posY + (offY * 10) - 0x1E00;
        s32 targetZ = posZ - 0x200;
        s32 old;

        old = *(s32 *)(self + 0x1c);
        *(s32 *)(self + 0x1c) = old + (targetX - old) / 16;
        old = *(s32 *)(self + 0x20);
        *(s32 *)(self + 0x20) = old + (targetY - old) / 16;
        old = *(s32 *)(self + 0x24);
        *(s32 *)(self + 0x24) = old + (targetZ - old) / 4;
    } else if (state == 1) {
        extern s16 gStaticData_0816A820[];
        s32 counter = *(s32 *)(self + 0x44);
        s32 off = gStaticData_0816A820[(counter * 9) & 0xff];

        *(s32 *)(self + 0x1c) = posX;
        *(s32 *)(self + 0x20) = posY + (off * 4) - 0xA00;
        *(s32 *)(self + 0x24) = posZ + 0x200;
    } else {
        s32 old;

        old = *(s32 *)(self + 0x1c);
        *(s32 *)(self + 0x1c) = old + (posX - old) / 16;
        old = *(s32 *)(self + 0x20);
        *(s32 *)(self + 0x20) = old + (posY - old) / 16;
        old = *(s32 *)(self + 0x24);
        *(s32 *)(self + 0x24) = old + ((posZ + 0x200) - old) / 4;
    }
}
#endif /* NON_MATCHING */
