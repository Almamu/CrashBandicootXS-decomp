#include "core.h"

/* The same small per-widget object `src/graphics/oam_count.c` already
 * names `struct sub_8006700_actor` (redeclared locally here per this
 * project's minimal-local-type convention for a type already anchored
 * in another translation unit - see e.g. settings_menu6.c's own
 * `struct threshold_table_entry` comment). Steps `field_24`'s low 5
 * bits down to 0 (redrawing/committing every step via
 * sub_8006600/sub_8006714/sub_8006700), then polls input
 * (`sub_80007AC`/`gUnknown_030007E0.pressed`) redrawing every frame
 * until the confirm button is newly pressed, then steps `field_24`
 * back up to 0x10 the same way, and finally forces `field_28` to
 * `0x40` and re-applies.
 *
 * Written as NAKED asm, not plain C: the loop/mask register scaffolding
 * never landed in the exact scratch registers (`r7`/`r8`) the ROM's own
 * allocator reaches for, no matter how the source was rephrased - the
 * same class of gcc-2.9 register-allocation difficulty documented at
 * length for `sub_8006600` (src/graphics/oam_count.c). See
 * docs/matching/issue-8-0x080060ac-overlay-ui.md. Every instruction
 * below is transcribed directly from and checked against the ROM's own
 * disassembly. */
struct sub_8006700_actor {
    u8 unused_00[0x10];
    s32 field_10;
    void *field_14;
    void *field_18;
    u32 field_1c;
    u32 field_20;
    u8 field_24;
    u8 unused_25[3];
    u16 field_28;
};

extern void sub_8006600(struct sub_8006700_actor *arg0);
extern void sub_8006714(struct sub_8006700_actor *arg0);
extern void sub_8006700(struct sub_8006700_actor *arg0);
extern void sub_80007AC(void *arg0);
extern void *gUnknown_03001304;

struct held_pressed_pair {
    u16 held;
    u16 pressed;
};
extern struct held_pressed_pair gUnknown_030007E0;

NAKED void sub_8006518(struct sub_8006700_actor *self)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, r8\n\t"
    "push {r7}\n\t"
    "add r5, r0, #0\n\t"
    "add r6, r5, #0\n\t"
    "add r6, #0x24\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r1, [r6]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 2f\n\t"
    "mov r2, #0x20\n\t"
    "neg r2, r2\n\t"
    "add r7, r2, #0\n\t"
    "1:\n\t"
    "add r4, r6, #0\n\t"
    "ldrb r2, [r6]\n\t"
    "lsl r0, r2, #0x1b\n\t"
    "lsr r0, r0, #0x1b\n\t"
    "sub r0, #1\n\t"
    "mov r1, #0x1f\n\t"
    "and r0, r1\n\t"
    "and r2, r7\n\t"
    "orr r2, r0\n\t"
    "strb r2, [r6]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006600\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006714\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006700\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r4, [r4]\n\t"
    "and r0, r4\n\t"
    "cmp r0, #0\n\t"
    "bne 1b\n\t"
    "2:\n\t"
    "add r4, r5, #0\n\t"
    "add r4, #0x24\n\t"
    "mov r0, #0x28\n\t"
    "add r0, r0, r5\n\t"
    "mov r8, r0\n\t"
    "3:\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006600\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006714\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006700\n\t"
    "ldr r0, 6f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80007AC\n\t"
    "ldr r1, 7f\n\t"
    "mov r0, #8\n\t"
    "ldrh r1, [r1, #2]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0\n\t"
    "beq 3b\n\t"
    "add r6, r4, #0\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r1, [r6]\n\t"
    "and r0, r1\n\t"
    "cmp r0, #0x10\n\t"
    "beq 5f\n\t"
    "mov r2, #0x20\n\t"
    "neg r2, r2\n\t"
    "add r7, r2, #0\n\t"
    "4:\n\t"
    "add r4, r6, #0\n\t"
    "ldrb r2, [r6]\n\t"
    "lsl r0, r2, #0x1b\n\t"
    "lsr r0, r0, #0x1b\n\t"
    "add r0, #1\n\t"
    "mov r1, #0x1f\n\t"
    "and r0, r1\n\t"
    "and r2, r7\n\t"
    "orr r2, r0\n\t"
    "strb r2, [r6]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006600\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006714\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006700\n\t"
    "mov r0, #0x1f\n\t"
    "ldrb r4, [r4]\n\t"
    "and r0, r4\n\t"
    "cmp r0, #0x10\n\t"
    "bne 4b\n\t"
    "5:\n\t"
    "mov r0, #0\n\t"
    "strh r0, [r5, #0x28]\n\t"
    "mov r0, #0x40\n\t"
    "mov r1, r8\n\t"
    "ldrb r1, [r1]\n\t"
    "orr r0, r1\n\t"
    "mov r2, r8\n\t"
    "strb r0, [r2]\n\t"
    "add r0, r5, #0\n\t"
    "bl sub_8006714\n\t"
    "pop {r3}\n\t"
    "mov r8, r3\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "6: .4byte gUnknown_03001304\n"
    "7: .4byte gUnknown_030007E0\n"
    );
}
