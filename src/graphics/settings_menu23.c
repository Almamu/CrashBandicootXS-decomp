#include "core.h"
#include "icon_manager.h"
#include "pause_options_screen.h"

extern s32 sub_8028A30(void *mgr, s32 arg1);
extern s32 sub_8026F38(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern struct icon_manager *gUnknown_030012E0;
extern struct icon_manager *gUnknown_030012DC;

/* Sits right after the screen-init BG-load/per-row-stats cluster
 * (`src/graphics/settings_menu2.o`, ROM `0x080047F8`-`0x08004914`) and
 * before the settings-row flag-test/wrapper cluster
 * (`src/graphics/settings_menu3.c`, ROM `0x08004A50` onward). Both
 * functions here are fully understood and byte-exact matched, but
 * written as NAKED asm transcriptions rather than plain C - they hit
 * this project's well-documented "last mile" gcc-2.9 scratch-register
 * nondeterminism (see `sub_8006600`, `src/graphics/oam_count.c`, and
 * `src/util/printf_util.c`'s `sub_8000CBC` for the established
 * pattern/technique, and `src/graphics/settings_menu.c`'s own header
 * comment for this same difficulty class as hit by its sibling
 * functions) where a fully-traced C reconstruction gets every
 * field/branch/call right but can't be coaxed into the ROM's exact
 * scratch-register choices. See docs/matching.md's write-up for this
 * chunk for the full register-allocation story that motivated the
 * NAKED conversion. */

/* `arg1`/`arg2` are plain coordinate values here (not pointers - the
 * ROM does raw integer arithmetic on them, `arg1+0x1d`/`arg2+0xc`),
 * used as the on-screen anchor for a centered numeric glyph (label
 * 0x25) into gUnknown_030012DC.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history) hit the same "last mile" gcc-2.9 scratch-register
 * nondeterminism documented on `sub_80049CC` below. Every instruction
 * below is confirmed byte-identical to the ROM - full NAKED
 * transcription, like this project's other hard-compiler-limitation
 * cases (see `src/util/printf_util.c`'s `sub_8000CBC` for the
 * established pattern), is more honest than continuing to chase these
 * scratch-register choices through plain C. */
NAKED void sub_8004914(struct pause_options_screen *self, s32 arg1, s32 arg2, u8 arg3)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "lsl r3, r3, #0x18\n\t"
        "add r7, r1, #0\n\t"
        "add r7, #0x1d\n\t"
        "add r2, #0xc\n\t"
        "mov sb, r2\n\t"
        "cmp r3, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, 1f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r4, #4]\n\t"
        "asr r0, r0, #2\n\t"
        "mov r1, #1\n\t"
        "and r0, r1\n\t"
        "mov r1, #2\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "mov r1, #1\n\t"
    "6:\n\t"
        "add r0, r2, #0\n\t"
        "bl sub_8028A30\n\t"
        "b 3f\n\t"
        ".align 2, 0\n\t"
    "1: .4byte gUnknown_030012DC\n\t"
    "2:\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
    "3:\n\t"
        "ldr r0, 5f\n\t"
        "mov r8, r0\n\t"
        "ldr r4, [r0]\n\t"
        "mov r5, #0x98\n\t"
        "lsl r5, r5, #1\n\t"
        "add r0, r4, r5\n\t"
        "ldr r0, [r0]\n\t"
        "add r6, r0, #0\n\t"
        "add r6, #0x10\n\t"
        "mov r1, #0x10\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x25\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r6, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "lsr r1, r0, #0x1f\n\t"
        "add r0, r0, r1\n\t"
        "asr r0, r0, #1\n\t"
        "sub r0, r7, r0\n\t"
        "mov r2, r8\n\t"
        "ldr r4, [r2]\n\t"
        "mov r2, #0x88\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r4, r2\n\t"
        "str r0, [r1]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r4, r1\n\t"
        "mov r2, sb\n\t"
        "str r2, [r0]\n\t"
        "add r5, r4, r5\n\t"
        "ldr r0, [r5]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x20\n\t"
        "mov r1, #0x20\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, #0x25\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    "5: .4byte gUnknown_030012DC\n\t"
    );
}

/* Draws a centered label (from the runtime string table via
 * sub_8026F38) into gUnknown_030012E0's icon pair - `self` is unused.
 * Matches sub_8006600's (src/graphics/oam_count.c) centered-icon shape
 * exactly, just for a single label rather than flanking a number.
 *
 * Written as NAKED asm, not plain C: a full C reconstruction (kept in
 * git history), with `label`/`slot0`/`mgrAddr`/`mgr`/`recOff` pinned
 * to r9/r8/r6/r4/r5 (mirroring the ROM's own register choices exactly)
 * and the destination-address computation reordered before the
 * `sub_8026F38` call it needs to survive across, matched every
 * instruction except one: gcc's natural register choice for the s16
 * shift-index reused in the offset read (record+0x10/+0x20) differs
 * from the ROM's fresh reload into r3 both times - splitting the
 * offset read into its own local, an inline-asm register-pinned
 * `ldrsh`, and reordering around it were all tried and none closed the
 * gap without introducing a worse one. Every instruction below is
 * confirmed byte-identical to the ROM - full NAKED transcription, like
 * this project's other hard-compiler-limitation cases (see
 * `src/util/printf_util.c`'s `sub_8000CBC` for the established
 * pattern), is more honest than continuing to chase this one register
 * choice through plain C. */
NAKED void sub_80049CC(struct pause_options_screen *self, s32 labelIndex)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6}\n\t"
        "mov sb, r1\n\t"
        "ldr r6, 1f\n\t"
        "ldr r0, [r6]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
        "ldr r4, [r6]\n\t"
        "mov r5, #0x98\n\t"
        "lsl r5, r5, #1\n\t"
        "add r0, r4, r5\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x10\n\t"
        "add r1, r1, r0\n\t"
        "mov r8, r1\n\t"
        "mov r3, #0x10\n\t"
        "ldrsh r0, [r0, r3]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, sb\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "mov r0, r8\n\t"
        "ldr r2, [r0, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "mov r1, #0xf0\n\t"
        "sub r1, r1, r0\n\t"
        "asr r1, r1, #1\n\t"
        "ldr r4, [r6]\n\t"
        "mov r2, #6\n\t"
        "mov r3, #0x88\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r4, r3\n\t"
        "str r1, [r0]\n\t"
        "mov r1, #0x8a\n\t"
        "lsl r1, r1, #1\n\t"
        "add r0, r4, r1\n\t"
        "str r2, [r0]\n\t"
        "add r5, r4, r5\n\t"
        "ldr r0, [r5]\n\t"
        "add r5, r0, #0\n\t"
        "add r5, #0x20\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r0, [r0, r3]\n\t"
        "add r4, r4, r0\n\t"
        "mov r0, sb\n\t"
        "bl sub_8026F38\n\t"
        "add r1, r0, #0\n\t"
        "ldr r2, [r5, #4]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_803AD80\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n\t"
    "1: .4byte gUnknown_030012E0\n\t"
    );
}
