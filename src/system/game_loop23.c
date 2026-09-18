#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010674` right
 * before `sub_80106DC` is left untouched raw. */

extern void *gUnknown_030012D8;
extern void sub_8010B6C(void *arg);

/* Refreshes the viewport's own collision box (`sub_8010B6C` on
 * `gUnknown_030012D8+0x108`), then increments its `+0x92` counter by
 * one as long as it isn't already zero (a saturating-at-zero
 * "recently hit" style counter, never incremented back up from 0). */
void sub_80106DC(void)
{
    u8 *p = (u8 *)gUnknown_030012D8;
    u8 *p2;

    sub_8010B6C(p + 0x108);
    p2 = (u8 *)gUnknown_030012D8 + 0x92;
    if (*p2 != 0) {
        *p2 = *p2 + 1;
    }
}

/* Neighbor-list "get prev" accessor - reads `self+0x60`, the field
 * `sub_800FEB0` (game_loop17.c) zeroes on reset. */
void *sub_8010708(void *selfArg)
{
    u8 *self = selfArg;
    return *(void **)(self + 0x60);
}

/* Neighbor-list "get next" accessor - reads `self+0x5c`. */
void *sub_801070C(void *selfArg)
{
    u8 *self = selfArg;
    return *(void **)(self + 0x5c);
}

/* Neighbor-list "set prev" mutator - writes `self+0x60`. */
void sub_8010710(void *selfArg, void *val)
{
    u8 *self = selfArg;
    *(void **)(self + 0x60) = val;
}

/* Neighbor-list "set next" mutator - writes `self+0x5c`. */
void sub_8010714(void *selfArg, void *val)
{
    u8 *self = selfArg;
    *(void **)(self + 0x5c) = val;
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * expected/*.s and src/*.c file) - trivial constant accessor, always
 * returns 3. */
u32 sub_8010718(void)
{
    return 3;
}
