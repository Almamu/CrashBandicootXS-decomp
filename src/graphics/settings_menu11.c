#include "core.h"
#include "icon_manager.h"

/* The three functions below (sub_8006124, sub_800619C, sub_80061E8) -
 * plus sub_8006518, src/graphics/settings_menu10.c - are all written as
 * NAKED asm, not plain C: they all hit the same class of gcc-2.9
 * register-allocation difficulty already documented at length for
 * `sub_8006600` (src/graphics/oam_count.c)/`sub_8005AE8` and friends -
 * the loop/self pointer and the icon-manager position-store's mask
 * register never land in the exact scratch register the ROM's own
 * allocator reaches for, no matter how the source is rephrased. Every
 * instruction below is transcribed directly from and checked against
 * the ROM's own disassembly. */

/* Same "results" sub-region self object `settings_menu6.c`'s
 * `struct pause_screen_results` documents (`field_6c`/`field_bc`/
 * `timeBuf` all line up) - the medal-icon-widget's (`sub_8005D44`)
 * companion label draw: formats `self->timeBuf` (already filled in by
 * sub_8005D44) centered on the medal icon via the shared
 * `gUnknown_030012DC` icon manager, using the same fixed
 * `gStaticData_0816B27C` position pair sub_8005D44 itself positions
 * the icon with. */
struct pause_screen_results {
    u8 unused_00[0x6c];
    u8 field_6c;
    u8 unused_6d[0x7c - 0x6d];
    u8 timeBuf[0xc];
    void *field_88;
    u8 unused_8c[0xbc - 0x8c];
    void *field_bc;
};

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern struct icon_manager *gUnknown_030012DC;

/* A fixed {x, y} screen-position pair, as consumed by sub_803AD80's
 * callers here - same shape settings_menu6.c's own `struct icon_pos`
 * documents (kept as a separate local type per this project's
 * minimal-local-type convention). */
struct icon_pos {
    s32 x;
    s32 y;
};
extern struct icon_pos gStaticData_0816B27C;

NAKED void sub_8006124(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, r6, lr}\n\t"
    "add r6, r0, #0\n\t"
    "add r0, #0x6c\n\t"
    "ldrb r0, [r0]\n\t"
    "cmp r0, #0\n\t"
    "beq 1f\n\t"
    "add r0, r6, #0\n\t"
    "add r0, #0xbc\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "1:\n\t"
    "ldr r5, 2f\n\t"
    "ldr r0, [r5]\n\t"
    "mov r4, #0x98\n\t"
    "lsl r4, r4, #1\n\t"
    "add r1, r0, r4\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x10\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "add r6, #0x7c\n\t"
    "ldr r2, [r2, #0x14]\n\t"
    "add r1, r6, #0\n\t"
    "bl sub_803AD80\n\t"
    "ldr r1, 3f\n\t"
    "lsr r0, r0, #1\n\t"
    "ldr r2, [r1]\n\t"
    "sub r2, r2, r0\n\t"
    "ldr r0, [r5]\n\t"
    "sub r2, #2\n\t"
    "ldr r3, [r1, #4]\n\t"
    "sub r3, #0x23\n\t"
    "mov r5, #0x88\n\t"
    "lsl r5, r5, #1\n\t"
    "add r1, r0, r5\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r4, r0, r4\n\t"
    "ldr r2, [r4]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "add r1, r6, #0\n\t"
    "bl sub_803AD80\n\t"
    "pop {r4, r5, r6}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "2: .4byte gUnknown_030012DC\n"
    "3: .4byte gStaticData_0816B27C\n"
    );
}

/* Same self object, `sub_8005A78`'s (the `field_88` icon widget)
 * companion label draw - the "results count" pair (`buf2c`/`buf46`,
 * already formatted by `sub_8005A78` itself) centered on that icon at
 * the fixed `gStaticData_0816B1E4` position, via `sub_8005E5C`
 * (src/graphics/settings_menu16.c) that actually draws the two small
 * strings. */
extern struct icon_pos gStaticData_0816B1E4;
extern void sub_8005E5C(struct pause_screen_results *self, void *buf1, void *buf2);

NAKED void sub_800619C(struct pause_screen_results *self)
{
    asm(
    "push {r4, r5, lr}\n\t"
    "add r4, r0, #0\n\t"
    "add r0, #0x88\n\t"
    "ldr r0, [r0]\n\t"
    "mov r1, #0\n\t"
    "mov r2, #0\n\t"
    "bl sub_8008890\n\t"
    "ldr r2, 1f\n\t"
    "ldr r0, 2f\n\t"
    "ldr r3, [r0]\n\t"
    "ldr r1, [r2]\n\t"
    "sub r1, #0x2c\n\t"
    "ldr r2, [r2, #4]\n\t"
    "sub r2, #8\n\t"
    "mov r5, #0x88\n\t"
    "lsl r5, r5, #1\n\t"
    "add r0, r3, r5\n\t"
    "str r1, [r0]\n\t"
    "mov r1, #0x8a\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r3, r1\n\t"
    "str r2, [r0]\n\t"
    "add r1, r4, #0\n\t"
    "add r1, #0x2c\n\t"
    "add r2, r4, #0\n\t"
    "add r2, #0x46\n\t"
    "add r0, r4, #0\n\t"
    "bl sub_8005E5C\n\t"
    "pop {r4, r5}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "1: .4byte gStaticData_0816B1E4\n"
    "2: .4byte gUnknown_030012DC\n"
    );
}

/* A different, still-unreconciled self object (only `field_24`, a
 * plain `s32` category index, is touched here) - draws a fixed-position
 * category/header label at (0xc2, 0x2c) via the same icon manager,
 * picking its source character from a lookup table
 * (`gStaticData_0816B1D0[self->field_24]`) fed through `sub_8026F38`
 * (the same "char code -> something sub_803AD80 can draw" conversion
 * `sub_8006600`/`sub_8005A78` already use for fixed digits like
 * `0x2e`/`0x14`). */
struct pause_screen_category_state {
    u8 unused_00[0x24];
    s32 field_24;
};

extern s32 sub_8026F38(s32 arg0);
extern void *gStaticData_0816B1D0[];

NAKED void sub_80061E8(struct pause_screen_category_state *self)
{
    asm(
    "push {r4, r5, r6, lr}\n\t"
    "ldr r1, 1f\n\t"
    "ldr r0, [r0, #0x24]\n\t"
    "lsl r0, r0, #2\n\t"
    "add r0, r0, r1\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_8026F38\n\t"
    "add r6, r0, #0\n\t"
    "ldr r5, 2f\n\t"
    "ldr r0, [r5]\n\t"
    "mov r4, #0x98\n\t"
    "lsl r4, r4, #1\n\t"
    "add r1, r0, r4\n\t"
    "ldr r2, [r1]\n\t"
    "mov r3, #0x10\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x14]\n\t"
    "add r1, r6, #0\n\t"
    "bl sub_803AD80\n\t"
    "lsr r0, r0, #1\n\t"
    "mov r2, #0xc2\n\t"
    "sub r2, r2, r0\n\t"
    "ldr r0, [r5]\n\t"
    "mov r3, #0x2c\n\t"
    "mov r5, #0x88\n\t"
    "lsl r5, r5, #1\n\t"
    "add r1, r0, r5\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x8a\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "add r4, r0, r4\n\t"
    "ldr r2, [r4]\n\t"
    "mov r3, #0x20\n\t"
    "ldrsh r1, [r2, r3]\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r2, #0x24]\n\t"
    "add r1, r6, #0\n\t"
    "bl sub_803AD80\n\t"
    "pop {r4, r5, r6}\n\t"
    "pop {r0}\n\t"
    "bx r0\n\t"
    ".align 2, 0\n"
    "1: .4byte gStaticData_0816B1D0\n"
    "2: .4byte gUnknown_030012DC\n"
    );
}
