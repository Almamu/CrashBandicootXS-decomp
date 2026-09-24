#include "core.h"

/* Same 0x40C-byte OAM shadow buffer `src/graphics/graphics.c` already
 * names `struct oam_shadow_buffer` (redeclared locally per this
 * project's minimal-local-type convention - see docs/naming.md). */
struct oam_shadow_buffer {
    s32 count;
    s32 field_04;
    s32 field_08;
    u8 table[0x400];
};

extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006AC8(struct oam_shadow_buffer *arg0, u32 *arg1);
extern s32 sub_803ADB4(s32 a, s32 b);

/* The same 91-entry-family, 12-entry "box preset" pair
 * `docs/rom_map.md` ties to `sub_801E788`'s centering math - see that
 * function's comment below for what's confirmed about them. */
extern s32 gStaticData_0816C644[12];
extern s32 gStaticData_0816C674[12];

/* GitHub issue #30's `sub_801E688`/`sub_801E788` - `LoadGraphicsPackage`'s
 * tile-cell-selection and viewport-centering helpers, already
 * characterized (but not carried to C) in docs/rom_map.md. Both operate
 * on the same "self" object `sub_801E640`/`sub_801E644`/`sub_801E8F8`/
 * `sub_801E964`/`sub_801E96C` (graphics_package_1e640.c/_1e8f8.c/_1e964.c)
 * build up - NOT the small 0x10-byte `LoadGraphicsPackage` scratch
 * buffer itself (that buffer never grows past +0xd), but a *larger*
 * caller-owned object that embeds it: every caller of these two
 * functions allocates something bigger (`u8 buf[0x70]`/`buf[0xe0]` in
 * the `settings_menu*.c` family, never the plain `buf[0x10]` the
 * `LoadGraphicsPackage`-only callers use) with room for the extra
 * fields these two touch (+0x10 through +0x24) - a hardware-OAM-attribute
 * template (+0x10..+0x17: attr0/attr1/attr2/filler) plus a handful of
 * scratch fields private to this pair. */

/* Picks the smallest-area entry from the shared 12-slot
 * `gStaticData_0816C644`/`674` "box preset" table (width/height pairs)
 * that's still large enough to hold a `width`x`height` box (each table
 * entry must be at least half of the requested dimension, i.e. the
 * request must fit at "50% zoom or better") - a best-fit box-size
 * selector. Stores the winning table index at `self+0x18` (split
 * across `self+0x13` bits 6-7 = index bits 0-1, `self+0x11` bits 6-7 =
 * index bits 2-3 - the same byte-packed shape `sub_801E788` below
 * unpacks), a "tile index" derived from the winning box's area at
 * `self+0x14` bits 0-9 (`0x400 - area/32`, clamped to 10 bits - reused
 * as the OAM `attr2` tile-index field by `sub_801E788`'s
 * `sub_8006AC8` insertion), and two Q8.8 fixed-point scale factors
 * (`self+0x20`/`self+0x24`, `sub_803ADB4`-divided: box-dimension<<8
 * over the requested dimension) that `sub_801E788` writes into the
 * shadow OAM buffer's affine-parameter overlay as a pure-scale
 * (no-rotation) 2x2 matrix. The trailing scale-classification write to
 * `self+0x11` bits 0-1 picks between "needs the scaled/clamped OAM
 * path" (3, when either scale factor undershoots 0x100 i.e. shrinks)
 * and "not shrunk" (both factors >= 0x100): within the latter, bits
 * become 1 ("oversized", when X's factor exceeds 0x100 *or* Y's does)
 * or 0 (only when both factors are exactly 0x100, i.e. 1:1 on both
 * axes) - confirmed precisely from the ROM's own fall-through shape
 * (the "Y > 0x100" check falls straight into the "set 1" block rather
 * than being a separate branch, so there is no "leave bits untouched"
 * case despite how the branch layout first looked) - `sub_801E96C`
 * (graphics_package_1e964.c) is the "reset before rebuild" pair that
 * clears this same byte's bits 4-9 beforehand, so a caller can rebuild
 * it field-by-field across several of these helpers.
 *
 * Byte-correct, but as a NAKED transcription, not real decompiled C -
 * tracked as parked, same as `sub_801E644`/`sub_801E990` elsewhere in
 * this cluster (see `docs/matching/issue-30-graphics-loading.md`).
 * Every operation here was already confirmed against the ROM (cross-
 * checked byte-for-byte against `asm/code_3_2_17_1e644.s`'s
 * disassembly) by an earlier plain-C reconstruction, but this is a
 * register-starved ~110-instruction function using all of r0-r8/sb/sl/
 * ip simultaneously in its search loop, and it hits the exact
 * `sub_801E788`-documented gcc-2.9/agbcc limitation on `r7`, just in a
 * new flavor: the ROM's own compile needs `r7` saved (its
 * `push {r4-r7,lr}` / `push {r5,r6,r7}` prologue and mirrored epilogue),
 * but this compiler's own callee-save/prologue-generation pass never
 * adds `r7` to that push/pop list no matter how `r7` is referenced from
 * inline asm - confirmed two ways: (1) an opaque `asm volatile` island
 * covering the whole function body (register-pinning only `self`/
 * `arg1`/`arg2` as plain `r0`/`r1`/`r2` inputs, leaving every other
 * register including `r7` as a bare clobber) compiled and linked with
 * every single body instruction byte-identical to the ROM, but the
 * compiler-synthesized prologue/epilogue silently dropped `r7` from
 * both push/pop lists (`push {r4,r5,r6,lr}` instead of
 * `push {r4,r5,r6,r7,lr}`, and the matching epilogue), even though the
 * body plainly uses `r7` throughout; (2) adding a dummy
 * `register s32 r7dummy asm("r7")` output operand (the same "real
 * register variable, not just a clobber string" fix that unblocks
 * other registers in this project) made no difference - `r7`
 * specifically never enters the save list via any inline-asm-based
 * hint, matching `sub_801E788`'s own documented conclusion that this is
 * a genuine, reproducible per-register gcc-2.9/agbcc gap, not something
 * C-level phrasing routes around. Transcribed instruction-for-
 * instruction from the ROM disassembly instead, confirmed
 * byte-identical via direct isolated-object comparison against
 * `asm/code_3_2_17_1e644.s`'s bytes for this function before
 * integrating - the same escape hatch already used for `sub_801E644`
 * (`graphics_package_1e640.c`) and `sub_801E990`
 * (`graphics_loading_1e990.c`). */
NAKED void sub_801E688(u8 *self, s32 arg1, s32 arg2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r6, r0, #0\n\t"
        "add r7, r1, #0\n\t"
        "mov r8, r2\n\t"
        "str r7, [r6, #8]\n\t"
        "str r2, [r6, #0xc]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #5\n\t"
        "mov ip, r0\n\t"
        "mov r3, #0\n\t"
        "ldr r1, =gStaticData_0816C644\n\t"
        "mov sb, r1\n\t"
        "ldr r2, =gStaticData_0816C674\n\t"
        "mov sl, r2\n\t"
        "mov r5, sl\n\t"
        "mov r4, sb\n\t"
        "1:\n\t"
        "ldr r2, [r4]\n\t"
        "lsl r0, r2, #1\n\t"
        "cmp r7, r0\n\t"
        "bgt 2f\n\t"
        "ldr r1, [r5]\n\t"
        "lsl r0, r1, #1\n\t"
        "cmp r8, r0\n\t"
        "bgt 2f\n\t"
        "add r0, r2, #0\n\t"
        "mul r0, r1\n\t"
        "cmp r0, ip\n\t"
        "bge 2f\n\t"
        "mov ip, r0\n\t"
        "str r3, [r6, #0x18]\n\t"
        "2:\n\t"
        "add r5, #4\n\t"
        "add r4, #4\n\t"
        "add r3, #1\n\t"
        "cmp r3, #0xb\n\t"
        "ble 1b\n\t"
        "ldr r4, [r6, #0x18]\n\t"
        "lsl r1, r4, #6\n\t"
        "mov r2, #0x3f\n\t"
        "add r0, r2, #0\n\t"
        "ldrb r3, [r6, #0x13]\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r6, #0x13]\n\t"
        "asr r0, r4, #2\n\t"
        "lsl r0, r0, #6\n\t"
        "add r5, r2, #0\n\t"
        "ldrb r1, [r6, #0x11]\n\t"
        "and r5, r1\n\t"
        "orr r5, r0\n\t"
        "strb r5, [r6, #0x11]\n\t"
        "mov r0, ip\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "add r0, #0x1f\n\t"
        "3:\n\t"
        "asr r0, r0, #5\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #3\n\t"
        "add r1, r2, #0\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r3, =0x000003FF\n\t"
        "add r0, r3, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, =0xFFFFFC00\n\t"
        "ldrh r2, [r6, #0x14]\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r6, #0x14]\n\t"
        "lsl r4, r4, #2\n\t"
        "mov r3, sb\n\t"
        "add r0, r4, r3\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #8\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_803ADB4\n\t"
        "add r7, r0, #0\n\t"
        "str r7, [r6, #0x20]\n\t"
        "add r4, sl\n\t"
        "ldr r0, [r4]\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r1, r8\n\t"
        "bl sub_803ADB4\n\t"
        "str r0, [r6, #0x24]\n\t"
        "cmp r7, #0xff\n\t"
        "ble 4f\n\t"
        "cmp r0, #0xff\n\t"
        "bgt 5f\n\t"
        "4:\n\t"
        "mov r0, #3\n\t"
        "orr r5, r0\n\t"
        "b 6f\n\t"
        ".pool\n\t"
        "5:\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "cmp r7, r1\n\t"
        "bgt 7f\n\t"
        "cmp r0, r1\n\t"
        "ble 8f\n\t"
        "7:\n\t"
        "mov r0, #4\n\t"
        "neg r0, r0\n\t"
        "and r5, r0\n\t"
        "mov r0, #1\n\t"
        "orr r5, r0\n\t"
        "b 6f\n\t"
        "8:\n\t"
        "mov r0, #4\n\t"
        "neg r0, r0\n\t"
        "and r5, r0\n\t"
        "6:\n\t"
        "strb r5, [r6, #0x11]\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (the
 * `matching_decomp_alignment_fix` precedent - see also `sub_801E644`
 * above and `sub_801E96C` in graphics_package_1e964.c). */
asm(".align 2, 0");

/* Computes `self`'s on-screen position from one of four modes packed
 * into `self+0x11` bits 0-1 (mode 2 is a no-op: no position math at
 * all), then unconditionally inserts `self`'s pre-built OAM attribute
 * template (`self+0x10..+0x17`) into the shadow OAM buffer
 * (`gUnknown_03001300`, `sub_8006AC8`) - and, unless mode was 0
 * (raw/no-centering, which just clears the affine-enable bits at
 * `self+0x13` bits 4-5), also allocates one affine-parameter group
 * from the shadow buffer's `field_08` counter and writes a pure-scale
 * (no rotation) 2x2 affine matrix into it from `sub_801E688`'s
 * `self+0x20`/`self+0x24` Q8.8 factors:
 *  - Mode 0: `self+0x12` (the OAM `attr1` field's low 9 bits, X
 *    position) = `self+0x00` as-is; `self+0x10` (`attr0` low byte, Y
 *    position) = `self+0x04` as-is - no centering, position copied
 *    straight through.
 *  - Mode 1: centers within table entry `self+0x18`'s box: X position
 *    = `self+0x00` minus half of `(gStaticData_0816C644[idx] -
 *    self+0x08)`; Y position = `self+0x04` minus half of
 *    `(gStaticData_0816C674[idx] - self+0x0c)`.
 *  - Mode 3: the mirror of mode 1 - X position = `self+0x00` plus half
 *    of `self+0x08`, minus the table's own X reference; Y position =
 *    `self+0x04` plus half of `self+0x0c`, minus the table's Y
 *    reference.
 *
 * The affine-group allocation resolves a real open question flagged by
 * this issue's third pass: the writes through `gUnknown_03001300` at
 * offsets that looked like they didn't fit the shadow buffer's 8-byte
 * hardware-OAM-entry stride actually do - `field_08 * 0x20 + 0x12`
 * (and its three siblings 8/16/24 bytes further on) is exactly
 * `(field_08 * 4 + 0/1/2/3) * 8 + 6`, i.e. the *filler* halfword (byte
 * offset +6) of four consecutive shadow-OAM entries starting at
 * `field_08 * 4` - real hardware overlays the OBJ affine-parameter
 * memory (PA/PB/PC/PD) on exactly that halfword of every 4th OAM
 * entry. The four writes here (scaleX, 0, 0, scaleY) are PA/PB/PC/PD
 * of a diagonal (no-rotation) scale-only matrix, and `self+0x13` bits
 * 1-5 (packed alongside the position bits `self+0x12`'s high byte
 * already holds) become that affine group's 5-bit selector index in
 * the sprite's own `attr1` field - a coherent, understood mechanism,
 * not a layout mismatch.
 *
 * Byte-correct, but as a NAKED transcription, not real decompiled C -
 * tracked as parked, same as `sub_801E644`/`sub_801E688`/`sub_801E990`
 * elsewhere in this cluster (see
 * `docs/matching/issue-30-graphics-loading.md`). Semantics and control
 * flow (branch order, mask constants, the `field_08`-delta-materialized
 * third clear-mask) were already confirmed instruction-for-instruction
 * against `asm/code_3_2_17_1e644.s` by an earlier plain-C
 * reconstruction, and heavy iteration (the negative-constant clear-mask
 * idiom, a `struct oam_shadow_buffer **addr = &gUnknown_03001300`
 * address cache matching `graphics_loading_21d80.c`'s established
 * pattern, explicit register pins for the loop-scoped `slot` value)
 * closed every gap but one: the ROM keeps `self` in `r7` for the whole
 * function (matching its 4-register `push {r4-r7}` list), but this
 * compiler only keeps `self+offset` dereferences compiled as a single
 * `ldrb/ldrh/ldr rX,[r7,#imm]` instruction when `self` is an *ordinary*
 * (non-`register`) local - the moment `self` is pinned to a specific
 * hard register via `register u8 *self asm("r7")` (confirmed with a
 * minimal one-line repro: `return self[0x11];` alone), this compiler
 * stops folding the offset into the load/store's immediate field and
 * instead always emits a separate `add rX, rX, #imm` before a
 * zero-offset dereference - a real, reproducible gcc-2.9/agbcc
 * limitation for asm-register-pinned pointer locals, unreachable from
 * *plain* C under this compiler. That framing is specifically about
 * normal C-level typing/register-pin semantics, though - it doesn't
 * apply to a `NAKED` hand transcription, which bypasses this
 * compiler's addressing-mode-folding pass entirely (there's no C-level
 * codegen left to fight - every instruction here is written literally,
 * the same escape hatch this cluster's sibling `sub_801E688` above and
 * `sub_801E644`/`sub_801E990` elsewhere already used for the same
 * underlying `r7`-addressing-mode-folding bug class). Transcribed
 * instruction-for-instruction from the ROM disassembly instead,
 * confirmed byte-identical via an isolated `cpp`+`agbcc` compile,
 * `arm-none-eabi-as` assemble, and a direct byte comparison against
 * `asm/code_3_2_17_1e644.s`'s bytes for this function before
 * integrating - matched on the first attempt, no further iteration
 * needed. */
NAKED void sub_801E788(u8 *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r7, r0, #0\n\t"
        "ldrb r1, [r7, #0x11]\n\t"
        "lsl r0, r1, #0x1e\n\t"
        "lsr r0, r0, #0x1e\n\t"
        "cmp r0, #1\n\t"
        "beq 2f\n\t"
        "cmp r0, #1\n\t"
        "blo 1f\n\t"
        "cmp r0, #3\n\t"
        "beq 3f\n\t"
        "b 4f\n\t"
        "1:\n\t"
        "ldr r1, [r7]\n\t"
        "ldr r2, =0x000001FF\n\t"
        "add r0, r2, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, =0xFFFFFE00\n\t"
        "ldrh r3, [r7, #0x12]\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r7, #0x12]\n\t"
        "ldr r0, [r7, #4]\n\t"
        "strb r0, [r7, #0x10]\n\t"
        "b 4f\n\t"
        ".pool\n\t"
        "2:\n\t"
        "ldr r0, =gStaticData_0816C644\n\t"
        "ldr r2, [r7, #0x18]\n\t"
        "lsl r2, r2, #2\n\t"
        "add r0, r2, r0\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r7, #8]\n\t"
        "sub r0, r0, r1\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r1, [r7]\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r3, =0x000001FF\n\t"
        "add r0, r3, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, =0xFFFFFE00\n\t"
        "ldrh r3, [r7, #0x12]\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r7, #0x12]\n\t"
        "ldr r0, =gStaticData_0816C674\n\t"
        "add r2, r2, r0\n\t"
        "ldr r0, [r2]\n\t"
        "ldr r1, [r7, #0xc]\n\t"
        "sub r0, r0, r1\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "ldrb r1, [r7, #4]\n\t"
        "sub r0, r1, r0\n\t"
        "strb r0, [r7, #0x10]\n\t"
        "b 4f\n\t"
        ".pool\n\t"
        "3:\n\t"
        "ldr r0, [r7, #8]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "ldr r1, [r7]\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, =gStaticData_0816C644\n\t"
        "ldr r3, [r7, #0x18]\n\t"
        "lsl r3, r3, #2\n\t"
        "add r0, r3, r0\n\t"
        "ldr r0, [r0]\n\t"
        "sub r1, r1, r0\n\t"
        "ldr r2, =0x000001FF\n\t"
        "add r0, r2, #0\n\t"
        "and r1, r0\n\t"
        "ldr r0, =0xFFFFFE00\n\t"
        "ldrh r2, [r7, #0x12]\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r7, #0x12]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "ldr r0, [r7, #0xc]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r2, r2, r0\n\t"
        "ldr r0, =gStaticData_0816C674\n\t"
        "add r3, r3, r0\n\t"
        "ldrb r3, [r3]\n\t"
        "sub r2, r2, r3\n\t"
        "strb r2, [r7, #0x10]\n\t"
        "4:\n\t"
        "mov r0, #3\n\t"
        "ldrb r3, [r7, #0x11]\n\t"
        "and r0, r3\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r7, #0x13]\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x21\n\t"
        "neg r1, r1\n\t"
        "and r0, r1\n\t"
        "strb r0, [r7, #0x13]\n\t"
        "ldr r6, =gUnknown_03001300\n\t"
        "b 6f\n\t"
        ".pool\n\t"
        "5:\n\t"
        "ldr r6, =gUnknown_03001300\n\t"
        "ldr r4, [r6]\n\t"
        "ldr r0, [r4, #8]\n\t"
        "add r3, r0, #0\n\t"
        "add r0, #1\n\t"
        "str r0, [r4, #8]\n\t"
        "mov r0, #7\n\t"
        "add r1, r3, #0\n\t"
        "and r1, r0\n\t"
        "lsl r1, r1, #1\n\t"
        "mov r0, #0xf\n\t"
        "neg r0, r0\n\t"
        "ldrb r2, [r7, #0x13]\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "asr r1, r3, #3\n\t"
        "mov r5, #1\n\t"
        "and r1, r5\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r2, #0x11\n\t"
        "neg r2, r2\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "asr r1, r3, #4\n\t"
        "and r1, r5\n\t"
        "lsl r1, r1, #5\n\t"
        "sub r2, #0x10\n\t"
        "and r0, r2\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r7, #0x13]\n\t"
        "ldrh r0, [r7, #0x20]\n\t"
        "lsl r1, r3, #2\n\t"
        "lsl r3, r3, #5\n\t"
        "add r3, r4, r3\n\t"
        "mov r2, #0\n\t"
        "strh r0, [r3, #0x12]\n\t"
        "add r0, r1, #1\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r4, r0\n\t"
        "strh r2, [r0, #0x12]\n\t"
        "add r0, r1, #2\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r4, r0\n\t"
        "strh r2, [r0, #0x12]\n\t"
        "ldrh r0, [r7, #0x24]\n\t"
        "add r1, #3\n\t"
        "lsl r1, r1, #3\n\t"
        "add r4, r4, r1\n\t"
        "strh r0, [r4, #0x12]\n\t"
        "6:\n\t"
        "ldr r0, [r6]\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0x10\n\t"
        "bl sub_8006AC8\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}
