#include "core.h"

/* Same large per-instance "self" object family as actor_part20.c - see
 * that file's header comment and docs/matching/issue-58-0x08030334-actor.md.
 * `gStaticData_0817C2B8` is a stride-8 keyframe-table, the same shape as
 * `gStaticData_0817A6B8` in `sub_802C208` (actor_part19.c): `{s16 baseOff;
 * s16 count; s16 subOffset; s16 pad}` records indexed by `self+0x28`'s
 * state. When `count > 0`, indexes a per-instance box-list pointer at
 * `self+subOffset` and reads its last entry's `{s32 x; s32 fn}`-shaped
 * pair; otherwise falls back to the record's own inline dword at `+4`.
 * Fires `sub_803AD84(self+addr, baseOff, count, fn)`. */

extern u8 gStaticData_0817C2B8[];
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_802A7B8(void *self);

/* Boss-weapon keyframe-table AABB lookup/dispatch. Every load/store,
 * branch and call confirmed correct against `sub_802C208`'s already-
 * parked twin (actor_part19.c) - same `gStaticData_*` stride-8 table
 * shape, same `sub_803AD84` dispatch tail. Transcribed as NAKED asm
 * rather than plain C for the identical reason documented there: this
 * agbcc build keeps the table's base address alive in `r7` for the
 * whole function (the "explicit `register T x asm(\"r7\")` compiles
 * correct instructions but silently drops r7 from the prologue/epilogue
 * push/pop list" gcc-2.9 bug - see docs/matching.md's `sub_8007DBC`/
 * `sub_8006600` entries), while this compiler's own *unforced* register
 * allocator never reaches r7 for this shape at all (confirmed: a direct
 * C translation compiles to noticeably shorter, differently-structured
 * code that never needs r7 or r6 to survive the call). No C-level
 * rephrasing/pinning can reach the ROM's exact register allocation - see
 * `sub_802C208`/`sub_802F748`/`sub_8033B44`/`sub_8033C84`/`sub_8033E80`
 * for every other instance of this identical hazard in this project.
 * After the keyframe/AABB dispatch, additionally fires the state-2/
 * table-index-3 event-table trampoline (via `sub_803AD80`) when
 * `self+0x12`'s anim flag is set, else falls back to `sub_802A7B8`. */
NAKED void sub_8030574(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, 1f\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r3, r0, #3\n\t"
        "add r0, r3, r1\n\t"
        "mov r7, #2\n\t"
        "ldrsh r2, [r0, r7]\n\t"
        "add r7, r1, #0\n\t"
        "cmp r2, #0\n\t"
        "ble 2f\n\t"
        "mov r1, #4\n\t"
        "ldrsh r0, [r0, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r0]\n\t"
        "lsl r0, r2, #3\n\t"
        "add r0, r0, r1\n\t"
        "sub r0, #8\n\t"
        "ldr r5, [r0]\n\t"
        "ldr r6, [r0, #4]\n\t"
        "add r3, r6, #0\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_0817C2B8\n"
    "2:\n\t"
        "add r0, r7, #4\n\t"
        "add r0, r3, r0\n\t"
        "ldr r3, [r0]\n\t"
    "3:\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "lsl r0, r0, #3\n\t"
        "add r0, r0, r7\n\t"
        "mov r7, #0\n\t"
        "ldrsh r1, [r0, r7]\n\t"
        "cmp r2, #0\n\t"
        "ble 4f\n\t"
        "lsl r0, r5, #0x10\n\t"
        "asr r0, r0, #0x10\n\t"
        "add r0, r0, r1\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r1, #0\n\t"
    "5:\n\t"
        "add r0, r4, r0\n\t"
        "bl sub_803AD84\n\t"
        "ldr r0, [r4, #0x28]\n\t"
        "cmp r0, #2\n\t"
        "bne 6f\n\t"
        "ldrb r0, [r4, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "cmp r4, #0\n\t"
        "beq 7f\n\t"
        "ldr r1, [r4, #0x50]\n\t"
        "mov r2, #8\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0xc]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A7B8\n\t"
    "7:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    );
}
