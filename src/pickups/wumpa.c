#include "core.h"
#include "match.h"
#include "actor.h"
#include "pickups.h"
#include "player.h"
#include "objects.h"
#include "memory.h"
#include "globals.h"
#include "level.h"

/* Calls `DrawSprite` (already matched in `sprite.c`) with the
 * global `gSpriteRenderer` as `self` - same tail-call shape as
 * `DrawSpriteObj` (`sprite_obj.c`), but here as a leading step rather
 * than the whole body. If `part+0x38` (a field not otherwise
 * characterized yet in this ROM region) is nonzero, also clears
 * `part->flags` bit 3 - written as `& -9` (the established
 * negative-constant bit-clear idiom, see `matching.md`'s `& -5`/`& -2`/
 * `& -3` entries) to reproduce the ROM's runtime `movs`+`rsbs` instead
 * of a folded immediate AND. */
void DrawWumpa(struct orbit_part *self)
{
    DrawSprite(gSpriteRenderer, self);
    if (self->animDone != 0) {
        MATCH_HOLD_REG(s32, mask, r0) = -9;
        MATCH_HOLD_REG(u8, flags, r1) = self->base.flags;

        mask &= flags;
        self->base.flags = mask;
    }
}

/* Always-2 stub - same shape as `GetSpriteObjClassId`'s always-true stub
 * (`sprite_obj.c`). */
s32 GetWumpaClassId(void)
{
    return 2;
}

/* Sets `self->table` then tail-calls `DestroySpriteObj` (already matched in
 * `sprite_obj.c`), which unconditionally overwrites `table` again
 * with `gEntityVtable` - so this function's own store is
 * immediately clobbered by the callee. Kept faithfully anyway; the
 * compiler can't see through the opaque call to know the store is
 * dead. */
void DestroyWumpa(struct orbit_part *self, u32 flags)
{
    self->base.table = (void *)gWumpaVtable;
    DestroySpriteObj(&self->base, flags);
}

/* Sets flag bit 6 and clears `state`. */
void ResetWumpaPickup(struct orbit_part *self)
{
    MATCH_HOLD_REG(u8, mask, r1) = 0x40;

    mask |= self->base.flags;
    *(volatile u8 *)&self->base.flags = mask;
    self->state = 0;
}

/* Re-initializes `self` via `InitSpriteObj` (already matched in
 * `sprite_obj.c`), then overwrites its table with
 * `gWumpaVtable` and runs `ResetWumpaPickup` on it. */
struct orbit_part *InitWumpa(struct orbit_part *self)
{
    InitSpriteObj(&self->base);
    self->base.table = (void *)gWumpaVtable;
    ResetWumpaPickup(self);
    return self;
}

extern void *_call_via_r1(void *arg0, void *arg1);

/* If `state` is zero and the player (`gPlayer`)'s top
 * flag bit is set, fires a `self->table+0x68`-driven trampoline (the
 * same idiom documented in `crate_list.c`/`player_contact.c`) on
 * `self` itself. Always returns 0. */
s32 CollideWumpa(struct orbit_part *self)
{
    if (self->state == 0) {
        struct player *player = gPlayer;

        if (player->flags.all >> 7) {
            const struct vtable_slot *methods = self->base.table;
            const struct vtable_slot *rec = &methods[13];
            s16 offset = rec->delta;

            _call_via_r1((u8 *)self + offset, rec->fn);
        }
    }
    return 0;
}

/* Sets `self->x`/`self->y` (Q8 fixed-point) from raw pixel `x`/`y`,
 * and mirrors the result into the orbit `anchor`. */
void SetWumpaPos(struct orbit_part *self, s32 x, s32 y)
{
    s32 storedX, storedY;

    self->base.x = x << 8;
    self->base.y = y << 8;
    storedX = *(volatile s32 *)&self->base.x;
    storedY = *(volatile s32 *)&self->base.y;
    self->anchor.x = storedX;
    self->anchor.y = storedY;
}

/* Sets the hop `mode` and resets its `phase`; mode 0xff instead starts
 * the payout (`StartWumpaPayout`, wumpa_update.c). */
void SetWumpaHop(struct orbit_part *self, s32 mode)
{
    u8 *p = &self->mode;
    u8 zero = 0;

    *p = mode;
    p++;
    *p = zero;
    if (mode == 0xff) {
        StartWumpaPayout(self);
    }
}

/* Trivial one-byte setter of `counter`. */
void SetWumpaCounter(struct orbit_part *self, u8 value)
{
    self->counter = value;
}

/* Distance-gate: if the player (`gPlayer`) is within 0x180
 * (384 px) of `self` on both axes, calls `UpdateSpriteObj` (already
 * matched in `sprite_obj.c`) on `self`. Otherwise sets `self->flags`
 * bit 0 and, unless `self->id == 0xFFFF`, marks its bit in the
 * same `gEntityFlags+0x108` bitmap `sprite.c` already
 * writes - identical idiom, reused verbatim including the
 * register-pinned `>> 5` (see that file's note on why a plain C shift
 * doesn't reproduce the ROM's exact instruction here). Its pins and
 * `asm volatile` aren't those of entity_bits.h's ENTITY_SET_GONE_BIT_ASR,
 * so the sequence stays spelled out. */
void UpdateStopwatch(struct actor *self)
{
    struct player *player = gPlayer;
    MATCH_HOLD_REG(s32, rawX, r0) = player->x;
    MATCH_HOLD_REG(s32, dxPart, r1);
    s32 dx;
    s32 dy;

    asm volatile("asr %0, %1, #8" : "=r"(dxPart) : "r"(rawX));
    dx = dxPart - (self->x >> 8);
    if (dx < 0) {
        dx = -dx;
    }
    if (dx > 0x180) {
        goto outOfRange;
    }
    {
        MATCH_HOLD_REG(s32, rawY, r0) = player->y;
        MATCH_HOLD_REG(s32, dyPart, r1);

        asm volatile("asr %0, %1, #8" : "=r"(dyPart) : "r"(rawY));
        dy = dyPart - (self->y >> 8);
    }
    if (dy < 0) {
        dy = -dy;
    }
    if (dy <= 0x180) {
        goto inRange;
    }

outOfRange:
    {
        MATCH_HOLD_REG(s32, mask, r0) = 1;

        mask |= self->flags;
        self->flags = mask;
    }
    {
        MATCH_HOLD_REG(s32, ffff, r0) = 0xFFFF;
        MATCH_HOLD_REG(u16, field08a, r4) = *(volatile u16 *)&self->id;

        if (field08a != ffff) {
            MATCH_HOLD_REG(u16, field08b, r3) = *(volatile u16 *)&self->id;
            struct entity_flags *base = gEntityFlags;
            MATCH_HOLD_REG(s32, word, r0);
            s32 wordOffset;

            asm volatile("add %0, %1, #0\n\tasr %0, %0, #5" : "=r"(word) : "r"((s32)field08b));
            wordOffset = word << 2;
            {
                u8 *bitmapAddr = (u8 *)base->bits0Copy;

                bitmapAddr = bitmapAddr + wordOffset;
                word = field08b - (word << 5);
                *(s32 *)bitmapAddr |= 1 << word;
            }
        }
    }
    return;

inRange:
    UpdateSpriteObj(self);
}

/* Allocates a new `struct actor`-shaped object (`OperatorNew(0x40)`,
 * same size as `CreateSpriteObj`'s constructor in `sprite_obj.c`),
 * re-initializes it via `InitSpriteObj`, overwrites its table with
 * `gStopwatchVtable`, and runs the empty `ResetStopwatch` on it before
 * setting `id`/`x`/`y` from the raw pixel arguments. `unused` is
 * the fourth argument of the spawn-table slot (SpawnStopwatch passes it
 * in r3); the function never reads it. */
struct actor *CreateStopwatch(u16 id, u16 x, u16 y, u16 unused)
{
    struct actor *self = OperatorNew(0x40);

    InitSpriteObj(self);
    self->table = (void *)gStopwatchVtable;
    ResetStopwatch(self);
    self->id = id;
    self->x = (s32)x << 8;
    self->y = (s32)y << 8;
    return self;
}

/* Empty stub. */
void ResetStopwatch(struct actor *self)
{
}

/* Same `table`-set/tail-call-`DestroySpriteObj` shape as `DestroyWumpa`
 * above, with a different vtable. */
void DestroyStopwatch(struct actor *self, u32 flags)
{
    self->table = (void *)gStopwatchVtable;
    DestroySpriteObj(self, flags);
}

/* Same re-init/table-set/`ResetStopwatch` shape as `CreateStopwatch` above,
 * but re-initializing an existing `self` instead of allocating a new
 * one - the same relationship `InitSpriteObj` itself has to
 * `CreateSpriteObj` (see `sprite_obj.c`'s note on that pair). */
struct actor *InitStopwatch(struct actor *self)
{
    InitSpriteObj(self);
    self->table = (void *)gStopwatchVtable;
    ResetStopwatch(self);
    return self;
}

/* Zeroes/initializes the action controller's fields (`struct act`,
 * action_obj.h) from `self+0x25` through `+0x34`, plus `state` (+8),
 * `part` (+0x10), `+0x14` and `frame`/`frames` (+0x18/+0x1C), and sets
 * `motionXPending`/`motionYPending` (+0x2F/+0x30) to 1. InitActionCtrl
 * calls it right after wiring up `gActionCtrlVtable`. The stores go
 * through the pinned byte cursor `q` the ROM steps. */
void ResetActionCtrl(struct act *selfArg)
{
    MATCH_HOLD_REG(struct act *, p, r3) = selfArg;
    MATCH_HOLD_REG(u8 *, q, r1) = &p->turboRun;
    MATCH_HOLD_REG(s32, zero, r0) = 0;

    *q = zero;
    p->state = zero;
    q += 3;
    *q = zero;
    q -= 5;
    *q = zero;
    q += 1;
    *q = zero;
    q += 7;
    *q = 1;
    q += 1;
    *q = 1;
    p->unk_14 = zero;
    p->part = (struct player *)zero;
    q -= 0xa;
    *q = zero;
    q += 4;
    *q = zero;
    q -= 5;
    *q = zero;
    q += 6;
    *q = zero;
    p->frame = zero;
    p->frames = zero;
    q += 8;
    *q = zero;
    q += 1;
    *q = zero;
}
