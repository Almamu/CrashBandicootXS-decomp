#include "core.h"

/* GitHub issue #19: 0x08015840-0x08016128, `graphics`-labeled chunk that
 * turned out to be the same "self" action-table object family documented
 * at length in actor_part17.c/actor_part18.c/actor_part38c.c/
 * actor_part38d.c - recategorized `graphics`->`actor` (see docs/matching/
 * issue-19-0x08015840-actor.md). Directly adjacent to actor_part38d.c's
 * matched span (which ends with the shared `sub_8015780` trampoline
 * helper this file's first function calls) and its parked `sub_80157C4`
 * right before this chunk starts. Non-adjacent to actor_part57b.c (this
 * chunk's other matched file) since the left-raw
 * `sub_80159F8`/`sub_8015C6C`/`sub_8015DF8` sit between them (see
 * asm/code_3_2_17_159f8.s). */

extern void sub_8015780(void *selfArg, s32 a, s32 b, s32 c, s32 d);
extern void sub_800B8A8(void *selfArg, s32 flags);
extern void sub_800B8C8(void *selfArg);
extern void sub_8011B90(void *selfArg);
extern u8 gStaticData_087E4224[];
extern void *gUnknown_030012D8;
extern void sub_8017264(void *selfArg, s32 a, s32 b, s32 c, s32 d);

/* Fires the mgr trampoline pair (actions `0`/`0x12`), then resets the
 * `0x27`/`0x2f`/`0x31` and `0x28`/`0x30`/`0x32` state/counter/table-index
 * pairs (same trio shape as `sub_8015508`/`sub_8015558` in
 * actor_part38c.c). */
void sub_8015840(void *selfArg)
{
    u8 *self = selfArg;

    sub_8015780(self, 0, 0x12, 0, 0);

    self[0x31] = 0;
    self[0x2f] = 1;
    self[0x27] = 0;
    self[0x32] = 0;
    self[0x30] = 1;
    self[0x28] = 0;
}

/* Sets `self+0xc`'s table pointer to `gStaticData_087E4224`, then
 * tail-calls `sub_800B8A8(self, flags)` - which promptly resets it back
 * to `gStaticData_087E3E7C` (see actor_part17.c) - same double-set
 * pattern as `sub_8017A78`. */
void sub_8015878(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E4224;
    sub_800B8A8(self, flags);
}

/* Resets via `sub_800B8C8` (table pointer to `gStaticData_087E3E7C`,
 * `self+8` cleared), re-points the table at `gStaticData_087E4224`, then
 * calls `sub_8011B90` (the child-object field-reset constructor
 * documented in actor_part39.c). Returns `self`. */
void *sub_801588C(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E4224;
    sub_8011B90(self);
    return self;
}

/* `self+0x14` word setter, always zero. */
void sub_80158AC(void *selfArg)
{
    *(s32 *)((u8 *)selfArg + 0x14) = 0;
}

/* `self+0x32` byte setter, always 1. */
void sub_80158B4(void *selfArg)
{
    ((u8 *)selfArg)[0x32] = 1;
}

/* `self+0x31` byte setter, always 1. */
void sub_80158BC(void *selfArg)
{
    ((u8 *)selfArg)[0x31] = 1;
}

/* `self+0x30` byte setter, always 1. */
void sub_80158C4(void *selfArg)
{
    ((u8 *)selfArg)[0x30] = 1;
}

/* `self+0x2f` byte setter, always 1. */
void sub_80158CC(void *selfArg)
{
    ((u8 *)selfArg)[0x2f] = 1;
}

/* `self+0x30` byte setter, always 0. */
void sub_80158D4(void *selfArg)
{
    ((u8 *)selfArg)[0x30] = 0;
}

/* `self+0x2f` byte setter, always 0. */
void sub_80158DC(void *selfArg)
{
    ((u8 *)selfArg)[0x2f] = 0;
}

/* `self+0x30` byte getter. */
u8 sub_80158E4(void *selfArg)
{
    return ((u8 *)selfArg)[0x30];
}

/* `self+0x2f` byte getter. */
u8 sub_80158EC(void *selfArg)
{
    return ((u8 *)selfArg)[0x2f];
}

/* Sets `self+0x32`/`self+0x30` to 1, and `self+0x28` to `val`. */
void sub_80158F4(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x32] = 1;
    self[0x30] = 1;
    self[0x28] = (u8)val;
}

/* Sets `self+0x31`/`self+0x2f` to 1, and `self+0x27` to `val`. */
void sub_8015908(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x31] = 1;
    self[0x2f] = 1;
    self[0x27] = (u8)val;
}

/* Sets `self+0x32` to 0, `self+0x30` to 1, and `self+0x28` to `val`. */
void sub_8015920(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x32] = 0;
    self[0x30] = 1;
    self[0x28] = (u8)val;
}

/* Sets `self+0x31` to 0, `self+0x2f` to 1, and `self+0x27` to `val`. */
void sub_8015938(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x31] = 0;
    self[0x2f] = 1;
    self[0x27] = (u8)val;
}

/* `self+0x2d` byte getter. */
u8 sub_8015950(void *selfArg)
{
    return ((u8 *)selfArg)[0x2d];
}

/* Big field reset: clears `self+0x26`/`self+8`/`self+0x24`/`self+0x25`,
 * sets `self+0x2c`/`self+0x2d` to 1, clears `self+0x14`/`self+0x10`/
 * `self+0x22`/`self+0x23`/`self+0x27`/`self+0x20`, sets `self+0x21` to
 * 6, clears `self+0x18`/`self+0x1c`, and clears the player's `+0x92`
 * byte. */
void sub_8015958(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    register u8 *p asm("r0") = self + 0x26;
    register s32 zero asm("r1") = 0;
    register s32 one asm("r2");

    *p = zero;
    *(s32 *)(self + 8) = zero;
    p -= 2;
    *p = zero;
    p += 1;
    *p = zero;
    p += 7;
    one = 1;
    *p = one;
    p += 1;
    *p = one;
    *(s32 *)(self + 0x14) = zero;
    *(s32 *)(self + 0x10) = zero;
    p -= 0xb;
    *p = zero;
    p += 1;
    *p = zero;
    p += 4;
    *p = zero;
    p -= 7;
    *p = zero;
    {
        register u8 *p21 asm("r2") = self + 0x21;
        *p21 = 6;
    }
    *(s32 *)(self + 0x18) = zero;
    *(s32 *)(self + 0x1c) = zero;
    ((u8 *)gUnknown_030012D8)[0x92] = zero;
}

/* Fires the mgr trampoline pair via `sub_8017264(self, 0, 0, 0, 0)`,
 * then resets `self+0x27`/`self+0x20`/`self+0x21`(=6)/`self+0x22`, the
 * player's `+0x92`, and `self+0x2c`(=1)/`self+0x24`/`self+0x2d`(=1)/
 * `self+0x25`. */
void sub_80159A4(void *selfArg)
{
    u8 *self = selfArg;

    sub_8017264(self, 0, 0, 0, 0);

    self[0x27] = 0;
    self[0x20] = 0;
    self[0x21] = 6;
    self[0x22] = 0;
    ((u8 *)gUnknown_030012D8)[0x92] = 0;
    self[0x2c] = 1;
    self[0x24] = 0;
    self[0x2d] = 1;
    self[0x25] = 0;
}
asm(".align 2, 0");
