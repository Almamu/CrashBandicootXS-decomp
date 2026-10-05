#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C8F8`/
 * `UpdateEnemyBob`/`sub_800C97C`, a family of three "sine-wave
 * oscillator" writers sharing the same 256-entry sine-ish table
 * `gSineTable` (already established elsewhere in this ROM,
 * `src/graphics/actor_part72.c`/`actor_part111.c`) and the global
 * frame counter `gRoomFrameCount`. All three read `owner`
 * (`self+0x70`) and write a single Q8.8 coordinate on it, derived as
 * `base + table[idx & 0xff] * self->0x44` (`self->0x44` acting as an
 * oscillation amplitude) - only the axis written, the phase-index
 * derivation, and the base field differ:
 *
 * - `sub_800C8F8`: X axis (`owner+0`), base `self->0x60`, phase index
 *   via `__udivsi3(gRoomFrameCount << 8, self->0x3c) -
 *   (self->0x40 - 0x100)`.
 * - `UpdateEnemyBob`: Y axis (`owner+4`), base `self->0x64`, phase index
 *   via `(gRoomFrameCount >> 1) - (self->0x40 - 0x100)` - *no*
 *   `__udivsi3` call, a plain half-rate frame-counter phase
 *   instead.
 * - `sub_800C97C`: Y axis (`owner+4`), base `self->0x64`, same
 *   `__udivsi3`-based phase index as `sub_800C8F8`.
 *
 * `__udivsi3` (already matched, `src/util/time_util.c`/
 * `src/graphics/hud_icon_widget5.c`) is `s32 __udivsi3(s32 value,
 * s32 divisor)` elsewhere - here it's re-used with `self->0x3c` as
 * the "divisor" slot, most plausibly for some angle/period-wrapping
 * role given the caller context, not resolved further in this pass.
 *
 * Originally parked as NAKED transcriptions: all three are straight-line (no
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
 * above and pinning registers directly. `UpdateEnemyBob` additionally
 * uses a 4-register push (`r4-r6`) though only 3 registers hold live
 * values in its own body - presumably 8-byte stack-alignment padding
 * this compiler build doesn't reproduce for a call-free leaf. Given
 * these are small (60-76B), single-purpose, and already fully
 * understood semantically (see above), transcribed instruction-for-
 * instruction from the ROM disassembly instead. Confirmed byte-
 * identical to `baserom.gba` at `0x0800C8F8`-`0x0800C9C8` (208 bytes,
 * all three functions) via the isolated cpp/agbcc/as + objcopy/cmp
 * pipeline (only `bl __udivsi3` and literal-pool relocation sites
 * differ) plus a full clean `rm -rf build && make NON_MATCHING=1
 * report` (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`). */
/* sub_800C8F8 is real C (issue #9-#11 NAKED retry): the product goes
 * into a fresh `v` pinned to r2 (the ROM's `mov r2, r1; mul r2, r0`),
 * which leaves r1 for the target. The phase bias goes through an inline
 * parameter (Wave) to keep the ROM's `phase + 0xFFFFFF00` literal
 * instead of a folded `+ 0x100`.
 */
#include "part_ctrl.h"

extern s16 gSineTable[];
extern u32 gRoomFrameCount;
extern s32 __udivsi3(s32 value, s32 divisor);

static inline s16 Wave(s16 *table, s32 t, s32 phase)
{
    return table[(t - phase) & 0xff];
}

void sub_800C8F8(struct part_ctrl *self)
{
    s16 *table = gSineTable;
    s32 t = __udivsi3(gRoomFrameCount << 8, self->period);
    register s32 v asm("r2");
    s32 w;
    struct ctrl_target *target;

    w = Wave(table, t, self->phase - 0x100);
    v = w * self->amplitude;
    target = self->target;
    target->x = self->baseX + v;
}

/* UpdateEnemyBob and sub_800C97C are real C (issue #10 retry). The ROM
 * saves a callee-saved register neither body uses (r5 in C940, r8 in
 * C97C). -fprologue-bugfix is not the cause: agbcc with or without it
 * and old_agbcc all emit the same code for these. What reproduces it is
 * an empty asm clobbering that register, which marks it live without
 * emitting code. The other pieces:
 * - C940: `target` pinned to r3 and the -0x100 bias created in r6 through
 *   a constant-init asm (brief item 10), which keeps it from being folded
 *   and loaded early.
 * - C97C: `table` pinned to r6, which puts `target` in r5 as in the ROM. */
void UpdateEnemyBob(struct part_ctrl *self)
{
    register struct ctrl_target *target asm("r3") = self->target;
    s16 *table = gSineTable;
    u32 t;
    s32 ph;
    register s32 k asm("r6");

    /* Empty: marks r5 as used so the prologue saves it, as in the ROM. */
    asm("" : : : "r5");
    t = gRoomFrameCount >> 1;
    ph = self->phase;
    /* Emits only the `ldr r6, =0xFFFFFF00`; see above. */
    asm("" : "=r"(k) : "0"(-0x100));
    target->y = self->baseY + Wave(table, t, ph + k) * self->amplitude;
}

void sub_800C97C(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;
    register s16 *table asm("r6") = gSineTable;
    s32 t = __udivsi3(gRoomFrameCount << 8, self->period);

    /* Empty: marks r8 as used so the prologue saves it, as in the ROM. */
    asm("" : : : "r8");
    target->y = self->baseY + Wave(table, t, self->phase - 0x100) * self->amplitude;
}

/* `UpdateEnemyCtrl` state 18's floating-popup spawner
 * (`sub_800C9C8(0x1D, 0, 0, 0x2B, 0, owner)`, per the Phase 1 doc) -
 * a thin wrapper around the already-matched `sub_8025B0C`
 * (`src/system/game_loop14.c`, the AABB-aware "spawn part near src"
 * primitive): forwards all six arguments (`gEntitySpawner` as the
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
extern void *gEntitySpawner;
extern void *sub_8025B0C(void *pool, s32 a, s32 b, s32 c, s32 d, s32 e, void *f);

void *sub_800C9C8(s32 a, s32 b, s32 c, s32 d, s32 e, void *f)
{
    u8 *obj;
    s32 flags;
    s32 mask;

    obj = sub_8025B0C(gEntitySpawner, a, b, c, d, e, f);
    flags = 4;
    flags |= obj[0xc];
    mask = -0x41;
    obj[0xc] = flags & mask;
    return obj;
}
