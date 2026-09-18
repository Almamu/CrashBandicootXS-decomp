#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Per docs/rom_map.md ("a rectangular BG-tilemap blit routine"):
 * streams 16-bit tile-index-pair values from `self`'s own data
 * (`self+0x1c`/`0x1e`, `0x20`, ...) two at a time, adds a per-call bias
 * byte (`gUnknown_03001530`) to each, and packs each pair into one
 * 16-bit VRAM write. Row stride is `0x20` halfwords - a standard
 * 32-tile-wide GBA BG tilemap row; row/column counts (both capped near
 * 32) come from `gUnknown_03001528`/`gUnknown_0300152C`, write base
 * from those plus `gUnknown_03001520`.
 *
 * Semantics are understood at the level above (docs/rom_map.md), but
 * this is transcribed as NAKED asm: the outer/inner loop counters, the
 * running output-row pointer, and the read cursor all stay live in
 * `sl`/`sb`/`r8`/`ip` across the whole nested loop, the same many-
 * high-register allocation gcc-2.9 difficulty documented throughout
 * this project. Mechanical, byte-verified transcription. */
extern s32 gUnknown_03001520;
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern s32 gUnknown_03001530;

NAKED void sub_8030D48(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r5, r0, #0\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x18\n\t"
        "lsl r2, r0, #0xb\n\t"
        "ldr r7, 2f\n\t"
        "ldr r0, [r7]\n\t"
        "mov r6, #0x20\n\t"
        "sub r1, r6, r0\n\t"
        "cmp r1, #0\n\t"
        "bge 3f\n\t"
        "add r1, #3\n\t"
    "3:\n\t"
        "asr r1, r1, #2\n\t"
        "lsl r1, r1, #1\n\t"
        "mov r0, #0xc0\n\t"
        "lsl r0, r0, #0x13\n\t"
        "add r1, r1, r0\n\t"
        "add r1, r2, r1\n\t"
        "ldr r3, 4f\n\t"
        "ldr r4, [r3]\n\t"
        "sub r0, r6, r4\n\t"
        "lsr r2, r0, #0x1f\n\t"
        "add r0, r0, r2\n\t"
        "asr r0, r0, #1\n\t"
        "lsl r0, r0, #5\n\t"
        "add r0, #2\n\t"
        "add r6, r1, r0\n\t"
        "mov r2, #0\n\t"
        "mov sl, r3\n\t"
        "cmp r2, r4\n\t"
        "bge 9f\n\t"
        "mov r8, r7\n\t"
        "ldr r1, 5f\n\t"
        "mov sb, r1\n\t"
    "6:\n\t"
        "mov r4, #0\n\t"
        "mov r0, r8\n\t"
        "ldr r1, [r0]\n\t"
        "lsr r0, r1, #0x1f\n\t"
        "add r1, r1, r0\n\t"
        "asr r1, r1, #1\n\t"
        "mov r0, #0x20\n\t"
        "add r0, r0, r6\n\t"
        "mov ip, r0\n\t"
        "add r7, r2, #1\n\t"
        "cmp r4, r1\n\t"
        "bge 8f\n\t"
        "mov r3, sb\n\t"
        "add r2, r6, #0\n\t"
    "7:\n\t"
        "ldrb r0, [r3]\n\t"
        "ldrh r6, [r5]\n\t"
        "add r1, r6, r0\n\t"
        "lsl r1, r1, #0x10\n\t"
        "lsr r1, r1, #0x10\n\t"
        "add r5, #2\n\t"
        "ldrh r6, [r5]\n\t"
        "add r0, r6, r0\n\t"
        "lsl r0, r0, #0x10\n\t"
        "add r5, #2\n\t"
        "lsr r0, r0, #8\n\t"
        "orr r1, r0\n\t"
        "strh r1, [r2]\n\t"
        "add r2, #2\n\t"
        "add r4, #1\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1]\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "cmp r4, r0\n\t"
        "blt 7b\n\t"
    "8:\n\t"
        "mov r6, ip\n\t"
        "add r2, r7, #0\n\t"
        "mov r1, sl\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r2, r0\n\t"
        "blt 6b\n\t"
    "9:\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001520\n"
    "2: .4byte gUnknown_03001528\n"
    "4: .4byte gUnknown_0300152C\n"
    "5: .4byte gUnknown_03001530\n"
    );
}
