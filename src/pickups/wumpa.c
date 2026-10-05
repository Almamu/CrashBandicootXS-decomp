#include "core.h"
#include "actor.h"
#include "pickups.h"

extern void DrawSprite(void *self, void *part);
extern void *gSpriteRenderer;

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
        register s32 mask asm("r0") = -9;
        register u8 flags asm("r1") = self->base.flags;

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

extern void DestroySpriteObj(struct actor *self, u32 arg1);

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
    register u8 mask asm("r1") = 0x40;

    mask |= self->base.flags;
    *(volatile u8 *)&self->base.flags = mask;
    self->state = 0;
}

extern struct actor *InitSpriteObj(struct actor *self);

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
extern void *gPlayer;

/* If `state` is zero and the player (`gPlayer`)'s top
 * flag bit is set, fires a `self->table+0x68`-driven trampoline (the
 * same idiom documented in `crate_list.c`/`player_contact.c`) on
 * `self` itself. Always returns 0. */
s32 CollideWumpa(struct orbit_part *self)
{
    if (self->state == 0) {
        struct actor *player = gPlayer;

        if (player->flags >> 7) {
            u8 *rec = (u8 *)self->base.table + 0x68;
            s16 offset = *(s16 *)rec;

            _call_via_r1((u8 *)self + offset, *(void **)(rec + 4));
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

extern void UpdateSpriteObj(struct actor *part);
extern void *gEntityFlags;

/* Distance-gate: if the player (`gPlayer`) is within 0x180
 * (384 px) of `self` on both axes, calls `UpdateSpriteObj` (already
 * matched in `sprite_obj.c`) on `self`. Otherwise sets `self->flags`
 * bit 0 and, unless `self->field_08 == 0xFFFF`, marks its bit in the
 * same `gEntityFlags+0x108` bitmap `sprite.c` already
 * writes - identical idiom, reused verbatim including the
 * register-pinned `>> 5` (see that file's note on why a plain C shift
 * doesn't reproduce the ROM's exact instruction here). */
void UpdateStopwatch(struct actor *self)
{
    struct actor *player = gPlayer;
    register s32 rawX asm("r0") = player->x;
    register s32 dxPart asm("r1");
    s32 dx;
    s32 dy;

    asm volatile("asr %0, %1, #8" : "=r" (dxPart) : "r" (rawX));
    dx = dxPart - (self->x >> 8);
    if (dx < 0) {
        dx = -dx;
    }
    if (dx > 0x180) {
        goto outOfRange;
    }
    {
        register s32 rawY asm("r0") = player->y;
        register s32 dyPart asm("r1");

        asm volatile("asr %0, %1, #8" : "=r" (dyPart) : "r" (rawY));
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
        register s32 mask asm("r0") = 1;

        mask |= self->flags;
        self->flags = mask;
    }
    {
        register s32 ffff asm("r0") = 0xFFFF;
        register u16 field08a asm("r4") = *(volatile u16 *)&self->field_08;

        if (field08a != ffff) {
            register u16 field08b asm("r3") = *(volatile u16 *)&self->field_08;
            void *base = gEntityFlags;
            register s32 word asm("r0");
            s32 wordOffset;

            asm volatile("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (word) : "r" ((s32)field08b));
            wordOffset = word << 2;
            {
                u8 *bitmapAddr = (u8 *)base + 0x108;

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

extern void *OperatorNew(s32 size);

/* Allocates a new `struct actor`-shaped object (`OperatorNew(0x40)`,
 * same size as `CreateSpriteObj`'s constructor in `sprite_obj.c`),
 * re-initializes it via `InitSpriteObj`, overwrites its table with
 * `gStopwatchVtable`, and runs the empty `ResetStopwatch` on it before
 * setting `field_08`/`x`/`y` from the raw pixel arguments. `unused` is
 * the fourth argument of the spawn-table slot (SpawnStopwatch passes it
 * in r3); the function never reads it. */
struct actor *CreateStopwatch(u16 id, u16 x, u16 y, u16 unused)
{
    struct actor *self = OperatorNew(0x40);

    InitSpriteObj(self);
    self->table = (void *)gStopwatchVtable;
    ResetStopwatch(self);
    self->field_08 = id;
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

/* Zeroes/initializes a run of fields from `self+0x25` through `+0x34`
 * (not otherwise characterized yet), plus `self+8`/`+0x10`/`+0x14`/
 * `+0x18`/`+0x1c`, and sets `+0x2f`/`+0x30` to 1. This is the
 * constructor sibling `InitActionCtrl` (still raw, `0x0801588C`) calls
 * right after wiring up `gActionCtrlVtable` - that caller stores
 * its own vtable pointer at `+0xc`, not `+0x18` the way `struct actor`
 * does, so `self` here is a *different*, still-unnamed "child object"
 * struct - the same one several large state machines in this ROM
 * region (`ActionCtrlHandleEvent` etc., left raw for now) read/write through
 * many more offsets not characterized here. Kept as a raw `void *`
 * rather than `struct actor *` to avoid implying it shares that
 * layout. */
void ResetActionCtrl(void *selfArg)
{
    register u8 *p asm("r3") = (u8 *)selfArg;
    register u8 *q asm("r1") = p + 0x29;
    register s32 zero asm("r0") = 0;

    *q = zero;
    *(s32 *)(p + 8) = zero;
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
    *(s32 *)(p + 0x14) = zero;
    *(s32 *)(p + 0x10) = zero;
    q -= 0xa;
    *q = zero;
    q += 4;
    *q = zero;
    q -= 5;
    *q = zero;
    q += 6;
    *q = zero;
    *(s32 *)(p + 0x18) = zero;
    *(s32 *)(p + 0x1c) = zero;
    q += 8;
    *q = zero;
    q += 1;
    *q = zero;
}
