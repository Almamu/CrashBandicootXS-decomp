#include "core.h"
#include "match.h"
#include "actor.h"
#include "box_part.h"
#include "util.h"
#include "crates.h"
#include "crate.h"
#include "globals.h"
#include "player.h"

/* Dedicated deep investigation (docs/matching/archive/issue-9-10-0x0800ceac-graphics.md):
 * these two functions sit in the still-raw span `tools/report_units.py`
 * tracked as parked (`base_object=None`) between the just-closed
 * `PlayerAnimWouldTouchCrate` (src/crates/crate_touch.c, issue #9/#10) and the
 * already-matched `BreakCrateTouchedByPlayer` (src/crates/crate_hit.c, issue #12,
 * the physics/collision subsystem's documented entry point). Both are
 * called *only* from `QueueCratePlayerCollision` (asm/code_3_2_17_d18c.s, the
 * subsystem's ~1960-byte collision-response commit function,
 * docs/matching/archive/issue-12-physics-collision.md) - recategorized
 * `graphics` -> `game_loop` here to match that caller, the same
 * recategorization issue #12 already applied to `BreakCrateTouchedByPlayer` itself.
 *
 * `sub_800CF70` is the function `docs/rom_map.md` (line 2137) already
 * partially flagged: "144 bytes before the physics/collision
 * subsystem's stated 0x0800D000 start, calls the same linked-list
 * walkers that subsystem uses and reaches the same 28-byte-record
 * chain - functionally part of it despite sitting just outside the
 * documented boundary". Reading the real bytes confirms that note
 * exactly (see its own doc comment below) and additionally reveals its
 * sibling `sub_800CEAC`, entirely unremarked anywhere until now. */

/* `struct hitbox_quad` (gfx.h): the caller (`QueueCratePlayerCollision`)
 * passes a pointer directly to the quad itself (`playerRecordBase + 4`),
 * not the 28-byte record's own base - so this function only ever sees
 * the 4-byte-wide quad and never reads the record's own leading word. */

/* Called once by `QueueCratePlayerCollision` (its only caller), passing the
 * player's (`gPlayer`) own hitbox quad (`player's +0x20`
 * table, indexed by the player's own `+0x2d` tag, at the record's
 * `+4` quad) together with `self`'s own cached `x>>8`/`y>>8` shift
 * values and `self`'s own already-built AABB (`box`, built by the
 * caller from `self`'s own `+0x20` table at the top of `QueueCratePlayerCollision`
 * - the same convention `BreakCrateTouchedByPlayer`'s "AABB1" documents).
 *
 * Builds a *hybrid* AABB - the player's hitbox dimensions, positioned
 * at `self`'s location (`quad->offX + xOffset`, `quad->offY +
 * yOffset`) - i.e. "if a player-shaped box were standing where `self`
 * currently is". When `gPlayer+0x90` (an unconfirmed player
 * state/mode byte, not documented elsewhere under this exact offset -
 * `+0x92`/`+0x94` are separately documented state bytes right next to
 * it, see `docs/matching/archive/issue-18-0x08014f8c-actor.md`) is nonzero,
 * the box is widened by 4 (2 either side: position shifted left by 2,
 * width padded by 4) before the mirror step - a "wide mode" hitbox
 * variant.
 *
 * The hybrid box is then mirrored horizontally/vertically around
 * `(xOffset, yOffset)` according to the PLAYER's own `+0x28` mirror
 * flags (bits 4/5 - the same mirror-flag convention `BreakCrateTouchedByPlayer`
 * documents, just keyed off the player's flags instead of `self`'s,
 * since the box represents the player's shape, not `self`'s).
 *
 * Finally tests the hybrid box against `box` (`self`'s own real AABB)
 * via `AabbOverlaps` and returns the boolean overlap result: "would a
 * player-shaped hitbox at `self`'s position overlap `self`'s own
 * actual hitbox" - used by `QueueCratePlayerCollision` to decide whether to treat
 * `self` as blocking/pushing a player-sized object at that spot (its
 * caller follows a `1` result with a `GetCrateAbove`(self) "get next"
 * list-walk step, consistent with a "can something occupy this slot"
 * gate feeding further list traversal).
 *
 * `self` itself (the first argument) is loaded into a callee-saved
 * register by the ROM's own prologue but never referenced again after
 * that - genuinely unused, not a transcription slip (confirmed against
 * the raw disassembly: no further read of r1's original register
 * contents `self` was copied from).
 *
 * Real C (issue #9-#11 NAKED retry, matches under both compilers; the
 * file is built with old_agbcc for `sub_800CF70`): the wide-mode x is
 * `x += xOffset; x -= 2;` as two statements (the ROM's `adds r1, r1, r7;
 * subs r1, #2`). */
u8 sub_800CEAC(struct crate *self, struct hitbox_quad *quad, struct aabb *box,
               s32 xOffset, s32 yOffset)
{
    struct aabb b;

    /* bumped: a crate's side stopped the player, so its box is 2 px wider
     * on each side */
    if (gPlayer->bumped) {
        s32 x = quad->offX;
        s32 y = quad->offY;
        u8 h = quad->h;
        s32 w;

        x += xOffset;
        x -= 2;
        y += yOffset;
        w = quad->w + 4;
        SetAabbPos(&b, x, y);
        SetAabbSize(&b, w, h);
    } else {
        s32 x = quad->offX;
        s32 y = quad->offY;
        u8 w = quad->w;
        u8 h = quad->h;

        SetAabbPos(&b, x + xOffset, y + yOffset);
        SetAabbSize(&b, w, h);
    }
    if (gPlayer->mirror.bits.flipX)
        b.x = xOffset * 2 - (b.x + b.w);
    if (gPlayer->mirror.bits.flipY)
        b.y = yOffset * 2 - (b.y + b.h);
    if (AabbOverlaps(box, &b))
        return 1;
    return 0;
}

/* `QueueCratePlayerCollision`'s single-step neighbor probe, called while its own
 * 5-slot "recently touched" ring-buffer counter (`gPlayer
 * +0x94`-adjacent counter at the caller's own stack cache) is `<= 4`
 * (confirmed at the call site: `cmp r3,#4; bgt` skips the call
 * entirely and uses `self` directly instead).
 *
 * Reads `self`'s doubly-linked neighbor pointers both ways
 * (`GetCrateAbove` = "get next", `GetCrateBelow` = "get prev" - the
 * established pair, see `src/crates/crate_break.c`). If *neither*
 * exists, returns `self` unchanged with no other side effect - `self`
 * is isolated in the list.
 *
 * Otherwise (at least one neighbor exists) sets `*foundFlag = 1` - an
 * out-param the caller pre-loads with its own edge-code byte before
 * the call and only overwrites when a neighbor is actually found.
 *
 * If there is no "prev" neighbor, or `prev`'s own `+0x4d & 0x7f`
 * state byte reads `1` (the same early-out gate `BreakCrateTouchedByPlayer`'s own
 * header documents - excluded from the physics AABB tests entirely),
 * returns `self` unchanged.
 *
 * Otherwise builds `prev`'s own AABB from the shared `self+0x20`
 * table (indexed by `prev`'s own `+0x2d` tag, quad at record `+4`,
 * `+4`/`+6`/`+8`/`+9` layout) offset by `prev.x>>8`/`prev.y>>8` and
 * mirrored per `prev`'s own `+0x28` flags - the exact "AABB1" shape
 * `BreakCrateTouchedByPlayer`'s header already documents at length, just for `prev`
 * instead of `self`. Tests it against the caller-supplied `box` via
 * `AabbOverlaps`; on overlap, returns `prev` instead of `self` - so the
 * caller can substitute the actual colliding neighbor in place of the
 * object it started the probe from.
 *
 * This confirms and sharpens `docs/rom_map.md`'s existing note (line
 * 2137): it doesn't just "call the same linked-list walkers" and
 * "reach the same 28-byte-record chain" in passing - the entire
 * function body *is* one AABB-build-and-overlap-test cycle from that
 * subsystem, gated by the subsystem's own `+0x4d` state-exclusion
 * convention, operating on the "prev" neighbor specifically. It is
 * "functionally part of" the physics/collision subsystem in the
 * strongest sense: same table convention, same early-out gate, same
 * overlap primitive, same caller.
 *
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the `0x7f` mask
 * loaded before the `ldrb` is old_agbcc's tell). `self` goes through a
 * local copy of the parameter: that is what makes the ROM copy r0 last,
 * after `box`/`foundFlag`, right before the first call. The mirror bits
 * read are `self`'s, not `prev`'s. */
struct crate *sub_800CF70(struct crate *selfArg, struct aabb *box, u8 *foundFlag)
{
    struct crate *self = selfArg;
    struct crate *next = GetCrateAbove(self);
    struct crate *prev = GetCrateBelow(self);

    if (next == NULL && prev == NULL)
        return self;
    *foundFlag = 1;
    if (prev == NULL || (prev->state & 0x7f) == 1)
        return self;
    {
        u8 *rec = (u8 *)&prev->anim->records[prev->tag];
        struct hitbox_quad *q = (struct hitbox_quad *)(rec + 4);
        struct aabb b;
        s32 px = prev->x >> 8;
        s32 py = prev->y >> 8;
        s32 x = q->offX;
        s32 y = q->offY;
        u8 w = q->w;
        u8 h = q->h;

        SetAabbPos(&b, x + px, y + py);
        SetAabbSize(&b, w, h);
        if (self->flipX)
            b.x = px * 2 - (b.x + b.w);
        if (self->flipY)
            b.y = py * 2 - (b.y + b.h);
        if (AabbOverlaps(&b, box))
            self = prev;
    }
    return self;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

/* GitHub issue #12: 0x0800D040-0x0800FC70, the physics/collision
 * subsystem documented in docs/rom_map.md ("Confirmed: a shared
 * physics/collision subsystem, entered from multiple different entity
 * types"). This first function of that subsystem sits right after
 * already-matched `game_loop` code - `QueueCratePlayerCollision` and `ApplyCrateCollision`
 * immediately after it are two of the subsystem's largest, most
 * tangled functions and are left untouched for now; see
 * docs/matching/archive/issue-12-physics-collision.md. */

/* Builds two AABBs - one for `self`, one for the player
 * (`gPlayer`) - from the shared "keyframe/hitbox record"
 * table convention already established by `GetSpriteBounds`/`GetSpriteHitbox`
 * in sprite.c (`self+0x20` -> a pointer-to-table, indexed by
 * `self+0x2d` at 0x1c/28-byte stride; here the {s16 offX, s16 offY, u8
 * w, u8 h} quad sits at the record's `+4`/`+6`/`+8`/`+9` instead of
 * `+0xc`/`+0xe`/`+0x10`/`+0x11`, the same "differently laid out"
 * variance `GetSpriteHitbox`'s doc comment already flags). `self+0x28`
 * bits 4/5 mirror each box horizontally/vertically around its own
 * object's position, exactly like the `sprite.c` pair. If the two
 * boxes overlap (`AabbOverlaps`), dispatches to `ExplodeCrate` or
 * `BreakCrateInStack` depending on a per-state-id lookup in
 * `gCrateKindExplosive`.
 *
 * Early-outs entirely when `self+0x4d & 0x7f == 1`.
 *
 * Real C (issue #12 NAKED retry, old_agbcc - this object is on the
 * Makefile's OLD_AGBCC_OBJS). The two boxes are one frame struct, and
 * `px`/`py` are shared by both blocks (the ROM keeps them in r7/r8 for
 * both). The ROM recomputes the player box's address (`add r0/r1, sp,
 * #16`) at each of its three uses; with plain `&f.b`, cse and gcse turn
 * them into one pseudo held in a callee-saved register across both
 * builder calls, which shifts px/py/&gPlayer up a register.
 * `BOX_ADDR` passes each use through MATCH_KEEP, which
 * hides the value from cse, so each is its own single-use pseudo that
 * combine folds into the `add` right before the call. The first build's
 * x/y are computed first so that the `add r0, sp, #16` comes after them.
 * The same fix closed `PlayerAnimWouldTouchCrate` (crate_touch.c); see
 * docs/matching/archive/sp-box-retry.md. */

void BreakCrateTouchedByPlayer(struct box_part *self)
{
    struct {
        struct aabb a;
        struct aabb b;
    } f;
    s32 px;
    s32 py;

    if ((self->physMode & 0x7f) == 1)
        return;
    {
        u8 *rec;
        struct hitbox_quad *pb;
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        rec = (u8 *)&(*self->keyframes)[self->frame];
        pb = (struct hitbox_quad *)(rec + 4);
        px = self->x >> 8;
        py = self->y >> 8;
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (self->mirrorX)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (self->mirrorY)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    {
        u8 *rec;
        struct hitbox_quad *pb;
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = gPlayer->x >> 8;
        py = gPlayer->y >> 8;
        rec = (u8 *)&gPlayer->anim->records[gPlayer->tag];
        pb = (struct hitbox_quad *)(rec + 4);
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        {
            s32 x = offX + px, y = offY + py;
            SetAabbPos(BOX_ADDR(&f.b), x, y);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirror.bits.flipX)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirror.bits.flipY)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    if (AabbOverlaps(&f.a, BOX_ADDR(&f.b)))
    {
        if (gCrateKindExplosive[self->state] == 1)
            ExplodeCrate((struct crate *)self, 1);
        else
            BreakCrateInStack((struct crate *)self, 0, 0, 0);
    }
}

/* The object ends word-aligned with zero fill, as the ROM does. */
asm(".align 2, 0");
