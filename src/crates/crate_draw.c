#include "core.h"
#include "match.h"
#include "crate.h"
#include "crates.h"
#include "objects.h"
#include "globals.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/archive/issue-13-graphics-fc70.md). This function sits
 * between the still-raw `CreateCrate` (real bytes in
 * asm/code_3_2_17_e560_ff0c.s) and `UpdateCrate` (real bytes in the
 * new asm/code_3_2_17_e560_104e4.s), so it needs its own file rather
 * than joining an existing one - see docs/workflow.md's "one file per
 * contiguous ROM region" rule. `self` is the crate (`struct crate`,
 * crate.h). See
 * docs/matching/archive/issue-13-fc70-second-continuation.md for the
 * register-pinning technique this needed. */

/* Unless `self` is busy (`state` bit 7) or its state (low 7 bits) is
 * nonzero, clears `animDone` and clamps `frame` to the current tag's
 * frame count (the same table lookup as `BreakCrateTouchedByPlayer`,
 * crate_hit.c). Always calls `DrawSprite(gSpriteRenderer, self)`, then -
 * only if `animDone` is set - clears `flags` bit 3. */
void DrawCrate(struct crate *selfArg)
{
    /* Pinned to r4: the ROM keeps `self` in r4 for the whole function
     * (matching every sibling in this file family). */
    MATCH_HOLD_REG(struct crate *, self, r4) = selfArg;
    u8 state = self->state;

    if ((state & 0x80) == 0) {
        u8 masked7f = state & 0x7f;

        if (masked7f == 0) {
            /* Reuses the already-zero `masked7f` register for this
             * store rather than a fresh `0` immediate, matching the
             * ROM's own register reuse (`strb r1,[r0]` right after
             * computing `r1 = state & 0x7f`). */
            self->animDone = masked7f;

            {
                MATCH_HOLD_REG(s32, idx, r3) = 0;
                {
                    MATCH_HOLD_REG(struct anim_table *, p, r0) = self->anim;
                    MATCH_HOLD_REG(u8 *, tagAddr, r2) = &self->tag;
                    {
                        MATCH_HOLD_REG(struct anim_rec *, table, r1) = p->records;
                        MATCH_HOLD_REG(u8, tag, r5) = *tagAddr;
                        /* Register-pinned r0: the ROM computes this
                         * address as `offset(r0) + table(r1)`, not
                         * `table + offset` - writing the addition with
                         * the offset as the left operand is what makes
                         * this compiler pick the same destination
                         * register (r0, the offset's own register)
                         * instead of reusing `table`'s (r1). */
                        MATCH_HOLD_REG(struct anim_rec *, record, r0) =
                            (struct anim_rec *)(tag * sizeof(*table) + (s32)table);
                        u8 limit = record->frames;

                        if (idx >= limit) {
                            idx = limit - 1;
                        }
                    }
                }
                self->frame = idx;
            }
        }
    }

    DrawSprite(gSpriteRenderer, self);

    if (self->animDone != 0) {
        /* Anchored: the ROM computes the `~8` clear-mask at runtime
         * (`movs r0,#9; rsbs r0,r0,#0`, the negative-constant
         * register-pinned mask idiom - see
         * matching_decomp_register_pinning and CollideCrateWithPlayer's own use
         * of it, crate_stack.c) rather than folding it into an 8-bit
         * AND immediate, which a plain `self[0xc] &= -9;` always did
         * instead. `self` is passed as an input purely so this
         * compiler knows the asm still depends on it - without that,
         * it reused r4 in place for the `self[0x38] != 0` check just
         * above, corrupting the address this block reads/writes. */
        // clang-format off
        asm volatile(
            "mov r0, #0x9\n\t"
            "neg r0, r0\n\t"
            "ldrb r1, [r4, #0xc]\n\t"
            "and r0, r0, r1\n\t"
            "strb r0, [r4, #0xc]\n\t"
            :
            : "r"(self)
            : "r0", "r1", "cc", "memory"
        );
        // clang-format on
    }
}
