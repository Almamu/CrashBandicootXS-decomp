#include "core.h"
#include "icon_manager.h"

/* sub_8028890 and sub_8028900 (below) are both matched, byte-exact -
 * see docs/matching/issue-46-hud-icon-widget.md's "NAKED-transcription
 * pass" section for sub_8028900's history (a NAKED asm transcription,
 * not plain C - blocked by this toolchain's confirmed r7-pinning bug,
 * see docs/matching.md's "Why not just pin r7"). */

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

/* Sums the advance width of `count` characters starting at `str`
 * (`MeasureText`'s fixed-count sibling): newline contributes nothing,
 * space contributes `spaceWidth`, everything else contributes
 * `glyphRecords[charLookup[c]].width`.
 *
 * The ROM pins `str` into `r8` and `&glyphRecords` (not the pointer
 * itself - the field's address, reloaded fresh each iteration) into
 * `sb`/r9, caches `&spaceWidth` into `ip` and `&charLookup` into `r6`,
 * and keeps `total`/`i`/`count` in `r3`/`r4`/`r5`, with a genuine `r7`
 * scratch inside the `else` arm (the glyph-index byte).
 *
 * Transcribed as NAKED asm (not plain C): pinning either `&spaceWidth`
 * or `&charLookup` to their ROM registers (`ip`/`r6`) makes `r7` drop
 * out of the push/pop list entirely - the confirmed toolchain bug, just
 * triggered indirectly (enough simultaneous hard-register pins starve
 * whatever's left for r7, even when r7 itself was never pinned - see
 * docs/matching.md's "Why not just pin r7"). Every instruction below is
 * checked byte-identical to the ROM. */
NAKED s32 sub_8028900(struct icon_manager *self, u8 *str, s32 count)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "mov r8, r1\n\t"
        "add r5, r2, #0\n\t"
        "mov r3, #0\n\t"
        "mov r4, #0\n\t"
        "cmp r3, r5\n\t"
        "bge 5f\n\t"
        "mov r1, #0x90\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r0\n\t"
        "mov ip, r1\n\t"
        "mov r7, #0x86\n\t"
        "lsl r7, r7, #1\n\t"
        "add r7, r7, r0\n\t"
        "mov sb, r7\n\t"
        "add r6, r0, #0\n\t"
        "add r6, #8\n\t"
    "1:\n\t"
        "mov r1, r8\n\t"
        "add r0, r1, r4\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0xa\n\t"
        "beq 4f\n\t"
        "cmp r0, #0x20\n\t"
        "bne 2f\n\t"
        "mov r7, ip\n\t"
        "ldr r0, [r7]\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "add r1, r6, r0\n\t"
        "mov r0, sb\n\t"
        "ldr r2, [r0]\n\t"
        "ldrb r7, [r1]\n\t"
        "lsl r0, r7, #1\n\t"
        "add r1, r7, #0\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "ldr r0, [r0]\n\t"
    "3:\n\t"
        "add r3, r3, r0\n\t"
    "4:\n\t"
        "add r4, #1\n\t"
        "cmp r4, r5\n\t"
        "blt 1b\n\t"
    "5:\n\t"
        "add r0, r3, #0\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    );
}
