#include "core.h"
#include "actor.h"

extern void *gUnknown_030012C0;
extern void *gUnknown_030012D0;

extern struct actor *sub_8011114(u16 arg0, u16 arg1, u16 arg2, s32 arg3);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern s32 sub_800815C(struct actor *part);
extern void sub_80111B8(void *part);

/* Spawns a part-object via `sub_8011114` at `(x,y)` when
 * `gUnknown_030012C0+0x8c` is clear (returns NULL otherwise), tags its
 * `+0x49`/`+0x4a`/`+0x4b` bytes from `p3`/`p5`/the (always-0, since
 * only reached on that branch) state flag, points `+0x20` at
 * `gUnknown_030012D0`'s shared resource table (fixed slot `0x8d`,
 * 4-byte stride), tags `+0x2d = 0xa`, builds the OAM/keyframe trio and
 * `+0x29` bitfield the same way `trigger_effect.c`'s
 * `sub_8020E84`-family functions do, and optionally fires
 * `sub_80111B8` when `flag6` is set.
 *
 * NAKED, not plain C: a real C reconstruction (declaring `x`/`y`/`p3`/
 * `p5`/`flag6` as plain untruncated words, so their narrowing happens
 * at the `sub_8011114`/field-store call sites instead of the prologue)
 * gets every field offset, call argument, and instruction *content*
 * right, but hits two independent, already-documented gcc-2.9 codegen
 * limits at once: (1) `flag6` has to stay live in `r7` across every
 * intervening call the same way the ROM keeps it there, and an
 * explicit `register T x asm("r7")` pin is a confirmed bug in this
 * toolchain that never makes it into the function's own `push`/`pop`
 * list (see `src/graphics/oam_count.c`, `src/graphics/actor_part.c`,
 * and the other `asm("r7")` call-outs project-wide); and (2) the
 * trailing `(*bf & -0x10) | (result & 0xf)` bitfield combine - even
 * with the established negative-literal register-pin idiom
 * (`sub_8023168`/`sub_80374D0` in docs/matching.md) - gets
 * constant-folded into a cheaper derived `sub`, one instruction
 * shorter than the ROM's genuine two-instruction `movs r1,#0x10` /
 * `rsbs r1,r1,#0` pair, the same unfixable value-propagation gap
 * already recorded for `sub_8025D74`/`sub_8001524`. Transcribed
 * straight from the confirmed-correct ROM disassembly. */
NAKED struct actor *sub_8025A64(void *unused0, u16 x, u16 y, u8 p3, u32 p5, u8 flag6)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r6, r3, #0\n\t"
        "add r0, sp, #0x18\n\t"
        "ldrb r7, [r0]\n\t"
        "mov r4, #0\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, #0x8c\n\t"
        "ldrb r5, [r0]\n\t"
        "cmp r5, #0\n\t"
        "bne 1f\n\t"
        "ldr r0, 3f\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r3, #0\n\t"
        "bl sub_8011114\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x49\n\t"
        "strb r6, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, r1, #0x4a\n\t"
        "ldr r0, [sp, #0x14]\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x4b\n\t"
        "strb r5, [r0]\n\t"
        "ldr r0, 4f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #0x8d\n\t"
        "lsl r3, r3, #2\n\t"
        "add r0, r0, r3\n\t"
        "str r0, [r4, #0x20]\n\t"
        "mov r0, #0xa\n\t"
        "sub r1, #0x1d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800815C\n\t"
        "add r2, r4, #0\n\t"
        "add r2, r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "cmp r7, #0\n\t"
        "beq 1f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80111B8\n\t"
    "1:\n\t"
        "add r0, r4, #0\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_030012C0\n"
    "3: .4byte 0x0000ffff\n"
    "4: .4byte gUnknown_030012D0\n"
    );
}
