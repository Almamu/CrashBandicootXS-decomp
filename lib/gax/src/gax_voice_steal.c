#include "gax_internal.h"

/* SFX voice `i`: the mixer's children past the song's own channels. */
#define GAX_SFX_VOICE(i) \
    ((struct GaxChannelState *)GAX_MIXER()->children[GAX_MIXER()->type->childCount + (i)])

/* The three functions in this file (issue #67) were NAKED behind a
 * "nested-pointer register allocation" note; written plainly against
 * the handler structs in gax_internal.h - re-deriving the player /
 * mixer chain at every use, as the ROM does - all three match outright.
 * See docs/matching/archive/gax-toolchain-retry.md. */

/* Per-frame mixer tick: once playing, clears the song's scratch
 * buffer, clamps and forwards a couple of song-header values into the
 * Info handler / player state, mixes into the current half of the
 * double-buffered output (`GaxMixFrame`), flips the half, and - when
 * player 1's song signals it (`songEnded`) - switches back to player 0,
 * relinking the song's channels into player 0's mixer. */
void GAX_play(void)
{
    s32 i;

    if (gGaxPlayerState->state == 0)
        return;
    if (GAX_SONG()->scratch != NULL)
        GaxZeroFill(GAX_SONG()->scratch, 0x40);
    if (GAX_SONG()->volume > 0xff)
        GAX_SONG()->volume = 0xff;
    GAX_INFO()->volume = GAX_SONG()->volume;
    gGaxPlayerState->field_180 = GAX_SONG()->field_0a;
    GAX_INFO()->playing = 1;
    GaxMixFrame(GAX_MIXER(), (u32 *)(gGaxPlayerState->outBuf +
                                     GAX_MIXER()->format->frames * gGaxPlayerState->outHalf));
    gGaxPlayerState->outHalf ^= 1;
    GAX_SONG()->songEnded = GAX_INFO()->songEnded;
    if (gGaxPlayerState->curChannelIdx == 1 && GAX_SONG()->songEnded != 0) {
        gGaxPlayerState->curChannelIdx = 0;
        GAX_SONG()->jingleEnded = 1;
        if (GAX_SONG()->sfxTypes != NULL) {
            for (i = 0; i < GAX_SONG()->numSfx; i++) {
                ((struct GaxChannelState *)GAX_PLAYER()[GAX_SONG()->layout->count + i])
                    ->children[0] = GAX_PLAYER()[1];
                GAX_MIXER()->children[GAX_MIXER()->type->childCount + i] =
                    GAX_PLAYER()[GAX_SONG()->layout->count + i];
            }
        }
    }
    gGaxPlayerState->playDone = 1;
}

/* UNUSED - no caller anywhere in the ROM. Unconditionally steals the
 * lowest-priority SFX voice for `instrument` (note 8, priority 0) and
 * returns its index. */
s32 GAX_fx(u32 instrument)
{
    s32 best = 0x7fffffff;
    u32 i;
    /* Self-initialized to silence -Wuninitialized: like the original,
     * `sel` stays unset if there is no SFX voice, and `= 0` changes the
     * code (#577). */
    s32 sel = sel;

    for (i = 0; i < GAX_MIXER()->extraChildren; i++) {
        if (GAX_SFX_VOICE(i)->priority <= best) {
            sel = i;
            best = GAX_SFX_VOICE(i)->priority;
        }
    }
    GAX_SFX_VOICE(sel)->pendingNote = 8;
    GAX_SFX_VOICE(sel)->pendingInstrument = instrument;
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
s32 GAX_fx_ex(u32 instrument, s32 channel, s32 priority, s32 pitch)
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

        v->pendingNote = pitch != -1 ? (pitch >> 5) + 2 : 8;
        GAX_SFX_VOICE(sel)->pendingInstrument = instrument;
        GAX_SFX_VOICE(sel)->priority = priority;
    }
    return sel;
}
