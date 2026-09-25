#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): the last four
 * functions of `asm/code_3_2_17_c6a8.s` - `sub_800C6A8`, the
 * `menu_ui` dialog-widget system's own 18-state `self+0x74` update
 * (called from all 31 confirmed `menu_ui` dispatch-table entries,
 * `docs/rom_map.md`'s "A parallel fork then found a genuine surprise"
 * section) - plus the three small `self+0x70`-relative accessor
 * triples the existing tracking comment already named
 * (`sub_800C860`/`sub_800C87C`/`sub_800C898`).
 *
 * Despite the "menu_ui" framing, `sub_800C6A8` operates on the exact
 * same field-offset conventions as the rest of this cluster's
 * `self`/`owner` object shape (`self+0x70` "owner", `self+0xc`
 * "anchor" record, `self+0x84` per-instance table) and calls the
 * exact same helpers `sub_800B704`/`sub_800B838`/`sub_803AD84`
 * already matched for `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`
 * (`actor_part113.c`) - not merely the same *convention* reused on a
 * different struct, but the *identical* struct/helper set, just
 * driven by dialog-widget vtable entries instead of the physics
 * cluster's own entries. Confirms `docs/rom_map.md`'s "general-purpose
 * stateful-widget convention" reading and sharpens it: `menu_ui`'s
 * widgets are literal instances of the same object type the rest of
 * this ROM neighborhood uses, not merely a structurally-similar
 * sibling.
 *
 * `sub_800C6A8`'s own case bodies never call `sub_800C8AC`/
 * `sub_800C8BC`/`sub_800C8CC` as functions - each case *manually
 * repeats* those three helpers' own instruction sequences inline
 * (confirmed by the `bl` targets: `sub_800B838`/`sub_800B704`
 * directly, never `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`
 * themselves) - so the C reconstruction below mirrors that same
 * inlining rather than calling the wrapper functions, to keep every
 * `bl` target identical to the ROM's own. */

extern void sub_800B704(void *selfArg, void *arg1, s32 index);
extern void sub_800B838(void *selfArg, void *arg1, s32 index);
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);

/* `self+0x74` is the state selector (1-18 valid, same range-check
 * shape as `sub_800B8DC`'s own 18-state machine - see the doc's "same
 * comparison/field shape... a general-purpose 'stateful widget'
 * convention" note). Every case body's own `bl` targets are
 * `sub_800B838`/`sub_800B704`/`sub_803AD84` directly - never
 * `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC` themselves - because each
 * case *manually repeats* those three helpers' own instruction
 * sequences inline rather than calling them (confirmed by the `bl`
 * targets seen in the ROM disassembly). Several case groups (states
 * {1,3,17}, {6,9,10,11}) compile the *exact same* inlined
 * `sub_800C8CC`-equivalent sequence at *different*, unmerged
 * addresses - a real, deliberate ROM shape (evidenced by the jump
 * table's own distinct target addresses for each group), not
 * something a real-C switch reconstruction reproduces safely: this
 * project's own `sub_800C18C`/`sub_800C314` investigations (see
 * `docs/matching/issue-9-10-0x0800b8dc-graphics.md`'s Phase 3 leaves
 * section) already document this exact agbcc build's cross-jump/
 * tail-merging pass collapsing textually-identical case bodies like
 * these into one shared block, which would silently produce
 * ROM-incorrect code here. Combined with the same `self`/`owner`
 * multi-field-liveness register-pressure shape this whole ROM
 * neighborhood's other dispatchers (`sub_800B8DC`/`sub_800BD48`/
 * `sub_800C074`/`sub_800C244`/`sub_800C40C`/`sub_800C5D4`) already
 * establish as resistant, this was transcribed directly as NAKED
 * asm rather than attempting real C. */
NAKED void sub_800C6A8(void *self, s32 state)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r5, r0, #0\n\t"
        "str r1, [r5, #0x74]\n\t"
        "sub r0, r1, #1\n\t"
        "cmp r0, #0x11\n\t"
        "bls 1f\n\t"
        "b 16f\n\t"
    "1:\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, =2f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".pool\n\t"
    "2:\n\t"
        ".4byte 12f\n\t" /* case 0 (state 1) */
        ".4byte 7f\n\t"  /* case 1 (state 2) */
        ".4byte 12f\n\t" /* case 2 (state 3) */
        ".4byte 9f\n\t"  /* case 3 (state 4) */
        ".4byte 4f\n\t"  /* case 4 (state 5) */
        ".4byte 6f\n\t"  /* case 5 (state 6) */
        ".4byte 15f\n\t" /* case 6 (state 7) */
        ".4byte 13f\n\t" /* case 7 (state 8) */
        ".4byte 6f\n\t"  /* case 8 (state 9) */
        ".4byte 6f\n\t"  /* case 9 (state 10) */
        ".4byte 6f\n\t"  /* case 10 (state 11) */
        ".4byte 16f\n\t" /* case 11 (state 12) */
        ".4byte 8f\n\t"  /* case 12 (state 13) */
        ".4byte 9f\n\t"  /* case 13 (state 14) */
        ".4byte 7f\n\t"  /* case 14 (state 15) */
        ".4byte 9f\n\t"  /* case 15 (state 16) */
        ".4byte 12f\n\t" /* case 16 (state 17) */
        ".4byte 8f\n\t"  /* case 17 (state 18) */
    "4:\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "ldr r0, =0xFFFFFE80\n\t"
        "mov r2, #0\n\t"
        "str r0, [r1, #0x60]\n\t"
        "str r0, [r1, #0x48]\n\t"
        "str r2, [r1, #0x4c]\n\t"
        "str r0, [r1, #0x50]\n\t"
        "mov r0, #0x80\n\t"
        "lsl r0, r0, #3\n\t"
        "str r0, [r1, #0x64]\n\t"
        "str r0, [r1, #0x54]\n\t"
        "str r2, [r1, #0x58]\n\t"
        "str r0, [r1, #0x5c]\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r2, [r1, #0xc]\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1, #0xc]\n\t"
        "b 16f\n\t"
        ".pool\n\t"
    "6:\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r4, #0\n\t"
        "ldrsh r0, [r3, r4]\n\t"
        "b 14f\n\t"
    "7:\n\t"
        "mov r0, #1\n\t"
        "str r0, [r5, #0x78]\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r0, r5, #0\n\t"
        "mov r2, #1\n\t"
        "bl sub_800B838\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r3, r1]\n\t"
        "b 14f\n\t"
    "8:\n\t"
        "mov r0, #1\n\t"
        "str r0, [r5, #0x78]\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r0, r5, #0\n\t"
        "mov r2, #1\n\t"
        "bl sub_800B838\n\t"
    "9:\n\t"
        "ldr r1, [r5, #0x38]\n\t"
        "ldr r0, [r5, #0x30]\n\t"
        "cmp r1, r0\n\t"
        "blt 10f\n\t"
        "mov r0, #4\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r3, r2]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x84\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r2, [r2, #0x10]\n\t"
        "ldr r3, [r3, #4]\n\t"
        "bl sub_803AD84\n\t"
        "b 11f\n\t"
    "10:\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r3, r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x84\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r3, [r3, #4]\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r5, #0x6c]\n\t"
        "cmp r0, #0x1b\n\t"
        "bne 16f\n\t"
    "11:\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "add r3, r1, #0\n\t"
        "add r3, #0x2d\n\t"
        "ldr r2, [r0]\n\t"
        "ldrb r4, [r3]\n\t"
        "lsl r0, r4, #3\n\t"
        "sub r0, r0, r4\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "sub r0, #1\n\t"
        "str r0, [r1, #0x30]\n\t"
        "b 16f\n\t"
    "12:\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r3, r1]\n\t"
        "b 14f\n\t"
    "13:\n\t"
        "mov r4, #3\n\t"
        "str r4, [r5, #0x78]\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r0, r5, #0\n\t"
        "mov r2, #3\n\t"
        "bl sub_800B838\n\t"
        "str r4, [r5, #0x7c]\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r0, r5, #0\n\t"
        "mov r2, #3\n\t"
        "bl sub_800B704\n\t"
        "mov r0, #0\n\t"
        "str r0, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r2, #0\n\t"
        "ldrsh r0, [r3, r2]\n\t"
    "14:\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x84\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r3, [r3, #4]\n\t"
        "bl sub_803AD84\n\t"
        "b 16f\n\t"
    "15:\n\t"
        "mov r0, #2\n\t"
        "str r0, [r5, #0x78]\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r0, r5, #0\n\t"
        "mov r2, #2\n\t"
        "bl sub_800B838\n\t"
        "mov r4, #0\n\t"
        "str r4, [r5, #0x68]\n\t"
        "ldr r3, [r5, #0xc]\n\t"
        "add r3, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r3, r1]\n\t"
        "add r0, r5, r0\n\t"
        "ldr r1, [r5, #0x70]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x84\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r2, [r2]\n\t"
        "ldr r3, [r3, #4]\n\t"
        "bl sub_803AD84\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #0x80\n\t"
        "str r4, [r0]\n\t"
    "16:\n\t"
        "ldr r0, [r5, #0x70]\n\t"
        "ldr r1, [r0]\n\t"
        "str r1, [r5, #0x60]\n\t"
        "ldr r0, [r0, #4]\n\t"
        "str r0, [r5, #0x64]\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".pool"
    );
}

/* Sets `self`'s X-axis homing bounds (`self+0x10`/`self+0x14`) to
 * `owner`'s current X position +/- `radius` (Q8.8, `radius << 8`),
 * and caches `p2`/`p3` into `self+0x58`/`self+0x5c`. */
void sub_800C860(void *selfArg, s32 radius, s32 p2, s32 p3)
{
    u8 *self = selfArg;
    u8 *owner = *(u8 **)(self + 0x70);
    register s32 x asm("r4");

    x = *(s32 *)owner;
    asm volatile("" : "+r" (x));
    *(s32 *)(self + 0x10) = x - (radius << 8);

    x = *(s32 *)owner;
    asm volatile("" : "+r" (x));
    *(s32 *)(self + 0x14) = x + (radius << 8);

    *(s32 *)(self + 0x5c) = p3;
    *(s32 *)(self + 0x58) = p2;
}

/* Same shape as `sub_800C860`, Y axis: `self+0x18` (high bound) /
 * `self+0x1c` (low bound) from `owner+4`, plus the same
 * `self+0x58`/`self+0x5c` cache. */
void sub_800C87C(void *selfArg, s32 radius, s32 p2, s32 p3)
{
    u8 *self = selfArg;
    u8 *owner = *(u8 **)(self + 0x70);
    register s32 y asm("r4");

    y = *(s32 *)(owner + 4);
    asm volatile("" : "+r" (y));
    *(s32 *)(self + 0x1c) = y - (radius << 8);

    y = *(s32 *)(owner + 4);
    asm volatile("" : "+r" (y));
    *(s32 *)(self + 0x18) = y + (radius << 8);

    *(s32 *)(self + 0x5c) = p3;
    *(s32 *)(self + 0x58) = p2;
}

/* Same X-axis bounds computation as `sub_800C860`'s first half, no
 * `p2`/`p3` cache. */
void sub_800C898(void *selfArg, s32 radius)
{
    u8 *self = selfArg;
    u8 *owner = *(u8 **)(self + 0x70);
    register s32 x asm("r2");

    x = *(s32 *)owner;
    asm volatile("" : "+r" (x));
    *(s32 *)(self + 0x10) = x - (radius << 8);

    x = *(s32 *)owner;
    asm volatile("" : "+r" (x));
    *(s32 *)(self + 0x14) = x + (radius << 8);
}

asm(".align 2, 0");
