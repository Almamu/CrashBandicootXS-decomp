#include "core.h"
#include "audio.h"

/* GAX2's handler instantiation/linking for one player (`GAX2_init`
 * play start, `GAX2_jingle` per-channel pool): carves a `struct
 * GaxHandler` header, the type's instance and its child-pointer array
 * out of `*bufp`/`*sizep` for every handler type of `layout` (and, for
 * player 0, the SFX voice types in `sfx`) - slot 2 holds the list of
 * alternative layouts and is skipped - then links every handler's
 * children by type, hands player 0's SFX voices to player 1 (or wires
 * them into the mixer), numbers the channels (`+0x53`), and fills the
 * mixer's DSP rate table (`step = rate * mixRate / 1000 * 2`). Returns 0
 * if the buffer runs out.
 *
 * Matched in the GAX NAKED retry (docs/matching/gax-naked-retry-2.md).
 * What it took:
 * - the division is a plain `/`: sub_8037E54 is lib1funcs' `__udivsi3`
 *   (see gax-toolchain-retry.md), so the call is a libcall, not an
 *   ordinary call - which is what lets GCSE carry the spilled
 *   `gGaxPlayerState` address into the rate loop (`ldr r1, =...`);
 * - the carving loop's child count goes through its own local (`cnt`)
 *   and `need` is one expression, so `n` lands in r8 and `need` in ip;
 * - the linking loop compares against `t->childTypes[j]` directly (the
 *   ROM loads it twice), and keeps `i + 1` in a block-scoped `next`;
 * - index-first address arithmetic (`*(i + layout->types)`, `i * 8 +
 *   base`, `(i + taps)->rate`) for the ROM's `adds rX, rIdx, rBase`
 *   operand order. */
struct GaxDspRate {
    u32 step;
    u32 value;
};

asm(".set __udivsi3, sub_8037E54");

/* Player 0's mixer handler (the SFX voices' owner). */
#define GAX_PLAYER0_MIXER() (((struct GaxMixerHandler **)gGaxPlayerState->channels[0])[0])

u8 GaxCreateHandlers(struct GaxHandlerLayout *layout, struct GaxHandlerType **sfx, u32 numSfx, u8 **bufp,
               u32 *sizep)
{
    u32 total = layout->count;
    s32 i;

    if (sfx != NULL && gGaxPlayerState->curChannelIdx == 0)
        total += numSfx;
    for (i = 0; i < total; i++) {
        struct GaxHandlerType *t;
        struct GaxHandler *h = (struct GaxHandler *)*bufp;

        if ((s32)i < (s32)layout->count)
            t = layout->types[i];
        else
            t = sfx[i - layout->count];
        if (i != 2) {
            u32 n, need, size;
            u32 cnt = t->childCount;

            if (i == 0)
                cnt += numSfx;
            n = cnt * 4;
            need = n + (sizeof(struct GaxHandler) + t->instanceSize);
            size = *sizep;
            if (size < need)
                return 0;
            GAX_PLAYER()[i] = h;
            h->type = t;
            h->format = gGaxPlayerState->format;
            h->children = (struct GaxHandler **)(*bufp + sizeof(struct GaxHandler) + t->instanceSize);
            *bufp = (u8 *)h->children + n;
            *sizep = size - need;
        }
    }
    {
        s32 next;

        for (i = 0; i < total; i = next) {
            struct GaxHandler *h = GAX_PLAYER()[i];
            struct GaxHandlerType *t;
            u32 j;

            if ((s32)i < (s32)layout->count)
                t = *(i + layout->types);
            else
                t = sfx[i - layout->count];
            next = i + 1;
            if (i != 2) {
                for (j = 0; j < t->childCount; j++) {
                    if (t->childTypes[j] != NULL) {
                        u32 k;

                        for (k = 0; k < total; k++) {
                            if (GAX_PLAYER()[k]->type == t->childTypes[j]) {
                                h->children[j] = GAX_PLAYER()[k];
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    if (gGaxPlayerState->curChannelIdx == 1) {
        if (sfx == NULL)
            goto done;
        for (i = 0; i < (s32)numSfx; i++)
            GAX_PLAYER()[layout->count + i] = GAX_PLAYER0_MIXER()->children[GAX_PLAYER0_MIXER()->type->childCount + i];
    }
    if (sfx != NULL) {
        for (i = 0; i < (s32)numSfx; i++) {
            GAX_PLAYER()[layout->count + i]->children[0] = GAX_PLAYER()[1];
            GAX_MIXER()->children[GAX_MIXER()->type->childCount + i] = GAX_PLAYER()[layout->count + i];
        }
    }
done:
    for (i = 0; i < (s32)(layout->count - 3); i++)
        ((struct GaxChannelState *)GAX_PLAYER()[i + 3])->index = i;
    if (gGaxPlayerState->echoTaps != 0) {
        struct GaxHandlerType *t;

        i = 0;
        t = layout->types[0];
        for (; i <= 2; i++) {
            struct GaxDspRate *r;
            u32 base = gGaxPlayerState->echoTaps;

            r = (struct GaxDspRate *)(i * 8 + base);
            r->step = (i + t->data.dsp->taps)->rate * gGaxPlayerState->format->mixRate / 1000 * 2;
            /* taps[i + 1].value, addressed off taps[i] like the ROM */
            r->value = ((u32 *)t->data.dsp)[i * 2 + 2];
        }
    }
    return 1;
}
