#include "gax_internal.h"

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

    p->orderPos = 0xFFFF;
    p->row = 0x4E20;
    p->tickCounter = 0;
    p->speed = 6;
    p->volume = 0xff;
    p->newRow = 0;
    p->newOrder = 0;
    p->patternBreak = 0;
    p->breakRow = 0;
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
 * (`tick`, the mixer's `pos`) already ran (`lastTick`), or when the
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
 * Plain C since #662 round 2 (it was pins, two `ldrsh` asm blocks and
 * retyped `speed`/`row` accesses). Two details: `p` is the parameter
 * itself (a `void *self` copied to `p` makes gcc copy `tick` first), and
 * the final low-byte test reads `p->speed` again rather than a local:
 * GCSE then keeps the halfword in r4 from the top (and reloads it after
 * a new row), as the ROM does. */
u32 GaxInfoPlay(struct GaxInfoHandler *p, u32 arg1, u32 tick)
{
    if (p->lastTick == tick) {
        return 0;
    }
    if (p->playing == 0) {
        return 0;
    }

    if ((p->speed & 0xff) != 0 && p->tickCounter == 0) {
        if (p->patternBreak != 0) {
            p->patternBreak = 0;
            p->row = p->type->data.song->patternRows;
        } else {
            p->row++;
        }
        if (p->speed >> 8) {
            p->speed = (p->speed >> 8) | ((p->speed & 0xff) << 8);
        }
        p->tickCounter = (u8)p->speed - 1;
        if (p->row >= p->type->data.song->patternRows) {
            p->row = 0;
            p->breakRow = 0;
            p->newOrder = 1;
            p->orderPos++;
            if (p->orderPos >= p->type->data.song->orderCount) {
                if (p->stopAtEnd != 0) {
                    p->playing = 0;
                    p->speed = 0;
                }
                p->songEnded = 1;
                p->orderPos = p->type->data.song->loopOrder;
            }
        } else {
            p->newOrder = 0;
        }
        p->newRow = 1;
    } else {
        p->tickCounter--;
        p->newRow = 0;
    }

    if ((p->speed & 0xff) == 0) {
        p->songEnded = 1;
    }
    p->lastTick = tick;
    if (p->firstTick == 0) {
        p->firstTick = tick;
    }
    if (p->muteTicks != 0) {
        p->muteTicks--;
    }

    return 0;
}
