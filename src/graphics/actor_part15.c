#include "core.h"
#include "actor.h"
#include "gobj_1a794.h"

/* This file's `self` is a bigger object (at least 0x108 bytes)
 * distinct from `struct actor` - the level-object layout `struct gobj`
 * (gobj_1a794.h) describes, which the player object shares. Most of
 * these functions are pure single-field get/set/increment/clear
 * accessors for it; fields `struct gobj` doesn't cover yet stay as
 * byte offsets. */

extern s32 UpdateGroundSprite(void *self);
extern void *GetSpriteBodyBox(void *dest, void *pt);
extern u8 gPlayerVtable[];
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern void DestroyCollisionQueue(void *arg0, s32 arg1);
extern void DestroyGroundSprite(void *self, u32 unusedArg);

/* `velB.z` (+0x5c) boolean getter (nonzero -> 1). */
u8 sub_800B324(void *selfArg)
{
    struct gobj *self = selfArg;

    if (self->velB.z != 0) {
        return 1;
    } else {
        return 0;
    }
}

/* `speedY` (+0x64) clear. */
void ClearPlayerSpeedY(void *selfArg)
{
    struct gobj *self = selfArg;
    self->speedY = 0;
}

/* Clamps `speedY`/`velB.x`/`velB.y` (+0x64/+0x54/+0x58) to `<= 0`. */
void sub_800B33C(void *selfArg)
{
    register struct gobj *self asm("r1") = selfArg;

    if (self->speedY > 0) {
        self->speedY = 0;
    }
    if (self->velB.x > 0) {
        self->velB.x = 0;
    }
    if (self->velB.y > 0) {
        self->velB.y = 0;
    }
}

/* Decrements the `self+0x91` countdown byte (if nonzero), then tail-
 * calls `UpdateGroundSprite` (still raw, in the CollideGroundSprite-sub_800A590
 * span). */
void UpdatePlayer(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x91] != 0) {
        self[0x91] -= 1;
    }
    UpdateGroundSprite(selfArg);
}

/* The `gPlayer` collision check used throughout this whole
 * session (`CheckPlayerContact`/`CollideCrateGridPartWithPlayer`/`CollideCrateGridPartWithObject` etc all call
 * this by name via an `extern` declaration, finally matched for
 * real): builds `selfArg`'s secondary AABB via `GetSpriteBodyBox`
 * (already matched), and - only if it has a region (`field_8 > 0`) -
 * tests it against `buf` via `AabbOverlaps` (already matched),
 * returning the low byte of that result; otherwise returns 0. */
u8 PlayerTouchesBox(void *selfArg, void *buf)
{
    s32 tmp[4];
    u8 result = 0;

    GetSpriteBodyBox(tmp, selfArg);
    if (tmp[2] > 0) {
        result = AabbOverlaps((struct aabb *)tmp, buf);
    }
    return result;
}

/* Overwrites `self->table` with `gPlayerVtable`, then (if
 * `self+0xb0`'s child object is set) fires its `table+0x50/0x54`-
 * driven trampoline via `_call_via_r2` with constant arg `3`, then
 * calls `DestroyCollisionQueue(self+0x108, 2)` and tail-calls `DestroyGroundSprite`
 * (already matched in `actor_part14.c`). */
void DestroyPlayer(void *selfArg, u32 arg1)
{
    u8 *self = selfArg;

    *(void **)(self + 0x18) = gPlayerVtable;
    {
        void *rec = *(void **)(self + 0xb0);

        if (rec != 0) {
            u8 *tbl = *(u8 **)((u8 *)rec + 0x18) + 0x50;
            s16 offset = *(s16 *)tbl;
            void *addr = (u8 *)rec + offset;
            void *fn = *(void **)(tbl + 4);

            _call_via_r2(addr, (void *)3, fn);
        }
    }
    DestroyCollisionQueue(self + 0x108, 2);
    DestroyGroundSprite(self, arg1);
}
