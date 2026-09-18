#include "core.h"
#include "actor.h"

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem documented in docs/rom_map.md ("Confirmed: a shared
 * physics/collision subsystem, entered from multiple different entity
 * types"). This first function of that subsystem sits right after
 * already-matched `game_loop` code - `sub_0800D18C` and `sub_800E08C`
 * immediately after it are two of the subsystem's largest, most
 * tangled functions and are left untouched for now; see
 * docs/matching/issue-12-physics-collision.md. */

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *gUnknown_030012D8;
extern u8 gStaticData_0816BBC4[];
extern void sub_800EEF0(void *self, u8 arg1);
extern void sub_800E7A8(void *self, u8 arg1, u8 arg2, u8 arg3);

/* Builds two AABBs - one for `self`, one for the player
 * (`gUnknown_030012D8`) - from the shared "keyframe/hitbox record"
 * table convention already established by `sub_8007B00`/`sub_8007B98`
 * in actor_part.c (`self+0x20` -> a pointer-to-table, indexed by
 * `self+0x2d` at 0x1c/28-byte stride; here the {s16 xOff, s16 yOff, u8
 * w, u8 h} quad sits at the record's `+4`/`+6`/`+8`/`+9` instead of
 * `+0xc`/`+0xe`/`+0x10`/`+0x11`, the same "differently laid out"
 * variance `sub_8007B98`'s doc comment already flags). `self+0x28`
 * bits 4/5 mirror each box horizontally/vertically around its own
 * object's position, exactly like the `actor_part.c` pair. If the two
 * boxes overlap (`sub_8001688`), dispatches to `sub_800EEF0` or
 * `sub_800E7A8` depending on a per-state-id lookup in
 * `gStaticData_0816BBC4`.
 *
 * Early-outs entirely when `self+0x4d & 0x7f == 1`.
 *
 * Written as NAKED asm, not plain C: this is the same AABB-build
 * primitive as the already-parked `sub_8007B98` (see actor_part.c),
 * just inlined twice (once for `self`, once for the player) instead
 * of called as a subroutine, plus the overlap dispatch tail. The ROM
 * keeps exactly two extra callee-saved registers live across both AABB
 * builds (`r8` and `sb`, the latter holding `&gUnknown_030012D8` so the
 * player pointer can be cheaply reloaded after the
 * `sub_803AFE4`/`sub_803AFDC` calls clobber it), and reuses `r7`/`r8`
 * for the X/Y "shift" values across *both* the self-block and the
 * player-block - no C reconstruction tried reproduced that with gcc
 * 2.9 (see docs/matching/issue-12-physics-collision.md for the
 * attempts). Transcribed instruction-for-instruction from the ROM
 * disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c) - `r6` holds
 * `self` throughout, matching the C reconstruction's own local
 * variable layout (stack offsets 0x0-0xc hold `self`'s AABB, 0x10-0x1c
 * the player's). */
NAKED void sub_800D040(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0x20\n\t"
        "add r6, r0, #0\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 1f\n\t"
        "b 7f\n\t"
    "1:\n\t"
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
        "bge 2f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "2:\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "mov r2, r8\n\t"
        "lsl r0, r2, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "3:\n\t"
        "ldr r3, 10f\n\t"
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
        "bge 4f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp, #0x10]\n\t"
        "ldr r2, [sp, #0x18]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x10]\n\t"
    "4:\n\t"
        "mov r2, sb\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 5f\n\t"
        "mov r3, r8\n\t"
        "lsl r0, r3, #1\n\t"
        "ldr r1, [sp, #0x14]\n\t"
        "ldr r2, [sp, #0x1c]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #0x14]\n\t"
    "5:\n\t"
        "add r1, sp, #0x10\n\t"
        "mov r0, sp\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "ldr r0, 11f\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x4e\n\t"
        "ldrb r1, [r1]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #1\n\t"
        "bne 6f\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800EEF0\n\t"
        "b 7f\n\t"
        ".align 2, 0\n"
    "10: .4byte gUnknown_030012D8\n"
    "11: .4byte gStaticData_0816BBC4\n"
    "6:\n\t"
        "add r0, r6, #0\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_800E7A8\n\t"
    "7:\n\t"
        "add sp, #0x20\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
