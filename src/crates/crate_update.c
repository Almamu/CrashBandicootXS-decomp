#include "core.h"
#include "crate.h"
#include "crates.h"

extern void *gPaletteCache;

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see crate_reset.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). Sits between the matched
 * `DrawCrate` (crate_draw.c) and `IsCrateInsideRect` (crate.c) in
 * ROM, so it needs its own file - see docs/workflow.md's "one file
 * per contiguous ROM region" rule. `self` throughout is the same
 * "collision box" object (`struct crate`, include/crate.h) every
 * other function in this subsystem operates on.
 *
 * A per-frame state-machine tick. While `self+0x4f` (a per-object
 * throttle counter several siblings in this family also drive) is
 * nonzero, decrements it and, only for the frame it does so,
 * dispatches once more on `self+0x4e` (the settle-state byte):
 * - `0x13`-`0x15`: re-enters the edge-settle chain (`UpdateTntCountdown`),
 *   also arming the global one-shot rescan flag `gCrateListChanged`
 *   (the same flag `UpdateCrateFall`, crate_break.c, reads).
 * - `0xf`: re-triggers `UpdateSlotCrate` when `self+0x4d`'s low 7 bits
 *   are already 0.
 * - `0xc`: once `self+0x4f` has reached 0 this frame, clears
 *   `self+0x50`.
 * - `3`: re-triggers `SolidifyOutlineCrates`.
 *
 * Unconditionally afterwards: while `self+0x4e == 0xc`, counts
 * `self+0x48` down toward 0; always calls `UpdateCrateFall` (the
 * position-wrap advance). Then, if `self+0x4d`'s bit 7 is set and
 * `self+0x38` is nonzero, re-derives `self+0x30`'s index via the same
 * `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride hitbox-
 * record clamp `DrawCrate` uses, clears `self+0x38` and
 * `self+0x4d`'s bit 7, and clears the "recently touched" object's
 * (`gPlayer`) own `+0x80` byte - then, depending on
 * `self+0x4e`: state 6 settles to state 7, tags `self+0x2d = 0x20`,
 * runs the `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` triplet, then
 * folds the low nibble of a `GetPaletteSlot` tile-cache lookup (keyed by
 * the freshly-retagged hitbox record's own `+0x14`) into `self+0x29`;
 * state 3 just tags `0x20` and runs the same triplet. If bit 7 was
 * clear instead, `self+0x4d`'s low 7 bits == 1 triggers
 * `FinishBrokenCrate`. Finally, unconditionally, calls `AdvanceSpriteAnim` and
 * hands `self+0x18`'s table's own `+0x60`/`+0x64` offset/function-
 * pointer pair off to the `_call_via_r1` table-trampoline (the same
 * convention `CheckEntityPlayerContact`/`UpdateEntity`, graphics.c, establish).
 *
 * Matches under old_agbcc. The old NAKED note blamed register pressure
 * on the field addresses; the source-level causes were (see
 * docs/matching/issue-12-13-25-naked-retry.md):
 * - the 0x13-0x15 range test is two nested `if`s on an `s32` copy of
 *   `kind` (one `&&` gets folded into an unsigned subtract-and-compare;
 *   testing the u8 field directly gives unsigned branches);
 * - the `0xf` test and its `state` test are nested too (one `&&` makes
 *   gcc merge the two adjacent byte compares into one word compare);
 * - the frame clamp is the PhysSetFrame inline, taking the index as a
 *   parameter, which keeps the constant 0 in its own register (r3) for
 *   the later `animDone`/`busy` stores;
 * - the tile-cache key's record is indexed from a local copy of the
 *   records pointer, which loads the table before the tag. */
void UpdateCrate(struct crate *self)
{
    if (self->timer != 0)
    {
        self->timer--;
        {
            s32 kind = self->kind;

            if (kind > 0x12)
            {
                if (kind <= 0x15)
                {
                    UpdateTntCountdown(self);
                    gCrateListChanged = 1;
                    goto done;
                }
            }
        }
        if (self->kind == 0xf)
        {
            if ((self->state & 0x7f) == 0)
            {
                UpdateSlotCrate(self);
                goto done;
            }
        }
        if (self->kind == 0xc)
        {
            if (self->timer == 0)
                self->paramA = 0;
        }
        else if (self->kind == 3)
            SolidifyOutlineCrates(self);
    done:;
    }
    if (self->kind == 0xc && self->u48.bounceTimer > 0)
        self->u48.bounceTimer--;
    UpdateCrateFall(self);
    if (self->state & 0x80)
    {
        if (self->animDone != 0)
        {
            PhysSetFrame(self, 0);
            self->animDone = 0;
            self->state &= 0x7f;
            PHYS_PLAYER->busy = 0;
            if (self->kind == 6)
            {
                struct anim_rec *recs;
                struct anim_rec *rec;

                self->kind = 7;
                self->tag = 0x20;
                ResetSpriteFrameTimer(self);
                ResetSpriteFrameIndex(self);
                SetSpriteAnimDone(self, 0);
                recs = self->anim->records;
                rec = &recs[self->tag];
                self->slot = GetPaletteSlot(gPaletteCache, rec->paletteId);
            }
            else if (self->kind == 3)
            {
                self->tag = 0x20;
                ResetSpriteFrameTimer(self);
                ResetSpriteFrameIndex(self);
                SetSpriteAnimDone(self, 0);
            }
        }
    }
    else if ((self->state & 0x7f) == 1)
        FinishBrokenCrate(self);
    AdvanceSpriteAnim((struct box_part *)(struct gobj *)self);
    PHYS_CALL(self, m60);
}
