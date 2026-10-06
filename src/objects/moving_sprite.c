#include "core.h"
#include "match.h"
#include "actor.h"
#include "gfx_part.h"
#include "objects.h"
#include "memory.h"
#include "gobj_1a794.h"

/* A velocity/position integrator: for each axis (`speedX` with `rampX`,
 * `speedY` with `rampY`), steps the speed toward the ramp's `target` by
 * its `step`, clamping so it never overshoots. Builds the direction byte
 * `dir` from the sign of each clamped speed (1=right, 2=left, 8=down,
 * 4=up, OR'd together, matching this ROM's earlier
 * `GetSpriteBounds`-style mirror-flag bit encoding). Caches the pre-move
 * position in `prevX`/`prevY` (read back by `GetSpritePrevPos`/
 * `GetSpritePrevY`/`GetSpritePrevX`), then applies the clamped speed to
 * `x`/`y`. Finally updates the global `gLastSpriteVelY`
 * with the Y velocity (a redundant-looking early write of 0 happens
 * only on the path where the Y velocity is already 0, so it's a
 * genuine no-op preserved as found) and returns whether either axis
 * is still moving.
 *
 * Fully matched as real C, closed using the exact fix worked out for
 * its near-identical twin `ApplyPlayerVelocity` (`docs/matching/archive/issue-9-0x08007634-actor.md`):
 * same per-axis clamp structure, `self` pinned to `r2`, `vs32`-forced
 * reloads for the ROM's own redundant `self->x`/`self->y` re-reads,
 * `vx` pinned to `r3` while `vy` stays an unpinned local (it lands in
 * `r1` naturally - pinning both at once is the same gcc-2.9
 * register-pin miscompile documented for `ApplyPlayerVelocity`). The trailing
 * `gLastSpriteVelY` block's "genuinely redundant" conditional store
 * gets proven dead by this compiler regardless of C-level phrasing, so
 * it's emitted verbatim via one opaque `asm volatile` block instead,
 * reproducing the ROM's own address-in-`r0`/value-in-`r2` register
 * choice directly. */
s32 ApplySpriteVelocity(void *arg0)
{
    MATCH_HOLD_REG(struct gobj *, w, r2) = arg0;
    MATCH_HOLD_REG(u8 *, flags, r1);
    s32 fx, fy;

    {
        MATCH_HOLD_REG(s32, v, r1) = w->speedX;
        MATCH_HOLD_REG(s32, target, r3) = w->rampX.target;

        if (v >= target)
            goto case1_ge;
        {
            s32 step = w->rampX.step;
            MATCH_HOLD_REG(s32, result, r0) = v + step;
            w->speedX = result;
            if (result <= target)
                goto case1_done;
            goto case1_clamp;
        }
    case1_ge:
        if (v <= target)
            goto case1_done;
        {
            s32 step = w->rampX.step;
            MATCH_HOLD_REG(s32, result, r0) = v - step;
            w->speedX = result;
            if (result >= target)
                goto case1_done;
        }
    case1_clamp:
        w->speedX = target;
    case1_done:;
    }

    {
        MATCH_HOLD_REG(s32, v, r1) = w->speedY;
        MATCH_HOLD_REG(s32, target, r3) = w->rampY.target;

        if (v >= target)
            goto case2_ge;
        {
            s32 step = w->rampY.step;
            MATCH_HOLD_REG(s32, result, r0) = v + step;
            w->speedY = result;
            if (result <= target)
                goto case2_done;
            goto case2_clamp;
        }
    case2_ge:
        if (v <= target)
            goto case2_done;
        {
            s32 step = w->rampY.step;
            MATCH_HOLD_REG(s32, result, r0) = v - step;
            w->speedY = result;
            if (result >= target)
                goto case2_done;
        }
    case2_clamp:
        w->speedY = target;
    case2_done:;
    }

    flags = &w->dir;
    *flags = 0;

    fx = w->speedX;
    if (fx > 0)
        *flags = 1;
    else if (fx < 0)
        *flags = 2;

    fy = w->speedY;
    {
        MATCH_HOLD_REG(s32, mask, r0);
        if (fy > 0) {
            mask = 8;
        } else if (fy < 0) {
            mask = 4;
        } else {
            goto skipY;
        }
        mask = mask | *flags;
        *flags = mask;
    }
skipY:

    {
        s32 x0 = w->x;
        s32 y0 = w->y;
        w->prevX = x0;
        w->prevY = y0;
    }
    {
        s32 x = *(vs32 *)&w->x;
        MATCH_HOLD_REG(s32, vx, r3) = w->speedX;
        x = x + vx;
        w->x = x;
        {
            s32 y = *(vs32 *)&w->y;
            s32 vy = w->speedY;
            y = y + vy;
            w->y = y;

            {
                MATCH_HOLD_REG(vs32 *, g, r0) = &gLastSpriteVelY;

                // clang-format off
                asm volatile(
                    "ldr r2, [%0, #0]\n\t"
                    "cmp r2, #0\n\t"
                    "beq 1f\n\t"
                    "cmp %1, #0\n\t"
                    "bne 1f\n\t"
                    "str %1, [%0, #0]\n\t"
                    "1:\n\t"
                    "str %1, [%0, #0]\n\t"
                    :
                    : "r"(g), "r"(vy)
                    : "r2", "cc", "memory"
                );
                // clang-format on
            }

            return (vx != 0 || vy != 0);
        }
    }
}

/* `prevX`/`prevY` (previous position, cached by `ApplySpriteVelocity` above)
 * get/set accessors. */
void SetSpritePrevPos(struct gfx_part *self, s32 x, s32 y)
{
    self->prevX = x;
    self->prevY = y;
}

void GetSpritePrevPos(struct gfx_vec *dest, struct gfx_part *self)
{
    s32 y = self->prevY;
    s32 x = self->prevX;
    dest->x = x;
    dest->y = y;
}

/* Q8-to-integer converters for the same previous-position fields. */
s32 GetSpritePrevY(struct gfx_part *self)
{
    return self->prevY >> 8;
}

s32 GetSpritePrevX(struct gfx_part *self)
{
    return self->prevX >> 8;
}

/* Constant-5 stub. */
s32 GetMovingSpriteClassId(void)
{
    return 5;
}

/* Same shape as `CreateSpriteObj`/`CreateMovingSprite`'s siblings elsewhere in
 * this ROM region: allocates a bigger (0x78-byte) part-object,
 * initializes it via `InitSpriteObj`, overwrites its table with
 * `gMovingSpriteVtable`, clears its extra fields via `ResetMovingSprite`
 * (see below), then sets `field_08` and the Q8 `x`/`y` position from
 * the three `u16` arguments. */
void *CreateMovingSprite(u16 arg0, u16 arg1, u16 arg2, u16 unused)
{
    struct actor *part = OperatorNew(0x78);

    InitSpriteObj(part);
    part->table = (void *)gMovingSpriteVtable;
    ResetMovingSprite(part);
    part->id = arg0;
    part->x = (s32)arg1 << 8;
    part->y = (s32)arg2 << 8;
    return part;
}

/* Overwrites `self->table`, then (if `self`'s mover is set) calls the
 * mover's destructor (`destroy`) with a constant argument `3` via
 * `_call_via_r2` (same convention as
 * `UpdatePartList`/`IsEntityNearCamera`), and finally tail-calls `DestroySpriteObj`
 * (already matched in `sprite_obj.c`). The trampoline's `addr =
 * rec + offset` needed computing before the `fn` load (reusing
 * `rec`'s own dying register), matching the accumulator-register
 * pattern used throughout this ROM region - computing them in the
 * opposite order aliases `rec` and `fn` onto the same register and
 * silently corrupts the address. */
void DestroyMovingSprite(struct actor *self, u32 arg1)
{
    self->table = (void *)gMovingSpriteVtable;

    {
        MATCH_HOLD_REG(struct mover *, rec, r2) = ((struct gobj *)self)->mover;

        if (rec != 0) {
            MATCH_HOLD_REG(struct actor_method *, m, r1) = &rec->vtable->destroy;
            MATCH_HOLD_REG(s32, offset, r0) = m->thisOffset;
            MATCH_HOLD_REG(void *, addr, r0);
            MATCH_HOLD_REG(void *, fn, r2);

            addr = (u8 *)rec + offset;
            fn = m->fn;
            _call_via_r2(addr, (void *)3, fn);
        }
    }

    DestroySpriteObj(self, arg1);
}

/* Part-object field clearer/initializer, called from every
 * `CreateMovingSprite`-family constructor above and below. Sets `flags` bit
 * 6, clears `flags2` bit 3 (same `-9` mask trick as `ClearPartSolid`),
 * zeroes the speeds and speed ramps `ApplySpriteVelocity` steps plus
 * `dir`/`mover`/`unk_40`, sets `hitAxes` to 8 and clears `probeTries`. */
void ResetMovingSprite(void *selfArg)
{
    struct gobj *self = selfArg;

    /* The two flag stores are retyped stores: as plain member stores the
     * zero for the fields below is loaded above the first one. */
    {
        MATCH_HOLD_REG(s32, mask, r0) = 0x40;
        MATCH_HOLD_REG(s32, byte, r1) = self->flags;
        MATCH_HOLD_REG(s32, result, r0);

        result = mask | byte;
        *(u8 *)&self->flags = result;
    }
    {
        MATCH_HOLD_REG(s32, mask, r0) = -9;
        MATCH_HOLD_REG(s32, byte, r1) = self->flags2;
        MATCH_HOLD_REG(s32, result, r0);

        result = mask & byte;
        *(u8 *)&self->flags2 = result;
    }

    self->speedX = 0;
    self->speedY = 0;
    self->rampX.start = 0;
    self->rampX.step = 0;
    self->rampX.target = 0;
    self->rampY.start = 0;
    self->rampY.step = 0;
    self->rampY.target = 0;
    self->hitAxes = 8;
    self->dir = 0;
    self->probeTries = 0;
    self->mover = 0;
    self->unk_40 = 0;
}

/* Same `InitSpriteObj`/table-swap/`ResetMovingSprite` shape as `CreateMovingSprite`
 * above, but re-initializes an existing `part` instead of allocating
 * a new one - the same relationship `InitSpriteObj` itself has to
 * `CreateSpriteObj`. */
struct actor *InitMovingSprite(struct actor *part)
{
    InitSpriteObj(part);
    part->table = (void *)gMovingSpriteVtable;
    ResetMovingSprite(part);
    return part;
}

/* Calls `UpdateSpriteObj` (already matched in `sprite_obj.c`), then (if
 * `self`'s mover is set) calls the mover's `m08` method (its per-frame
 * update) via `_call_via_r2` with `self` itself as the second
 * argument. Same `addr`-before-`fn` ordering fix as `DestroyMovingSprite`
 * above. */
void UpdateMovingSprite(struct actor *self)
{
    MATCH_HOLD_REG(struct mover *, rec, r2) = ((struct gobj *)self)->mover;

    UpdateSpriteObj(self);
    rec = ((struct gobj *)self)->mover;
    if (rec != 0) {
        MATCH_HOLD_REG(struct mover_vtable *, tbl, r1) = rec->vtable;
        MATCH_HOLD_REG(s32, offset, r0) = tbl->m08.thisOffset;
        MATCH_HOLD_REG(void *, addr, r0);
        MATCH_HOLD_REG(void *, fn, r2);
        MATCH_HOLD_REG(void *, arg1, r1);

        addr = (u8 *)rec + offset;
        fn = tbl->m08.fn;
        arg1 = self;
        _call_via_r2(addr, arg1, fn);
    }
}
asm(".align 2, 0");
