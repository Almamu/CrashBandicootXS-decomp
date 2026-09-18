#include "core.h"
#include "actor.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). */

extern u8 gStaticData_087E4074[];
extern void sub_8026EB4(void *ptr);
extern void sub_8008484(struct actor *self, u32 arg1);

/* Sets `self->table`, then - if `self`'s own `+0x4e` state byte is 3 -
 * frees `self+0x48` (a heap pointer, unless it's the sentinel `-1` or
 * already `NULL`) and clears `self+0x59`, before tail-calling
 * `sub_8008484` (already matched, `actor_part6.c`) - same table-set/
 * tail-call shape as `sub_80119D8` (`actor_part39.c`). */
void sub_801071C(struct actor *self, u32 arg1)
{
    self->table = gStaticData_087E4074;

    if (*((u8 *)self + 0x4e) == 3) {
        void *p = *(void **)((u8 *)self + 0x48);
        if ((u32)((u8 *)p + 1) > 1) {
            if (p != NULL) {
                sub_8026EB4(p);
            }
            *((u8 *)self + 0x59) = 0;
        }
    }

    sub_8008484(self, arg1);
}

extern struct actor *sub_80084A4(struct actor *self);
extern void sub_800FEB0(void *selfArg);

/* Re-initializes `self` via `sub_80084A4` (already matched,
 * `actor_part6.c`), sets `self->table`, clears `self+0x59`, then
 * resets `self`'s own collision-response state via `sub_800FEB0`
 * (`game_loop22.c`). */
struct actor *sub_801075C(struct actor *self)
{
    sub_80084A4(self);
    self->table = gStaticData_087E4074;
    *((u8 *)self + 0x59) = 0;
    sub_800FEB0(self);
    return self;
}

/* Bresenham-line-style step algorithm (see `sub_800FDC8`,
 * game_loop29's neighbor file, for the general 4-octant version this
 * is a fixed single-octant variant of): walks `dy` steps, accumulating
 * `count`; whenever the running error term `err` is non-negative,
 * steps `y` by `yStep` and folds the error by `diff = 2*dx - 2*dy`
 * (returning the *current* `count` immediately, without incrementing
 * it, the moment `y` reaches `bound`), otherwise just folds by
 * `2*dx`. Returns `-1` if the walk completes all `dy` steps without
 * `y` ever reaching `bound`. */
s32 sub_8010784(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDx = dx * 2;
    s32 diff = twoDx - dy * 2;
    s32 err = twoDx - dy;
    s32 n = dy - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                y += yStep;
                if (y >= bound) {
                    return count;
                }
                err += diff;
            } else {
                err += twoDx;
            }
            count++;
            n--;
        } while (n != -1);
    }
    return -1;
}

/* Same shape as `sub_8010784`, but stepping `x` (2nd count-position
 * pair swapped relative to that function) instead of `y` - walks `dx`
 * steps this time, checking `y >= bound` each time the error term
 * folds. */
s32 sub_80107C4(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound)
{
    s32 twoDy = dy * 2;
    s32 diff = twoDy - dx * 2;
    s32 err = twoDy - dx;
    s32 n = dx - 1;

    if (n != -1) {
        do {
            if (err >= 0) {
                count++;
                err += diff;
            } else {
                err += twoDy;
            }
            y += yStep;
            if (y >= bound) {
                return count;
            }
            n--;
        } while (n != -1);
    }
    return -1;
}
