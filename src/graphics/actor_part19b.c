#include "core.h"

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_802C2FC" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c2fc.s) is used otherwise. Semantics are fully
 * understood and every load/store, branch and call is confirmed
 * correct; the sole residual gap is a single dead `flag = 0`
 * initializer (materialized by the ROM as `movs r0, #0` / `mov r8, r0`
 * right after `frame` is obtained, even though every reachable path to
 * `flag`'s use overwrites it with `0x100` first) that this compiler's
 * dead-store elimination always removes here, however the assignment
 * is phrased (plain statement, combined with the declaration, before/
 * after the `GetAnimFrameData` call, with an empty-asm/`volatile`
 * anti-DCE hint - `volatile register` also spills the variable to the
 * stack, a bigger mismatch). Screen-space visibility test and OAM
 * setup for one sprite frame: derives the top-left corner from
 * `self+0x1c`/`self+0x20` minus half the frame's tile size, culls if
 * fully off-screen, then builds the OAM attribute words (position,
 * `sub_803B060`'s flag byte, and a priority/palette nibble from
 * `self+0x18`/`self+0x14`) and calls `SetupSpriteFrameOam`. */
extern u8 *GetAnimFrameData(void *self);
extern void SetupSpriteFrameOam(u8 *frame, u32 arg1, u32 arg2, s32 priority);
extern s32 sub_803B060(void *self);

void sub_802C2FC(void *selfArg)
{
    register u8 *self asm("r6") = selfArg;
    s32 rawX = *(s32 *)(self + 0x1c);
    s32 rawY = *(s32 *)(self + 0x20);
    u8 *frame;
    register u32 flag asm("r8") = 0;
    register u32 packed asm("r3");

    frame = GetAnimFrameData(self);

    {
        register s32 x asm("r4") = rawX >> 8;
        register s32 y asm("r5") = rawY >> 8;
        s32 w = frame[0];
        s32 h = frame[1];

        x -= w * 4;
        y -= h * 4;

        if (y > 0x9f) return;
        if (y + h * 8 < 0) return;
        if (x > 0xef) return;
        if (x + w * 8 < 0) return;

        flag = 0x100;
        {
            s32 attr = sub_803B060(self);
            packed = (y & 0xff) | ((u32)(x & 0x1ff) << 16) | attr | flag;
        }
    }

    {
        s32 v = *(s32 *)(self + 0x18);
        u32 pre;
        register u32 attr2 asm("r2");

        if (*(s32 *)(self + 0x14) & 0x8000) {
            pre = ((v << 12) | 0x800) << 16;
        } else {
            pre = (u32)(v << 28);
        }
        attr2 = pre >> 16;

        {
            register u8 *argFrame asm("r0") = frame;
            register u32 argPacked asm("r1") = packed;
            register s32 argPriority asm("r3") = 0x140;

            SetupSpriteFrameOam(argFrame, argPacked, attr2, argPriority);
        }
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
