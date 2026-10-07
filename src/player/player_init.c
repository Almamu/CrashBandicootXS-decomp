#include "core.h"
#include "match.h"
#include "actor.h"
#include "player.h"
#include "objects.h"
#include "gfx_part.h"
#include "globals.h"
#include "math_util.h"

/* GitHub issue #9/#10: 0x0800B3F0 - the player object's constructor
 * (`struct player`, player.h). */

/* Re-initializes `self` (via `InitGroundSprite`, already matched in
 * `ground_sprite.cpp`), then overwrites its table with
 * `gPlayerVtable` and clears its trailing `+0x108`/`+0x10c`
 * fields via `ResetCollisionQueue` (still raw - a two-field, 4-byte-plus-byte
 * clear). Allocates a fresh `struct actor`-shaped child object
 * (`CreateSpriteObj(0, 0, 0, 0)`, the same allocator `sprite_obj.cpp`'s
 * `CreateSpriteObj` is - called here with an extra, unused 4th zero
 * argument, the same calling convention already used by
 * `spawn_pickups.c`'s own callers of it) and hooks it up at
 * `self+0xb0`: points its own `+0x20` table-entry pointer at the
 * `gSpriteBankSet` shared table's `(0xcc << 1)` slot (the same
 * idiom `spawn_pickups.c` uses throughout), clears its
 * `+0x2d` byte, and builds it via the standard `ResetSpriteFrameTimer`/
 * `ResetSpriteFrameIndex`/`SetSpriteAnimDone` OAM trio. Clears `self+0xb4`, then
 * calls `ResetPlayer` (matched in `player_reset.c`) to finish resetting
 * `self`'s velocity/state fields and hook the `self+0xb0` child up via
 * its own `GetSpriteAnimPaletteSlot` call. Finally sets `id` (`self+8`) and
 * the Q8 `x`/`y` position from the three `u16` arguments - the same
 * tail `CreateGroundSprite` (`ground_sprite.cpp`) uses for its own, smaller
 * `struct actor` - and returns `self`. */
struct player *InitPlayer(struct player *self, u16 arg1, u16 arg2, u16 arg3, u16 unused)
{
    struct gfx_part *child;

    InitGroundSprite((struct actor *)self);
    self->vtable = (const struct player_vtable *)gPlayerVtable;
    ResetCollisionQueue(&self->collisionQueue);

    child = CreateSpriteObj(0, 0, 0, 0);
    self->child = (struct box_part *)child;
    child->bank = (struct anim_bank *)(SPRITE_BANK_BASE + (0xcc << 1));

    /* Register-pinned: the ROM keeps this `0` constant alive in `sl`
     * across all three `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` calls
     * (a callee-saved register survives a `bl`) so it can reuse the
     * same value for `self+0xb4` afterward, instead of reloading a
     * fresh `0` there - a plain `0` literal for both stores lets this
     * compiler fold each into its own cheap `movs`/immediate instead,
     * needing only 2 (not 3) high registers preserved across the whole
     * function. `SetSpriteAnimDone`'s own `0` argument is a separate, fresh
     * literal in the ROM too (`movs r1, #0`), not sourced from `sl`. */
    {
        MATCH_HOLD_REG(s32, zero, sl) = 0;

        /* a retyped store: through the member, gcc stores a 0 it has in
         * r0 instead of `sl` */
        *(u8 *)&child->tag = zero;
        ResetSpriteFrameTimer(child);
        ResetSpriteFrameIndex(child);
        SetSpriteAnimDone(child, 0);

        self->maskTrailIdx = zero;
    }
    ResetPlayer(self);

    self->id = arg1;
    self->x = INT_TO_Q8((s32)arg2);
    self->y = INT_TO_Q8((s32)arg3);

    return self;
}
