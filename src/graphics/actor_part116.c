#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C8F8`/
 * `sub_800C940`/`sub_800C97C`, a family of three "sine-wave
 * oscillator" writers sharing the same 256-entry sine-ish table
 * `gStaticData_0816A820` (already established elsewhere in this ROM,
 * `src/graphics/actor_part72.c`/`actor_part111.c`) and the global
 * frame counter `gUnknown_0300082C`. All three read `owner`
 * (`self+0x70`) and write a single Q8.8 coordinate on it, derived as
 * `base + table[idx & 0xff] * self->0x44` (`self->0x44` acting as an
 * oscillation amplitude) - only the axis written, the phase-index
 * derivation, and the base field differ:
 *
 * - `sub_800C8F8`: X axis (`owner+0`), base `self->0x60`, phase index
 *   via `sub_8037E54(gUnknown_0300082C << 8, self->0x3c) -
 *   (self->0x40 - 0x100)`.
 * - `sub_800C940`: Y axis (`owner+4`), base `self->0x64`, phase index
 *   via `(gUnknown_0300082C >> 1) - (self->0x40 - 0x100)` - *no*
 *   `sub_8037E54` call, a plain half-rate frame-counter phase
 *   instead.
 * - `sub_800C97C`: Y axis (`owner+4`), base `self->0x64`, same
 *   `sub_8037E54`-based phase index as `sub_800C8F8`.
 *
 * `sub_8037E54` (already matched, `src/util/time_util.c`/
 * `src/graphics/hud_icon_widget5.c`) is `s32 sub_8037E54(s32 value,
 * s32 divisor)` elsewhere - here it's re-used with `self->0x3c` as
 * the "divisor" slot, most plausibly for some angle/period-wrapping
 * role given the caller context, not resolved further in this pass.
 *
 * NAKED transcriptions, not real C: all three are straight-line (no
 * branches) and an isolated real-C attempt got very close - correct
 * fields, correct table lookup, correct final store - but could not
 * reproduce two ROM-specific micro-choices: (1) the ROM's `-0x100`
 * combination is always materialized as a *32-bit* literal-pool
 * constant `0xFFFFFF00` added to `self->0x40` (since 256 doesn't fit
 * Thumb's 8-bit immediate `subs` range), while a natural `... - 0x100`
 * C expression here lets the compiler re-associate the subtraction
 * into a same-value-but-different-encoding form that avoids the extra
 * literal; and (2) `sub_800C8F8`/`sub_800C97C`'s final
 * `tableVal * self->0x44` product needs a `mov`+`muls` register copy
 * to free up a register for `owner`, but which operand gets copied
 * (and to which register) depends on downstream allocator choices an
 * isolated single-function compile couldn't be made to reproduce
 * exactly, even after matching the constant-materialization form
 * above and pinning registers directly. `sub_800C940` additionally
 * uses a 4-register push (`r4-r6`) though only 3 registers hold live
 * values in its own body - presumably 8-byte stack-alignment padding
 * this compiler build doesn't reproduce for a call-free leaf. Given
 * these are small (60-76B), single-purpose, and already fully
 * understood semantically (see above), transcribed instruction-for-
 * instruction from the ROM disassembly instead. Confirmed byte-
 * identical to `baserom.gba` at `0x0800C8F8`-`0x0800C9C8` (208 bytes,
 * all three functions) via the isolated cpp/agbcc/as + objcopy/cmp
 * pipeline (only `bl sub_8037E54` and literal-pool relocation sites
 * differ) plus a full clean `rm -rf build && make NON_MATCHING=1
 * report` (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`). */
NAKED void sub_800C8F8(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r5, =gStaticData_0816A820\n\t"
        "ldr r0, =gUnknown_0300082C\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #0x3c]\n\t"
        "bl sub_8037E54\n\t"
        "ldr r1, [r4, #0x40]\n\t"
        "ldr r2, =0xFFFFFF00\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "mov r1, #0xff\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r5\n\t"
        "mov r2, #0\n\t"
        "ldrsh r1, [r0, r2]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "add r2, r1, #0\n\t"
        "mul r2, r0, r2\n\t"
        "ldr r1, [r4, #0x70]\n\t"
        "ldr r0, [r4, #0x60]\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r1]\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}

/* Y-axis sibling of `sub_800C8F8` above, but *without* the
 * `sub_8037E54` call - see the family doc comment above. */
NAKED void sub_800C940(void *self)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "ldr r3, [r0, #0x70]\n\t"
        "ldr r4, =gStaticData_0816A820\n\t"
        "ldr r1, =gUnknown_0300082C\n\t"
        "ldr r1, [r1]\n\t"
        "lsr r1, r1, #1\n\t"
        "ldr r2, [r0, #0x40]\n\t"
        "ldr r6, =0xFFFFFF00\n\t"
        "add r2, r2, r6\n\t"
        "sub r1, r1, r2\n\t"
        "mov r2, #0xff\n\t"
        "and r1, r2\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r4\n\t"
        "mov r4, #0\n\t"
        "ldrsh r2, [r1, r4]\n\t"
        "ldr r1, [r0, #0x44]\n\t"
        "mul r1, r2, r1\n\t"
        "ldr r0, [r0, #0x64]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r3, #4]\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}

/* Y-axis sibling of `sub_800C8F8` above, *with* the `sub_8037E54`
 * call (same phase derivation as `sub_800C8F8`, but writing owner's Y
 * axis / `self->0x64` base like `sub_800C940`) - see the family doc
 * comment above. */
NAKED void sub_800C97C(void *self)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "mov r6, r8\n\t"
        "push {r6}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r5, [r4, #0x70]\n\t"
        "ldr r6, =gStaticData_0816A820\n\t"
        "ldr r0, =gUnknown_0300082C\n\t"
        "ldr r0, [r0]\n\t"
        "lsl r0, r0, #8\n\t"
        "ldr r1, [r4, #0x3c]\n\t"
        "bl sub_8037E54\n\t"
        "ldr r1, [r4, #0x40]\n\t"
        "ldr r2, =0xFFFFFF00\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "mov r1, #0xff\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r6\n\t"
        "mov r2, #0\n\t"
        "ldrsh r1, [r0, r2]\n\t"
        "ldr r0, [r4, #0x44]\n\t"
        "mul r1, r0, r1\n\t"
        "ldr r0, [r4, #0x64]\n\t"
        "add r0, r0, r1\n\t"
        "str r0, [r5, #4]\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}

/* `sub_800B8DC` state 18's floating-popup spawner
 * (`sub_800C9C8(0x1D, 0, 0, 0x2B, 0, owner)`, per the Phase 1 doc) -
 * a thin wrapper around the already-matched `sub_8025B0C`
 * (`src/system/game_loop14.c`, the AABB-aware "spawn part near src"
 * primitive): forwards all six arguments (`gUnknown_030012E4` as the
 * pool) and, on return, ORs bit 2 into the new object's `+0xc` flags
 * byte while clearing bit 6 (`(obj[0xc] | 4) & ~0x40` - the ROM
 * itself computes the mask as `-0x41`, which is numerically identical
 * to `~0x40` via `NOT(x) = -x-1`).
 *
 * Matched as real C - the OR/mask computation needed its inputs
 * declared as separate top-of-block locals assigned in the ROM's own
 * evaluation order (`flags = 4; flags |= obj[0xc]; mask = -0x41;
 * obj[0xc] = flags & mask;`) to reproduce the exact register roles;
 * a single combined expression let the compiler swap operand load
 * order and pick a cheaper single-instruction positive-immediate
 * mask load instead of the ROM's own `movs #0x41; neg` two-
 * instruction materialization. `sub_8025B0C`'s own already-matched
 * NAKED signature only declares 5 real parameters after the pool
 * pointer (`arg1, arg2, margin, z, src`) - this call site's own sixth
 * argument (`f`) is written to the stack but never read back by the
 * callee (a dead argument at this call site), so this file declares
 * its own wider 6-parameter extern prototype purely to reproduce
 * that harmless extra stack store byte-for-byte. */
extern void *gUnknown_030012E4;
extern void *sub_8025B0C(void *pool, s32 a, s32 b, s32 c, s32 d, s32 e, void *f);

void *sub_800C9C8(s32 a, s32 b, s32 c, s32 d, s32 e, void *f)
{
    u8 *obj;
    s32 flags;
    s32 mask;

    obj = sub_8025B0C(gUnknown_030012E4, a, b, c, d, e, f);
    flags = 4;
    flags |= obj[0xc];
    mask = -0x41;
    obj[0xc] = flags & mask;
    return obj;
}
