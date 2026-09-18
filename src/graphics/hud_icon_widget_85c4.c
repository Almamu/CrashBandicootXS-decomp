#include "core.h"
#include "icon_manager.h"

/* sub_8028808 (at the end of this file) is matched, byte-exact.
 * sub_80285C4/InitHudIconWidgetA/InitHudIconWidgetB below are also
 * matched now, but as NAKED asm transcriptions rather than plain C -
 * see docs/matching/issue-46-hud-icon-widget.md's "NAKED-transcription
 * pass" section for why: all three are blocked by this toolchain's
 * confirmed r7-pinning bug (an explicit `register T x asm("r7")`, or
 * even an indirect starve of the unforced allocator's own r7 choice,
 * compiles with no push/pop of r7 at all - see docs/matching.md's "Why
 * not just pin r7"), which a NAKED function sidesteps entirely since
 * nothing asks gcc's allocator to decide anything. */
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);

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
 * Field map (see include/icon_manager.h): +8+charByte is
 * `charLookup[charByte]` (glyphIndex); +0x110/+0x114 are `posX`/`posY`
 * (only `posX`'s low 9 bits and `posY`'s low byte are read here);
 * +0x10c is `glyphRecords` (reloaded fresh around the `sub_8006AC8`
 * call, matching the ROM); +0x108/+0x124 are `field_108`/`field_124`.
 *
 * Transcribed as NAKED asm (not plain C): `self` sits in `r3` here, with
 * too many other simultaneously-live values (the glyph index, three
 * re-derived `rec` pointers, `self` itself across the `sub_8006AC8`
 * call) for this compiler's allocator to fit into r4-r7 the way the ROM
 * does - tried caching `&self->posX`/`&self->glyphRecords` into explicit
 * locals and plain repeated field access, neither changed the register
 * choice. Every instruction below is checked byte-identical to the
 * ROM. */
NAKED void sub_80285C4(struct icon_manager *self, u8 charByte)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r3, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r1, r1, #0x18\n\t"
        "add r0, #8\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r2, [r0]\n\t"
        "mov r0, #0x88\n\t"
        "lsl r0, r0, #1\n\t"
        "add r6, r3, r0\n\t"
        "ldr r1, [r6]\n\t"
        "ldr r4, 1f\n\t"
        "add r0, r4, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, 2f\n\t"
        "ldrh r7, [r3, #2]\n\t"
        "and r0, r7\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r3, #2]\n\t"
        "mov r0, #0x8a\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r3, r0\n\t"
        "sub r4, #0xf3\n\t"
        "add r5, r3, r4\n\t"
        "ldr r0, [r5]\n\t"
        "lsl r4, r2, #1\n\t"
        "add r4, r4, r2\n\t"
        "lsl r4, r4, #2\n\t"
        "add r0, r4, r0\n\t"
        "ldrb r0, [r0, #8]\n\t"
        "ldrb r1, [r1]\n\t"
        "add r0, r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "ldr r0, [r5]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0, #4]\n\t"
        "lsl r1, r1, #6\n\t"
        "mov r0, #0x3f\n\t"
        "ldrb r7, [r3, #1]\n\t"
        "and r0, r7\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3, #1]\n\t"
        "mov r1, #0x84\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r3, r1\n\t"
        "ldr r1, [r0]\n\t"
        "mov r7, #0x92\n\t"
        "lsl r7, r7, #1\n\t"
        "add r0, r3, r7\n\t"
        "ldr r0, [r0]\n\t"
        "mul r0, r2, r0\n\t"
        "add r1, r1, r0\n\t"
        "ldr r2, 3f\n\t"
        "add r0, r2, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, 4f\n\t"
        "ldrh r7, [r3, #4]\n\t"
        "and r0, r7\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r3, #4]\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r3, #0\n\t"
        "bl sub_8006AC8\n\t"
        "ldr r0, [r5]\n\t"
        "add r4, r4, r0\n\t"
        "ldr r0, [r6]\n\t"
        "ldr r1, [r4]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r6]\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte 0x000001FF\n"
    "2: .4byte 0xFFFFFE00\n"
    "3: .4byte 0x000003FF\n"
    "4: .4byte 0xFFFFFC00\n"
    "5: .4byte gUnknown_03001300\n"
    );
}

/* Constructs a `struct icon_manager` for the "A" icon/text widget
 * family: zeroes the leading OAM-scratch pair of words, resets cursor
 * position and left margin, points `record` at `gStaticData_087E4DAC`
 * (the first store to `gStaticData_087E4DAC` is a genuinely dead write
 * that's really in the ROM - see actor_aabb_setup.c's
 * sub_803AFF0/sub_803B024 for the identical documented pattern),
 * configures the line-height/space-width/glyph-stride fields, and
 * builds `charLookup` from the `gStaticData_08174D84` font-glyph-order
 * table (see include/icon_manager.h).
 *
 * Transcribed as NAKED asm (not plain C): the charLookup-building loop
 * keeps the font table's leading count byte permanently in `r7` and a
 * constant zero in `ip` across the whole loop - `register T x
 * asm("r7")` cannot be used to force this, since that's the confirmed
 * silent ABI-violation bug in this toolchain (no push/pop of r7 at all
 * - see docs/matching.md's "Why not just pin r7"). The preceding
 * `record`/`posX`/`posY`/`field_118`/`field_12c` preamble also has its
 * own address-computation-order-vs-store-order mismatches that plain C
 * never reproduced simultaneously (tried: plain struct-field
 * assignment both orderings, raw `(u8 *)self + N` casts both orderings,
 * a shared base-pointer local - see the second-pass writeup for the
 * full account). Every instruction below is checked byte-identical to
 * the ROM. */
NAKED struct icon_manager *InitHudIconWidgetA(struct icon_manager *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "ldr r0, 5f\n\t"
        "str r0, [r1]\n\t"
        "mov r1, #0x88\n\t"
        "lsl r1, r1, #1\n\t"
        "add r2, r4, r1\n\t"
        "add r1, #4\n\t"
        "add r0, r4, r1\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "str r1, [r2]\n\t"
        "mov r2, #0x8c\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r4, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, #0x14\n\t"
        "add r0, r4, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "ldr r2, 6f\n\t"
        "bl sub_803A94C\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "ldr r0, 7f\n\t"
        "str r0, [r1]\n\t"
        "mov r2, #0x8e\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r4, r2\n\t"
        "mov r0, #9\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0x90\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "mov r0, #4\n\t"
        "str r0, [r1]\n\t"
        "add r2, #0xc\n\t"
        "add r1, r4, r2\n\t"
        "ldr r0, 8f\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0x92\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "mov r0, #2\n\t"
        "str r0, [r1]\n\t"
        "sub r2, #0x1c\n\t"
        "add r1, r4, r2\n\t"
        "ldr r0, 9f\n\t"
        "str r0, [r1]\n\t"
        "mov r2, #0\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #8\n\t"
        "ldr r6, 10f\n\t"
        "ldrb r7, [r6]\n\t"
        "mov ip, r2\n\t"
    "1:\n\t"
        "add r0, r5, r2\n\t"
        "mov r1, ip\n\t"
        "strb r1, [r0]\n\t"
        "mov r1, #0\n\t"
        "add r3, r2, #1\n\t"
        "cmp r7, r2\n\t"
        "beq 3f\n\t"
    "2:\n\t"
        "add r1, #1\n\t"
        "cmp r1, #0x4f\n\t"
        "bhi 4f\n\t"
        "add r0, r1, r6\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, r2\n\t"
        "bne 2b\n\t"
        "add r0, r5, r2\n\t"
    "3:\n\t"
        "strb r1, [r0]\n\t"
    "4:\n\t"
        "add r2, r3, #0\n\t"
        "cmp r2, #0xff\n\t"
        "bls 1b\n\t"
        "add r0, r4, #0\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "5: .4byte gStaticData_087E4DAC\n"
    "6: .4byte 0x05000002\n"
    "7: .4byte gStaticData_087E4D64\n"
    "8: .4byte gStaticData_085A4E70\n"
    "9: .4byte gStaticData_08174DD4\n"
    "10: .4byte gStaticData_08174D84\n"
    );
}

/* Same shape as InitHudIconWidgetA above, "B" icon/text widget family -
 * different data tables and line-height/glyph-stride constants. Same
 * NAKED-transcription rationale and residual r7 gap as
 * InitHudIconWidgetA - see that function's comment for the full
 * account. */
NAKED struct icon_manager *InitHudIconWidgetB(struct icon_manager *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "ldr r0, 5f\n\t"
        "str r0, [r1]\n\t"
        "mov r1, #0x88\n\t"
        "lsl r1, r1, #1\n\t"
        "add r2, r4, r1\n\t"
        "add r1, #4\n\t"
        "add r0, r4, r1\n\t"
        "mov r1, #0\n\t"
        "str r1, [r0]\n\t"
        "str r1, [r2]\n\t"
        "mov r2, #0x8c\n\t"
        "lsl r2, r2, #1\n\t"
        "add r0, r4, r2\n\t"
        "str r1, [r0]\n\t"
        "add r2, #0x14\n\t"
        "add r0, r4, r2\n\t"
        "str r1, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "ldr r2, 6f\n\t"
        "bl sub_803A94C\n\t"
        "mov r0, #0x98\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "ldr r0, 7f\n\t"
        "str r0, [r1]\n\t"
        "mov r2, #0x8e\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r4, r2\n\t"
        "mov r0, #0x10\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0x90\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "mov r0, #6\n\t"
        "str r0, [r1]\n\t"
        "add r2, #8\n\t"
        "add r1, r4, r2\n\t"
        "mov r0, #4\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0x86\n\t"
        "lsl r0, r0, #1\n\t"
        "add r1, r4, r0\n\t"
        "ldr r0, 8f\n\t"
        "str r0, [r1]\n\t"
        "add r2, #4\n\t"
        "add r1, r4, r2\n\t"
        "ldr r0, 9f\n\t"
        "str r0, [r1]\n\t"
        "mov r0, #0x3f\n\t"
        "ldrb r1, [r4, #3]\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x40\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #3]\n\t"
        "mov r2, #0\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #8\n\t"
        "ldr r6, 10f\n\t"
        "ldrb r7, [r6]\n\t"
        "mov ip, r2\n\t"
    "1:\n\t"
        "add r0, r5, r2\n\t"
        "mov r1, ip\n\t"
        "strb r1, [r0]\n\t"
        "mov r1, #0\n\t"
        "add r3, r2, #1\n\t"
        "cmp r7, r2\n\t"
        "beq 3f\n\t"
    "2:\n\t"
        "add r1, #1\n\t"
        "cmp r1, #0x4b\n\t"
        "bhi 4f\n\t"
        "add r0, r1, r6\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, r2\n\t"
        "bne 2b\n\t"
        "add r0, r5, r2\n\t"
    "3:\n\t"
        "strb r1, [r0]\n\t"
    "4:\n\t"
        "add r2, r3, #0\n\t"
        "cmp r2, #0xff\n\t"
        "bls 1b\n\t"
        "add r0, r4, #0\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "5: .4byte gStaticData_087E4DAC\n"
    "6: .4byte 0x05000002\n"
    "7: .4byte gStaticData_087E4D1C\n"
    "8: .4byte gStaticData_081751D4\n"
    "9: .4byte gStaticData_085A551C\n"
    "10: .4byte gStaticData_08175188\n"
    );
}

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
