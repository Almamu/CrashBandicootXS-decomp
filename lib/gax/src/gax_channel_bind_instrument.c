#include "gax_internal.h"
#include "match.h"

/* Binds a new instrument/entry to a per-channel voice object (`self`) from
 * `table->0x10[cmd]` and resets most of the voice's envelope/state fields
 * to their defaults, then (if the freshly-bound entry's own first byte is
 * non-zero, meaning it's some kind of "invalid"/placeholder entry) clears
 * the binding back out; finally, if a binding is still in place, records
 * `cmd` into the current song's `+0x34` per-slot table at index
 * `self->0x53`. Channel/voice/song object shapes aren't modeled yet
 * (same situation as the neighboring GAX2 engine internals in this
 * directory) - kept as raw offsets throughout.
 *
 * Was NAKED asm, not plain C, for two prior passes: the ROM's own
 * register choreography for `self` - reloaded fresh from `ip` (never
 * spilled to a callee-saved register) into a rotating cast of r0/r1/r3
 * exactly when each group of field writes needs it, with `r3` itself
 * mutated in place (`adds r3, #0x23`) once its prior value is no longer
 * needed - always needed one extra callee-saved register that the ROM's
 * version doesn't spend. Closed this pass using the same "self lives in
 * ip for the whole leaf-ish function" idiom already established for
 * `sub_80259D4` (entity_flags.c, see
 * docs/matching/archive/naked-sub_80259d4-matched.md): `self` is pinned
 * to a `MATCH_HOLD_REG(void *, selfIP, ip)` local, materialized from the
 * incoming `r0`
 * together with `cmd`'s own copy (`r4`) via one opaque `asm volatile`
 * instruction pair (this compiler always schedules a lone `n`-copy ahead
 * of the `self`-stash otherwise, regardless of C statement order), and
 * every "mov rX, ip" the ROM does to re-derive `self` for the next group
 * of field writes is reproduced as its own register-pinned local
 * (`s1`/`s3`/`s0`/`s3b`/... below, one per ROM `mov`). The one genuine
 * surprise: the final `str r2, [r3, #0x3c]` (clearing the binding back
 * out) reuses the register holding the already-materialized `zero16`
 * constant, but plain C (even referencing the same pinned local, `*(s32
 * *)(...) = zero16;`) let `-O2`'s constant propagation flatten it back
 * to a fresh literal load in a different scratch register - a single
 * opaque `asm volatile("str %1, [%0, #0x3c]" ...)` anchor, spelling out
 * the exact instruction with both already-pinned operands, was needed to
 * stop that. */
void GaxChannelSetInstrument(struct GaxChannelState *self, struct GaxInfoHandler *info, u32 cmd,
                             struct GaxSongData *table)
{
    MATCH_HOLD_REG(void *, selfIP, ip);
    MATCH_HOLD_REG(s32, n, r4);

    asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(selfIP), "=r"(n) : "r"(self), "r"(cmd));

    if (n != 0) {
        void **entryTable = *(void ***)((u8 *)table + 0x10);
        void *entry = entryTable[n];

        {
            MATCH_HOLD_REG(u8 *, s1, r1) = (u8 *)selfIP;
            *(void **)(s1 + 0x3c) = entry;
        }

        {
            MATCH_HOLD_REG(u8, zero8, r1) = 0;
            MATCH_HOLD_REG(u16, zero16, r2) = 0;
            MATCH_HOLD_REG(u8 *, s3, r3) = (u8 *)selfIP;
            *(u16 *)(s3 + 0x38) = zero16;

            {
                MATCH_HOLD_REG(u8 *, s0, r0) = (u8 *)selfIP + 0x22;
                *s0 = zero8;
            }

            *(u16 *)(s3 + 0x3a) = zero16;

            {
                void *entry2 = *(void **)(s3 + 0x3c);
                u8 v8 = *(u8 *)((u8 *)entry2 + 8);
                MATCH_HOLD_REG(u8 *, s3b, r3) = s3 + 0x23;
                *s3b = v8;

                {
                    MATCH_HOLD_REG(u8 *, s0b, r0) = (u8 *)selfIP;
                    *(u16 *)(s0b + 0x36) = zero16;
                    *(u8 *)(s0b + 0x1f) = zero8;
                    *(u8 *)(s0b + 0x20) = zero8;
                }

                {
                    MATCH_HOLD_REG(u8, ff, r0) = 0xff;
                    MATCH_HOLD_REG(u8 *, s1b, r1) = (u8 *)selfIP;
                    *(u8 *)(s1b + 0x15) = ff;

                    {
                        void *entry3 = *(void **)(s1b + 0x3c);
                        u8 v84 = *((u8 *)entry3 + 0x84);
                        MATCH_HOLD_REG(u8 *, s3c, r3) = (u8 *)selfIP;
                        *(u8 *)(s3c + 0x1e) = v84;
                        *(u16 *)(s3c + 0x32) = zero16;
                        *(u16 *)(s3c + 0x30) = zero16;

                        if (*(u8 *)entry3 != 0) {
                            asm volatile("str %1, [%0, #0x3c]" :: "r"(s3c), "r"(zero16));
                        }
                    }
                }
            }
        }

        {
            MATCH_HOLD_REG(u8 *, s1c, r1) = (u8 *)selfIP;
            void *bound = *(void **)(s1c + 0x3c);
            if (bound != 0) {
                void *songPtr = gGaxPlayerState->songPtr;
                u8 *slotTable = *(u8 **)((u8 *)songPtr + 0x34);
                if (slotTable != 0) {
                    MATCH_HOLD_REG(u8 *, s0d, r0) = (u8 *)selfIP + 0x53;
                    u8 idx = *s0d;
                    slotTable[idx * 4] = n;
                }
            }
        }
    }
}
