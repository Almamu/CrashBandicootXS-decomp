#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;
extern void sub_8037F3C(void *dest, s32 count);
extern void sub_803A5A8(struct GaxMixerHandler *mixer, u32 buf);

/* SFX voice `i`: the mixer's children past the song's own channels. */
#define GAX_SFX_VOICE(i) \
    ((struct GaxChannelState *)GAX_MIXER()->children[GAX_MIXER()->type->childCount + (i)])

/* The three functions in this file (issue #67) were NAKED behind a
 * "nested-pointer register allocation" note; written plainly against
 * the handler structs in include/audio.h - re-deriving the player /
 * mixer chain at every use, as the ROM does - all three match outright.
 * See docs/matching/gax-toolchain-retry.md. */

/* Per-frame mixer tick: once playing, clears the song's scratch
 * buffer, clamps and forwards a couple of song-header values into the
 * Info handler / player state, mixes into the current half of the
 * double-buffered output (`sub_803A5A8`), flips the half, and - when
 * player 1's song signals it (`field_21`) - switches back to player 0,
 * relinking the song's channels into player 0's mixer. */
void sub_8038C88(void)
{
    s32 i;

    if (gUnknown_03001630->state == 0)
        return;
    if (GAX_SONG()->scratch != NULL)
        sub_8037F3C(GAX_SONG()->scratch, 0x40);
    if (GAX_SONG()->field_10 > 0xff)
        GAX_SONG()->field_10 = 0xff;
    GAX_INFO()->field_1f = GAX_SONG()->field_10;
    gUnknown_03001630->field_180 = GAX_SONG()->field_0a;
    GAX_INFO()->field_1a = 1;
    sub_803A5A8(GAX_MIXER(), gUnknown_03001630->field_18
                                 + GAX_MIXER()->format->frames * gUnknown_03001630->field_2c);
    gUnknown_03001630->field_2c ^= 1;
    GAX_SONG()->field_39 = GAX_INFO()->field_21;
    if (gUnknown_03001630->curChannelIdx == 1 && GAX_SONG()->field_39 != 0) {
        gUnknown_03001630->curChannelIdx = 0;
        GAX_SONG()->field_3a = 1;
        if (GAX_SONG()->sfxTypes != NULL) {
            for (i = 0; i < GAX_SONG()->numSfx; i++) {
                ((struct GaxChannelState *)GAX_PLAYER()[GAX_SONG()->layout->count + i])->children[0]
                    = GAX_PLAYER()[1];
                GAX_MIXER()->children[GAX_MIXER()->type->childCount + i]
                    = GAX_PLAYER()[GAX_SONG()->layout->count + i];
            }
        }
    }
    gUnknown_03001630->field_43 = 1;
}

/* UNUSED - no caller anywhere in the ROM. Unconditionally steals the
 * lowest-priority SFX voice for `instrument` (note 8, priority 0) and
 * returns its index. */
s32 sub_8038DC0(u32 instrument)
{
    s32 best = 0x7fffffff;
    u32 i;
    s32 sel;

    for (i = 0; i < GAX_MIXER()->extraChildren; i++) {
        if (GAX_SFX_VOICE(i)->priority <= best) {
            sel = i;
            best = GAX_SFX_VOICE(i)->priority;
        }
    }
    GAX_SFX_VOICE(sel)->field_24 = 8;
    GAX_SFX_VOICE(sel)->field_25 = instrument;
    GAX_SFX_VOICE(sel)->priority = 0;
    return sel;
}

/* The SFX voice allocator (docs/audio.md): picks SFX voice `channel`
 * (or, for -1, the lowest-priority one not above `priority`), refusing
 * a voice already playing something more important, and queues
 * `instrument` on it at `pitch` (-1 = default note 8) with `priority`.
 * Returns the voice index or -1.
 *
 * The final "found one" test compares against 0x0FFFFFFF, not -1 - a
 * quirk of the original source (so a refused voice still gets
 * written); reproduced as-is. */
s32 sub_8038E74(u32 instrument, s32 channel, s32 priority, s32 pitch)
{
    s32 sel = -1;
    s32 best = priority;
    u32 i;

    if (channel == -1) {
        for (i = 0; i < GAX_MIXER()->extraChildren; i++) {
            if (GAX_SFX_VOICE(i)->priority <= best) {
                sel = i;
                best = GAX_SFX_VOICE(i)->priority;
            }
        }
    } else {
        sel = channel;
        if ((u32)sel >= GAX_MIXER()->extraChildren)
            sel = -1;
        if (sel != -1 && priority < GAX_SFX_VOICE(sel)->priority)
            sel = -1;
    }
    if (sel != 0x0fffffff) {
        struct GaxChannelState *v = GAX_SFX_VOICE(sel);

        v->field_24 = pitch != -1 ? (pitch >> 5) + 2 : 8;
        GAX_SFX_VOICE(sel)->field_25 = instrument;
        GAX_SFX_VOICE(sel)->priority = priority;
    }
    return sel;
}
