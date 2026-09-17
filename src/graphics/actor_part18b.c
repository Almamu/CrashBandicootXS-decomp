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

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_80145E4" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_17_145e4.s) is used otherwise. Every load/store and
 * branch matches the ROM - the one residual gap is the opening bit-test
 * (`gUnknown_030007E0 & 0x100`) materializing its `u16` result into a
 * scratch register first and only then copying it into the register
 * `flag` keeps for the rest of the function (`lsrs r0,#0x10` +
 * `adds r5,r0,#0`, two instructions/4 bytes), where the ROM computes it
 * directly into that same register in one instruction (`lsrs r5,r0,
 * #0x10`). Confirmed this is specifically triggered by the *nested* `if
 * (part[0x38] != 0)` inside the `else` branch sharing `flag`'s live
 * range across both branches - deleting that inner `if` (or replacing
 * it with an unconditional block) restores the ROM's direct-target
 * form, but the inner `if` itself is real, required control flow, not
 * something to remove. Tried: pinning `flag` to a fixed register
 * (`register u16 flag asm("r5")`) - this instead makes gcc drop the
 * `u16` truncation semantics entirely on the defining assignment, and
 * then re-widens/re-truncates on every later read of `flag`, a strictly
 * worse mismatch than the one residual copy; pinning `part` to a fixed
 * register - no change; replacing the nested `if` with an equivalent
 * `goto` - no change (same control-flow graph). */
extern void sub_8012D24(void *self);

/* Clears `self+0x18`. If `gUnknown_030007E0` bit `0x100` is set, fires
 * the usual base+offset+fn-pointer trampoline pair and clears
 * `self+0x1c` too. Otherwise, while `part+0x38` is set, resets the same
 * flag/counter/table-index trio as `sub_801426C` via `sub_8015780`
 * (storing the raw masked bit value, not a normalized boolean, since
 * the ROM reuses the same register for both the branch test and the
 * stores here - unlike `sub_8014524`'s `!= 0`-normalized version of the
 * same test), then tail-calls `sub_8012D24`. */
void sub_80145E4(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;
    s32 zero = 0;
    u16 flag;

    *(s32 *)(self + 0x18) = zero;
    flag = gUnknown_030007E0 & 0x100;

    if (flag != 0) {
        u8 *mgr = *(u8 **)(self + 0xc);
        u8 *off;
        sub_803AD80(self + *(s16 *)(mgr + 0x20), (void *)0x10,
                    *(void **)(mgr + 0x24));
        off = *(u8 **)(self + 0xc) + 0x50;
        sub_803AD84(self + *(s16 *)off, *(void **)(self + 0x10),
                    (void *)3, *(void **)(off + 4));
        *(s32 *)(self + 0x1c) = zero;
    } else {
        u8 *part = *(u8 **)(self + 0x10);

        if (part[0x38] != 0) {
            sub_8015780(self, 0, 0x12, 0, flag);
            {
                register u8 *p1 asm("r0");
                register u8 *p2 asm("r1");

                p1 = self + 0x31;
                *p1 = flag;
                p2 = self + 0x2f;
                {
                    register s32 one asm("r0") = 1;
                    *p2 = one;
                    p2 -= 8;
                    *p2 = flag;
                    p2 += 0xb;
                    *p2 = flag;
                    p2 -= 2;
                    *p2 = one;
                }
                p1 = self + 0x28;
                *p1 = flag;
            }
        }
        sub_8012D24(self);
    }
}
#endif /* NON_MATCHING */
