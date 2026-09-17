#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-62-0x08033804-actor.md,
 * "Parked, not matched: sub_8033CF8" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_31784_33cf8.s) is used otherwise.
 * `sub_80339DC`'s sibling: sets `self`'s position fields from the
 * singleton's own position plus a different fixed offset, and - while
 * `self+0x64` (a cooldown slot) is zero - measures `self`'s distance to
 * the player the same way `sub_80339DC` does and, if within range,
 * picks one of three spawn "kinds" (5/6/8, via `sub_8000E1C(3)`) and
 * calls `sub_802E170` at `self`'s position; cycles `self+0x68` against
 * a threshold from `gUnknown_030015DC`'s table the same way. Once
 * `self+0x34` passes `0x4B00` and the singleton's own "kind"
 * (`sub_80338D0`) is 3, resets `self` back to its idle animation
 * state. Semantics are fully understood and every load/store, branch
 * and call is confirmed correct; the ROM keeps `self` in `r5`, the
 * player pointer in `r6`, and the `self+0x64` "slot" cache in `r7` for
 * the whole function (`push {r4,r5,r6,r7,lr}`). Explicitly pinning
 * `self+0x64` to `r7` here compiles, but this agbcc build does not
 * treat a plain low-register (`r0`-`r7`) `register` variable pinned
 * with no other high-register use in the same function as needing
 * callee-save treatment - it never emits the matching push/pop for
 * `r7`, which would silently corrupt the caller's `r7` if shipped, so
 * it was reverted here. (Confirmed with an isolated test: a bare
 * `register s32 x asm("r7")` variable used across a call compiles
 * without any `push`/`pop` of `r7` at all, unlike `r4`-`r6`, which are
 * saved correctly; `sub_80339DC`'s ROM disassembly needs an *additional*
 * high register - `sb` - in the same function before `r7` gets included
 * in the real prologue's save set, via the `mov r7,sb`/`push {r6,r7}`
 * relay idiom, which does not apply here.) */
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);
extern s32 sub_80338D0(void);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8000E1C(s32 arg0);
extern void sub_802E170(s32 kind, s32 x, s32 y, s32 z, s32 arg4);
extern void *gUnknown_03000884;

void sub_8033CF8(void *selfArg)
{
    u8 *self = selfArg;
    s32 slot;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x1E00;
    *(s32 *)(self + 0x20) = sub_80338F4() - 0x3000;
    *(s32 *)(self + 0x24) = sub_80338E8() - 0x100;

    slot = *(s32 *)(self + 0x64);
    if (slot == 0) {
        u8 *player = gUnknown_03000884;
        s32 angle = sub_803ADB4(*(s32 *)(player + 0x24) - *(s32 *)(self + 0x24), -0x1AA);

        if (angle > 0) {
            s32 scale = sub_803ADB4(0x1000, angle);
            s32 rawDx = (*(s32 *)(player + 0x1c) - *(s32 *)(self + 0x1c)) * scale;
            s32 rawDy = (*(s32 *)(player + 0x20) - *(s32 *)(self + 0x20)) * scale;
            s32 dx = rawDx >> 12;
            s32 dy = rawDy >> 12;
            s32 signDx = rawDx >> 31;
            s32 signDy = rawDy >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                s32 kind = (u16)sub_8000E1C(3);
                u8 *table;

                if (kind == 0) {
                    sub_802E170(5, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                } else if (kind == 1) {
                    sub_802E170(6, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                } else {
                    sub_802E170(8, *(s32 *)(self + 0x1c), *(s32 *)(self + 0x20), *(s32 *)(self + 0x24), slot);
                }

                *(s32 *)(self + 0x68) += 1;
                table = sub_80338C4();
                if (*(s32 *)(self + 0x68) == *(s32 *)(table + 0x20)) {
                    *(s32 *)(self + 0x68) = 0;
                    table = sub_80338C4();
                    slot = *(s32 *)(table + 0x24);
                } else {
                    table = sub_80338C4();
                    slot = *(s32 *)(table + 0x1c);
                }
                *(s32 *)(self + 0x64) = slot;
            }
        }
    } else {
        *(s32 *)(self + 0x64) = slot - 1;
    }

    if (*(s32 *)(self + 0x34) > 0x4B00 && sub_80338D0() == 3) {
        *(s32 *)(self + 0x28) = 0;
        *(s32 *)(self + 0x44) = 0;
        *(s32 *)(self + 0xc) = 2;
        *(u16 *)(self + 0x10) = *(u16 *)(*(u8 **)self + 0x18);
        self[0x12] = 0;
        *(s32 *)(self + 8) = 0;
    }
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
