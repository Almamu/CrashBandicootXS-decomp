#include "core.h"
#include "actor_anim.h"

/* Counts how many of category `categoryIdx`'s sub-effect-table entries
 * (see `struct sub_effect_record`/`category_descriptor.sub_effect_table`
 * in actor_anim.h) match one of two fixed sets of `variantA` byte
 * values - a different set depending on the category's own `type`
 * field. Record 0 (the table's own header, `field_04` holding the real
 * entry count) doubles as entry 0 for this scan too. */
s32 sub_802968C(s32 categoryIdx)
{
    struct sub_effect_record *table;
    s32 total;
    register s32 count asm("r4");
    s32 i;

    count = 0;
    table = gStaticData_08175558[categoryIdx].sub_effect_table;
    if (gStaticData_08175558[categoryIdx].type == 0) {
        i = 0;
        total = table[0].field_04;
        /* Manual pre-rotated `if (count<total) do {...} while (++i<total)`
         * instead of a plain `for` - gcc 2.9's own loop rotation of a
         * `for` here strength-reduces the ascending index into a
         * countdown (`sub r?,#1; cmp r?,#0`), losing the ROM's separate
         * up-counting index register and its extra callee-saved `r5`
         * (`adds r5,#1; cmp r5,r2; blt`) - see docs/workflow.md step 3. */
        if (count < total) {
            do {
                u8 v = table[i].variantA;
                if (v == 1 || v == 3 || v == 4 || v == 8 || v == 9 || v == 0xa ||
                    v == 0x1c || v == 0x1d || v == 0x1e || v == 0x1f || v == 0x23) {
                    count++;
                }
                i++;
            } while (i < total);
        }
    } else {
        /* This branch's own loop is a plain decrementing walk over
         * `table` (pointer-increment, reusing `total2` itself as the
         * down-counter, no separate index register) - a different
         * shape from the type==0 branch above, matching the ROM's own
         * `subs r2,#1`/`adds r3,#0x14` pattern exactly. The entry guard
         * reuses `count` (always 0 here, since this branch is only
         * reached before the type==0 branch's loop ever runs) rather
         * than testing `total2 > 0` directly - the ROM's own
         * `cmp r4,r2` compares the (zero) accumulator against the
         * count, not the count against a literal 0 (see
         * docs/workflow.md step 3). */
        s32 total2 = table[0].field_04;

        if (count < total2) {
            do {
                register u8 v asm("r1") = table->variantA;

                if ((u8)(v - 0x13) <= 4) {
                    count++;
                } else {
                    /* This compiler eliminates a plain `u8 v2 = v;`
                     * copy here entirely (comparing `v` directly), but
                     * the ROM keeps a real `adds r0, r1, #0` copy - an
                     * inline-asm island forces the redundant move back
                     * in, matching the ROM's exact register (`r0`)
                     * and instruction (see docs/workflow.md step 3). */
                    register s32 v2 asm("r0");
                    asm("add %0, %1, #0" : "=r"(v2) : "r"((s32)v));
                    if (v2 == 0x1b || v2 == 0x1e) {
                        count++;
                    }
                }
                table++;
                total2--;
            } while (total2 != 0);
        }
    }

    return count;
}
