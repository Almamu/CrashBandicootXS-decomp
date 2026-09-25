#include "core.h"

/* GitHub issue #9/#10, dedicated deep-investigation session:
 * `sub_800AFF4` (0x0800AFF4-0x0800B270), the second function in the
 * `sub_800AC2C`-through-`sub_800AFF4` raw span `tools/report_units.py`
 * tracked as parked (`base_object=None`). `sub_800AC2C` itself (the
 * 38-case player action-state jump-table dispatcher directly above
 * this function) is left untouched - same standing exclusion as
 * `sub_8018008`, issue #22 - real bytes for it stay in
 * `asm/code_3_2_16_ac2c.s`, now trimmed to end right after it.
 *
 * `docs/rom_map.md`'s "eight more core reads" passage had already
 * flagged this function's shape from one angle ("reaches [the 28-byte-
 * record table] through a *child* object's `+0x20` field and reads a
 * third offset, byte `+0x16` this time, clamping the result into
 * `self+0x30`" - actually the *child's* own `+0x30`, not `self`'s; see
 * below) - reading the raw bytes directly confirms and extends that.
 *
 * `self` is the same wide, still-unnamed "big object" struct
 * (0x108+ bytes) referenced by raw offset throughout this ROM
 * neighborhood (`actor_part16.c`/`actor_part79.c`/`actor_part108.c`
 * etc.) - `self+0xc` (flags byte), `self+0x18` (per-category
 * `{s16 offset; void *fn}` trampoline table pointer, the
 * `sub_803AD7C` convention `actor_part108.c` already established),
 * `self+0x20` (per-tag 28-byte-record table pointer,
 * `*(self+0x20) + tag*0x1c`, the exact convention `actor_part79.c`
 * documents from a sibling call site), `self+0x28` bit 4 (the
 * mirror-flag bit `actor_part16.c`/`actor_part17.c`/`actor_part108.c`
 * already read), `self+0x2d` (per-tag selector byte), and `self+0x8c`
 * (a `gUnknown_0300082C`-relative deadline - the exact
 * `IsTimerArmed`/`SetTimer` convention `actor_part16.c` names:
 * `*(u32 *)(self+0x8c) > gUnknown_0300082C` means "still armed").
 * `self+0xb0` is a pointer to a single "child" companion object (the
 * same object across every use in this function); `self+0xb4` is a
 * write index into an 8-slot circular buffer of `self`'s own recent
 * `{x, y}` Q8 positions at `self+0xb8` (`struct { s32 x, y; }
 * posHistory[8]`, 8-byte stride); `self+0x38` is a one-shot flag
 * consumed at the very end.
 *
 * Read together, this is the per-frame update for a "stars orbiting a
 * dizzy head" companion effect, gated on `gUnknown_030012C0+0x78`
 * (the central game-state "mode" field several other functions in
 * this doc already gate on):
 *
 * - **mode == 3** ("just got hit" / stun-entry): every ~8 frames
 *   (`gUnknown_0300082C & 7 == 0`) re-rolls `gUnknown_03000818` to
 *   `(u16)sub_8000E1C(2) + 2 - (mirrored ? 2 : 0)` (0/1 if mirrored,
 *   2/3 otherwise - which side the effect "starts" from, based on
 *   facing). Clamps that value against the child's own hitbox/variant
 *   record's `+0x16` byte (via the child's own `+0x20`-table,
 *   `+0x2d`-tag convention) and stores the clamp into the child's own
 *   `+0x30`. Repositions the child directly next to `self`'s own
 *   *current* position (`self.x +/- 0x600` depending on the mirror
 *   flag, `self.y - 0x1300`, both Q8 - a fixed offset near the head).
 *   Toggles the child's own `+0x2d` tag between `1`/`2` on a 4-frame
 *   parity of `gUnknown_0300082C` (a flicker), then fires the child's
 *   own `+0x18`-table `+0x20`/`+0x24` trampoline (the `sub_803AD7C`
 *   refresh/notify convention). Also (regardless of the mode-3 gate,
 *   using `self`'s own blink deadline at `self+0x8c`) draws `self`
 *   itself via `sub_8007A84(gUnknown_030012CC, self)` (matched,
 *   `actor_part.c` - queues `self`'s own OAM using its own Q8
 *   position) either unconditionally (mode == 3, or the deadline has
 *   expired) or, while the deadline is still armed, only on the same
 *   4-frame parity - a standard hit-invincibility blink. Once that
 *   deadline is no longer armed while mode == 3, calls
 *   `sub_80231EC(gUnknown_030012C0, 2)` (matched pattern,
 *   `actor_part84.c`/`actor_part58.c` - a mode-transition/"state
 *   close" call) - ends the stun state, transitioning mode 3 -> 2.
 *
 * - Unconditionally (any mode): pushes `self`'s own current `{x, y}`
 *   into the 8-slot `self+0xb8` position-history ring buffer at
 *   `self+0xb4`, advancing the index mod 8.
 *
 * - **mode == 1 or mode == 2** (ongoing idle-orbit): every ~8 frames,
 *   random-walks `gUnknown_0300081C` by `sub_8000E1C(3) - 1` (-1/0/+1),
 *   clamped to `[0, 3]`. Clamps that value against the same child
 *   record's `+0x16` byte and stores it into the child's own `+0x30`
 *   (same clamp-and-store idiom as the mode-3 branch, different source
 *   counter). Reads the *oldest* surviving entry of the position-
 *   history ring buffer (the slot about to be overwritten next frame -
 *   effectively `self`'s own position from up to 8 frames ago, a
 *   fixed trailing delay) and adds a rotating offset built from the
 *   shared 256-entry sine-ish table `gStaticData_0816A820` (already
 *   confirmed `extern s16 gStaticData_0816A820[];`,
 *   `actor_part72.c`): `child.x = oldX + (table[frame & 0xff] << 4)`,
 *   `child.y = oldY + (table[(frame >> 1) & 0xff] << 3) - 0x1800`
 *   (Q8 `-24.0`) - two different angular speeds (full-speed X,
 *   half-speed Y) around a point 24 px above the trailed position, the
 *   classic elliptical "orbiting stars" motion. Sets the child's own
 *   `+0x2d` tag to `mode - 1` (`0`/`1`) and fires the same
 *   `+0x18`-table trampoline refresh.
 *
 * - Finally, if `self+0x38` is nonzero, clears bit 3 (`0x08`) of
 *   `self+0xc` - a one-shot "orbit effect (re)armed" flag consumed
 *   once.
 *
 * **Matching**: not attempted as a C reconstruction. This is the
 * exact register-pressure shape this session's own risk flag named up
 * front (`sb`/`sl`/`r8`/`ip` all simultaneously live - the ring-buffer
 * base pointers `sb`/`sl` and the saved-mode/saved-child-address
 * `r8`/`ip` all stay live across the trig-table lookups and the two
 * child-record clamp blocks) and the same shape this project has
 * already proven resistant to gcc 2.9 reconstruction on several other
 * functions this session (`sub_800CD00`, `sub_800A178`/`sub_800A420`,
 * `sub_8026AE8`/`sub_8026A18`) - transcribed directly as byte-exact
 * NAKED asm instead, verified instruction-for-instruction against the
 * ROM disassembly (`asm/code_3_2_16_ac2c.s`'s own former content at
 * this address) and confirmed byte-exact via the isolated
 * `cpp`/`agbcc`/`as` + `objcopy`/`cmp` pipeline against `baserom.gba`
 * at `0x0800AFF4`-`0x0800B270` (the only differences at relocation
 * sites - `bl` calls and `.4byte` literals - which resolve correctly
 * once linked). See docs/matching/issue-9-10-0x0800aff4-graphics.md
 * for the full write-up. */

NAKED void sub_800AFF4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "add r7, r0, #0\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "bne 7f\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #7\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x28\n\t"
        "add r1, r1, r7\n\t"
        "mov r8, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 1f\n\t"
        "ldr r4, 22f\n\t"
        "mov r0, #2\n\t"
        "bl sub_8000E1C\n\t"
        "mov r2, r8\n\t"
        "ldrb r2, [r2]\n\t"
        "lsl r1, r2, #0x1b\n\t"
        "lsr r1, r1, #0x1f\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "add r0, #2\n\t"
        "lsl r1, r1, #1\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [r4]\n\t"
    "1:\n\t"
        "ldr r0, 22f\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0xb0\n\t"
        "ldr r5, [r2]\n\t"
        "ldr r4, [r0]\n\t"
        "ldr r0, [r5, #0x20]\n\t"
        "add r3, r5, #0\n\t"
        "add r3, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r6, [r3]\n\t"
        "lsl r0, r6, #3\n\t"
        "sub r0, r0, r6\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "add r3, r2, #0\n\t"
        "cmp r4, r0\n\t"
        "blt 2f\n\t"
        "sub r4, r0, #1\n\t"
    "2:\n\t"
        "str r4, [r5, #0x30]\n\t"
        "mov r1, r8\n\t"
        "ldrb r1, [r1]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r7, #4]\n\t"
        "ldr r2, [r3]\n\t"
        "ldr r4, 23f\n\t"
        "add r0, r0, r4\n\t"
        "ldr r5, 24f\n\t"
        "add r1, r1, r5\n\t"
        "b 4f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012C0\n"
    "21: .4byte gUnknown_0300082C\n"
    "22: .4byte gUnknown_03000818\n"
    "23: .4byte 0xFFFFFA00\n"
    "24: .4byte 0xFFFFED00\n"
    "3:\n\t"
        "ldr r0, [r7]\n\t"
        "ldr r1, [r7, #4]\n\t"
        "ldr r2, [r3]\n\t"
        "mov r6, #0xc0\n\t"
        "lsl r6, r6, #3\n\t"
        "add r0, r0, r6\n\t"
        "ldr r4, 25f\n\t"
        "add r1, r1, r4\n\t"
    "4:\n\t"
        "str r0, [r2]\n\t"
        "str r1, [r2, #4]\n\t"
        "ldr r0, 26f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #4\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "ldr r0, [r3]\n\t"
        "mov r1, #1\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "25: .4byte 0xFFFFED00\n"
    "26: .4byte gUnknown_0300082C\n"
    "5:\n\t"
        "ldr r0, [r3]\n\t"
        "mov r1, #2\n\t"
    "6:\n\t"
        "add r0, #0x2d\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r3]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r5, #0x20\n\t"
        "ldrsh r1, [r2, r5]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x24]\n\t"
        "bl sub_803AD7C\n\t"
    "7:\n\t"
        "ldr r0, 27f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "beq 9f\n\t"
        "mov r2, #0\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldr r1, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r1, [r1]\n\t"
        "cmp r0, r1\n\t"
        "bls 8f\n\t"
        "mov r2, #1\n\t"
    "8:\n\t"
        "cmp r2, #0\n\t"
        "beq 9f\n\t"
        "mov r0, #4\n\t"
        "and r1, r0\n\t"
        "cmp r1, #0\n\t"
        "beq 10f\n\t"
    "9:\n\t"
        "ldr r0, 29f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_8007A84\n\t"
    "10:\n\t"
        "ldr r0, 27f\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r0, [r3, #0x78]\n\t"
        "cmp r0, #3\n\t"
        "bne 12f\n\t"
        "mov r4, #0\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x8c\n\t"
        "ldr r1, 28f\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r1]\n\t"
        "cmp r2, r0\n\t"
        "bls 11f\n\t"
        "mov r4, #1\n\t"
    "11:\n\t"
        "cmp r4, #0\n\t"
        "bne 12f\n\t"
        "add r0, r3, #0\n\t"
        "mov r1, #2\n\t"
        "bl sub_80231EC\n\t"
    "12:\n\t"
        "ldr r2, [r7]\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0xb4\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r4, r7, #0\n\t"
        "add r4, #0xb8\n\t"
        "add r0, r4, r0\n\t"
        "str r2, [r0]\n\t"
        "ldr r2, [r7, #4]\n\t"
        "ldr r0, [r1]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0xbc\n\t"
        "add r0, r3, r0\n\t"
        "str r2, [r0]\n\t"
        "ldr r5, [r1]\n\t"
        "add r2, r5, #1\n\t"
        "add r0, r2, #0\n\t"
        "str r1, [sp]\n\t"
        "mov sb, r4\n\t"
        "mov sl, r3\n\t"
        "cmp r2, #0\n\t"
        "bge 13f\n\t"
        "add r0, r5, #0\n\t"
        "add r0, #8\n\t"
    "13:\n\t"
        "asr r0, r0, #3\n\t"
        "lsl r0, r0, #3\n\t"
        "sub r0, r2, r0\n\t"
        "ldr r6, [sp]\n\t"
        "str r0, [r6]\n\t"
        "ldr r0, 27f\n\t"
        "ldr r0, [r0]\n\t"
        "ldr r0, [r0, #0x78]\n\t"
        "mov r8, r0\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #1\n\t"
        "bhi 18f\n\t"
        "ldr r0, 28f\n\t"
        "ldr r1, [r0]\n\t"
        "mov r2, #7\n\t"
        "and r1, r2\n\t"
        "add r4, r0, #0\n\t"
        "cmp r1, #0\n\t"
        "bne 16f\n\t"
        "ldr r5, 30f\n\t"
        "mov r0, #3\n\t"
        "bl sub_8000E1C\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x10\n\t"
        "ldr r1, [r5]\n\t"
        "add r1, r1, r0\n\t"
        "sub r1, #1\n\t"
        "str r1, [r5]\n\t"
        "cmp r1, #3\n\t"
        "ble 14f\n\t"
        "mov r1, #3\n\t"
    "14:\n\t"
        "cmp r1, #0\n\t"
        "bge 15f\n\t"
        "mov r1, #0\n\t"
    "15:\n\t"
        "str r1, [r5]\n\t"
    "16:\n\t"
        "ldr r0, 30f\n\t"
        "mov r1, #0xb0\n\t"
        "add r1, r1, r7\n\t"
        "mov ip, r1\n\t"
        "ldr r5, [r1]\n\t"
        "ldr r3, [r0]\n\t"
        "ldr r0, [r5, #0x20]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r6, [r2]\n\t"
        "lsl r0, r6, #3\n\t"
        "sub r0, r0, r6\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r3, r0\n\t"
        "blt 17f\n\t"
        "sub r3, r0, #1\n\t"
    "17:\n\t"
        "str r3, [r5, #0x30]\n\t"
        "ldr r0, [sp]\n\t"
        "ldr r3, [r0]\n\t"
        "lsl r3, r3, #3\n\t"
        "add sb, r3\n\t"
        "ldr r5, 31f\n\t"
        "ldr r1, [r4]\n\t"
        "mov r4, #0xff\n\t"
        "add r0, r1, #0\n\t"
        "and r0, r4\n\t"
        "lsl r0, r0, #1\n\t"
        "add r0, r0, r5\n\t"
        "mov r6, #0\n\t"
        "ldrsh r2, [r0, r6]\n\t"
        "add r3, sl\n\t"
        "lsr r1, r1, #1\n\t"
        "and r1, r4\n\t"
        "lsl r1, r1, #1\n\t"
        "add r1, r1, r5\n\t"
        "mov r4, #0\n\t"
        "ldrsh r0, [r1, r4]\n\t"
        "mov r5, ip\n\t"
        "ldr r4, [r5]\n\t"
        "lsl r2, r2, #4\n\t"
        "mov r6, sb\n\t"
        "ldr r1, [r6]\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #3\n\t"
        "ldr r1, [r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, 32f\n\t"
        "add r0, r0, r1\n\t"
        "str r2, [r4]\n\t"
        "str r0, [r4, #4]\n\t"
        "ldr r0, [r5]\n\t"
        "mov r1, r8\n\t"
        "sub r1, #1\n\t"
        "add r0, #0x2d\n\t"
        "strb r1, [r0]\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r2, [r0, #0x18]\n\t"
        "mov r3, #0x20\n\t"
        "ldrsh r1, [r2, r3]\n\t"
        "add r0, r0, r1\n\t"
        "ldr r1, [r2, #0x24]\n\t"
        "bl sub_803AD7C\n\t"
    "18:\n\t"
        "add r0, r7, #0\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 19f\n\t"
        "mov r0, #9\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [r7, #0xc]\n\t"
        "and r0, r4\n\t"
        "strb r0, [r7, #0xc]\n\t"
    "19:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "27: .4byte gUnknown_030012C0\n"
    "28: .4byte gUnknown_0300082C\n"
    "29: .4byte gUnknown_030012CC\n"
    "30: .4byte gUnknown_0300081C\n"
    "31: .4byte gStaticData_0816A820\n"
    "32: .4byte 0xFFFFE800\n"
    );
}
