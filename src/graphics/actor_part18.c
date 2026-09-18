#include "core.h"

/* This file (and actor_part18b.c, its non-adjacent continuation) covers
 * part of `gStaticData_0816BF20`, the 42-slot per-level action dispatch
 * table documented in docs/rom_map.md ("`gStaticData_0816BF20` is a
 * 42-slot, fully-populated action dispatch table") - `self` is the
 * player/action object those table entries are invoked on, not the
 * small (0x1c-byte) `struct actor` from include/actor.h. `self+0xc` is
 * a per-category table of `{s16 offset; void *fn}` pairs (at least two
 * entries known so far, `+0x20`/`+0x24` and `+0x50`/`+0x54`) fed
 * through the `sub_803AD80`/`sub_803AD84` trampolines together with
 * `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * base+offset+fn-pointer convention already named in actor_part17.c's
 * doc comments. The `+0x27`/`+0x28`/`+0x29`/`+0x2f`/`+0x30`/`+0x31`/
 * `+0x32` bytes are a state/flag/table-index trio pair this whole
 * action-table family shares; none of the three objects' full shapes
 * are pinned down yet, so every access here stays a raw offset rather
 * than a guessed struct. */

extern u32 gUnknown_030007E0;
extern void *gUnknown_030012BC;
extern void PlaySfx(void *arg0, s32 sfxId, s32 arg2);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern u8 sub_800AAEC(void *self, s32 action);
extern void sub_801434C(void *self);
extern void sub_8015508(void *self);
extern void sub_8015780(void *self, s32 a, s32 b, s32 c, s32 d);

/* Clears `part+0x38`'s "busy" flag by resetting the shared
 * flag/counter/table-index trio (`+0x31`/`+0x2f`/`+0x27` and
 * `+0x32`/`+0x30`/`+0x28`) via `sub_8015780`, but only while that flag
 * is actually set. */
void sub_801426C(void *selfArg)
{
    u8 *self = selfArg;
    u8 *part = *(u8 **)(self + 0x10);

    if (part[0x38] != 0) {
        sub_8015780(self, 0, 0x12, 0, 0);
        self[0x31] = 0;
        self[0x2f] = 1;
        self[0x27] = 0;
        self[0x32] = 0;
        self[0x30] = 1;
        self[0x28] = 0;
    }
}

/* On the "confirm" input edge (checked via `sub_800AAEC(part, 0xb)`),
 * plays a sound, clears two `part+0xd` bits (the runtime `& -2`/`& -3`
 * negation rather than a folded mask - see docs/matching.md), and hands
 * off to `sub_8015508`. Otherwise, while `part+0x38` is set, fires the
 * usual base+offset+fn-pointer trampoline pair and tail-calls
 * `sub_801434C` (still raw/parked - see docs/matching.md, "Parked, not
 * matched: sub_801434C" - real bytes live in asm/code_3_2_17_1434c.s,
 * linked right after this object). */
void sub_80142B0(void *selfArg)
{
    u8 *self = selfArg;
    u32 snap = *(u32 *)&gUnknown_030007E0;

    if ((*(u16 *)((u8 *)&snap + 2) & 1) != 0
        && sub_800AAEC(*(void **)(self + 0x10), 0xb) == 1) {
        PlaySfx(gUnknown_030012BC, 0xc, 0x100);

        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 2;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }
        {
            register u8 *part asm("r1") = *(u8 **)(self + 0x10);
            register s32 mask asm("r0") = 3;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }

        sub_8015508(self);
        return;
    }

    if ((*(u8 **)(self + 0x10))[0x38] != 0) {
        u8 *mgr = *(u8 **)(self + 0xc);
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x14,
                    *(void **)(mgr + 0x24));
        {
            u8 *off = *(u8 **)(self + 0xc) + 0x50;
            sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                        (void *)0, *(void **)(off + 4));
        }
        sub_801434C(self);
    }
}

extern u8 sub_8000760(void *dummy);
extern u8 sub_8012A7C(void *self);
extern void sub_80122CC(void *self);
extern void *gUnknown_03001304;

/* The shared handler `sub_80142B0` tail-calls: same "confirm" edge check
 * (short-circuits before reaching `sub_8012A7C` when it fires), then
 * (once `sub_8012A7C(self)` is clear) dispatches on `sub_8000760`'s
 * D-pad-remap result - `1` fires one trampoline pair, `0`/`2` fires
 * another - before falling into a shared tail that, when the input
 * snapshot's `0x180` bits are clear and `sub_800AAEC(part, 2)` just
 * fired, runs a third trampoline pair and finishes with
 * `sub_80122CC`.
 *
 * Written as NAKED asm, not plain C: every load/store, branch and call
 * was already confirmed correct and in the ROM's own case order, but
 * two small blocks (the `case 0`/`case 2` field-write trio's
 * value-vs-address evaluation order, and the closing
 * `snap & 0x180` block's address/constant/load ordering) never landed
 * in gcc 2.9's own scheduling order - see docs/matching.md, "Parked,
 * not matched: sub_801434C". Transcribed instruction-for-instruction
 * from the ROM disassembly instead, the same escape hatch used for
 * `sub_8001CB8`/`sub_8001DB4` (src/system/link_cable.c). */
NAKED void sub_801434C(void *selfArg)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "str r0, [sp]\n\t"
        "mov r0, sp\n\t"
        "ldrh r1, [r0, #2]\n\t"
        "mov r0, #1\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "mov r1, #0xb\n\t"
        "bl sub_800AAEC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #1\n\t"
        "bne 1f\n\t"
        "ldr r0, 21f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0xc\n\t"
        "bl PlaySfx\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "mov r0, #2\n\t"
        "neg r0, r0\n\t"
        "ldrb r2, [r1, #0xd]\n\t"
        "and r0, r2\n\t"
        "strb r0, [r1, #0xd]\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "mov r0, #3\n\t"
        "neg r0, r0\n\t"
        "ldrb r2, [r1, #0xd]\n\t"
        "and r0, r2\n\t"
        "strb r0, [r1, #0xd]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8015508\n\t"
        "b 10f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030007E0\n"
    "21: .4byte gUnknown_030012BC\n"
    "1:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8012A7C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r6, r0, #0x18\n\t"
        "cmp r6, #0\n\t"
        "beq 2f\n\t"
        "b 10f\n\t"
    "2:\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8000760\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "cmp r5, #1\n\t"
        "beq 6f\n\t"
        "cmp r5, #1\n\t"
        "bgt 3f\n\t"
        "cmp r5, #0\n\t"
        "beq 4f\n\t"
        "b 5f\n\t"
        ".align 2, 0\n"
    "22: .4byte gUnknown_03001304\n"
    "3:\n\t"
        "cmp r5, #2\n\t"
        "bne 5f\n\t"
    "4:\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0x1b\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #1\n\t"
        "bl sub_803AD84\n\t"
        "mov r1, #0\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r1, [r0]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x2f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r2]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x27\n\t"
        "strb r1, [r0]\n\t"
        "b 5f\n\t"
    "6:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "mov r1, #2\n\t"
        "bl sub_800AAEC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #1\n\t"
        "bne 7f\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0x15\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #2\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0x11\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #4\n\t"
    "8:\n\t"
        "bl sub_803AD84\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r6, [r0]\n\t"
        "sub r0, #2\n\t"
        "strb r5, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r6, [r0]\n\t"
    "5:\n\t"
        "mov r0, sp\n\t"
        "mov r5, #0xc0\n\t"
        "lsl r5, r5, #1\n\t"
        "ldrh r0, [r0]\n\t"
        "and r5, r0\n\t"
        "cmp r5, #0\n\t"
        "bne 9f\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "mov r1, #2\n\t"
        "bl sub_800AAEC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r6, r0, #0x18\n\t"
        "cmp r6, #1\n\t"
        "bne 9f\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0x12\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #2\n\t"
        "bl sub_803AD84\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r5, [r0]\n\t"
        "sub r0, #2\n\t"
        "strb r6, [r0]\n\t"
        "sub r0, #8\n\t"
        "strb r5, [r0]\n\t"
    "9:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80122CC\n\t"
    "10:\n\t"
        "add sp, #4\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
