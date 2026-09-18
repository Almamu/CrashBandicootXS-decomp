#include "core.h"

/* Sits right after actor_part19g.c's `sub_802C6C0` and before
 * actor_part19d.c's `sub_802C904` - directly adjacent to both now,
 * closing the raw gap issue #53 tracked. Same `self` object and
 * conventions documented in actor_part19g.c/actor_part74.c: the
 * 12-byte `{s16 x, y, z, sizeX, sizeY, sizeZ}` AABB record (per
 * docs/matching/issue-54-actor-d3a8.md's "Pinning down the 12-byte
 * AABB-record layout" section) and the shared "used"-state transition
 * idiom (`+0xc = 0x12`, `+0x10`/`+0x12` anim reset, `+8` accumulator
 * reset). */

extern void *gUnknown_03000884;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *sub_800014C(void *dst, const void *src, u32 byteCount);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_8022FEC(void *self);

/* Called from `sub_802C6C0` (actor_part19g.c) once `self` (a "used"
 * pickup, state `0x12`) has stayed used for `self+0x44 == 0x14`
 * frames: walks the whole `self+0x4c`-rooted circular actor list
 * (rooted at `gUnknown_03000884`, the same sentinel-head list every
 * other `self+0x4c`/`self+0x48` teardown/unlink helper in this ROM
 * region walks - `sub_802AA4C`/`sub_802C19C`/`sub_802C394`) looking
 * for every OTHER actor whose type byte (`*(u8*)(*(u8**)(node+0x30))`,
 * the same type-byte indirection `sub_802C540` dispatches on) is `4`
 * and that overlaps `self`'s own translated `self+0x38` AABB (both
 * boxes translated into world space by each object's own `+0x1c`/
 * `+0x20`/`+0x24` `>>8` position, exactly like `sub_802D7B0`/
 * `sub_802DD9C`'s player-overlap test) - a proximity "chain pickup"
 * that fires the shared used-state transition (sound cue `PlaySfx(...,
 * 4, 0x100)`, lap-counter tie `sub_8022FEC`, `+0x44`/`+0x12`/`+8`
 * cleared, `+0xc = 0x12`, anim base reloaded from the node's own part
 * table `+0xd8`) on every type-4 node found overlapping, skipping
 * `self` itself and any node already in the used state. Every box's
 * `sub_800014C` call is the same confirmed no-op `memcpy(dst, dst,
 * 0xc)` self-copy documented in actor_part74.c - kept byte-faithful,
 * not simplified away.
 *
 * Written as NAKED asm, not plain C: this is the exact same
 * heavy-stack-AABB-plus-register-reuse shape already documented (and
 * already NAKED-parked for the same reason) for `sub_802D7B0`/
 * `sub_802DD9C` in `actor_part74.c`/`75.c` - two 12-byte scratch AABB
 * records built via raw `ldm`/`stm` block copies inside one 0x24-byte
 * frame, plus a genuinely function-lifetime-pinned `r7` holding the
 * second box's scratch address across the whole loop body (including
 * both `sub_800014C` calls) - the same categorical "r7 cannot be
 * pinned in this toolchain" limitation from
 * `matching_decomp_register_pinning` memory point 10 and
 * docs/matching/issue-54-actor-d3a8.md's `sub_802D3A8` writeup. A real
 * C reconstruction (`self` pinned `sb`/`r9`, `node` pinned `r4`, the
 * zero constant pinned `r8`, matching the ROM's own register roles for
 * everything else) got every isolated compile within a handful of
 * register-letter differences of the ROM before this exact r7 wall was
 * hit again; not something more C-level rephrasing was likely to fix,
 * so parked here the same way as its two siblings rather than
 * continuing to chase it. Every instruction below is confirmed
 * byte-identical to the ROM (`expected/code_3.s`), not an inferred
 * control-flow guess. */
NAKED void sub_802C7A8(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #0x24\n\t"
        "mov sb, r0\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r4, [r0, #0x4c]\n\t"
        "add r7, sp, #0x18\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "1:\n\t"
        "ldr r0, [r4, #0x30]\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #4\n\t"
        "beq 2f\n\t"
        "b 7f\n\t"
        "2:\n\t"
        "cmp r4, sb\n\t"
        "bne 3f\n\t"
        "b 7f\n\t"
        "3:\n\t"
        "add r1, sp, #0xc\n\t"
        "mov r0, sb\n\t"
        "add r0, #0x38\n\t"
        "ldm r0!, {r2, r3, r5}\n\t"
        "stm r1!, {r2, r3, r5}\n\t"
        "mov r6, sb\n\t"
        "ldr r0, [r6, #0x1c]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r2, [r6, #0x20]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r1, [r6, #0x24]\n\t"
        "asr r1, r1, #8\n\t"
        "add r5, sp, #0xc\n\t"
        "ldrh r3, [r5]\n\t"
        "add r0, r3, r0\n\t"
        "strh r0, [r5]\n\t"
        "ldrh r0, [r5, #2]\n\t"
        "add r0, r0, r2\n\t"
        "strh r0, [r5, #2]\n\t"
        "ldrh r6, [r5, #4]\n\t"
        "add r1, r6, r1\n\t"
        "strh r1, [r5, #4]\n\t"
        "mov r1, sp\n\t"
        "add r0, r5, #0\n\t"
        "ldm r0!, {r2, r3, r6}\n\t"
        "stm r1!, {r2, r3, r6}\n\t"
        "mov r0, sp\n\t"
        "mov r1, sp\n\t"
        "mov r2, #0xc\n\t"
        "bl sub_800014C\n\t"
        "add r1, sp, #0x18\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x38\n\t"
        "ldm r0!, {r2, r3, r6}\n\t"
        "stm r1!, {r2, r3, r6}\n\t"
        "ldr r0, [r4, #0x1c]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r2, [r4, #0x20]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r1, [r4, #0x24]\n\t"
        "asr r1, r1, #8\n\t"
        "ldrh r3, [r7]\n\t"
        "add r0, r3, r0\n\t"
        "strh r0, [r7]\n\t"
        "ldrh r0, [r7, #2]\n\t"
        "add r0, r0, r2\n\t"
        "strh r0, [r7, #2]\n\t"
        "ldrh r6, [r7, #4]\n\t"
        "add r1, r6, r1\n\t"
        "strh r1, [r7, #4]\n\t"
        "add r1, r5, #0\n\t"
        "add r0, r7, #0\n\t"
        "ldm r0!, {r2, r3, r6}\n\t"
        "stm r1!, {r2, r3, r6}\n\t"
        "add r0, r5, #0\n\t"
        "add r1, r5, #0\n\t"
        "mov r2, #0xc\n\t"
        "bl sub_800014C\n\t"
        "mov r1, sp\n\t"
        "mov r0, #4\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r6, #4\n\t"
        "ldrsh r3, [r5, r6]\n\t"
        "mov r6, #0xa\n\t"
        "ldrsh r0, [r5, r6]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 4f\n\t"
        "mov r6, #0xa\n\t"
        "ldrsh r0, [r1, r6]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 4f\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r6, #2\n\t"
        "ldrsh r3, [r5, r6]\n\t"
        "mov r6, #8\n\t"
        "ldrsh r0, [r5, r6]\n\t"
        "add r0, r3, r0\n\t"
        "cmp r2, r0\n\t"
        "bge 4f\n\t"
        "mov r6, #8\n\t"
        "ldrsh r0, [r1, r6]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "ble 4f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r2, [r1, r0]\n\t"
        "mov r6, #0\n\t"
        "ldrsh r3, [r5, r6]\n\t"
        "mov r0, #6\n\t"
        "ldrsh r5, [r5, r0]\n\t"
        "add r0, r3, r5\n\t"
        "cmp r2, r0\n\t"
        "bge 4f\n\t"
        "mov r5, #6\n\t"
        "ldrsh r0, [r1, r5]\n\t"
        "add r0, r2, r0\n\t"
        "cmp r0, r3\n\t"
        "bgt 5f\n\t"
        "4:\n\t"
        "mov r0, #0\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
        "9: .4byte gUnknown_03000884\n"
        "5:\n\t"
        "mov r0, #1\n\t"
        "6:\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "ldr r0, [r4, #0xc]\n\t"
        "cmp r0, #0x12\n\t"
        "beq 7f\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #4\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8022FEC\n\t"
        "mov r6, r8\n\t"
        "str r6, [r4, #0x44]\n\t"
        "mov r0, #0x12\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "add r0, #0xd8\n\t"
        "ldrh r0, [r0]\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r6, [r4, #0x12]\n\t"
        "str r6, [r4, #8]\n\t"
        "7:\n\t"
        "ldr r4, [r4, #0x4c]\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "cmp r4, r0\n\t"
        "beq 8f\n\t"
        "b 1b\n\t"
        "8:\n\t"
        "add sp, #0x24\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
        "10: .4byte gUnknown_030012BC\n"
        "11: .4byte gUnknown_030012C0\n"
        "12: .4byte gUnknown_03000884\n"
    );
}

asm(".align 2, 0");
