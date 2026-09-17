#include "core.h"
#include "actor.h"

/* This file's `self` is a bigger, still-unnamed object (at least
 * 0x108 bytes) distinct from `struct actor` - most of these
 * functions are pure single-field get/set/increment/clear accessors
 * for it, so raw offset casts are used throughout rather than a named
 * struct, since most individual fields' real meaning isn't confirmed
 * beyond "a byte/word at this offset". */

extern s32 sub_800A528(void *self);
extern void *sub_8007CF8(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);
extern u8 gStaticData_087E3E04[];
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern void sub_8010E14(void *arg0, s32 arg1);
extern void sub_800A650(void *self, u32 unusedArg);

/* `self+0x5c` boolean getter (nonzero -> 1). */
u8 sub_800B324(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0x5c) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/* `self+0x64` clear. */
void sub_800B334(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x64) = 0;
}

/* Clamps `self+0x64`/`self+0x54`/`self+0x58` to `<= 0`. */
void sub_800B33C(void *selfArg)
{
    register u8 *self asm("r1") = selfArg;

    if (*(s32 *)(self + 0x64) > 0) {
        *(s32 *)(self + 0x64) = 0;
    }
    if (*(s32 *)(self + 0x54) > 0) {
        *(s32 *)(self + 0x54) = 0;
    }
    if (*(s32 *)(self + 0x58) > 0) {
        *(s32 *)(self + 0x58) = 0;
    }
}

/* Decrements the `self+0x91` countdown byte (if nonzero), then tail-
 * calls `sub_800A528` (still raw, in the sub_800A0FC-sub_800A590
 * span). */
void sub_800B360(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x91] != 0) {
        self[0x91] -= 1;
    }
    sub_800A528(selfArg);
}

/* The `gUnknown_030012D8` collision check used throughout this whole
 * session (`sub_8009CA0`/`sub_80096C0`/`sub_80099F0` etc all call
 * this by name via an `extern` declaration, finally matched for
 * real): builds `selfArg`'s secondary AABB via `sub_8007CF8`
 * (already matched), and - only if it has a region (`field_8 > 0`) -
 * tests it against `buf` via `sub_8001688` (already matched),
 * returning the low byte of that result; otherwise returns 0. */
u8 sub_800B37C(void *selfArg, void *buf)
{
    s32 tmp[4];
    u8 result = 0;

    sub_8007CF8(tmp, selfArg);
    if (tmp[2] > 0) {
        result = sub_8001688(tmp, buf);
    }
    return result;
}

/* Overwrites `self->table` with `gStaticData_087E3E04`, then (if
 * `self+0xb0`'s child object is set) fires its `table+0x50/0x54`-
 * driven trampoline via `sub_803AD80` with constant arg `3`, then
 * calls `sub_8010E14(self+0x108, 2)` and tail-calls `sub_800A650`
 * (already matched in `actor_part14.c`). */
void sub_800B3AC(void *selfArg, u32 arg1)
{
    u8 *self = selfArg;

    *(void **)(self + 0x18) = gStaticData_087E3E04;
    {
        void *rec = *(void **)(self + 0xb0);

        if (rec != 0) {
            u8 *tbl = *(u8 **)((u8 *)rec + 0x18) + 0x50;
            s16 offset = *(s16 *)tbl;
            void *addr = (u8 *)rec + offset;
            void *fn = *(void **)(tbl + 4);

            sub_803AD80(addr, (void *)3, fn);
        }
    }
    sub_8010E14(self + 0x108, 2);
    sub_800A650(self, arg1);
}
