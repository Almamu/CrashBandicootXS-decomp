#include "gax_internal.h"
#include "match.h"

/* GAX2's work-RAM size estimator (GitHub issue #66's range, formerly the
 * raw asm/code_3_2_20c.s): computes how many bytes `GAX2_init`
 * (gax_playstart.c) will carve out of the caller's work buffer for song
 * header `p` and stores the requirement in `p->workSize`. It resolves the
 * same defaults `GAX2_init` does (default handler layout, the song's
 * default mix rate and SFX-voice count, no SFX voices without SFX types),
 * then adds up the player state (0x18c, +0xf0 with flags bit 2), the
 * handler array and format, every layout handler's header, instance and
 * children (`GaxCreateHandlers`'s carving, skipping slot 2, which holds the
 * list of alternative layouts), the largest alternative layout (unless
 * flags bit 4), 0x58 per SFX voice, the echo buffer for the highest DSP
 * tap rate across the layouts (`rate * mixRate / 1000` samples), the
 * mixer code (0x130 or 0xdc bytes) and two frame-sized mix buffers
 * (`mixRate * 1000 / 59727` samples each).
 *
 * Matched in the GAX NAKED retry (docs/matching/archive/gax-naked-retry-2.md).
 * What it took:
 * - `p->layout` and `p->flags` are re-read at every use, never cached:
 *   GCSE turns them into the ROM's spilled copies (`[sp, #0x10]`, and
 *   the halfword `strh`/`ldrh [sp, #0x18]` for the flags), stored in the
 *   first loop's preheader, and keeps the carving loop re-reading
 *   `layout->count` without strength reduction;
 * - both divisions are plain `/` (lib1funcs' `__udivsi3`);
 * - index-first `*(i + p->layout->types)` / `*(k + list->layouts)` for
 *   the ROM's `adds rX, rIdx, rBase` operand order;
 * - one counter `i` for the carving loop, the alternative layouts' inner
 *   loop and the tap scans (that's what puts it in r4 behind the inner
 *   loop's walking pointer in r3); the second alternative-layout scan
 *   tests `types[2]` in the condition and keeps `k + 1` in `next`;
 * - the MATCH_BARRIER()s below: see the comment there. */
/* `layout->types[2]`'s slot holds a list of alternative layouts
 * (`struct GaxLayoutList`, gax_internal.h). */
void GAX2_estimate(struct GaxSongHeader *p)
{
    u32 size = 0;
    u32 rate;
    u32 maxRate = 0;
    u32 mixRate;
    u32 numSfx;
    u32 i;

    /* Four empty asms (no code). They only lengthen the RTL: GCSE sizes
     * its expression hash table from the insn count and creates the
     * preheader copies of `p->layout`, `rate << 5` and `p->flags` in
     * hash-bucket order, which fixes their stack-slot order. Without
     * them the slots come out 0x18/0x10/0x14 instead of the ROM's
     * 0x10/0x14/0x18 and nothing else differs; any 4-11 extra insns give
     * the ROM's order. The original source evidently had a few more RTL
     * insns here that later passes removed - not identified. */
    MATCH_BARRIER();
    MATCH_BARRIER();
    MATCH_BARRIER();
    MATCH_BARRIER();
    if (p->layout == NULL)
        p->layout = &gGaxDefaultSong;
    if (p->mixRate == 0xffff)
        mixRate = p->layout->types[1]->data.song->mixRate;
    else
        mixRate = p->mixRate;
    if (p->numSfx == 0xffff)
        numSfx = p->layout->types[1]->data.song->numSfx;
    else
        numSfx = p->numSfx;
    if (p->sfxTypes == NULL)
        p->numSfx = numSfx = 0;
    rate = gGaxMixRates[GaxFindMixRate(mixRate)].rate;
    if (p->flags & 4)
        size += 0xf0;
    size += 0x18c;
    size += (p->layout->count + numSfx) * 4;
    size += 8;
    for (i = 0; i < p->layout->count; i++) {
        struct GaxHandlerType *t = *(i + p->layout->types);

        if (i != 2) {
            size += 0xc + t->instanceSize;
            size += t->childCount * 4;
            if (i == 0)
                size += numSfx * 4;
        }
    }
    if (!(p->flags & 0x10)) {
        struct GaxLayoutList *list = (struct GaxLayoutList *)p->layout->types[2];

        if (list != NULL) {
            s32 max = 0;
            s32 k;

            for (k = 0; k < (s32)list->count; k++) {
                struct GaxHandlerLayout *l = list->layouts[k];
                s32 need = (l->count + numSfx) * 4;

                for (i = 0; i < l->count; i++) {
                    struct GaxHandlerType *t = l->types[i];

                    if (i != 2) {
                        need += 0xc;
                        need += t->instanceSize;
                        need += t->childCount * 4;
                        if (i == 0)
                            need += numSfx * 4;
                    }
                }
                if (need > max)
                    max = need;
            }
            size += max;
        }
    }
    for (i = 0; i < numSfx; i++)
        size += 0x58;
    {
        struct GaxDspTap *tap;

        i = 0;
        tap = p->layout->types[0]->data.dsp->taps;
        for (; i <= 2; i++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
            tap++;
        }
    }
    if (!(p->flags & 0x10) && p->layout->types[2] != NULL) {
        struct GaxLayoutList *list = (struct GaxLayoutList *)p->layout->types[2];
        s32 k;
        s32 next;

        for (k = 0; k < (s32)list->count; k = next) {
            struct GaxDspTap *tap;
            struct GaxHandlerLayout *l;

            l = *(k + list->layouts);
            i = 0;
            next = k + 1;
            tap = l->types[0]->data.dsp->taps;
            for (; i <= 2; i++) {
                if (tap->rate > maxRate)
                    maxRate = tap->rate;
                tap++;
            }
        }
    }
    if (maxRate != 0) {
        size += maxRate * rate / 1000 * 2;
        size += 0x18;
    }
    if (p->layout->types[1]->data.song->halfRateFx != 0 || (p->flags & 0x20))
        size += 0x130;
    else
        size += 0xdc;
    {
        u32 frames = rate * 1000 / 0xe94f;

        size += frames * 2;
        size += frames * 2;
    }
    p->workSize = size + 0x20;
}
