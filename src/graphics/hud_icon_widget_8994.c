#include "core.h"
#include "icon_manager.h"

/* Widest single line of `text`: like sub_8028900 but walks a
 * NUL-terminated string and tracks a running per-line width, resetting
 * on each newline and keeping the running maximum; the final result is
 * whichever of the last line's width or the running maximum is larger
 * (covers un-terminated final lines).
 *
 * The ROM pins `self` into `ip`, `text` into `r4`, `&spaceWidth` into
 * `r8`, and `&glyphRecords` into `r6` (dereferenced fresh each
 * iteration, like sub_8028900's identical field), keeping
 * `curWidth`/`maxWidth` in `r3`/`r5`. `charLookup[c]` itself is computed
 * directly off `self`+8+`c` (not through a separately cached base,
 * unlike sub_8028900), matching the ROM's own `mov r1, ip; adds r1, #8;
 * adds r1, r1, r0` shape.
 *
 * Transcribed as NAKED asm (not plain C): the prior parked pass's
 * doc comment attributed the gap to needing a 32-bit literal-pool-loaded
 * bitfield mask, but the actual remaining blocker (confirmed while
 * transcribing) is the same class of r7-register-pressure toolchain bug
 * documented for sub_8028900/InitHudIconWidgetA/B above - a NAKED
 * transcription sidesteps it regardless of the specific gap's nature,
 * since nothing asks gcc's allocator to decide anything. Every
 * instruction below is checked byte-identical to the ROM. */
NAKED s32 MeasureText(struct icon_manager *self, u8 *text)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "mov ip, r0\n\t"
        "add r4, r1, #0\n\t"
        "mov r5, #0\n\t"
        "mov r3, #0\n\t"
        "ldrb r0, [r4]\n\t"
        "cmp r0, #0\n\t"
        "beq 7f\n\t"
        "mov r1, #0x90\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, ip\n\t"
        "mov r8, r1\n\t"
        "mov r6, #0x86\n\t"
        "lsl r6, r6, #1\n\t"
        "add r6, ip\n\t"
    "1:\n\t"
        "cmp r0, #0xa\n\t"
        "beq 2f\n\t"
        "cmp r0, #0x20\n\t"
        "bne 4f\n\t"
        "mov r7, r8\n\t"
        "ldr r0, [r7]\n\t"
        "b 5f\n\t"
    "2:\n\t"
        "cmp r3, r5\n\t"
        "bls 3f\n\t"
        "add r5, r3, #0\n\t"
    "3:\n\t"
        "mov r3, #0\n\t"
        "b 6f\n\t"
    "4:\n\t"
        "mov r1, ip\n\t"
        "add r1, #8\n\t"
        "add r1, r1, r0\n\t"
        "ldr r2, [r6]\n\t"
        "ldrb r7, [r1]\n\t"
        "lsl r0, r7, #1\n\t"
        "add r1, r7, #0\n\t"
        "add r0, r0, r1\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r2\n\t"
        "ldr r0, [r0]\n\t"
    "5:\n\t"
        "add r3, r3, r0\n\t"
    "6:\n\t"
        "add r4, #1\n\t"
        "ldrb r0, [r4]\n\t"
        "cmp r0, #0\n\t"
        "bne 1b\n\t"
    "7:\n\t"
        "add r0, r3, #0\n\t"
        "cmp r0, r5\n\t"
        "bhs 8f\n\t"
        "add r0, r5, #0\n\t"
    "8:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
