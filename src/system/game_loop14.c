#include "core.h"
#include "actor.h"

/* Spawns via `sub_8025BAC` (using `src`'s Q8 X shifted to tile units as
 * the 4th arg), computes an AABB for both the new part and `src` via
 * the same `sub_8007B98` primitive `actor_part11.c` documents, averages
 * their half-widths plus `margin`, and uses `src`'s `+0x28` mirror bit
 * to place the new part to either side of `src`'s centre on X - Y is a
 * plain `src.y + z` offset. Finally seeds the part's velocity-ish
 * fields (`+0x60`/`+0x48`/`+0x4c`/`+0x50`, the same family
 * `sub_8009F50` zeroes in actor_part8.c) from `z`, negated when the
 * mirror bit is set. The ROM's own call site into `sub_8025BAC` does
 * not visibly set up its `testY`/`mirrorFlag` stack args before the
 * `bl` - they land wherever this function's own `sub sp, #0x28` scratch
 * buffer happens to hold at that point, an implicit stack-reuse
 * coincidence transcribed verbatim below (`str r5, [sp]` / `str r4,
 * [sp, #4]`, matching this function's own already-live locals).
 *
 * NAKED, not plain C: semantics are fully traced against the ROM
 * (register-by-register for the AABB/half-width/mirror-bit dance, and
 * the stack-reuse quirk above), but this is a large function using
 * `r8` across most of its body, and a plain-C reconstruction hits the
 * same family of gcc-2.9 register-allocation/instruction-scheduling
 * gaps already confirmed unfixable on the smaller siblings
 * `sub_8025A64` (game_loop29.c)/`sub_8025D74` (game_loop15.c) -
 * transcribed straight from the confirmed-correct ROM disassembly
 * rather than re-chasing the same wall. See
 * docs/matching/issue-41-game-loop-25894.md. */
NAKED struct actor *sub_8025B0C(void *arg0, void *arg1, void *arg2, s32 margin, s32 z, void *src)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x28\n\t"
        "mov r8, r3\n\t"
        "ldr r7, [sp, #0x44]\n\t"
        "ldr r6, [sp, #0x48]\n\t"
        "ldr r3, [r6]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r5, [r6, #4]\n\t"
        "asr r5, r5, #8\n\t"
        "add r4, r6, #0\n\t"
        "add r4, r4, #0x28\n\t"
        "ldrb r4, [r4]\n\t"
        "lsl r4, r4, #0x1b\n\t"
        "lsr r4, r4, #0x1f\n\t"
        "str r5, [sp]\n\t"
        "str r4, [sp, #4]\n\t"
        "bl sub_8025BAC\n\t"
        "add r5, r0, #0\n\t"
        "add r0, sp, #8\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r4, [sp, #0x10]\n\t"
        "add r0, sp, #0x18\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_8007B98\n\t"
        "ldr r0, [sp, #0x20]\n\t"
        "lsr r1, r4, #0x1f\n\t"
        "add r4, r4, r1\n\t"
        "asr r4, r4, #1\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "add r4, r4, r0\n\t"
        "add r4, r8\n\t"
        "ldr r0, [r5]\n\t"
        "asr r1, r0, #8\n\t"
        "add r3, r5, #0\n\t"
        "add r3, r3, #0x28\n\t"
        "ldrb r2, [r3]\n\t"
        "lsl r0, r2, #0x1b\n\t"
        "add r2, r1, r4\n\t"
        "cmp r0, #0\n\t"
        "bge 1f\n\t"
        "sub r2, r1, r4\n\t"
    "1:\n\t"
        "ldr r0, [r5, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "ldr r1, [sp, #0x40]\n\t"
        "add r0, r0, r1\n\t"
        "lsl r1, r2, #8\n\t"
        "str r1, [r5]\n\t"
        "lsl r0, r0, #8\n\t"
        "str r0, [r5, #4]\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "neg r0, r7\n\t"
        "mov r1, #0x40\n\t"
        "str r0, [r5, #0x60]\n\t"
        "str r0, [r5, #0x48]\n\t"
        "str r1, [r5, #0x4c]\n\t"
        "str r0, [r5, #0x50]\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "mov r0, #0x40\n\t"
        "str r7, [r5, #0x60]\n\t"
        "str r7, [r5, #0x48]\n\t"
        "str r0, [r5, #0x4c]\n\t"
        "str r7, [r5, #0x50]\n\t"
    "3:\n\t"
        "add r0, r5, #0\n\t"
        "add sp, #0x28\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}

/* Clamps `testX`/`testY` into `[0, extent)` using
 * `gUnknown_03001308`'s sub-object `+0x10`/`+0x14` extents (the same
 * "current level dimensions" object `game_loop3.c`/`game_loop5.c`
 * read), spawns via `sub_8009ED0`, tags the mirror bit from
 * `mirrorFlag`, points `+0x20` at `gUnknown_030012D0`'s shared table
 * (`idx*0xc` stride - a different slot layout than `sub_8025A64`'s
 * fixed `0x8d*4`), tags `+0x2d = idx`, builds the OAM/keyframe trio,
 * registers into a `sub_800CCE0`-owned manager's own `+0xc` trampoline
 * record via `sub_803AD80` (storing the manager itself at `+0x44`),
 * clears flags bits 1/2 (`& ~6`), and registers into
 * `gUnknown_030012F0`'s list via `sub_8008E94`.
 *
 * NAKED, not plain C: semantics are fully traced against the ROM, but a
 * plain-C reconstruction hits the same unfixable `& -N`-mask
 * constant-folding gap confirmed on `sub_8025A64` (game_loop29.c) - in
 * fact three separate instances of it here (the `& ~0x11` mirror-bit
 * combine, the `& -0x10 | (result & 0xf)` bitfield combine identical to
 * `sub_8025A64`'s, and the final `& -5 & -3` flags clear) - so
 * transcribed straight from the confirmed-correct ROM disassembly
 * rather than re-chasing the same wall three more times. See
 * docs/matching/issue-41-game-loop-25894.md. */
NAKED struct actor *sub_8025BAC(void *unused0, s32 x, s32 idx, s32 testX, s32 testY, u8 mirrorFlag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r5, r1, #0\n\t"
        "add r7, r2, #0\n\t"
        "ldr r2, [sp, #0x14]\n\t"
        "ldr r6, [sp, #0x18]\n\t"
        "cmp r3, #0\n\t"
        "bge 1f\n\t"
        "mov r3, #0\n\t"
    "1:\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r0, #0x10]\n\t"
        "ldr r0, [r1, #0x10]\n\t"
        "lsl r4, r0, #8\n\t"
        "asr r0, r4, #8\n\t"
        "cmp r3, r0\n\t"
        "blt 2f\n\t"
        "lsr r0, r4, #8\n\t"
        "sub r3, r0, #1\n\t"
    "2:\n\t"
        "cmp r2, #0\n\t"
        "bge 3f\n\t"
        "mov r2, #0\n\t"
    "3:\n\t"
        "ldr r0, [r1, #0x14]\n\t"
        "lsl r4, r0, #8\n\t"
        "asr r0, r4, #8\n\t"
        "cmp r2, r0\n\t"
        "blt 4f\n\t"
        "lsr r0, r4, #8\n\t"
        "sub r2, r0, #1\n\t"
    "4:\n\t"
        "ldr r0, 8f\n\t"
        "lsl r1, r3, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r3, #0\n\t"
        "bl sub_8009ED0\n\t"
        "add r4, r0, #0\n\t"
        "neg r1, r6\n\t"
        "orr r1, r6\n\t"
        "add r2, r4, #0\n\t"
        "add r2, r2, #0x28\n\t"
        "lsr r1, r1, #0x1f\n\t"
        "lsl r1, r1, #4\n\t"
        "mov r0, #0x11\n\t"
        "neg r0, r0\n\t"
        "ldrb r3, [r2]\n\t"
        "and r0, r3\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r1, r5, #1\n\t"
        "add r1, r1, r5\n\t"
        "lsl r1, r1, #2\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r4, #0x20]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, r0, #0x2d\n\t"
        "strb r7, [r0]\n\t"
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
        "mov r0, #0x10\n\t"
        "bl sub_8026EDC\n\t"
        "bl sub_800CCE0\n\t"
        "str r0, [r4, #0x44]\n\t"
        "ldr r2, [r0, #0xc]\n\t"
        "mov r3, #0x18\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r2, [r2, #0x1c]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "mov r0, #5\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "and r0, r1\n\t"
        "mov r1, #3\n\t"
        "neg r1, r1\n\t"
        "and r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8008E94\n\t"
        "add r0, r4, #0\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "7: .4byte gUnknown_03001308\n"
    "8: .4byte 0x0000ffff\n"
    "9: .4byte gUnknown_030012D0\n"
    "10: .4byte gUnknown_030012F0\n"
    );
}

/* Same early-out and `+0x49`/`+0x4a`/`+0x4b` tagging shape as
 * `sub_8025A64`, but spawns via `sub_801173C` with a "special" 4th
 * argument (`0xFFFF` when `p5` is set or `p4 == 0xff`, `0` otherwise)
 * instead of a fixed table slot, and fires `sub_801191C`/
 * `sub_8011870` instead of `sub_80111B8`.
 *
 * NAKED, not plain C: same shape as `sub_8025A64` (game_loop29.c),
 * including the same `flag6`-in-`r7`-across-calls confirmed toolchain
 * bug (an explicit `register T x asm("r7")` pin never makes it into
 * this compiler's own `push`/`pop` list - see `src/graphics/oam_count.c`
 * and the other `asm("r7")` call-outs project-wide) plus the same
 * consecutive-byte-offset (`+0x49`/`+0x4a`/`+0x4b`) address-reuse this
 * compiler won't reproduce from plain field-store C - transcribed
 * straight from the confirmed-correct ROM disassembly. See
 * docs/matching/issue-41-game-loop-25894.md. */
NAKED struct actor *sub_8025CA4(void *unused0, u16 x, u16 y, u8 p3, u8 p4, u8 p5)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r7, r3, #0\n\t"
        "ldr r5, [sp, #0x14]\n\t"
        "add r0, sp, #0x18\n\t"
        "ldrb r6, [r0]\n\t"
        "mov r4, #0\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, #0x8c\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 4f\n\t"
        "cmp r6, #0\n\t"
        "bne 1f\n\t"
        "cmp r5, #0xff\n\t"
        "bne 2f\n\t"
    "1:\n\t"
        "ldr r3, 6f\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "add r0, r3, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "5: .4byte gUnknown_030012C0\n"
    "6: .4byte 0x0000ffff\n"
    "2:\n\t"
        "ldr r0, 11f\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "lsl r2, r2, #0x10\n\t"
        "lsr r2, r2, #0x10\n\t"
        "mov r3, #0\n\t"
    "3:\n\t"
        "bl sub_801173C\n\t"
        "add r4, r0, #0\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, r1, #0x49\n\t"
        "mov r0, #0\n\t"
        "strb r7, [r1]\n\t"
        "add r1, r1, #1\n\t"
        "strb r5, [r1]\n\t"
        "add r1, r1, #1\n\t"
        "strb r0, [r1]\n\t"
        "cmp r5, #0xff\n\t"
        "bne 9f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801191C\n\t"
    "9:\n\t"
        "cmp r6, #0\n\t"
        "beq 4f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8011870\n\t"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "11: .4byte 0x0000ffff\n"
    );
}
asm(".align 2, 0");

/* Loads a `{tableIdx:u16, p1:u16, p2:u16, p3:u16}` record from `rec`,
 * indexes `*table` by `tableIdx` (4-byte stride) to get a function
 * pointer, and tail-calls it as `fn(self, p1, p2, p3)`. On real
 * hardware this indirect call has to go through one of this ROM's
 * fixed per-register interworking trampolines
 * (`src/system/reg_trampolines.c`) rather than a direct `blx` - which
 * specific trampoline (here, `sub_803AD8C`/"bx r5") depends purely on
 * which register this compiler's allocator happens to land the
 * function pointer in, hence the `register ... asm("r5")` pin plus the
 * empty-asm "keep this value live" barrier right before the call. */
extern void sub_803AD8C(void *a0, u16 a1, u16 a2, u16 a3);

void sub_8025D28(void **table, void *self, u16 *rec)
{
    register void *tablePtr asm("r1") = *table;
    register u16 idx asm("r3") = rec[0];
    register s32 shifted asm("r0") = idx << 2;
    void *entry = (u8 *)shifted + (s32)tablePtr;
    u16 p1 = rec[1];
    u16 p2 = rec[2];
    u16 p3 = rec[3];
    register void *fn asm("r5") = *(void **)entry;

    asm("" :: "r"(fn));
    sub_803AD8C(self, p1, p2, p3);
}

/* Stores `{a, b}` into the two Q8 words at `self+0`/`self+4`. */
void sub_8025D4C(void *self, s32 a, s32 b)
{
    *(s32 *)((u8 *)self + 4) = b;
    *(s32 *)self = a;
}

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - identical
 * body to `sub_8025A44` above (a second copy at a different ROM
 * address, same as `sub_8025A5C`/`sub_8025D6C` below). */
void sub_8025D54(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Zeroes the two Q8 position words at `self+0`/`self+4` - identical
 * body to `sub_8025A5C` above. */
void sub_8025D6C(void *self)
{
    *(s32 *)self = 0;
    *(s32 *)((u8 *)self + 4) = 0;
}
