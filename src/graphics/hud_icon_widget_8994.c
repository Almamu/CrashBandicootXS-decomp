#include "core.h"
#include "icon_manager.h"

/* NOT YET BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_8994.s) is used otherwise. */
#if NON_MATCHING

/* Widest single line of `text`: like sub_8028900 but walks a
 * NUL-terminated string and tracks a running per-line width, resetting
 * on each newline and keeping the running maximum; the final result is
 * whichever of the last line's width or the running maximum is larger
 * (covers un-terminated final lines).
 *
 * The ROM pins `self` into `ip`, `text` into `r4`, `&spaceWidth` into
 * `r8`, and `&glyphRecords` into `r6` (dereferenced fresh each
 * iteration, like sub_8028900's identical field), keeping
 * `curWidth`/`maxWidth` in `r3`/`r5` - reproduced with the same explicit
 * register pins and `goto`-based tail-merge established for
 * sub_8028808/sub_8028900 (`ip`/`r8`/`r4` are all safe to pin in this
 * toolchain, unlike `r7` - see docs/matching.md's "Why not just pin
 * r7"). `charLookup[c]` itself is computed directly off `self`+8+`c`
 * (not through a separately cached base, unlike sub_8028900), matching
 * the ROM's own `mov r1, ip; adds r1, #8; adds r1, r1, r0` shape. */
s32 MeasureText(struct icon_manager *selfArg, u8 *textArg)
{
    register struct icon_manager *self asm("ip") = selfArg;
    register u8 *text asm("r4") = textArg;
    u32 maxWidth = 0;
    register u32 curWidth asm("r3") = 0;
    u8 c = *text;

    if (c == 0) {
        goto end;
    }

    {
        register u32 *spaceWidthAddr asm("r8");
        register struct icon_glyph_metrics **glyphRecordsAddr asm("r6");

        asm volatile(
            "mov r1, #0x90\n\tlsl r1, r1, #1\n\tadd r1, %2\n\tmov %0, r1\n\t"
            "mov r6, #0x86\n\tlsl r6, r6, #1\n\tadd r6, %2\n\tadd %1, r6, #0"
            : "=r"(spaceWidthAddr), "=r"(glyphRecordsAddr)
            : "r"(self)
            : "r1", "r6"
        );

    loop:
        {
            s32 val;

            if (c == '\n') {
                goto newline;
            }
            if (c != ' ') {
                goto dispatch;
            }
            val = *spaceWidthAddr;
            goto accumulate;
        newline:
            if (curWidth > maxWidth) {
                maxWidth = curWidth;
            }
            curWidth = 0;
            goto tail;
        dispatch:
            {
                u8 glyphIndex = self->charLookup[c];
                struct icon_glyph_metrics *rec =
                    (struct icon_glyph_metrics *)((u8 *)*glyphRecordsAddr + glyphIndex * 12);
                val = rec->width;
            }
        accumulate:
            curWidth += val;
        }
    tail:
        text++;
        c = *text;
        if (c != 0) {
            goto loop;
        }
    }
end:
    return curWidth >= maxWidth ? curWidth : maxWidth;
}

#endif /* NON_MATCHING */
