#include "gax_internal.h"
#include <libgcc.h>
#include "match.h"

/* GAX2's play-start/init entry point (per docs/audio.md): initializes the
 * runtime player-state object at gGaxPlayerState (magic, songPtr,
 * curChannelIdx/channels/0x24/0x41/0x43 defaults), validates the caller's
 * item count against a 0x18B (395) sanity maximum, fills in default fields
 * (instrument-bank pointer gGaxDefaultSong, default volume 0xFF) when
 * the caller left them zero, looks up a starting song-slot index
 * (GaxFindMixRate) and computes its fixed-point tempo (__divsi3), copies
 * two whole gGaxArmDownmix/0803A73C/0803A818 initializer blocks into
 * the player state (0x48+/0x9c+, 21 and 56 words), computes the sanity-
 * checked buffer bounds for channel/pattern data, arms the loop/priority
 * defaults, and finally allocates the channel table (GaxCreateHandlers) and
 * re-arms the hardware DMA1/SOUNDCNT output path (GaxResetSoundHardware, matched
 * above in gax_hw_reset.c) - showing GAX2's fatal-error screen
 * (GaxFatalError) on any sanity-check failure along the way instead of
 * returning normally.
 *
 * Matched in GAX retry 5 (docs/matching/archive/gax-naked-retry-5.md) after four
 * earlier passes (gax-toolchain-retry.md, gax-naked-retry-2/3/4.md). The
 * last pieces were: indexed copies of the constant ARM-code tables (GCSE
 * hoists their addresses to the first block in the ROM's order), a
 * separate counter for the filterCode copy, `layout` copied from a
 * block-local read after the first types[] load, and three no-code
 * register nudges (commented at each use). #662 round 4 replaced the
 * third, an r3 hold, with `layout` walking on to the alternative-layout
 * list; #662 round 9 the second, a use of the aligned size, with
 * GaxPlayerState.outBuf's pointer type (see there). */

#define ALIGN4(buf, size)                                  \
    {                                                      \
        u32 pad_ = (((u32)(buf) + 4) & ~3) - (u32)(buf);   \
        (buf) += pad_;                                     \
        (size) -= pad_;                                    \
    }

u8 GAX2_init(struct GaxSongHeader *p)
{
    struct GaxChannelFormat *fmt;
    u32 maxRate = 0;
    u8 *buf = p->workBuf;
    u32 size = p->workSize;
    u32 n;
    u32 i;
    s32 idx;
    s32 k;
    struct GaxHandlerLayout *layout;

    if (size <= 0x18b)
        goto fail;
    gGaxPlayerState = (struct GaxPlayerState *)buf;
    buf += 0x18c;
    size -= 0x18c;
    if (p->layout == NULL)
        p->layout = &gGaxDefaultSong;
    if (p->sfxTypes == NULL)
        p->numSfx = 0;
    if (p->mixRate == 0xffff)
        p->mixRate = p->layout->types[1]->data.song->mixRate;
    if (p->numSfx == 0xffff)
        p->numSfx = p->layout->types[1]->data.song->numSfx;
    if (p->volume == 0xffff)
        p->volume = 0xff;
    gGaxPlayerState->magic = 0x47415832;
    gGaxPlayerState->songPtr = p;
    gGaxPlayerState->state = 0;
    gGaxPlayerState->curChannelIdx = 0;
    gGaxPlayerState->echoTaps = 0;
    gGaxPlayerState->skipSongChannels = 0;
    gGaxPlayerState->playDone = 1;
    n = p->layout->count;
    if (p->sfxTypes != NULL)
        n += p->numSfx;
    gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx] = buf;
    size = size - n * 4;
    fmt = (struct GaxChannelFormat *)(buf + n * 4);
    buf = (u8 *)fmt + 8;
    size -= 8;
    gGaxPlayerState->format = fmt;
    idx = GaxFindMixRate(p->mixRate);
    fmt->bits = 8;
    fmt->channels = 1;
    fmt->mixRate = gGaxMixRates[idx].rate;
    fmt->frames = fmt->mixRate * 1000 / 0xe94f;
    gGaxPlayerState->timerReload = gGaxMixRates[idx].timer;
    gGaxPlayerState->fxEcho = 0;
    if (gGaxVersionStringPtr[2] != 'X' || gGaxVersionStringPtr[1] != 'A' ||
        gGaxVersionStringPtr[0] != 'G')
        gGaxPlayerState->timerReload <<= 1;
    if (size < (gGaxPlayerState->format->frames + 4) * 2)
        goto fail;
    gGaxPlayerState->mixBuf = (u32)buf;
    buf += (gGaxPlayerState->format->frames + 4) * 2;
    size -= (gGaxPlayerState->format->frames + 4) * 2;
    ALIGN4(buf, size);
    gGaxPlayerState->outHalf = 0;
    if (size < gGaxPlayerState->format->frames * 2)
        return 0;
    gGaxPlayerState->outBuf = (s8 *)buf;
    buf += gGaxPlayerState->format->frames * 2;
    size -= gGaxPlayerState->format->frames * 2;
    GaxZeroFill(gGaxPlayerState->outBuf, gGaxPlayerState->format->frames * 2);
    ALIGN4(buf, size);
    {
        struct GaxDspTap *tap;
        struct GaxHandlerLayout *l;
        struct GaxHandlerType *t0;

        /* `layout` is copied from a block-local read after the first
         * types[] load, as in the ROM (`adds r4, r0, #0` between them) */
        i = 0;
        l = p->layout;
        t0 = l->types[0];
        layout = l;
        tap = t0->data.dsp->taps;
        for (; i <= 2; i++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
            /* no code: an extra reference that gives `maxRate` r8 and
             * `fmt` r9, as in the ROM (#662 rounds 3-11). global.c ranks
             * them by (int)(floor_log2(refs) * refs / live length *
             * 10000), refs counted after combine (by loop depth), live
             * length from flow. cse1 gives the prologue `maxRate = 0` a
             * REG_EQUAL 0 note, and local-alloc's update_equiv_regs,
             * seeing that constant set first, doubles maxRate's live
             * length (282 -> 564 insns; the later non-constant sets drop
             * the equivalence but not the doubling). So fmt, 9 refs over
             * 197 insns (1370), beats maxRate, 18 over 564 (1276); this
             * use, counted twice inside the loop, makes it 20 (1413).
             * Without it the ROM's code needs 2 more maxRate refs, the
             * init-to-carve stretch (84 flow insns) under ~65, or fmt
             * live over ~255. What doesn't work:
             * - the `maxRate = 0` moved later (after the size check,
             *   before the state stores, after the carve or the mix-rate
             *   lookup, before the scan): the ROM's prologue zero moves
             *   and the zero stores before the carve (numSfx, state,
             *   curChannelIdx, echoTaps, skipSongChannels) lose r8;
             *   only "before the state stores" flips the order (1484),
             *   28 off; seeding the max from the first tap changes the
             *   ROM's three-iteration scans; nothing before the carve
             *   can follow it in the ROM's order;
             * - every split of fmt (a copy, an inline carve helper):
             *   the order flips, but the ROM's `mov r1, r9` for the + 8
             *   and the format store become a lo register (42-48 off);
             * - inline helpers on fmt or maxRate (#851's parameter
             *   pseudo): folded by cse1 before flow; `&maxRate` to an
             *   inline can't be purged in C (process_reg_param copies
             *   the ADDRESSOF), so maxRate goes to the stack;
             * - `maxRate = maxRate > rate ? maxRate : rate`: 20-23 refs,
             *   but an extra `mov r0, r8` per scan; the other max
             *   spellings, declaration orders, retypes, zero-initialized
             *   locals as in GAX2_estimate, the 0xffff test spellings,
             *   old_agbcc and natural_enum.py (267,662 variants, round
             *   9) find nothing. */
            MATCH_USE(maxRate);
            tap++;
        }
    }
    if (!(p->flags & 0x10) && layout->types[2] != NULL) {
        /* `layout` walks on to the song's alternative layouts: types[2]
         * is a list with a layout's own shape (a count, then pointers;
         * GaxLayoutList), and the same variable holds it. As one pseudo
         * the song layout and the list conflict with the inner scan's
         * `next` (r3), so global-alloc puts them in r4, as in the ROM
         * (#662 round 4). With a separate `subs` local the song layout
         * alone takes the free r3; that took an r3 pin held over the
         * first tap scan. Indexed as gax_work_size.c indexes types[]
         * (`*(i + types)`), for the ROM's `adds r0, r0, r4`. */
        layout = (struct GaxHandlerLayout *)layout->types[2];
        {
            s32 j;
            s32 next;

            for (j = 0; j < (s32)layout->count; j = next) {
                struct GaxDspTap *tap;
                struct GaxHandlerLayout *l = (struct GaxHandlerLayout *)*(j + layout->types);

                i = 0;
                next = j + 1;
                tap = l->types[0]->data.dsp->taps;
                for (; i <= 2; i++) {
                    if (tap->rate > maxRate)
                        maxRate = tap->rate;
                    tap++;
                }
            }
        }
    }
    if (maxRate != 0) {
        struct GaxPlayerState *g;
        u8 *echo;
        u32 left;
        u32 len;

        ALIGN4(buf, size);
        if (size <= 23)
            goto fail;
        g = gGaxPlayerState;
        g->echoTaps = (u32)buf;
        echo = buf + 24;
        buf = echo;
        left = size - 24;
        size = left;
        len = maxRate * fmt->mixRate / 1000 * 2;
        if (left < len)
            goto fail;
        g->echoBuf = (u32)echo;
        g->echoLen = len;
        buf = echo + len;
        size = left - len;
        ALIGN4(buf, size);
        GaxZeroFill(echo, len);
    }
    {
        /* Indexed copies of the constant tables: GCSE's PRE hoists the
         * `&gGaxPlayerState`, `p->layout`, 0803A73C and 0803A818 loads to
         * the end of this first block, in the ROM's order, and loop.c
         * strength-reduces each index into the `ldmia` pointer. */
        const u32 *src;

        for (k = 0; k <= 20; k++)
            gGaxPlayerState->downmixCode[k] = gGaxArmDownmix[k];
        src = gGaxArmEcho;
        for (k = 0; k <= 55; k++)
            gGaxPlayerState->echoCode[k] = src[k];
        src = gGaxArmResample;
        {
            s32 words;
            if (p->layout->types[1]->data.song->halfRateFx != 0 || (u16)(p->flags & 0x20)) {
                gGaxPlayerState->fullResampler = 1;
                words = 76;
            } else {
                gGaxPlayerState->fullResampler = 0;
                words = 55;
            }
            if (size < words * 4)
                goto fail;
            gGaxPlayerState->mixCode = buf;
            buf += words * 4;
            size -= words * 4;
            for (k = 0; (s32)k < words; k++)
                ((u32 *)gGaxPlayerState->mixCode)[k] = src[k];
        }
    }
    if ((u16)(p->flags & 4)) {
        if (size <= 239)
            goto fail;
        gGaxPlayerState->filterCode = buf;
        buf += 240;
        size -= 240;
        {
            /* its own counter: sharing `k` makes it conflict with the
             * 380 offset constant and pushes `k` out of r2 above */
            s32 m;

            for (m = 0; m <= 59; m++)
                ((u32 *)gGaxPlayerState->filterCode)[m] = gGaxArmFilter[m];
        }
    } else {
        gGaxPlayerState->filterCode = NULL;
    }
    gGaxPlayerState->filter = 0;
    if (!GaxCreateHandlers(p->layout, p->sfxTypes, p->numSfx, &buf, &size))
        goto fail;
    ALIGN4(buf, size);
    GAX_MIXER()->extraChildren = p->numSfx;
    GAX_MIXER()->mixBuf = gGaxPlayerState->mixBuf;
    GAX_MIXER()->type->init(GAX_MIXER());
    gGaxPlayerState->workBuf = buf;
    gGaxPlayerState->workSize = size;
    GaxResetSoundHardware();
    gGaxPlayerState->state = 1;
    GAX_INFO()->stopAtEnd = (p->flags >> 3) & 1;
    GAX_SONG()->songEnded = 0;
    GAX_SONG()->jingleEnded = 0;
    /* nested, not `&&`: keeps the ROM's u16 test and its two
     * `fxEcho = 0` stores sharing the tested zero */
    if ((u16)(p->flags & 2)) {
        if (((u32 *)GAX_MIXER()->type->data.dsp)[1] != 0)
            gGaxPlayerState->fxEcho = 1;
        else
            gGaxPlayerState->fxEcho = 0;
    } else
        gGaxPlayerState->fxEcho = 0;
    return 1;
fail:
    if (p->showErrors)
        GaxFatalError(gGaxErrNameInit, gGaxErrOutOfMemory);
    return 0;
}
