#include "core.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"

/* Counts how many of category `categoryIdx`'s sub-effect-table entries
 * (see `struct sub_effect_record`/`category_descriptor.spawnTable`
 * in actor_anim.h) match one of two fixed sets of `kind` byte
 * values - a different set depending on the category's own `type`
 * field. Record 0 (the table's own header, `link` holding the real
 * entry count) doubles as entry 0 for this scan too. */
s32 CountCategoryCrates(s32 categoryIdx)
{
    struct sub_effect_record *table;
    s32 total;
    s32 count;
    s32 i;

    count = 0;
    table = gActorCategories[categoryIdx].spawnTable;
    if (gActorCategories[categoryIdx].type == CATEGORY_TYPE_POLAR) {
        i = 0;
        total = table[0].link;
        /* Manual pre-rotated `if (count<total) do {...} while (++i<total)`
         * instead of a plain `for` - gcc 2.9's own loop rotation of a
         * `for` here strength-reduces the ascending index into a
         * countdown (`sub r?,#1; cmp r?,#0`), losing the ROM's separate
         * up-counting index register and its extra callee-saved `r5`
         * (`adds r5,#1; cmp r5,r2; blt`) - see docs/workflow.md step 3. */
        if (count < total) {
            do {
                u8 v = table[i].kind;
                if (v == 1 || v == 3 || v == 4 || v == 8 || v == 9 || v == 0xa || v == 0x1c ||
                    v == 0x1d || v == 0x1e || v == 0x1f || v == 0x23) {
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
        s32 total2 = table[0].link;

        if (count < total2) {
            do {
                u8 v = table->kind;

                if ((u8)(v - 0x13) <= 4) {
                    count++;
                } else {
                    /* Reads the field again: cse turns the second read
                     * into the ROM's `adds r0, r1, #0` copy of `v`. */
                    if (table->kind == 0x1b || table->kind == 0x1e) {
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


void AddActorMissedNitro(void)
{
    gActorMissedNitros++;
}

s32 GetActorMissedNitros(void)
{
    return gActorMissedNitros;
}

s32 GetActorCheckpoint(void)
{
    return gActorCheckpoint;
}
