#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C18C`/
 * `sub_800C1E8`, the X-axis/Y-axis "homing velocity-target setter"
 * pair the Phase 1 doc's own priority list flagged as the cluster's
 * next likely-real-C win. Both take only `self` and write into
 * `self+0x70` ("owner"): given `owner`'s position on the relevant
 * axis relative to `gUnknown_030012D8`'s own object (the player/
 * camera), and `self`'s own `0x10`/`0x14` (X) or `0x18`/`0x1c` (Y)
 * bounds, picks one of four `{vx, vy}` pairs and writes them into
 * `owner+0x48`/`0x4c`/`0x50` (X) or `owner+0x54`/`0x58`/`0x5c` (Y) -
 * the same "velocity-target triple" convention the Phase 1 doc's
 * field table already documents at those offsets. `self+0x58`/`0x5c`
 * hold the actual homing speed magnitude (X and Y share the same
 * pair - this is a diagonal-speed setting, not two independent
 * speeds), and the middle "near player" band always sets vy to
 * `self->0x5c` unconditionally regardless of axis, which is
 * consistent with velocity-target components for the *other* axis
 * carrying the diagonal-approach rate while this axis's own drift
 * stops.
 *
 * NAKED transcription, not real C: an isolated real-C attempt got
 * the branch *polarity* and all four `{vx, vy}` cases semantically
 * right (confirmed via a side-by-side disassembly diff), but this
 * agbcc build's cross-jump/tail-merging pass keeps unifying the
 * "vx=0, vy=0x10" case's three stores with the shared "near band"
 * tail's own three stores into one block, while the ROM keeps that
 * exact 3-instruction sequence *duplicated* at both of its two call
 * sites (confirmed byte-for-byte identical in the ROM disassembly:
 * `movs r1, #0` / `movs r0, #0x10` / `b _0800C1DA` appears twice,
 * verbatim) - i.e. whatever produced the ROM did *not* cross-jump
 * here, but this toolchain's -O2 does, regardless of `goto`-based
 * restructuring, register-variable pins, or `volatile`-qualified
 * stores (all tried). Not a register-allocation gap this project's
 * usual pin toolbox addresses - a genuine code-layout/tail-merging
 * divergence between this agbcc build and whatever produced the ROM.
 * Transcribed instruction-for-instruction instead, following this
 * project's usual NAKED-transcription conventions (unified-syntax
 * mnemonics to divided/suffix-less form, GNU-as local numeric labels,
 * the ROM's own mid-function `.pool` split reproduced exactly).
 * Confirmed byte-identical to `baserom.gba` at
 * `0x0800C18C`-`0x0800C244` (184 bytes total) via the isolated
 * cpp/agbcc/as + objcopy/cmp pipeline (only `bl`-shaped relocation
 * sites differ - actually neither function makes any `bl` call, so
 * the isolated objects matched with zero relocation differences at
 * all) plus a full clean `rm -rf build && make NON_MATCHING=1 report`
 * (no warnings) and `rm -rf build crashbandicootxs.elf
 * crashbandicootxs.gba crashbandicootxs.map && make compare`
 * (`crashbandicootxs.gba: La suma coincide`). */
NAKED void sub_800C18C(void *self)
{
    asm(
        "push {r4, lr}\n\t"
        "add r3, r0, #0\n\t"
        "ldr r2, [r3, #0x70]\n\t"
        "ldr r4, [r2]\n\t"
        "ldr r0, =gUnknown_030012D8\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0]\n\t"
        "sub r1, r4, r0\n\t"
        "cmp r1, #0x14\n\t"
        "ble 1f\n\t"
        "ldr r0, [r3, #0x10]\n\t"
        "cmp r4, r0\n\t"
        "bge 2f\n\t"
        "mov r1, #0\n\t"
        "mov r0, #0x10\n\t"
        "b 3f\n\t"
        ".pool\n\t"
    "2:\n\t"
        "ldr r0, [r3, #0x58]\n\t"
        "neg r0, r0\n\t"
        "ldr r1, [r3, #0x5c]\n\t"
        "str r0, [r2, #0x48]\n\t"
        "str r1, [r2, #0x4c]\n\t"
        "str r0, [r2, #0x50]\n\t"
        "b 4f\n\t"
    "1:\n\t"
        "mov r0, #0x14\n\t"
        "neg r0, r0\n\t"
        "cmp r1, r0\n\t"
        "bge 5f\n\t"
        "ldr r0, [r3, #0x14]\n\t"
        "cmp r4, r0\n\t"
        "ble 6f\n\t"
        "mov r1, #0\n\t"
        "mov r0, #0x10\n\t"
        "b 3f\n\t"
    "6:\n\t"
        "ldr r1, [r3, #0x58]\n\t"
        "b 7f\n\t"
    "5:\n\t"
        "mov r1, #0\n\t"
    "7:\n\t"
        "ldr r0, [r3, #0x5c]\n\t"
    "3:\n\t"
        "str r1, [r2, #0x48]\n\t"
        "str r0, [r2, #0x4c]\n\t"
        "str r1, [r2, #0x50]\n\t"
    "4:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}

/* Y-axis mirror of `sub_800C18C` above - same shape, `self+0x18`/
 * `0x1c` bounds instead of `0x10`/`0x14`, `owner+4` instead of
 * `owner+0` for the position read, `gUnknown_030012D8`'s own `+4`
 * instead of `+0`, and `owner+0x54`/`0x58`/`0x5c` instead of
 * `owner+0x48`/`0x4c`/`0x50` for the velocity-target triple. Same
 * NAKED-transcription rationale as `sub_800C18C` above (identical
 * cross-jump-merging divergence hit on an isolated real-C attempt).
 * Confirmed byte-identical to `baserom.gba` at
 * `0x0800C1E8`-`0x0800C244` (92 bytes) via the same pipeline, plus
 * the same full clean `make NON_MATCHING=1 report`/`make compare`
 * verification. */
NAKED void sub_800C1E8(void *self)
{
    asm(
        "push {r4, lr}\n\t"
        "add r3, r0, #0\n\t"
        "ldr r2, [r3, #0x70]\n\t"
        "ldr r4, [r2, #4]\n\t"
        "ldr r0, =gUnknown_030012D8\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "sub r1, r4, r0\n\t"
        "cmp r1, #0x14\n\t"
        "ble 1f\n\t"
        "ldr r0, [r3, #0x1c]\n\t"
        "cmp r4, r0\n\t"
        "bge 2f\n\t"
        "mov r1, #0\n\t"
        "mov r0, #0x10\n\t"
        "b 3f\n\t"
        ".pool\n\t"
    "2:\n\t"
        "ldr r0, [r3, #0x58]\n\t"
        "neg r0, r0\n\t"
        "ldr r1, [r3, #0x5c]\n\t"
        "str r0, [r2, #0x54]\n\t"
        "str r1, [r2, #0x58]\n\t"
        "str r0, [r2, #0x5c]\n\t"
        "b 4f\n\t"
    "1:\n\t"
        "mov r0, #0x14\n\t"
        "neg r0, r0\n\t"
        "cmp r1, r0\n\t"
        "bge 5f\n\t"
        "ldr r0, [r3, #0x18]\n\t"
        "cmp r4, r0\n\t"
        "ble 6f\n\t"
        "mov r1, #0\n\t"
        "mov r0, #0x10\n\t"
        "b 3f\n\t"
    "6:\n\t"
        "ldr r1, [r3, #0x58]\n\t"
        "b 7f\n\t"
    "5:\n\t"
        "mov r1, #0\n\t"
    "7:\n\t"
        "ldr r0, [r3, #0x5c]\n\t"
    "3:\n\t"
        "str r1, [r2, #0x54]\n\t"
        "str r0, [r2, #0x58]\n\t"
        "str r1, [r2, #0x5c]\n\t"
    "4:\n\t"
        "pop {r4}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
asm(".align 2, 0");
