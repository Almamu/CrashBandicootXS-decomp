#include "core.h"
#include "actor.h"

extern void sub_800D040(void *part);
extern void sub_80109A4(void *part, s32 mode, s32 playerX, s32 playerY);
extern void *gUnknown_03001308;
extern void *gUnknown_030012D8;

/* Another per-frame spatial-hash-grid pass over `manager`, scoped to
 * the same 3-bucket window `[baseIdx, baseIdx+2]` (`baseIdx` computed
 * the same way as `sub_80091D4`'s: `max(gUnknown_03001308`'s
 * sub-object's own `x >> 8`, `0)`), reading the player
 * (`gUnknown_030012D8`) rather than writing to the grid.
 *
 * If the player's `+0x88` byte is `3`: for every windowed node, calls
 * `sub_800D040(part)`.
 *
 * Otherwise: computes a dispatch value from the player (`*(void **)
 * (player+0x44) + 8`'s pointee by default; `0` if the player's `+0x88`
 * byte is `1`, further overridden to `0xd` if that byte is also `1`
 * *and* the player's `+0xa` byte is `0x13`), then for every windowed
 * node calls `sub_80109A4(part, dispatchValue, player->x, player->y)`.
 *
 * Written as NAKED asm, not plain C: fully understood (both callee
 * signatures traced from their own call sites, every field offset and
 * branch confirmed against the ROM disassembly), but real-C attempts
 * following this shape couldn't reproduce the ROM's register choice for
 * the `r8`-pinned "current bucket's list head" pointer, which is reused
 * for a *different* base address (`gridHead[0]` vs. bucket-255's own
 * "+0x10 past the player-derived struct" address) between the two
 * branches above - the same category of cross-branch register-role gap
 * documented for `sub_80091D4` above. Parked as a direct transcription
 * of the ROM's own confirmed-correct instructions instead. */
NAKED void sub_8009868(void *manager)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x10]\n\t"
        "ldr r6, [r0]\n\t"
        "asr r6, r6, #8\n\t"
        "cmp r6, #0\n\t"
        "bge 1f\n\t"
        "mov r6, #0\n\t"
    "1:\n\t"
        "add r2, r6, #2\n\t"
        "ldr r0, 3f\n\t"
        "ldr r3, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x88\n\t"
        "ldrb r1, [r0]\n\t"
        "mov r0, #0x10\n\t"
        "add r0, r0, r4\n\t"
        "mov r8, r0\n\t"
        "cmp r1, #3\n\t"
        "bne 7f\n\t"
    "4:\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r8\n\t"
        "ldr r4, [r0]\n\t"
        "sub r5, r2, #1\n\t"
        "cmp r4, #0\n\t"
        "beq 6f\n\t"
    "5:\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_800D040\n\t"
        "ldr r4, [r4, #4]\n\t"
        "cmp r4, #0\n\t"
        "bne 5b\n\t"
    "6:\n\t"
        "add r2, r5, #0\n\t"
        "cmp r2, r6\n\t"
        "bge 4b\n\t"
        "b 12f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_03001308\n"
    "3: .4byte gUnknown_030012D8\n"
    "7:\n\t"
        "ldr r0, [r3, #0x44]\n\t"
        "ldr r7, [r0, #8]\n\t"
        "ldr r0, [r3]\n\t"
        "mov sl, r0\n\t"
        "ldr r0, [r3, #4]\n\t"
        "mov sb, r0\n\t"
        "cmp r1, #1\n\t"
        "bne 8f\n\t"
        "mov r7, #0\n\t"
        "ldrb r3, [r3, #0xa]\n\t"
        "cmp r3, #0x13\n\t"
        "bne 8f\n\t"
        "mov r7, #0xd\n\t"
    "8:\n\t"
        "add r4, #0x10\n\t"
        "mov r8, r4\n\t"
    "9:\n\t"
        "lsl r0, r2, #2\n\t"
        "add r0, r8\n\t"
        "ldr r4, [r0]\n\t"
        "sub r5, r2, #1\n\t"
        "cmp r4, #0\n\t"
        "beq 11f\n\t"
    "10:\n\t"
        "ldr r0, [r4]\n\t"
        "add r1, r7, #0\n\t"
        "mov r2, sl\n\t"
        "mov r3, sb\n\t"
        "bl sub_80109A4\n\t"
        "ldr r4, [r4, #4]\n\t"
        "cmp r4, #0\n\t"
        "bne 10b\n\t"
    "11:\n\t"
        "add r2, r5, #0\n\t"
        "cmp r2, r6\n\t"
        "bge 9b\n\t"
    "12:\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
asm(".align 2, 0");
