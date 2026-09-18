#include "core.h"

/* Continuation of actor_part18.c's `gStaticData_0816BF20` action-table
 * entries - non-adjacent to it since the parked `sub_801434C` sits raw
 * between them (asm/code_3_2_17_1434c.s). See actor_part18.c's own
 * top-of-file comment for the shared field-offset conventions
 * (`self+0xc`/`self+0x10`/`+0x27`.."+0x32" etc.) these functions use. */

extern u32 gUnknown_030007E0;
extern void *gUnknown_03001304;
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);
extern u8 sub_8000760(void *dummy);
extern void sub_8015780(void *self, s32 a, s32 b, s32 c, s32 d);

/* Same shape as `sub_801426C` (actor_part18.c) - resets the same
 * flag/counter/table-index trio via `sub_8015780` while `part+0x38` is
 * set. */
void sub_80144E0(void *selfArg)
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

/* While `part+0x38` is set: computes `v = (gUnknown_030007E0 bit 0x100)
 * != 0`, forced to `1` when `sub_8000760`'s D-pad-remap result is `2` or
 * in `[7,8]`. If still clear, resets the same flag/counter/table-index
 * trio as `sub_801426C` via `sub_8015780`; otherwise fires the usual
 * base+offset+fn-pointer trampoline pair. */
void sub_8014524(void *selfArg)
{
    u8 *self = selfArg;
    u8 *part = *(u8 **)(self + 0x10);

    if (part[0x38] != 0) {
        void *dummy = gUnknown_03001304;
        u16 m = gUnknown_030007E0 & 0x100;
        u8 v = m != 0;
        s32 st = sub_8000760(dummy);

        switch (st) {
        case 2:
        case 7:
        case 8:
            v = 1;
            break;
        }

        if (v == 0) {
            sub_8015780(self, 0, 0x12, 0, v);
            self[0x31] = v;
            self[0x2f] = 1;
            self[0x27] = v;
            self[0x32] = v;
            self[0x30] = 1;
            self[0x28] = v;
        } else {
            u8 *mgr = *(u8 **)(self + 0xc);
            u8 *off;
            sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x10,
                        *(void **)(mgr + 0x24));
            off = *(u8 **)(self + 0xc) + 0x50;
            sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                        (void *)3, *(void **)(off + 4));
            {
                u8 zero = 0;
                self[0x31] = zero;
                self[0x2f] = 1;
                self[0x27] = zero;
            }
        }
    }
}
/* Trailing byte count isn't a multiple of 4 and this is the last
 * actually-emitted function in the file (sub_80145E4 below is
 * NON_MATCHING-guarded, so it compiles to nothing here in the default
 * build) - without this, `as` pads with its default NOP fill instead of
 * the ROM's zero fill (see docs/matching.md's alignment-padding
 * gotcha). */
asm(".align 2, 0");

extern void sub_8012D24(void *self);

/* Clears `self+0x18`. If `gUnknown_030007E0` bit `0x100` is set, fires
 * the usual base+offset+fn-pointer trampoline pair and clears
 * `self+0x1c` too. Otherwise, while `part+0x38` is set, resets the same
 * flag/counter/table-index trio as `sub_801426C` via `sub_8015780`
 * (storing the raw masked bit value, not a normalized boolean, since
 * the ROM reuses the same register for both the branch test and the
 * stores here - unlike `sub_8014524`'s `!= 0`-normalized version of the
 * same test), then tail-calls `sub_8012D24`.
 *
 * Written as NAKED asm, not plain C: every load/store and branch
 * matches the ROM - the one residual gap was the opening bit-test
 * (`gUnknown_030007E0 & 0x100`) materializing its `u16` result into a
 * scratch register before copying it into the register `flag` keeps
 * for the rest of the function, where the ROM computes it directly
 * into that same register in one instruction - see docs/matching.md,
 * "Parked, not matched: sub_80145E4". Transcribed
 * instruction-for-instruction from the ROM disassembly instead, the
 * same escape hatch used for `sub_8001CB8`/`sub_8001DB4`
 * (src/system/link_cable.c). */
NAKED void sub_80145E4(void *selfArg)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #4\n\t"
        "add r4, r0, #0\n\t"
        "mov r6, #0\n\t"
        "str r6, [r4, #0x18]\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #0x80\n\t"
        "lsl r1, r1, #1\n\t"
        "and r0, r1\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r5, r0, #0x10\n\t"
        "cmp r5, #0\n\t"
        "beq 1f\n\t"
        "ldr r1, [r4, #0xc]\n\t"
        "mov r2, #0x20\n\t"
        "ldrsh r0, [r1, r2]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0x24]\n\t"
        "mov r1, #0x10\n\t"
        "bl sub_803AD80\n\t"
        "ldr r2, [r4, #0xc]\n\t"
        "add r2, #0x50\n\t"
        "mov r1, #0\n\t"
        "ldrsh r0, [r2, r1]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r1, [r4, #0x10]\n\t"
        "ldr r3, [r2, #4]\n\t"
        "mov r2, #3\n\t"
        "bl sub_803AD84\n\t"
        "str r6, [r4, #0x1c]\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030007E0\n"
    "1:\n\t"
        "ldr r0, [r4, #0x10]\n\t"
        "add r0, #0x38\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "str r5, [sp]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x12\n\t"
        "mov r3, #0\n\t"
        "bl sub_8015780\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x31\n\t"
        "strb r5, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "sub r1, #8\n\t"
        "strb r5, [r1]\n\t"
        "add r1, #0xb\n\t"
        "strb r5, [r1]\n\t"
        "sub r1, #2\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x28\n\t"
        "strb r5, [r0]\n\t"
    "2:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8012D24\n\t"
    "3:\n\t"
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
