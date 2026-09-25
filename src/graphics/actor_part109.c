#include "core.h"

/* GitHub issue #9/#10: 0x0800CD00, `sub_800AAEC`'s (`actor_part108.c`)
 * only caller/callee companion - `sub_800AAEC` calls this once per
 * `gUnknown_0300130C` list entry whose own `+0x18`-table `+0x48`
 * trampoline (`sub_803AD7C`) reports state `3`, passing that entry as
 * `self` and its own `x` (the target "action" index) straight through.
 *
 * Early-outs (returns `0`) when `self+0x4e` (a state/type byte, the
 * same offset `docs/rom_map.md`'s "eight more core reads" investigation
 * already reads as an object's own state byte elsewhere in this ROM
 * region) is `5` or `0xa`.
 *
 * Otherwise builds THREE AABBs via the shared `sub_803AFE4`(set-pos)/
 * `sub_803AFDC`(set-size) primitive (`struct aabb` from
 * `actor_part.c`/`src/system/game_loop6.c`), all from the same
 * "keyframe/hitbox record" table convention documented at length in
 * `game_loop6.c`'s own `sub_800D040` header comment: `+0x20` is a
 * pointer-to-table, indexed by a `+0x2d` tag byte at 28-byte stride
 * (`docs/rom_map.md`'s own cross-reference from this exact function:
 * "matching `gStaticData_0816BC98`'s stride exactly, but clearly a
 * different table instance" - reinforcing the project's established
 * "shared convention, not shared struct" reading, since `self` here is
 * a plain `gUnknown_0300130C` list entry, not the physics subsystem's
 * own object type), with the record's own `{s16 offX, s16 offY, u8 w,
 * u8 h}` quad at `+4`/`+6`/`+8`/`+9` this time (yet another layout
 * variant of the same convention, alongside `actor_part.c`'s
 * `+0xc`/`+0xe`/`+0x10`/`+0x11` and `game_loop6.c`'s own `+4`/`+6`/
 * `+8`/`+9`, which this function's first two AABBs match exactly).
 * Each AABB is mirrored horizontally/vertically around its own
 * object's integer position when that object's own `+0x28` bits 4/5
 * (the mirror-flag convention `actor_part16.c`/`actor_part17.c`/
 * `game_loop6.c` all already read) are set:
 *
 *   - AABB1: from `self`'s own `+0x20`-table, indexed by `self`'s own
 *     `+0x2d` tag - `self`'s current hitbox.
 *   - AABB2: from the player's (`gUnknown_030012D8`) own `+0x20`-table,
 *     indexed by the PLAYER's own `+0x2d` tag - the player's current
 *     hitbox.
 *   - AABB3 (reuses AABB2's stack slot): from the player's `+0x20`-
 *     table again, but indexed by `x` (this function's own second
 *     argument, the caller's target action index) instead of the
 *     player's `+0x2d` - the player's hitbox FOR the target action.
 *
 * If AABB1 overlaps AABB2 (`sub_8001640`, the inclusive/touching-counts
 * variant - `src/graphics/aabb_util.c`), bails out and returns `0`
 * immediately: `self`'s hitbox already overlaps the player's CURRENT
 * hitbox, so this is not a fresh trigger. Otherwise, returns `1` only
 * if AABB1 overlaps AABB3 - `self`'s hitbox overlaps the player's
 * hitbox for the action `x` the caller is testing. Read together with
 * `sub_800AAEC`, this is a "would performing action `x` right now hit
 * `self`, given the player isn't already touching it in its current
 * pose" gate - consistent with `sub_800AAEC`'s own role gating the
 * 42-slot action-dispatch table's action codes `0xB`/`0x10`.
 *
 * NAKED transcription, not real C: this is the same AABB-build
 * primitive `game_loop6.c`'s `sub_800D040` already documents at length
 * (inlined twice there, three times here), which that function's own
 * header comment already records as resistant to C reconstruction -
 * "the ROM keeps exactly two extra callee-saved registers live across
 * both AABB builds (r8 and sb) ... and reuses r7/r8 for the X/Y 'shift'
 * values across *both* the self-block and the player-block - no C
 * reconstruction tried reproduced that with gcc 2.9". This function is
 * a strict superset of that same shape (three AABB builds instead of
 * two, r7/r8/sb kept live across all three plus an extra `sl`-held
 * argument), so it was transcribed directly as byte-exact NAKED asm
 * rather than re-attempting a C reconstruction already known to fail
 * on the simpler two-AABB case - see
 * docs/matching/issue-9-10-0x0800aaec-graphics.md. Every load, store,
 * branch and register choice below is copied instruction-for-
 * instruction from the ROM disassembly (formerly
 * `asm/code_3_2_17.s`'s `sub_800CD00`, now split out into this file -
 * that fragment is trimmed to end right before this function, with the
 * remainder from `sub_800CEAC` onward moved to the new
 * `asm/code_3_2_17_ceac.s`). */
NAKED u8 sub_800CD00(void *self, s32 x)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x20\n\t"
        "add r6, r0, #0\n\t"
        "mov sl, r1\n\t"
        "add r0, #0x4e\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #5\n\t"
        "bne 1f\n\t"
        "b 10f\n\t"
    "1:\n\t"
        "cmp r0, #0xa\n\t"
        "bne 2f\n\t"
        "b 10f\n\t"
    "2:\n\t"
        "ldr r1, [r6, #0x20]\n\t"
        "add r2, r6, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldrb r3, [r2]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, r1, r0\n\t"
        "add r3, r1, #4\n\t"
        "ldr r0, [r6]\n\t"
        "asr r7, r0, #8\n\t"
        "ldr r0, [r6, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "mov r8, r0\n\t"
        "mov r0, #4\n\t"
        "ldrsh r1, [r1, r0]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r3, r0]\n\t"
        "ldrb r4, [r3, #4]\n\t"
        "ldrb r5, [r3, #5]\n\t"
        "add r1, r1, r7\n\t"
        "add r2, r8\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
        "add r3, r6, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r1, [r3]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "3:\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 4f\n\t"
        "mov r2, r8\n\t"
        "lsl r0, r2, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "4:\n\t"
        "ldr r3, 5f\n\t"
        "mov sb, r3\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r1, [r0]\n\t"
        "asr r7, r1, #8\n\t"
        "ldr r1, [r0, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "mov r8, r1\n\t"
        "ldr r2, [r0, #0x20]\n\t"
        "add r0, #0x2d\n\t"
        "ldrb r3, [r0]\n\t"
        "lsl r1, r3, #3\n\t"
        "sub r1, r1, r3\n\t"
        "lsl r1, r1, #2\n\t"
        "ldr r0, [r2]\n\t"
        "add r1, r0, r1\n\t"
        "add r0, r1, #4\n\t"
        "mov r2, #4\n\t"
        "ldrsh r1, [r1, r2]\n\t"
        "mov r3, #2\n\t"
        "ldrsh r2, [r0, r3]\n\t"
        "ldrb r4, [r0, #4]\n\t"
        "ldrb r5, [r0, #5]\n\t"
        "add r1, r1, r7\n\t"
        "add r2, r8\n\t"
        "add r0, sp, #0x10\n\t"
        "bl sub_803AFE4\n\t"
        "add r0, sp, #0x10\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 6f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x10]\n\t"
    "6:\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 7f\n\t"
        "mov r3, r8\n\t"
        "lsl r0, r3, #1\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r2, [sp, #0x1c]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x14]\n\t"
    "7:\n\t"
        "add r6, sp, #0x10\n\t"
        "mov r0, sp\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8001640\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 10f\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "ldr r0, [r0, #0x20]\n\t"
        "mov r2, sl\n\t"
        "lsl r1, r2, #3\n\t"
        "sub r1, r1, r2\n\t"
        "lsl r1, r1, #2\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r1\n\t"
        "add r3, r0, #4\n\t"
        "mov r2, #4\n\t"
        "ldrsh r1, [r0, r2]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r3, r0]\n\t"
        "ldrb r4, [r3, #4]\n\t"
        "ldrb r5, [r3, #5]\n\t"
        "add r1, r1, r7\n\t"
        "add r2, r8\n\t"
        "add r0, r6, #0\n\t"
        "bl sub_803AFE4\n\t"
        "add r0, r6, #0\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
        "mov r1, sb\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 8f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x10]\n\t"
    "8:\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 9f\n\t"
        "mov r3, r8\n\t"
        "lsl r0, r3, #1\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r2, [sp, #0x1c]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x14]\n\t"
    "9:\n\t"
        "mov r0, sp\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8001640\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #1\n\t"
        "bne 10f\n\t"
        "mov r0, #1\n\t"
        "b 11f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012D8\n"
    "10:\n\t"
        "mov r0, #0\n\t"
    "11:\n\t"
        "add sp, #0x20\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    );
}
