#include "core.h"
#include "icon_manager.h"

/* sub_8028808 (at the end of this file) is matched, byte-exact.
 * sub_80285C4/InitHudIconWidgetA/InitHudIconWidgetB are NOT YET
 * BYTE-MATCHING - see docs/matching/issue-46-hud-icon-widget.md for the
 * full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_2_20_85c4.s) is used otherwise. */
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

#if NON_MATCHING

extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_8006AC8(void *arg0, void *arg1);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern u8 gStaticData_08174D84[];
extern u8 gStaticData_08174DD4[];
extern u8 gStaticData_087E4DAC[];
extern u8 gStaticData_087E4D64[];
extern u8 gStaticData_087E4D1C[];
extern u8 gStaticData_085A4E70[];
extern u8 gStaticData_085A551C[];
extern u8 gStaticData_08175188[];
extern u8 gStaticData_081751D4[];

/* Builds one glyph's OAM-scratch draw request (`self->oam_scratch`) from
 * `self->glyphRecords[glyphIndex]` and the current cursor position, hands
 * it to `sub_8006AC8` to actually draw, then advances `posX` by the
 * glyph's width. `charByte` is looked up through `charLookup` first -
 * callers pass a raw character byte, not a glyph index.
 *
 * Residual gap: `self` is kept in `ip`/`r12` here instead of the ROM's
 * `r3` - this function has too many simultaneously-live values (the
 * glyph index, three re-derived `rec` pointers, `self` itself across the
 * `sub_8006AC8` call) for this compiler to fit into r4-r7 the way the
 * ROM does; tried caching `&self->posX`/`&self->glyphRecords` into
 * explicit locals (matching the ROM's own address-caching shape) and
 * plain repeated field access, neither changed the register choice. */
void sub_80285C4(struct icon_manager *self, u8 charByte)
{
    u8 glyphIndex = self->charLookup[charByte];
    struct icon_glyph_metrics *rec;

    *(u16 *)((u8 *)self + 2) = (*(u16 *)((u8 *)self + 2) & 0xFE00) | (self->posX & 0x1FF);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->oam_scratch[0] = rec->field_8 + *((u8 *)&self->posY);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->oam_scratch[1] = (self->oam_scratch[1] & 0x3F) | (u8)(rec->field_4 << 6);

    *(u16 *)((u8 *)self + 4) = (*(u16 *)((u8 *)self + 4) & 0xFC00)
        | ((self->field_108 + glyphIndex * self->field_124) & 0x3FF);

    sub_8006AC8(gUnknown_03001300, self);

    rec = (struct icon_glyph_metrics *)((u8 *)self->glyphRecords + glyphIndex * 12);
    self->posX += rec->width;
}

/* Constructs a `struct icon_manager` for the "A" icon/text widget
 * family: zeroes the leading OAM-scratch pair of words, resets cursor
 * position and left margin, points `record` at `gStaticData_087E4D64`
 * (the first store to `gStaticData_087E4DAC` is a genuinely dead write
 * that's really in the ROM - see actor_aabb_setup.c's
 * sub_803AFF0/sub_803B024 for the identical documented pattern),
 * configures the line-height/space-width/glyph-stride fields, and
 * builds `charLookup` from the `gStaticData_08174D84` font-glyph-order
 * table (see include/icon_manager.h).
 *
 * The `record`/`posX`/`posY`/`field_118`/`field_12c` preamble's address
 * computations and store order now follow the ROM via inline-asm address
 * anchors (see the comment right above that block); the exact zero-value
 * register reuse across all four stores isn't fully nailed down yet
 * (splitting the anchors into separate asm blocks loses the single
 * shared `r1`=0 the ROM keeps live the whole time). The
 * `field_11c`/`spaceWidth`/`field_128`/`field_124`/`glyphRecords` run
 * right after also has its own chain-vs-fresh address-computation
 * mismatches (confirmed via a normalized instruction-by-instruction diff
 * against the raw ROM listing, not just eyeballing the disassembly -
 * this project's own cautionary tale about isolated-compile
 * "looks-matched" claims applies here too) - not chased further since
 * the loop below blocks a full match regardless.
 *
 * Residual gap: the charLookup-building loop. The ROM keeps the table's
 * leading count byte permanently in `r7` and a constant zero in `ip`
 * across the whole loop; this compiler's *unforced* allocator instead
 * puts count in `ip` and the loop index's precomputed `+1` in `r7` (the
 * two roles swapped) - `register T x asm("r7")` cannot be used to force
 * the ROM's exact assignment, since that's a confirmed silent
 * ABI-violation bug in this toolchain (see the loop's own comment
 * below). A few other small shape differences remain too (the initial
 * `movs r2,#0` ROM does first vs. last here; the ROM recomputing the
 * conditional store's address fresh instead of caching it). Tried: the
 * loop as one literal inline-asm block (matches ROM exactly
 * instruction-for-instruction, but then *nothing* keeps `self` alive
 * past it without also landing in a clobbered register, and forcing
 * `self` back to r4 via a pin reintroduces the same r7 bug for any value
 * the asm needs to keep alive past its own scope); several `for`/`while`
 * phrasings for the inner search loop (the `j` increment's start value
 * and shape change whether gcc peels the first iteration, independent of
 * the r7 issue). */
struct icon_manager *InitHudIconWidgetA(struct icon_manager *selfArg)
{
    register struct icon_manager *self asm("r4") = selfArg;
    s32 zero;
    struct icon_record **recordAddr;
    s32 i;
    u8 count;

    /* This compiler caches `self+0x130` (`record`'s address) across both
     * stores when the assignment is expressed as plain C (`self->record
     * = X; ... self->record = Y;` collapses to one address computation
     * reused for both), but the ROM recomputes it fresh each time -
     * forced via the same inline-asm address anchor already established
     * for the identical pattern in actor_aabb_setup.c's
     * sub_803AFF0/sub_803B024. */
    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    *recordAddr = (struct icon_record *)gStaticData_087E4DAC;

    /* The `posX`/`posY`/`field_118`/`field_12c` zero-init (address
     * computed ascending, stored descending) plus `zero`'s own store
     * (which reuses this same zeroed r1, not a fresh materialization) -
     * one literal instruction block matching the ROM exactly, the same
     * technique that reaches a full match in sub_8028A78
     * (hud_icon_widget_8a78.c), which shares this exact preamble. */
    asm volatile(
        "mov r1, #0x88\n\tlsl r1, r1, #1\n\tadd r2, %1, r1\n\t"
        "add r1, r1, #4\n\tadd r0, %1, r1\n\t"
        "mov r1, #0\n\tstr r1, [r0]\n\tstr r1, [r2]\n\t"
        "mov r2, #0x8c\n\tlsl r2, r2, #1\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, r2, #0x14\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, %0"
        : "=m"(zero)
        : "r"(self)
        : "r0", "r1", "r2", "memory"
    );
    sub_803A94C(&zero, self, 0x05000002);

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    *recordAddr = (struct icon_record *)gStaticData_087E4D64;
    self->field_11c = 9;
    self->spaceWidth = 4;
    self->field_128 = gStaticData_085A4E70;
    self->field_124 = 2;
    self->glyphRecords = (struct icon_glyph_metrics *)gStaticData_08174DD4;

    /* The charLookup-building loop: `self+8`/the font table are pinned
     * to r5/r6 (safe - see below), but `count` (the table's leading
     * byte) and the loop index's precomputed `+1` are left as plain,
     * completely unpinned locals so gcc's own allocator picks their
     * registers naturally. `register T x asm("r7")` (or an inline-asm
     * `r7` clobber/output) must never be used here even if it looks like
     * it would match better - it's a confirmed silent ABI-violation bug
     * in this agbcc/gcc-2.9-arm toolchain (r7 assigned via an explicit
     * pin compiles with no push/pop of r7 at all, clobbering the
     * caller's r7 - see docs/matching.md's "Why not just pin r7" and
     * `matching_decomp_register_pinning` memory point 10). Values can
     * only safely live in r7 across calls here via natural, unforced
     * allocation. */
    {
        register u8 *lookupBase asm("r5") = self->charLookup;
        register u8 *table asm("r6") = gStaticData_08174D84;

        count = table[0];
        for (i = 0; i <= 0xFF; i++) {
            u8 j = 0;

            lookupBase[i] = 0;
            if (count == i) {
                continue;
            }
            for (;;) {
                j++;
                if (j > 0x4F) {
                    break;
                }
                if (table[j] == i) {
                    lookupBase[i] = j;
                    break;
                }
            }
        }
    }
    return self;
}

/* Same shape as InitHudIconWidgetA above, "B" icon/text widget family -
 * different data tables and line-height/glyph-stride constants. Same
 * preamble fix and same residual charLookup-loop gap as
 * InitHudIconWidgetA - see that function's comments for the full
 * account. */
struct icon_manager *InitHudIconWidgetB(struct icon_manager *selfArg)
{
    register struct icon_manager *self asm("r4") = selfArg;
    s32 zero;
    struct icon_record **recordAddr;
    s32 i;
    u8 count;

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    *recordAddr = (struct icon_record *)gStaticData_087E4DAC;

    /* Same merged posX/posY/field_118/field_12c/zero anchor as
     * InitHudIconWidgetA above - see that function's own comment. */
    asm volatile(
        "mov r1, #0x88\n\tlsl r1, r1, #1\n\tadd r2, %1, r1\n\t"
        "add r1, r1, #4\n\tadd r0, %1, r1\n\t"
        "mov r1, #0\n\tstr r1, [r0]\n\tstr r1, [r2]\n\t"
        "mov r2, #0x8c\n\tlsl r2, r2, #1\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, r2, #0x14\n\tadd r0, %1, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, %0"
        : "=m"(zero)
        : "r"(self)
        : "r0", "r1", "r2", "memory"
    );
    sub_803A94C(&zero, self, 0x05000002);

    asm volatile("mov r0, #0x98\n\tlsl r0, r0, #1\n\tadd %0, %1, r0" : "=r"(recordAddr) : "r"(self) : "r0");
    *recordAddr = (struct icon_record *)gStaticData_087E4D1C;

    /* field_124's address is chained from field_11c's (+8) in the ROM
     * here (unlike InitHudIconWidgetA, where it's a fresh computation) -
     * forced the same way as the other address anchors above. */
    {
        s32 *field11cAddr;
        s32 *field124Addr;

        asm volatile("mov r2, #0x8e\n\tlsl r2, r2, #1\n\tadd %0, %1, r2" : "=r"(field11cAddr) : "r"(self) : "r2");
        *field11cAddr = 0x10;
        self->spaceWidth = 6;
        asm volatile("add %0, %1, #8" : "=r"(field124Addr) : "0"(field11cAddr));
        *field124Addr = 4;
    }
    *((u8 *)self + 3) = (*((u8 *)self + 3) & 0x3F) | 0x40;
    self->field_128 = gStaticData_081751D4;
    self->glyphRecords = (struct icon_glyph_metrics *)gStaticData_085A551C;

    {
        register u8 *lookupBase asm("r5") = self->charLookup;
        register u8 *table asm("r6") = gStaticData_08175188;

        count = table[0];
        for (i = 0; i <= 0xFF; i++) {
            u8 j = 0;

            lookupBase[i] = 0;
            if (count == i) {
                continue;
            }
            for (;;) {
                j++;
                if (j > 0x4B) {
                    break;
                }
                if (table[j] == i) {
                    lookupBase[i] = j;
                    break;
                }
            }
        }
    }
    return self;
}

#endif /* NON_MATCHING */

/* sub_8028808 is matched, byte-exact.
 *
 * Per-character dispatcher used while drawing/measuring one glyph at a
 * time: newline resets `posX` to the left margin and advances `posY` by
 * one line height; space just advances `posX` by `spaceWidth`; anything
 * else is forwarded to `record`'s slot-4 trampoline (the glyph-draw
 * callee, `sub_80285C4` per the widget's own vtable) via `sub_803AD80`.
 *
 * The 3-way `if`/`else if`/`else` is written as explicit `goto`s so the
 * *middle* arm (the space case) ends up inline and the other two become
 * jumped-to blocks in test order, with the shared two-instruction tail
 * ("dest += self->offset") reached via a `destAddr`/`offset` pair - this
 * reproduces the ROM's exact block layout.
 *
 * The `self`/`charByte` prologue needed a literal inline-asm block: this
 * compiler always widens a `u8` parameter (`charByte`) to its
 * zero-extended byte value before doing anything else, regardless of
 * where that's first used in the C source, while a pointer parameter
 * (`self`) is only materialized into its own register lazily, at first
 * use - so plain C, however reordered or register-pinned, only ever
 * produced the widen-then-copy order, never the ROM's copy-then-widen.
 * Taking `charByte` as a raw `u32` (avoiding the implicit byte-promotion
 * invariant entirely) and spelling out all three prologue instructions
 * as one asm block - self-copy first, then the in-place `lsl`/`lsr`
 * widen matching the ROM's own register reuse (`lsl r1,r1,#0x18` in
 * place, not into a fresh register) - fixed it. */
void sub_8028808(struct icon_manager *self, u32 charByte)
{
    register u32 raw asm("r1") = charByte;
    register struct icon_manager *s asm("r3");
    register u32 c asm("r4");
    register u32 *destAddr asm("r2");
    register u32 offset asm("r0");

    asm volatile(
        "add %0, %3, #0\n\t"
        "lsl %2, %2, #0x18\n\t"
        "lsr %1, %2, #0x18"
        : "=r"(s), "=r"(c), "+r"(raw)
        : "r"(self)
    );
    if (c == '\n')
        goto newline;
    if (c != ' ')
        goto dispatch;
    destAddr = &s->posX;
    offset = (u8 *)&s->spaceWidth - (u8 *)s;
    goto tail;
newline:
    /* Chained address anchor: the ROM computes `&field_118` as
     * `&posX + 8` (sharing the `0x88<<1` offset register), not as two
     * independent field-offset computations. */
    {
        register u32 *posXAddr asm("r1");
        register u32 *field118Addr asm("r0");

        asm volatile(
            "mov r2, #0x88\n\tlsl r2, r2, #1\n\tadd %0, %2, r2\n\t"
            "add r2, r2, #8\n\tadd %1, %2, r2"
            : "=r"(posXAddr), "=r"(field118Addr)
            : "r"(s)
            : "r2"
        );
        *posXAddr = *field118Addr;
    }
    asm volatile(
        "mov r0, #0x8a\n\tlsl r0, r0, #1\n\tadd %0, %2, r0\n\t"
        "add r0, r0, #8"
        : "=r"(destAddr), "=r"(offset)
        : "r"(s)
    );
tail:
    {
        register u32 *fieldAddr asm("r1") = (u32 *)((u8 *)s + offset);
        *destAddr += *fieldAddr;
    }
    return;
dispatch:
    {
        struct icon_slot *slot = &s->record->slots[4];
        sub_803AD80((u8 *)s + slot->offset, c, slot->ptr);
    }
}
