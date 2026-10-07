#include "core.h"
#include "match.h"
#include "bosses.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "gobj_1a794.h"
#include "math_util.h"

/* GitHub issue #22, ROM 0x080187FC-0x08018884 - non-adjacent to
 * airship_fireball.c since the raw `UpdateTiny`/`SetTinyState`/
 * `PickTinyHopTarget`/`SpawnTinyFallingLeaves` block sits between them (see
 * asm/code_3_2_17_18008.s). `obj` is a plain InitCtrl controller
 * (`struct gfx_ctrl`, bosses.h) whose `state` counts 0/1/2; `other` is
 * the sprite object it controls (`struct gobj`, gobj_1a794.h). */

/* `_call_via_r3` is declared by gobj_1a794.h. */

/* A two-state (`obj->state`: 0 then 1 then 2) "charge" handler. State 0
 * calls method_50 (SetCtrlTargetAnim) with animation 8 and advances to
 * state 1. State 1 adds `0x400` per call to `other->y` until it reaches
 * `(layer 0's heightPx << 8) + 0x2000`, then advances to state 2 (a "fully charged" terminal
 * state this function no longer touches).
 *
 * Written with explicit `goto`s (see docs/workflow.md's "force block
 * ordering" note) since a plain `if (state == 1) {...} else if (state
 * > 1) {} else if (state == 0) {...}` compiles correctly but with the
 * state-1/state-0 bodies physically swapped from the ROM's actual
 * layout - this compiler inverts the branch and reorders the blocks
 * even though nothing about that changes the emitted-instruction count
 * or the C's meaning. */
void UpdateStompedHopPad(void *objArg, void *otherArg)
{
    MATCH_HOLD_REG(struct gfx_ctrl *, obj, r3) = objArg;
    MATCH_HOLD_REG(struct gobj *, other, r2) = otherArg;
    s32 state = obj->state;

    if (state == 1)
        goto case1;
    if (state > 1)
        goto end;
    if (state != 0)
        goto end;

    {
        struct actor_method *method;
        s16 offset;
        void *addr;
        void *fn;

        obj->state = 1;
        method = &obj->vtable->method_50;
        offset = method->thisOffset;
        addr = (u8 *)obj + offset;
        fn = method->fn;
        _call_via_r3(addr, other, 8, fn);
    }
    goto end;

case1:
    {
        s32 timer = other->y + 0x400;
        struct bg_scroll_layer *subObj;
        s32 threshold;

        other->y = timer;
        subObj = gLevelLayers->layer0;
        threshold = INT_TO_Q8(subObj->heightPx) + 0x2000;
        if (timer >= threshold) {
            obj->state = 2;
        }
    }
end:;
}

/* Sets the method table to `gStompedHopPadVtable`, then
 * tail-calls `DestroyCtrl` - same double-set pattern as
 * `DestroyBossCtrl`/`DestroyMegaMixCtrl`. */
void DestroyStompedHopPadCtrl(void *selfArg, s32 flags)
{
    struct gfx_ctrl *self = selfArg;

    self->vtable = (struct gfx_vtable *)gStompedHopPadVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl`, then re-points the table at
 * `gStompedHopPadVtable`. Returns `self`. */
void *CreateStompedHopPadCtrl(void *selfArg)
{
    struct gfx_ctrl *self = selfArg;

    InitCtrl(self);
    self->vtable = (struct gfx_vtable *)gStompedHopPadVtable;
    return self;
}

/* While `other->animDone` is set: ORs bit 0 into `other->flags`, then
 * (unless `other->id` is the sentinel `0xFFFF`) sets bit `id` in
 * `gEntityFlags->bits0Copy` - the same bitmap-set idiom `CheckSpritePickup` uses via
 * `part->field_08`. The first argument is taken but never read
 * anywhere in this function's ROM body.
 *
 * Needs several `MATCH_HOLD_REG` pins (matching the ROM's own
 * register choices) plus a `volatile` reload of `other->id` and one raw
 * `asm` for the index shift - without them this compiler happily
 * proves `id`'s zero-extended value never has its top bit set and
 * folds the ROM's `asrs`/`adds` shift-setup pair into a single `lsr`,
 * and CSEs away the ROM's second, seemingly redundant `ldrh` reload of
 * the same address (needed there only because the ROM's register
 * allocator picks a different register for the value on each side of
 * the branch). */
void UpdateOneShotAnimCtrl(void *unusedArg, void *otherArg)
{
    MATCH_HOLD_REG(struct gobj *, other, r1) = otherArg;

    (void)unusedArg;

    if (other->animDone != 0) {
        MATCH_HOLD_REG(s32, one, r0) = 1;
        MATCH_HOLD_REG(u8, flags, r2) = other->flags;

        one |= flags;
        other->flags = one;

        ENTITY_SET_GONE_BIT_ASR(other, r4);
    }
}
