#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. Confirmed by
 * docs/rom_map.md as a `category_vtable` slot (`gStaticData_081756C4`,
 * type 1, slot 6) - part of this actor's per-frame dispatch table.
 *
 * Zero-fills one 0x40-byte (8bpp) tile right before BG char block 3
 * (`BG_CHAR_ADDR(3) - 0x40`..`BG_CHAR_ADDR(3)`, a blank/transparent
 * filler tile), then DMA3-fills a 0xffff halfword into BG char block 3
 * itself and runs `sub_8031604`'s VRAM fill-level meter generator (see
 * docs/rom_map.md's "procedurally-generated VRAM fill-level meter"
 * finding). While the small tracker object (`gUnknown_03001534`)'s
 * state (`gUnknown_03001538`) is non-zero: forces a BG2CNT preset
 * toggle (via `gUnknown_03001524`/`gUnknown_03001520` and
 * `sub_80312C4`), looks up a keyframe-table tilemap pointer through the
 * tracker object's own table-index (`+0xc`) and accumulator (`+8`,
 * `>>8`) fields and blits it via `sub_8030D48` (docs/rom_map.md's
 * confirmed "rectangular BG-tilemap blit routine"), sets DISPCNT's
 * bit10 (the same window/mosaic-family bit `sub_8030C98` clears), and
 * DMAs a 0x10-halfword palette strip from `gStaticData_08167AD4` into
 * BG palette bank 1 (`0x05000020`). Once there, one of two mutually
 * exclusive tails run based on the tracker's state: state 5 mirrors
 * palette index 8/0/0xf (slots `+0x10`/`+8`/`+2`/`+0x1e`) all down to
 * black; state 4 clears individual palette slots (`+0x1e`/`+2`/`+8`/
 * `+0x10`) as `gUnknown_0300153C` (a frame/flags counter) crosses four
 * successive thresholds (9, 0x31, 0x4f, 0x6d) - a fade/flash-out
 * sequence for the effect's palette strip.
 *
 * Semantics are fully understood (every load/store, branch and call
 * accounted for above), but transcribed as NAKED asm: the ROM's own
 * build keeps three values alive across the two intervening calls
 * (`sub_8030D48`/`sub_80312C4`) in callee-saved registers -
 * `&gUnknown_03001538` in `r7`, the DMA3 register base in `r5`, and a
 * reusable zero constant in `r6`. A plain-C reconstruction pinning
 * `&gUnknown_03001538` to `r7` via `register s32 *p asm("r7")` hits
 * this project's documented r7 gcc-2.9 hazard in a new, more dangerous
 * form here: this compiler decides r7 is "free" between the two
 * dereferences (nothing else in the C source visibly still needs it)
 * and silently reuses it as scratch space for an unrelated `= 0`
 * assignment in between, then reloads from that now-clobbered register
 * for the second dereference - a genuine correctness bug, not just a
 * byte mismatch, so this was abandoned immediately rather than risk a
 * silently-wrong "match" (see docs/workflow.md step 3's warning about
 * treating isolated compiles as proof, and the r7 hazard entries this
 * project has hit repeatedly - `sub_802C208` et al., actor_part19.c). */
extern s32 gUnknown_03001538;
extern u8 gUnknown_03001524;
extern s32 gUnknown_03001520;
extern void *gUnknown_03001534;
extern s32 gUnknown_0300153C;
extern u8 gStaticData_08167AD4[];

extern void sub_8031604(void);
extern void sub_8030D48(void *arg0);
extern void sub_80312C4(void);

NAKED void sub_8031504(void)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #4\n\t"
        "ldr r1, 1f\n\t"
        "mov r2, #0\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x3c\n\t"
    "2:\n\t"
        "str r2, [r0]\n\t"
        "sub r0, #4\n\t"
        "cmp r0, r1\n\t"
        "bge 2b\n\t"
        "mov r1, sp\n\t"
        "ldr r2, 3f\n\t"
        "add r0, r2, #0\n\t"
        "strh r0, [r1]\n\t"
        "ldr r5, 4f\n\t"
        "str r1, [r5]\n\t"
        "ldr r0, 5f\n\t"
        "str r0, [r5, #4]\n\t"
        "ldr r0, 6f\n\t"
        "str r0, [r5, #8]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "bl sub_8031604\n\t"
        "ldr r7, 7f\n\t"
        "ldr r0, [r7]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r1, 9f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, 10f\n\t"
        "mov r6, #0\n\t"
        "str r6, [r0]\n\t"
        "ldr r0, 11f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r3, [r2, #8]\n\t"
        "asr r3, r3, #8\n\t"
        "ldr r1, [r2, #0xc]\n\t"
        "ldr r4, [r2]\n\t"
        "lsl r0, r1, #1\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r4\n\t"
        "mov r1, #2\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r0, r3\n\t"
        "ldr r1, [r2, #4]\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8030D48\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #0x13\n\t"
        "ldrh r0, [r2]\n\t"
        "mov r3, #0x80\n\t"
        "lsl r3, r3, #3\n\t"
        "add r1, r3, #0\n\t"
        "orr r0, r1\n\t"
        "strh r0, [r2]\n\t"
        "bl sub_80312C4\n\t"
        "ldr r1, 12f\n\t"
        "ldr r0, 13f\n\t"
        "str r0, [r5]\n\t"
        "str r1, [r5, #4]\n\t"
        "ldr r0, 14f\n\t"
        "str r0, [r5, #8]\n\t"
        "ldr r0, [r5, #8]\n\t"
        "ldr r0, [r7]\n\t"
        "cmp r0, #5\n\t"
        "bne 15f\n\t"
        "strh r6, [r1, #0x10]\n\t"
        "ldrh r0, [r1, #0x10]\n\t"
        "strh r0, [r1, #8]\n\t"
        "ldrh r0, [r1, #8]\n\t"
        "strh r0, [r1, #2]\n\t"
        "ldrh r0, [r1, #2]\n\t"
        "strh r0, [r1, #0x1e]\n\t"
        "b 8f\n\t"
        ".align 2, 0\n"
    "1: .4byte 0x0600BFC0\n"
    "3: .4byte 0x0000FFFF\n"
    "4: .4byte 0x040000D4\n"
    "5: .4byte 0x0600C000\n"
    "6: .4byte 0x81000800\n"
    "7: .4byte gUnknown_03001538\n"
    "9: .4byte gUnknown_03001524\n"
    "10: .4byte gUnknown_03001520\n"
    "11: .4byte gUnknown_03001534\n"
    "12: .4byte 0x05000020\n"
    "13: .4byte gStaticData_08167AD4\n"
    "14: .4byte 0x80000010\n"
    "15:\n\t"
        "cmp r0, #4\n\t"
        "bne 8f\n\t"
        "ldr r2, 16f\n\t"
        "ldr r0, [r2]\n\t"
        "cmp r0, #9\n\t"
        "bls 17f\n\t"
        "strh r6, [r1, #0x1e]\n\t"
    "17:\n\t"
        "ldr r0, [r2]\n\t"
        "cmp r0, #0x31\n\t"
        "bls 18f\n\t"
        "strh r6, [r1, #2]\n\t"
    "18:\n\t"
        "cmp r0, #0x4f\n\t"
        "bls 19f\n\t"
        "strh r6, [r1, #8]\n\t"
    "19:\n\t"
        "cmp r0, #0x6d\n\t"
        "bls 8f\n\t"
        "strh r6, [r1, #0x10]\n\t"
    "8:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "16: .4byte gUnknown_0300153C\n"
    );
}
