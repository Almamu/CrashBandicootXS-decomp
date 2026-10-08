#include "gax_internal.h"
#include "match.h"

/* Binds instrument `cmd` of the song's `instruments[]` table to a channel
 * and resets the channel's per-note state (envelope, vibrato, sequence,
 * portamento); an `empty` placeholder instrument leaves the channel
 * unbound. If an instrument is bound, `cmd` is also recorded in the song
 * header's `scratch` table at the channel's `index` (4 bytes per
 * channel).
 *
 * Was NAKED asm, not plain C, for two prior passes: the ROM's own
 * register choreography for `self` - reloaded fresh from `ip` (never
 * spilled to a callee-saved register) into a rotating cast of r0/r1/r3
 * exactly when each group of field writes needs it, with `r3` itself
 * mutated in place (`adds r3, #0x23`) once its prior value is no longer
 * needed - always needed one extra callee-saved register that the ROM's
 * version doesn't spend. Closed this pass using the same "self lives in
 * ip for the whole leaf-ish function" idiom already established for
 * `SetEntityIdActivated` (entity_flags.c, see
 * docs/matching/archive/naked-sub_80259d4-matched.md): `self` is pinned
 * to a `MATCH_HOLD_REG(struct GaxChannelState *, selfIP, ip)` local, materialized from the
 * incoming `r0`
 * together with `cmd`'s own copy (`r4`) via one opaque `asm volatile`
 * instruction pair (this compiler always schedules a lone `n`-copy ahead
 * of the `self`-stash otherwise, regardless of C statement order), and
 * every "mov rX, ip" the ROM does to re-derive `self` for the next group
 * of field writes is reproduced as its own local (`s1`/`s3`/`s0`/`s3b`/...
 * below, one per ROM `mov`; pinned where the allocator doesn't pick the
 * ROM's register by itself). The one genuine
 * surprise: the final `str r2, [r3, #0x3c]` (clearing the binding back
 * out) reuses the register holding the already-materialized `zero16`
 * constant, but plain C (even referencing the same pinned local,
 * `s3c->instrument = zero16`) let `-O2`'s constant propagation flatten it back
 * to a fresh literal load in a different scratch register - a single
 * opaque `asm volatile("str %1, [%0, #0x3c]" ...)` anchor, spelling out
 * the exact instruction with both already-pinned operands, was needed to
 * stop that. Most of the other stores of a pinned constant are retyped
 * (`*(u16 *)&s3->envPos = zero16`): as a plain struct-member store gcc
 * copies the value into a scratch register first (and turns `ff` into
 * `-1`). */
void GaxChannelSetInstrument(struct GaxChannelState *self, struct GaxInfoHandler *info, u32 cmd,
                             struct GaxSongData *table)
{
    MATCH_HOLD_REG(struct GaxChannelState *, selfIP, ip);
    s32 n;

    asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(selfIP), "=r"(n) : "r"(self), "r"(cmd));

    if (n != 0) {
        struct GaxChannelInstrument **entryTable = table->instruments;
        struct GaxChannelInstrument *entry = entryTable[n];

        {
            MATCH_HOLD_REG(struct GaxChannelState *, s1, r1) = selfIP;
            s1->instrument = entry;
        }

        {
            u8 zero8 = 0;
            MATCH_HOLD_REG(u16, zero16, r2) = 0;
            struct GaxChannelState *s3 = selfIP;
            *(u16 *)&s3->envPos = zero16;

            {
                u8 *s0 = &selfIP->released;
                *s0 = zero8;
            }

            *(u16 *)&s3->vibratoPhase = zero16;

            {
                struct GaxChannelInstrument *entry2 = s3->instrument;
                u8 v8 = entry2->vibratoDelay;
                MATCH_HOLD_REG(u8 *, s3b, r3) = &s3->vibratoDelay;
                *s3b = v8;

                {
                    struct GaxChannelState *s0b = selfIP;
                    *(u16 *)&s0b->seqPos = zero16;
                    *(u8 *)&s0b->cutTimer = zero8;
                    s0b->seqLoopCount = zero8;
                }

                {
                    u8 ff = 0xff;
                    MATCH_HOLD_REG(struct GaxChannelState *, s1b, r1) = selfIP;
                    *(u8 *)&s1b->vol15 = ff;

                    {
                        struct GaxChannelInstrument *entry3 = s1b->instrument;
                        u8 seqSpeed = entry3->seqSpeed;
                        struct GaxChannelState *s3c = selfIP;
                        s3c->cutDelay = seqSpeed;
                        *(u16 *)&s3c->slideRate = zero16;
                        *(u16 *)&s3c->slideTarget = zero16;

                        if (entry3->empty != 0) {
                            /* s3c->instrument = NULL, from zero16's register */
                            asm volatile("str %1, [%0, #0x3c]" : : "r"(s3c), "r"(zero16));
                        }
                    }
                }
            }
        }

        {
            MATCH_HOLD_REG(struct GaxChannelState *, s1c, r1) = selfIP;
            struct GaxChannelInstrument *bound = s1c->instrument;
            if (bound != 0) {
                struct GaxSongHeader *songPtr = gGaxPlayerState->songPtr;
                u8 *slotTable = songPtr->scratch;
                if (slotTable != 0) {
                    u8 *s0d = &selfIP->index;
                    u8 idx = *s0d;
                    slotTable[idx * 4] = n;
                }
            }
        }
    }
}
