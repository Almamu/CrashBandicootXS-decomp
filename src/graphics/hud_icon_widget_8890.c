#include "core.h"
#include "icon_manager.h"

/* sub_8028890 is matched, byte-exact. sub_8028900 is NOT YET
 * BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md for the
 * full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8890.s) is used otherwise. */

extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

/* Same per-character logic as sub_8028808 (asm/code_3_2_20_85c4.s),
 * inlined into a loop over a NUL-terminated string instead of
 * dispatching through the trampoline for each character - this widget
 * family's "draw this whole string" entry point. Unlike sub_8028808,
 * `&posX`/`&posY` are cached once outside the loop (`r6`/`r7` in the
 * ROM) rather than recomputed per character, and there's no shared
 * "dest += offset" tail - each arm does its own complete
 * address/load/add/store.
 *
 * Needed explicit register pins (`self`=r4, `str`=r5, the cached
 * `&posX`/`&posY`=r6/r7) and a `goto`-based rewrite of the
 * if/else-if/else to reproduce the ROM's exact block order (newline
 * tested first with a forward `beq`, space inline as the fallthrough,
 * dispatch last) - the same techniques already established for
 * sub_8028808's identical-shaped dispatcher. */
void sub_8028890(struct icon_manager *selfArg, u8 *strArg)
{
    register struct icon_manager *self asm("r4") = selfArg;
    register u8 *str asm("r5") = strArg;
    u8 c = *str;

    if (c == 0) {
        return;
    }

    {
        register u32 *posXAddr asm("r6") = &self->posX;
        /* NOT pinned to r7 - see docs/matching.md's "Why not just pin
         * r7" (an explicit r7 pin silently drops it from push/pop,
         * corrupting the caller's r7). gcc's own unforced allocator
         * picks r7 for this anyway, since it's the only register left. */
        u32 *posYAddr = &self->posY;

    loop:
        if (c == '\n') {
            goto newline;
        }
        if (c != ' ') {
            goto dispatch;
        }
        *posXAddr += self->spaceWidth;
        goto tail;
    newline:
        *posXAddr = self->field_118;
        *posYAddr += self->field_11c;
        goto tail;
    dispatch:
        {
            struct icon_slot *slot = &self->record->slots[4];
            sub_803AD80((u8 *)self + slot->offset, c, slot->ptr);
        }
    tail:
        str++;
        c = *str;
        if (c != 0) {
            goto loop;
        }
    }
}
/* Ends 2 bytes short of a 4-byte boundary; the ROM zero-pads the gap,
 * this compiler's own trailing alignment fill doesn't - see
 * docs/matching.md's alignment-padding gotcha (also documented in
 * docs/matching/issue-46-hud-icon-widget.md's "Real gotchas" section). */
asm(".align 2, 0");

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8890.s) is used otherwise. */
#if NON_MATCHING

/* Sums the advance width of `count` characters starting at `str`
 * (`MeasureText`'s fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`.
 *
 * The ROM pins `str` into `r8` and `&glyphRecords` (not the pointer
 * itself - the field's address, reloaded fresh each iteration) into
 * `sb`/r9, caches `&spaceWidth` into `ip` and `&charLookup` into `r6`,
 * and keeps `total`/`i`/`count` in `r3`/`r4`/`r5`, with a genuine `r7`
 * scratch inside the `else` arm (the glyph-index byte). `r8`/`sb` pin
 * safely here (see docs/matching.md's "Why not just pin r7" for why
 * `r7` itself never can), and `total`/`i` pin safely too, but pinning
 * *either* `&spaceWidth` or `&charLookup` to their ROM registers
 * (`ip`/`r6`) makes `r7` drop out of the push/pop list entirely - the
 * same categorical bug, just triggered indirectly (enough simultaneous
 * hard-register pins apparently starve whatever's left for r7, even
 * when r7 itself was never pinned). Left `&spaceWidth`/`&charLookup`
 * unpinned so gcc's own unforced allocator picks *some* pair of
 * registers for them (r6/r7 here, not ip/r6 like the ROM) - correctly
 * saved, but not the ROM's exact register numbers. The newline/space/
 * else arms share a single `total += val` accumulate point reached via
 * `goto`, matching the ROM's own tail-merge (the same technique used
 * for sub_8028808's identical-shaped dispatcher), and the two address
 * computations are forced into independent fresh `mov`/`lsl` pairs
 * (`sub_803A94C`-style anchor) rather than the constant-delta
 * subtraction gcc otherwise derives one from the other with. */
s32 sub_8028900(struct icon_manager *self, u8 *strArg, s32 count)
{
    register u8 *str asm("r8") = strArg;
    register s32 total asm("r3") = 0;
    register s32 i asm("r4") = 0;

    if (i < count) {
        u32 *spaceWidthAddr;
        register struct icon_glyph_metrics **glyphRecordsAddr asm("sb");
        u8 *charLookupBase;

        /* Forces independent fresh computations for `&spaceWidth` and
         * `&glyphRecords` - left as plain C, this compiler notices the
         * two field offsets are a constant `0x14` apart and derives one
         * from the other via subtraction instead of two fresh `mov`/
         * `lsl` pairs like the ROM. */
        asm volatile(
            "mov r1, #0x90\n\tlsl r1, r1, #1\n\tadd %0, r1, %3\n\t"
            "mov r7, #0x86\n\tlsl r7, r7, #1\n\tadd r7, r7, %3\n\tmov %1, r7\n\t"
            "add %2, %3, #0\n\tadd %2, %2, #8"
            : "=r"(spaceWidthAddr), "=r"(glyphRecordsAddr), "=r"(charLookupBase)
            : "r"(self)
            : "r1", "r7"
        );

    loop:
        {
            u8 c = str[i];
            s32 val;

            if (c == '\n') {
                goto skip;
            }
            if (c == ' ') {
                val = *spaceWidthAddr;
                goto accumulate;
            }
            {
                u8 glyphIndex = charLookupBase[c];
                struct icon_glyph_metrics *rec =
                    (struct icon_glyph_metrics *)((u8 *)*glyphRecordsAddr + glyphIndex * 12);
                val = rec->width;
            }
        accumulate:
            total += val;
        }
    skip:
        i++;
        if (i < count) {
            goto loop;
        }
    }
    return total;
}

#endif /* NON_MATCHING */
