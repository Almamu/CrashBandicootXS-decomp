#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

/* sub_80057E0 + sub_80058C0: mutually address-adjacent, bracketed by
 * the already-matched sub_800570C (settings_menu18.c) before and
 * sub_800599C (settings_menu19.c) after - own object file for the
 * same reason settings_menu20.c documents. See
 * docs/matching/issue-7-0x08004d74-overlay-ui.md. */

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 sub_8026F38(s32 arg0);
extern void sub_800570C(struct pause_screen_results *self);
extern struct icon_pos gStaticData_0816B21C[];
extern void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2);

/* Shows whichever of `icons9c[1..4]` has a matching bit set in
 * `self->field_10`'s flag byte (bits 1/4/8/2 - a different bit set
 * than sub_800570C's, same handle), always shows `icons9c[0]`
 * unconditionally, then draws a fixed "x/28"-shaped fraction readout:
 * first `gStaticData_0816B21C[0]`'s position (offset by -0x14/-4) with
 * `self->field_2f`'s buffer at a fixed slot, then repositions to
 * (0xb4, 0x80) and calls `sub_8005E5C` with `self->field_32`/
 * `self->field_49` (the count/total buffers `sub_8005B80` -
 * src/graphics/settings_menu6.c - already fills for this same icon
 * row).
 *
 * Written as NAKED asm, not plain C: same register-pressure class of
 * difficulty as `sub_8005E5C` (src/graphics/settings_menu16.c) - the ROM
 * keeps `self` in r5 and evolves a single register (r6) through three
 * different offset meanings via incremental arithmetic on its own prior
 * value, needing only r4-r6 total, something no plain-C restructuring
 * reached. Every instruction below is transcribed directly from and
 * checked against the ROM's own disassembly. */
NAKED void sub_80057E0(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, lr}\n\t"
    "add r5, r0, #0\n\t"
    "ldr r1, [r5, #0x10]\n\t"
    "mov r0, #1\n\t"
    "ldrb r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 1f\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xa0\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "1:\n\t"
    "ldr r1, [r5, #0x10]\n\t"
    "mov r0, #4\n\t"
    "ldrb r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 2f\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xa4\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "2:\n\t"
    "ldr r1, [r5, #0x10]\n\t"
    "mov r0, #8\n\t"
    "ldrb r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 3f\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xa8\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "3:\n\t"
    "ldr r1, [r5, #0x10]\n\t"
    "mov r0, #2\n\t"
    "ldrb r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 4f\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0xac\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "4:\n\t"
    "add r0, r5, #0\n\t"
    "add r0, #0x9c\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "ldr r0, 5f\n\t"
    "ldr r4, 6f\n\t"
    "ldr r3, [r4]\n\t"
    "ldr r1, [r0]\n\t"
    "sub r1, #0x14\n\t"
    "ldr r2, [r0, #4]\n\t"
    "sub r2, #4\n\t"
    "mov r6, #0x88\n\t"
    "lsl r6, r6, #1\n\t"
    "add r0, r3, r6\n\t"
    "str r1, [r0]\n\t"
    "mov r1, #0x8a\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r3, r1\n\t"
    "str r2, [r0]\n\t"
    "add r6, #0x20\n\t"
    "add r0, r3, r6\n\t"
    "ldr r2, [r0]\n\t"
    "mov r1, #0x20\n\t"
    "ldrsh r0, [r2, r1]\n\t"
    "add r0, r3, r0\n\t"
    "add r1, r5, #0\n\t"
    "add r1, #0x2f\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r3, [r4]\n\t"
    "mov r1, #0xb4\n\t"
    "mov r2, #0x80\n\t"
    "mov r4, #0x88\n\t"
    "lsl r4, r4, #1\n\t"
    "add r0, r3, r4\n\t"
    "str r1, [r0]\n\t"
    "sub r6, #0x1c\n\t"
    "add r0, r3, r6\n\t"
    "str r2, [r0]\n\t"
    "add r1, r5, #0\n\t"
    "add r1, #0x32\n\t"
    "add r2, r5, #0\n\t"
    "add r2, #0x49\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8005E5C\n\t"
    "pop {r4, r5, r6}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "5: .4byte gStaticData_0816B21C\n"
    "6: .4byte gUnknown_030012DC\n"
    );
}

extern struct icon_pos gStaticData_0816B258[];

/* Same shape as sub_80057E0 above for the `iconsB0[3]` row: hides all
 * three icons unconditionally (no per-bit gating this time), then
 * draws three fixed "x/20"-shaped fraction readouts at
 * `gStaticData_0816B258[2]/[1]/[0]`'s positions (offset -4/+0xe, same
 * pattern as sub_80057E0's single readout) with `self->field_38`/
 * `field_3b`/`field_3e`, then a final one at (0xb4, 0x80) via
 * `sub_8005E5C` with `self->field_35`/`field_4c` (the total/threshold
 * buffers `sub_8005C58` - src/graphics/settings_menu6.c - fills for
 * this row).
 *
 * Written as NAKED asm, not plain C: same register-pressure class of
 * difficulty as `sub_80057E0`/`sub_8005E5C` above - parked the same way,
 * every instruction below is transcribed directly from and checked
 * against the ROM's own disassembly. */
NAKED void sub_80058C0(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "add r7, r0, #0\n\t"
    "add r5, r7, #0\n\t"
    "add r5, #0xb0\n\t"
    "mov r4, #2\n\t"
    "1:\n\t"
    "ldm r5!, {r0}\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "sub r4, #1\n\t"
    "cmp r4, #0\n\t"
    "bge 1b\n\t"
    "ldr r4, 2f\n\t"
    "ldr r6, 3f\n\t"
    "ldr r0, [r6]\n\t"
    "ldr r2, [r4, #0x10]\n\t"
    "sub r2, #4\n\t"
    "ldr r3, [r4, #0x14]\n\t"
    "add r3, #0xe\n\t"
    "mov r5, #0x88\n\t"
    "lsl r5, r5, #1\n\t"
    "add r1, r0, r5\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r5, #0x20\n\t"
    "add r1, r0, r5\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x38\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r0, [r6]\n\t"
    "ldr r2, [r4, #8]\n\t"
    "sub r2, #4\n\t"
    "ldr r3, [r4, #0xc]\n\t"
    "add r3, #0xe\n\t"
    "mov ip, r3\n\t"
    "mov r3, #0x88\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "mov r3, ip\n\t"
    "str r3, [r1]\n\t"
    "add r1, r0, r5\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x3b\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r0, [r6]\n\t"
    "ldr r2, [r4]\n\t"
    "sub r2, #4\n\t"
    "ldr r3, [r4, #4]\n\t"
    "add r3, #0xe\n\t"
    "mov r4, #0x88\n\t"
    "lsl r4, r4, #1\n\t"
    "add r1, r0, r4\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r5, r0, r5\n\t"
    "ldr r2, [r5]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x3e\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "bl sub_803AD80\n\t"
    "ldr r3, [r6]\n\t"
    "mov r1, #0xb4\n\t"
    "mov r2, #0x80\n\t"
    "add r0, r3, r4\n\t"
    "str r1, [r0]\n\t"
    "add r4, #4\n\t"
    "add r0, r3, r4\n\t"
    "str r2, [r0]\n\t"
    "add r1, r7, #0\n\t"
    "add r1, #0x35\n\t"
    "add r2, r7, #0\n\t"
    "add r2, #0x4c\n\t"
    "add r0, r7, #0\n\t"
    "bl sub_8005E5C\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gStaticData_0816B258\n"
    "3: .4byte gUnknown_030012DC\n"
    );
}
