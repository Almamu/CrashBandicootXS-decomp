#include "gax_internal.h"
#include "match.h"

/* All three fixed per-type function-pointer constants docs/audio.md
 * records for the GAX2_SoundHandler "Info" type (init_fn/unknown_fn/
 * play_fn = 0x080393FD/0x08039439/0x0803943D) live in this cluster:
 * GaxInfoInit (init_fn), GaxInfoUnknown (unknown_fn, a no-op stub), and
 * GaxInfoPlay (play_fn) are all matched here. The object they operate
 * on is the shared `struct GaxInfoHandler` (gax_internal.h) - the song
 * position every Channel handler reads through its `children[0]`. */

/* Resets the shared per-instance state every SoundHandler "Info"-type
 * object carries, regardless of which higher-level init function called
 * it (GaxInfoInit below, and GaxInfoRestart, both reset different subsets
 * of fields first and then fall through to this shared core). The
 * `orderPos`/`row` values make the first tick start order 0: `row` is
 * past any pattern's end, so it wraps and `orderPos` steps to 0. */
void GaxInfoResetPosition(void *self)
{
    struct GaxInfoHandler *p = self;
    u16 val;
    u8 zeroByte;
    MATCH_HOLD_REG(u16, zeroHalf, r3);

    /* 0xFFFF/0x4E20 need to go through a named temp before the store -
     * assigning the literal straight to the dereferenced address loads
     * it into a scratch register and copies that into the real
     * destination register first, one instruction more than the ROM's
     * direct `ldr r0,=...; strh r0,[...]` (same gotcha documented on
     * GAX2_new in gax_new.c). The two zero-fill temps
     * (`zeroByte`/`zeroHalf`) both get set right after the first store,
     * matching the ROM's `movs r2,#0; movs r3,#0` pair, rather than
     * being zeroed right before each individual use; the empty asm
     * statements keep them there once the stores are struct fields. */
    val = 0xFFFF;
    p->orderPos = val;
    zeroByte = 0;
    MATCH_KEEP(zeroByte);
    zeroHalf = 0;
    MATCH_KEEP(zeroHalf);
    val = 0x4E20;
    p->row = val;
    p->tickCounter = zeroByte;
    p->speed = 6;
    p->volume = 0xff;
    p->newRow = zeroByte;
    p->newOrder = zeroByte;
    p->patternBreak = zeroByte;
    p->breakRow = zeroHalf;
}

/* GAX2_SoundHandler "Info" type's init_fn (ROM 0x080393FD, see
 * docs/audio.md). */
void GaxInfoInit(void *self)
{
    struct GaxInfoHandler *p = self;

    p->firstTick = 0;
    p->lastTick = 0;
    p->playing = 0;
    p->muteTicks = 0;
    p->stopAtEnd = 0;
    p->songEnded = 0;
    GaxInfoResetPosition(self);
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in expected/*.s, no
 * pointer to 0x0803941D in baserom.gba). Restarts the song from the top
 * while it keeps playing: the same position reset as GaxInfoInit, but
 * `playing` set and `muteTicks` at 2, so the channels stay silent for the
 * first two ticks (GaxChannelPlay/GaxFxChannelPlay drop their instrument
 * while it is nonzero). */
void GaxInfoRestart(void *self)
{
    struct GaxInfoHandler *p = self;
    u32 zero;

    GaxInfoResetPosition(self);
    zero = 0;
    p->muteTicks = 2;
    p->firstTick = zero;
    p->playing = 1;
}

/* GAX2_SoundHandler "Info" type's unknown_fn (ROM 0x08039439, see
 * docs/audio.md) - a no-op stub. */
void GaxInfoUnknown(void)
{
}

/* GAX2_SoundHandler "Info" type's play_fn (ROM 0x0803943D, see
 * docs/audio.md's per-type function-pointer table): advances the song
 * position once per mixer tick. Every Channel handler shares this one
 * Info handler, so it bails out (still returning 0) when this tick
 * (`chanArg`, the mixer's `pos`) already ran (`lastTick`), or when the
 * song isn't playing (`playing == 0`).
 *
 * When a row is due (`speed`'s low byte nonzero and `tickCounter` run
 * down to 0), it steps `row` - or jumps straight to the pattern's end
 * when the one-shot `patternBreak` flag is set - swaps `speed`'s two bytes
 * if the high one is set (alternating row speeds), and reloads
 * `tickCounter` from the new low byte. At the end of a pattern
 * (`row >= song->patternRows`) it resets `row`/`breakRow`, flags
 * `newOrder` and steps `orderPos`; past the last order
 * (`song->orderCount`) it stops the song when `stopAtEnd` is set, flags
 * `songEnded` and loops back to `song->loopOrder`. Otherwise it just
 * counts `tickCounter` down and clears `newRow`. Finally it records
 * the tick in `lastTick` (and `firstTick`, if that's still unset) and
 * counts `muteTicks` down if armed.
 *
 * Several compiler-quirk fixups were needed to match:
 * - The `tickCounter==0` check's "known zero" value needs materializing
 *   into its own register *only after* confirming `speed(byte)!=0`
 *   (nested `if`, not a single `&&`) - the ROM only computes this copy
 *   inside the outer branch, then reuses it both for the second
 *   comparison and (via `goto`) as the literal 0 later stored into
 *   `patternBreak`.
 * - The two `row`/`orderPos` reads compared against the song's table
 *   are 16-bit *signed* loads at a non-immediate offset (0x16/0x14) -
 *   Thumb's `ldrsh` has no immediate-offset form, only register-offset,
 *   and gcc's default codegen for this pattern picks the wrong register
 *   pairing (and a redundant sign-extend masking pass) compared to the
 *   ROM's `movs r0,#0x16; ldrsh r1,[r3,r0]` shape - transcribed as a
 *   tiny raw-asm block per read to force the ROM's exact register
 *   choice.
 * - The `tickCounter`/`newRow` "not retriggering" fallback path needs
 *   its decrement computed into a *fresh* register (pinned to r0)
 *   rather than modified in place on the register `tickCounter` was
 *   loaded into - otherwise gcc reuses that register directly instead
 *   of the ROM's separate `subs r0,r1,#1` / `movs r1,#0` pair. */
u32 GaxInfoPlay(void *self, u32 arg1, u32 chanArg)
{
    struct GaxInfoHandler *p = self;
    MATCH_HOLD_REG(u32, c, r6) = chanArg;
    u8 b18lo;
    MATCH_HOLD_REG(u16, h18, r4);
    MATCH_HOLD_REG(u8, b1c, r1);

    if (p->lastTick == c) {
        return 0;
    }
    if (p->playing == 0) {
        return 0;
    }

    b18lo = *(u8 *)&p->speed;
    h18 = p->speed;
    b1c = p->tickCounter;

    if (b18lo != 0) {
        u32 zero = b1c;
        if (zero == 0) {
            u8 *flag = &p->patternBreak;

            if (*flag != 0) {
                *flag = zero;
                p->row = p->type->data.song->patternRows;
            } else {
                p->row = p->row + 1;
            }

            {
                u16 v18 = p->speed;
                u16 lo = v18 >> 8;
                if (lo != 0) {
                    MATCH_HOLD_REG(u16, mask, r0) = 0xff;
                    MATCH_HOLD_REG(u32, hi, r0);
                    hi = mask & v18;
                    hi <<= 8;
                    lo |= hi;
                    p->speed = lo;
                }
            }

            {
                u8 dec = *(u8 *)&p->speed - 1;
                h18 = 0;
                p->tickCounter = dec;

                {
                    MATCH_HOLD_REG(s32, cnt, r1);
                    struct GaxHandlerType *base0;
                    u16 thresh;

                    asm("movs r0, #0x16\n\tldrsh r1, [%1, r0]" : "=r"(cnt) : "r"(p) : "r0");
                    base0 = p->type;
                    thresh = base0->data.song->patternRows;

                    if (!(cnt < thresh)) {
                        u8 one;
                        MATCH_HOLD_REG(s32, cnt2, r1);
                        u16 thresh2;

                        /* Stored through plain `u16 *` casts: a direct
                         * field store copies `h18` into r0 first. */
                        *(u16 *)&p->row = h18;
                        *(u16 *)&p->breakRow = h18;
                        one = 1;
                        p->newOrder = one;
                        p->orderPos = p->orderPos + 1;

                        asm("movs r0, #0x14\n\tldrsh r1, [%1, r0]" : "=r"(cnt2) : "r"(p) : "r0");
                        thresh2 = base0->data.song->orderCount;

                        if (!(cnt2 < thresh2)) {
                            if (p->stopAtEnd != 0) {
                                p->playing = 0;
                                p->speed = h18;
                            }
                            p->songEnded = one;
                            p->orderPos = p->type->data.song->loopOrder;
                        }
                    } else {
                        p->newOrder = h18;
                    }
                }
            }
            p->newRow = 1;
            h18 = p->speed;
            goto after_retrigger;
        }
    }
    {
        MATCH_HOLD_REG(u8, dec, r0) = b1c - 1;
        u8 zeroB = 0;
        p->tickCounter = dec;
        p->newRow = zeroB;
    }
after_retrigger:

    if ((h18 & 0xff) == 0) {
        p->songEnded = 1;
    }

    p->lastTick = c;
    if (p->firstTick == 0) {
        p->firstTick = c;
    }
    if (p->muteTicks != 0) {
        p->muteTicks = p->muteTicks - 1;
    }

    return 0;
}
