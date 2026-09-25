#include "core.h"

/* GitHub issues #9/#10/#41's remaining piece of `sub_8026628`'s own
 * "umbrella 5-mode dispatcher" cluster (`game_loop43.c`,
 * docs/matching/issue-9-10-41-0x08026628-game-loop.md): that pass fully
 * derived both of these axis resolvers' semantics from their own raw
 * bytes (see that doc's "`sub_8026AE8`/`sub_8026A18`: which axis each
 * one actually resolves" section) but left them raw, flagging them as
 * "more in the shape of this ROM region's other gcc-2.9-resistant
 * tile-scan loops ... than this dispatcher was." This pass confirms
 * that and closes both as NAKED transcriptions.
 *
 * `s32 fn(void *self, struct probe_pos *pos, s32 span, s32 *outValue,
 * s32 submode)`:
 *
 *   - `sub_8026A18` (the Y-axis/floor-ceiling resolver, `mode ==
 *     4`/`8` in `sub_8026628`): scans `pos->x` over `[x, x+span-1]>>3`
 *     at a fixed `pos->y>>3` tile row, clamping the start index up to
 *     0 if it computes to exactly -1 and the end index down by one if
 *     it lands exactly on `(*(struct tile_cache **)(self+0x20))+0x10`
 *     (that cache's own cached width-in-tiles field, `struct
 *     tile_cache::unk010` in `game_loop3.c` - confirmed genuinely read
 *     here, unlike that struct's own comment there which predates this
 *     pass), calling `sub_8025130(self->0x20, tileX, tileY, submode,
 *     &scratch)` (matched, `game_loop3.c`) per tile until a hit or the
 *     range is exhausted. On a hit, accumulates into `*outValue` using
 *     `pos->y & 7`: `submode == 2` adds `(8-(y&7))<<8`, `submode == 0`
 *     subtracts `(y&7)<<8` (any other submode value leaves `*outValue`
 *     untouched on a hit - dead code in practice, since `sub_8026628`
 *     only ever passes `0`/`2` here).
 *   - `sub_8026AE8` (the X-axis/wall resolver, `mode == 1`/`2`): the
 *     same shape, scanning `pos->y` over the same span at a fixed
 *     `pos->x>>3` tile column, clamped against `unk014` (the cache's
 *     cached height-in-tiles field) instead of `unk010`. On a hit,
 *     using `pos->x & 7`: `submode == 3` adds `1+(8-(x&7))<<8`
 *     (`(*outValue+1)` is read before the shift-add, giving the extra
 *     `+1` epsilon term `sub_8026628`'s own doc flagged as this
 *     resolver's asymmetry versus the Y-axis one), `submode == 1`
 *     subtracts `(x&7)<<8` from `(*outValue-1)` (any other submode
 *     value again leaves `*outValue` untouched - `sub_8026628` only
 *     ever passes `1`/`3` here).
 *   - Both finish with the same tail: if `self+0x2a` (the "flag held
 *     set" byte `docs/matching/issue-9-10-0x0800a884-graphics.md`
 *     already named for this same `self`/`gUnknown_03001308`-shaped
 *     object) is nonzero *and* the scan's own scratch out-flag from
 *     its last `sub_8025130` call is nonzero, writes that scratch byte
 *     into `self+0x29` (the same doc's "dispatch nibble" byte).
 *     Returns the scan's own hit flag either way.
 *
 * Both are byte-identical in shape to `sub_8025130`/`sub_8025228`
 * (`game_loop3.c`) and `sub_8025460` (`game_loop4.c`) next door: the
 * same gcc-2.9 register-allocation-permutation gap already forced
 * those NAKED (self/pos/outValue/hit packed into `sl`/r7/r8/`sb`
 * simultaneously, restored via a single `pop {r3,r4,r5}` +
 * `mov r8,r3` / `mov sb,r4` / `mov sl,r5` triplet right before the
 * epilogue). Confirmed the same gap here too: an isolated-compile
 * attempt at plain C (a `switch`-free straight-line reconstruction,
 * `struct`-typed per this project's standing "prefer structs"
 * instruction, then a raw-offset-cast variant to rule out the struct
 * itself as the cause) never reproduced the ROM's own `sl`/`r7`/`r8`/
 * `sb` assignment - the unpinned compile put `self` on the stack
 * instead of `sl` and freed `r6` for a hoisted-cache-pointer local the
 * ROM's own code re-derives from `self->0x20` fresh every loop
 * iteration instead of caching; pinning `self`/`pos`/`outValue`/`hit`
 * to their ROM registers via `register ... asm("sl")` etc. made it
 * *worse* in the same way `game_loop3.c`'s own `sub_8024F24` comment
 * already documented for this exact bug class - the compiler stopped
 * treating the pinned `pos` register as a base pointer at some call
 * sites and instead started passing `pos` itself in for `submode`
 * (`add r3, r7, #0` where the ROM has `ldr r3, [sp, #0x2c]`), a
 * mis-optimization, not just a relocation-site difference. Recognized
 * the pattern rather than iterating further (per this project's own
 * established practice for this exact register-shuttle shape) and
 * closed both as hand-transcribed NAKED functions instead, instruction
 * for instruction from the ROM's own disassembly
 * (`0x08026A18`-`0x08026BC0`, the tail of what was
 * `asm/code_3_2_17_266bc.s`).
 *
 * Confirmed byte-identical to the ROM's own raw bytes at
 * `0x08026A18`-`0x08026BC0` (424 bytes total, both functions, the only
 * differences being the two `bl sub_8025130` relocation sites, which
 * resolve correctly once linked) via the isolated `cpp`/`agbcc`/`as` +
 * `objcopy`/`cmp` pipeline against `baserom.gba`, plus a full clean
 * `rm -rf build && make NON_MATCHING=1 report` (no warnings) and
 * `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
 * crashbandicootxs.map && make compare` (`crashbandicootxs.gba: La
 * suma coincide`). Both end with the same zero-byte `.align 2, 0` gap
 * `sub_8026628` itself had (confirmed via objdump against the raw ROM
 * bytes: `0000` right after each `bx r1`, not the assembler's default
 * `mov r8, r8` inter-function padding) - the established
 * `matching_decomp_alignment_fix` idiom.
 *
 * `asm/code_3_2_17_266bc.s` is trimmed to end right after
 * `sub_8026A14`'s own `bx lr` (that tiny stub, and everything before
 * it back to `sub_80266BC`, stays genuinely raw and out of this pass's
 * scope); this file's own two functions pick up immediately after in
 * `ldscript.txt`, with `src/system/game_loop44.o` (`sub_8026BC0`)
 * following right behind, unchanged. */

struct probe_pos
{
    s32 x;
    s32 y;
};

struct tile_cache;

extern void *sub_8025130(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);

NAKED s32 sub_8026A18(void *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "mov sl, r0\n\t"
        "add r7, r1, #0\n\t"
        "mov r8, r3\n\t"
        "mov r0, #0\n\t"
        "mov sb, r0\n\t"
        "add r0, sp, #4\n\t"
        "mov r1, sb\n\t"
        "strb r1, [r0]\n\t"
        "ldr r6, [r7, #4]\n\t"
        "ldr r4, [r7]\n\t"
        "add r2, r4, r2\n\t"
        "sub r5, r2, #1\n\t"
        "asr r4, r4, #3\n\t"
        "asr r6, r6, #3\n\t"
        "asr r5, r5, #3\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r4, r0\n\t"
        "bne 1f\n\t"
        "mov r4, #0\n\t"
        "1:\n\t"
        "mov r2, sl\n\t"
        "ldr r0, [r2, #0x20]\n\t"
        "ldr r0, [r0, #0x10]\n\t"
        "cmp r5, r0\n\t"
        "bne 2f\n\t"
        "sub r5, #1\n\t"
        "2:\n\t"
        "mov r0, sl\n\t"
        "add r0, #0x2a\n\t"
        "str r0, [sp, #8]\n\t"
        "cmp r4, r5\n\t"
        "bgt 5f\n\t"
        "3:\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "add r2, sp, #4\n\t"
        "str r2, [sp]\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r6, #0\n\t"
        "ldr r3, [sp, #0x2c]\n\t"
        "bl sub_8025130\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "mov r0, #1\n\t"
        "mov sb, r0\n\t"
        "4:\n\t"
        "add r4, #1\n\t"
        "cmp r4, r5\n\t"
        "bgt 5f\n\t"
        "mov r1, sb\n\t"
        "cmp r1, #0\n\t"
        "beq 3b\n\t"
        "5:\n\t"
        "mov r2, sb\n\t"
        "cmp r2, #0\n\t"
        "beq 9f\n\t"
        "ldr r0, [sp, #0x2c]\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "cmp r0, #2\n\t"
        "bne 9f\n\t"
        "ldr r0, [r7, #4]\n\t"
        "mov r1, #7\n\t"
        "and r0, r1\n\t"
        "mov r1, #8\n\t"
        "sub r1, r1, r0\n\t"
        "lsl r1, r1, #8\n\t"
        "mov r2, r8\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r2]\n\t"
        "b 9f\n\t"
        "7:\n\t"
        "ldr r0, [r7, #4]\n\t"
        "mov r1, #7\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #8\n\t"
        "mov r2, r8\n\t"
        "ldr r1, [r2]\n\t"
        "sub r1, r1, r0\n\t"
        "str r1, [r2]\n\t"
        "9:\n\t"
        "ldr r1, [sp, #8]\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "add r0, sp, #4\n\t"
        "ldrb r1, [r0]\n\t"
        "cmp r1, #0\n\t"
        "beq 8f\n\t"
        "mov r0, sl\n\t"
        "add r0, #0x29\n\t"
        "strb r1, [r0]\n\t"
        "8:\n\t"
        "mov r0, sb\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
/* Trailing byte count is already flush to a 4-byte boundary (208 B
 * total), but the ROM's own `.align 2, 0` still emits a genuine
 * zero-byte `0000` halfword here (confirmed via objdump against the
 * raw ROM bytes) rather than the assembler's default inter-function
 * padding - the established `matching_decomp_alignment_fix` idiom. */
asm(".align 2, 0");

NAKED s32 sub_8026AE8(void *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0xc\n\t"
        "mov sl, r0\n\t"
        "add r7, r1, #0\n\t"
        "mov r8, r3\n\t"
        "mov r0, #0\n\t"
        "mov sb, r0\n\t"
        "add r0, sp, #4\n\t"
        "mov r1, sb\n\t"
        "strb r1, [r0]\n\t"
        "ldr r4, [r7, #4]\n\t"
        "ldr r6, [r7]\n\t"
        "add r2, r4, r2\n\t"
        "sub r5, r2, #1\n\t"
        "asr r6, r6, #3\n\t"
        "asr r4, r4, #3\n\t"
        "asr r5, r5, #3\n\t"
        "mov r0, #1\n\t"
        "neg r0, r0\n\t"
        "cmp r4, r0\n\t"
        "bne 1f\n\t"
        "mov r4, #0\n\t"
        "1:\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "ldr r0, [r0, #0x14]\n\t"
        "cmp r5, r0\n\t"
        "bne 2f\n\t"
        "sub r5, #1\n\t"
        "2:\n\t"
        "mov r0, sl\n\t"
        "add r0, #0x2a\n\t"
        "str r0, [sp, #8]\n\t"
        "cmp r4, r5\n\t"
        "bgt 5f\n\t"
        "3:\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "add r1, sp, #4\n\t"
        "str r1, [sp]\n\t"
        "add r1, r6, #0\n\t"
        "add r2, r4, #0\n\t"
        "ldr r3, [sp, #0x2c]\n\t"
        "bl sub_8025130\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "mov r0, #1\n\t"
        "mov sb, r0\n\t"
        "4:\n\t"
        "add r4, #1\n\t"
        "cmp r4, r5\n\t"
        "bgt 5f\n\t"
        "mov r1, sb\n\t"
        "cmp r1, #0\n\t"
        "beq 3b\n\t"
        "5:\n\t"
        "mov r0, sb\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldr r1, [sp, #0x2c]\n\t"
        "cmp r1, #1\n\t"
        "beq 7f\n\t"
        "cmp r1, #3\n\t"
        "bne 9f\n\t"
        "mov r0, r8\n\t"
        "ldr r2, [r0]\n\t"
        "add r2, #1\n\t"
        "ldr r1, [r7]\n\t"
        "mov r0, #7\n\t"
        "and r1, r0\n\t"
        "mov r0, #8\n\t"
        "sub r0, r0, r1\n\t"
        "lsl r0, r0, #8\n\t"
        "add r2, r2, r0\n\t"
        "mov r1, r8\n\t"
        "str r2, [r1]\n\t"
        "b 9f\n\t"
        "7:\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "sub r0, #1\n\t"
        "ldr r1, [r7]\n\t"
        "mov r2, #7\n\t"
        "and r1, r2\n\t"
        "lsl r1, r1, #8\n\t"
        "sub r0, r0, r1\n\t"
        "mov r1, r8\n\t"
        "str r0, [r1]\n\t"
        "9:\n\t"
        "ldr r1, [sp, #8]\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "add r0, sp, #4\n\t"
        "ldrb r1, [r0]\n\t"
        "cmp r1, #0\n\t"
        "beq 8f\n\t"
        "mov r0, sl\n\t"
        "add r0, #0x29\n\t"
        "strb r1, [r0]\n\t"
        "8:\n\t"
        "mov r0, sb\n\t"
        "add sp, #0xc\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
/* Same zero-byte `.align 2, 0` gap (216 B total, already 4-byte
 * flush) as `sub_8026A18` above. */
asm(".align 2, 0");
