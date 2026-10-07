#include "core.h"
#include "match.h"
#include "orbit_part.h"
#include "hud.h"
#include "pickups.h"
#include "util.h"
#include "audio.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "math_util.h"

/* GitHub issue #12/#14 Phase 2 mop-up: the last 5 raw functions of the
 * still-large 24-function tail past `AddCollisionCandidate`
 * (`asm/code_3_2_17_e560_10d54.s`) - see
 * docs/matching/archive/issue-14-0x08010d54-physics-apply.md's "Not integrated
 * this pass" section for the individual draft characterizations this
 * file finishes integrating: `CheckExtraLifePickup`, `PickUpExtraLife`, `UpdateExtraLife`,
 * `CreateExtraLife`, `SendExtraLifeToHud`. All 5 operate on the same still-unnamed
 * "part" object `src/pickups/extra_life.c`/`wumpa_update.c` already
 * document (the older functions use raw `u8 *` offsets; the ones
 * turned into C by the issue #15 NAKED retry use `struct orbit_part`,
 * include/orbit_part.h). This closes out
 * the entire `0x08010D54` chunk (issue #12/#14): every function between
 * `AddCollisionCandidate` and the already-matched `src/pickups/wumpa.c`
 * (`DrawWumpa`) is now matched. */

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15
 * NAKED retry: CreateExtraLife matches only under it, and the rest of the
 * file compiles identically under either compiler. */

/* `frame = min(0, frameCount - 1)` against the part's current animation
 * record (same clamp as wumpa_update.c's copy). */
static inline void OrbitClampFrame(struct orbit_part *self)
{
    s32 frame = 0;
    s32 count = self->bank->records[self->tag].frameCount;

    CLAMP_INDEX(frame, count);
    self->frame = frame;
}

/* Called from `PickUpExtraLife` below only (the "randomized-behavior"
 * family's own bounds-check gate). No existing cross-reference
 * elsewhere in the codebase. Gated: does nothing unless the orbit is
 * active (`self+0x4a != 0`) and either its phase hasn't wrapped past
 * `0x16` yet or the player (`gPlayer`) is in state
 * `+0x88 == 3` - the exact same opening gate `CheckWumpaPickup`
 * (`extra_life.c`) already uses. Once past that gate, proceeds only
 * when flags bit 3 is clear and flags bit 2 is set (same bit-test idiom
 * as `CheckWumpaPickup`), builds both `self`'s and the player's AABB via
 * `GetSpriteHitbox` (unlike `CheckWumpaPickup`, always the "secondary" AABB
 * build for both sides - no `player+0xa == 0x13` branch here), and on
 * overlap sets flags bit 3 and calls `PickUpExtraLife(self, 0)` (the fixed,
 * non-randomized despawn-offset path). */
void CheckExtraLifePickup(struct orbit_part *self)
{
    struct aabb selfBox;
    struct aabb playerBox;
    struct player *player;

    if (self->mode != 0 && self->phase <= 0x16) {
        if (gPlayer->ctrlMode != 3) {
            return;
        }
    }

    {
        u8 flags = self->base.flags;
        u32 shifted = (u32)flags << 0x18;

        if ((shifted >> 0x1b) & 1) {
            return;
        }
        if (!((shifted >> 0x1a) & 1)) {
            return;
        }
    }

    selfBox = GetSpriteHitbox((struct box_part *)self);
    player = gPlayer;
    playerBox = GetSpriteHitbox((struct box_part *)player);

    if (AabbOverlaps(&playerBox, &selfBox)) {
        MATCH_HOLD_REG(s32, bit, r0) = 8;

        bit |= self->base.flags;
        self->base.flags = bit;
        PickUpExtraLife(self, 0);
    }
}

/* `docs/rom_map.md`: part of "the randomized-behavior... famil[y]"
 * alongside `CheckPlayerCtrlTurn`. Plays a hit SFX, sets `timer` to
 * `0xa0`, then either derives a randomized `(dx,dy)` offset pair from
 * `rand()` (`randomize` nonzero - `counter` tags which of three
 * `rand()`-driven bands the x-offset came from, `state = 2`) or uses a
 * fixed `(0xb400,0xc00)` offset and fires `ShowHudLives(gHud)`
 * (`state = 1`). Either way: `flags |= 0x10`, `screenSpace = 1`, then
 * calls `WorldToScreen(self, x>>8, y>>8, &outX, &outY)` and re-derives
 * `x`/`y` plus `velX`/`velY` (a "distance to travel" pair, `-FixedDiv(newPos<<8 - offset, 0x1400)`)
 * from the results - the exact same tail shape `PickUpWumpa`/
 * `SendExtraLifeToHud`/`SendWumpaToHud` (`wumpa_update.c`) all share. */
void PickUpExtraLife(struct orbit_part *self, u8 randomize)
{
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    MATCH_KEEP_VOLATILE(self);

    PlaySfx(gAudioContext, SFX_EXTRA_LIFE, 0x100);
    self->timer = 0xa0;

    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        self->counter = lowbit;
        if (lowbit) {
            if (rv & 2) {
                dx = INT_TO_Q8((rv & 0x3f) + 5);
            } else {
                dx = INT_TO_Q8(0xeb - (rv & 0x3f));
            }
        } else {
            dx = INT_TO_Q8((rv & 0x7f) + 0x24);
        }
        dy = INT_TO_Q8((rv & 0x1f) + 0x10);
        self->state = 2;
    } else {
        dx = 0xb400;
        dy = 0xc00;
        self->state = 1;
        ShowHudLives(gHud);
    }

    {
        MATCH_HOLD_REG(s32, mask, r0) = 0x10;

        mask |= self->base.flags;
        self->base.flags = mask;
    }
    {
        MATCH_HOLD_REG(u8, one, r0) = 1;

        /* Retyped store: through the plain member, the `1` is built
         * after the field's address instead of before it. */
        *(u8 *)&self->screenSpace = one;
    }

    WorldToScreen(self, Q8_TO_INT(self->base.x), Q8_TO_INT(self->base.y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    self->base.x = newX;
    self->velX = -FixedDiv(newX - dx, 0x1400);

    newY = INT_TO_Q8(outY);
    self->base.y = newY;
    self->velY = -FixedDiv(newY - dy, 0x1400);
}

/* `docs/rom_map.md`: "a bounds-checked, mode-selected object state
 * machine that self-destructs off-screen" - a rotating/orbiting
 * hazard/projectile behavior, uses the shared sine table
 * `gSineTable` this whole neighborhood references. Modes
 * 1/2 integrate `self->x`/`self->y` by `self->0x40`/`self->0x44` and,
 * on reaching an on-screen "arrival" bound (mode 1) or a wrapping
 * `self->0x3c` timer threshold (mode 2), fire a hit SFX,
 * `AddLife(gLevelState)`, set flags bit 0, and (unless
 * `self->8 == 0xffff`) set `self->8`'s bit in the
 * `gEntityFlags+0x108` collision bitmap - the same inline idiom
 * `UpdateWumpa` (`wumpa_update.c`) also duplicates per mode. Any other
 * mode (0, or 3+): gated by `self->0x4a`, increments `self->0x49` or
 * `self->0x4b` (wrapping the gate off after 32 ticks). The shared tail:
 * unless `self->0x48 != 0`, either computes an orbit step via
 * `gSineTable[(self->0x49 & 0x7f)]` and `FixedMul` added
 * into `self->0x50`, storing to `self->y` (when `self->0x4a` is clear),
 * or calls `UpdateExtraLifeHop` (`extra_life.c`'s own orbit-position updater)
 * when `self->0x4a` is set - then always tail-calls `UpdateSpriteObj`
 * (already matched, `sprite_obj.cpp`).
 *
 *
 * old_agbcc. The MATCH_KEEP copy in mode 1 reproduces its recomputed
 * `x + velX`. In mode 2 the timer is re-read through `self` after the
 * store and the id compared without a local, so both become the ROM's
 * reloads from memory (`ldrh` at the compare); two extra references on
 * each velocity give it r0 and the position r1 (third near-miss sweep;
 * the draft was 22 halfwords off). */
void UpdateExtraLife(struct orbit_part *self)
{
    u8 state = self->state;

    if (state == 1) {
        s32 x = self->base.x, vx = self->velX, y;

        self->base.x = x + vx;
        /* Hides that `vx` is unchanged, so the bound check below
         * recomputes `x + vx` as the ROM does instead of reusing the
         * stored sum. */
        MATCH_KEEP(vx);
        y = self->base.y + self->velY;
        self->base.y = y;
        if (Q8_TO_INT(x + vx) <= 0xb4 && Q8_TO_INT(y) <= 0xc) {
            /* Extra reference: puts velX in r3 and y in r2, as in the
             * ROM. */
            MATCH_USE(vx);
            PlaySfx(gAudioContext, SFX_HUD_COLLECT, 0x100);
            AddLife(gLevelState);
            self->base.flags |= 1;
            if (self->base.id != ENTITY_ID_NONE)
                ENTITY_SET_GONE_BIT_OF(self->base.id, state);
        }
    } else if (state == 2) {
        s32 fire;

        /* Two extra references on each velocity: it wins r0 and the
         * position/sum r1, as in the ROM. */
        {
            s32 x = self->base.x;
            s32 v = self->velX;

            MATCH_USE(v);
            MATCH_USE(v);
            self->base.x = x + v;
        }
        {
            s32 y = self->base.y;
            s32 v = self->velY;

            MATCH_USE(v);
            MATCH_USE(v);
            self->base.y = y + v;
        }
        fire = 0;
        if (self->counter == 0) {
            s32 t = self->timer - 4;

            self->timer = t;
            if (t < 0x40)
                fire = 1;
        } else {
            s32 t;

            self->timer = self->timer + 0xc;
            t = self->timer;
            if (t > 0x1b0)
                fire = 1;
        }
        if (fire) {
            self->base.flags |= 1;
            if (self->base.id != ENTITY_ID_NONE)
                ENTITY_SET_GONE_BIT(self->base.id);
        }
    } else {
        if (self->mode == 0)
            self->counter++;
        else if (++self->phase > 0x1f)
            self->mode = 0;
    }

    if (self->state == 0) {
        if (self->mode == 0) {
            s32 sn = gSineTable[(self->counter & 0x7f) * 2];
            sn = FixedMul(sn, 0x280);
            self->base.y = self->anchor.y + sn;
        } else {
            UpdateExtraLifeHop(self);
        }
    }
    UpdateSpriteObj(&self->base);
}

/* `struct actor *CreateExtraLife(u16 arg0, u16 arg1, u16 arg2, s32 arg3)` -
 * spawns a part-object; extern already declared in `drop_extra_life.c`.
 * `arg3` is never actually read (the ROM hardcodes the field it would
 * feed - `self+0x29`/`+0x2a`/`+0x2b` - to a compile-time `0`
 * regardless), matching the extern's own always-`0` call sites.
 * Allocates a `0x54`-byte object (`OperatorNew`), re-initializes it
 * (`InitSpriteObj`), repoints `self->table` (`self+0x18`) at
 * `gExtraLifeVtable`, clears the "spawned/active" gate byte via
 * `ResetExtraLifePickup` (`extra_life.c`), stores `arg0` at `self+8` and
 * `arg1`/`arg2` (Q8-scaled) at `self+0`/`self+4`, mirrored into
 * `self+0x4c`/`self+0x50` (the orbit anchor `SetExtraLifePos`/
 * `UpdateExtraLifeHop` also use), joins the `gTouchableList`
 * `dual_array_manager` list (`AddToPartList`), derives `self+0x30` from
 * the same `table[self->0x2d]->+0x16` clamp idiom `PickUpWumpa`/
 * `SendWumpaToHud` (`wumpa_update.c`) use, clears bits 0/5 of `self+0x28`,
 * and always tags `self+0x29`/`+0x2a`/`+0x2b` all `0`, returning the
 * new part.
 *
 * Under old_agbcc this is plain C: the `0` sentinel held in `r8` across
 * AddToPartList is just the `zero` local below, and the anchor copy is a
 * struct copy of the head x/y pair (`ORBIT_POS`). */
struct orbit_part *CreateExtraLife(u16 id, u16 x, u16 y, s32 unused)
{
    struct orbit_part *self;
    u8 zero;

    self = OperatorNew(0x54);
    InitSpriteObj(&self->base);
    self->base.table = (void *)gExtraLifeVtable;
    ResetExtraLifePickup(self);
    zero = 0;
    self->base.id = id;
    self->base.x = INT_TO_Q8(x);
    self->base.y = INT_TO_Q8(y);
    self->anchor = ORBIT_POS(self);
    AddToPartList(gTouchableList, self);
    OrbitClampFrame(self);
    self->flipX = 0;
    self->flipY = 0;
    self->counter = zero;
    self->mode = zero;
    self->phase = zero;
    return self;
}

/* `void SendExtraLifeToHud(void *part)` - extern already declared in
 * `drop_extra_life.c`; the documented "mutually exclusive alternative" is
 * `SendWumpaToHud` (`wumpa_update.c`, already matched). Plays a hit SFX,
 * sets `state = 1`, nudges `x -= mode << 8`, sets `screenSpace = 1`,
 * calls `WorldToScreen(self, x>>8, y>>8, &outX, &outY)` and re-derives
 * `x`/`y` plus `velX`/`velY`
 * (the same `-FixedDiv(newPos<<8 - offset, 0x1400)` "distance to
 * travel" idiom `PickUpExtraLife`/`PickUpWumpa`/`SendWumpaToHud` all share),
 * with fixed `0xb400`/`0xc00` offsets on x/y respectively, then fires
 * `ShowHudLives(gHud)` - unlike `SendWumpaToHud`'s
 * `ShowHudWumpa`. Notably simpler than its `SendWumpaToHud` sibling: no
 * `timer`/`frame` table-lookup-clamp setup here at all. */
void SendExtraLifeToHud(struct orbit_part *self)
{
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gAudioContext, SFX_EXTRA_LIFE, 0x100);
    self->state = 1;
    {
        MATCH_HOLD_REG(s32, off, r0) = self->mode;
        MATCH_HOLD_REG(s32, shifted, r1) = INT_TO_Q8(off);

        self->base.x -= shifted;
    }
    self->screenSpace = 1;

    WorldToScreen(self, Q8_TO_INT(self->base.x), Q8_TO_INT(self->base.y), &outX, &outY);

    newX = INT_TO_Q8(outX);
    self->base.x = newX;
    self->velX = -FixedDiv(newX - 0xb400, 0x1400);

    newY = INT_TO_Q8(outY);
    self->base.y = newY;
    self->velY = -FixedDiv(newY - 0xc00, 0x1400);

    ShowHudLives(gHud);
}

/* GitHub issue #12/#14 Phase 2, "accessor cluster" group: `UpdateExtraLifeHop`
 * through `CheckWumpaPickup` (11 functions, `0x08011248`-`0x08011448`),
 * carved out of the middle of the still-unexamined 24-function tail
 * documented in docs/matching/archive/issue-14-0x08010d54-physics-apply.md.
 * `self` here is a further, still-unnamed "part"-shaped object -
 * distinct from `struct actor` (only 0x1c bytes) and from the
 * `struct collision_queue` `AddCollisionCandidate`/`DestroyCollisionQueue`/`ResetCollisionQueue`
 * (collision_queue.cpp) operate on - the same "big, mostly-uncharacterized
 * object, individual fields named only by offset" situation already
 * documented for this object family in `src/player/player_update.c`'s
 * own file header. The fields these functions use are named in `struct
 * orbit_part` (orbit_part.h).
 *
 * Confirms this is a small "orbiting hazard" behavior mixed into the
 * same object type `CreateExtraLife`/`SendExtraLifeToHud` (drop_extra_life.c,
 * Phase 2's neighboring group) spawn/manage - `SetExtraLifePos` seeds an
 * orbit anchor+start position, `SetExtraLifeHop` (re)starts the orbit at a
 * given mode/phase 0, `SetExtraLifeCounter` sets an adjacent still-unexamined
 * byte, `UpdateExtraLifeHop` is the per-frame orbit-position update,
 * `CollideExtraLife` fires a `self->table`-driven hit trampoline once the
 * object is "spawned" (`self+0x48 == 0`) and the player has a specific
 * flag set, `DrawExtraLife` re-derives visibility from a
 * `DrawSprite`/`self+0x38` gate and clears flags bit 3 when gated off,
 * `DestroyExtraLife`/`InitExtraLife`/`ResetExtraLifePickup` are a small
 * init/reset/table-repoint trio (same `InitSpriteObj`/table-swap shape
 * documented throughout `moving_sprite.c`), and `CheckWumpaPickup` is the
 * per-frame player-proximity/hit-resolve step: gated by the same
 * orbit-mode/phase fields, it AABB-tests against the player (choosing
 * primary vs. secondary AABB build depending on the player's own
 * current state, `player+0xa == 0x13`), and on overlap sets flags bit
 * 3, tail-calls the despawn picker `PickUpWumpa` (Phase 2's own
 * neighboring group, not read this pass - only extern'd here) with a
 * mode that differs per AABB path, and (primary-AABB path only) plays
 * a hit SFX. */

extern void *_call_via_r1(void *arg0, void *fn);

/* Per-frame orbit-position update. Reads the current orbit phase
 * (`self+0x4b`) twice, at two different scales into the shared sine
 * table (`*4` for the y-offset, `*2` for the x-offset - two different
 * "speeds" around the same table, not a copy/paste of the same lookup)
 * and combines each with a scale factor via the overflow-avoiding
 * fixed-point multiply `FixedMul`: the y-offset always uses the
 * fixed scale `0x800`, while the x-offset's scale comes from a local
 * copy of the 3-entry table `gExtraLifeHopWidths`, indexed by
 * `self+0x4a - 1` (so `self+0x4a` must be 1-3 to select a scale; mode 3
 * yields an x-offset that's discarded - see below).
 *
 * `self+4` (`y`) is always `self+0x50` (anchor y) minus the y-offset.
 * `self` (`x`) is `self+0x4c` (anchor x) minus the x-offset when
 * `self+0x4a == 1`, plus the x-offset when `self+0x4a == 2`, or just
 * the anchor x unchanged otherwise (`self+0x4a == 3`, or in practice
 * any other value - the local `gExtraLifeHopWidths` lookup still runs
 * for mode 3, its result simply unused). */
/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the sine sample goes
 * through one reused local (`sn`), which old_agbcc keeps in r2 across
 * both calls exactly like the ROM; current agbcc renumbers the first
 * lookup's registers. Every other function in this file compiles the
 * same under either compiler. */
void UpdateExtraLifeHop(struct orbit_part *self)
{
    struct three_words scales = *(const struct three_words *)gExtraLifeHopWidths;
    s32 dy;
    s32 sn;

    sn = gSineTable[self->phase * 4];
    dy = FixedMul(sn, 0x800);
    self->base.y = self->anchor.y - dy;
    sn = gSineTable[self->phase * 2];
    sn = FixedMul(sn, scales.a[self->mode - 1]);
    if (self->mode == 1)
        self->base.x = self->anchor.x - sn;
    else if (self->mode == 2)
        self->base.x = self->anchor.x + sn;
    else
        self->base.x = self->anchor.x;
}

/* Re-derives visibility via `DrawSprite(gSpriteRenderer, self)`
 * (already matched, `sprite.cpp`), then clears `flags` bit 3 when
 * `animDone` is set - the same "consumed/hit"
 * flag bit `CheckWumpaPickup` below sets. */
void DrawExtraLife(struct orbit_part *self)
{

    DrawSprite(gSpriteRenderer, self);
    if (self->animDone != 0) {
        MATCH_HOLD_REG(s32, mask, r0) = 9;
        mask = -mask;
        mask &= self->base.flags;
        self->base.flags = mask;
    }
}

/* Trivial - always returns 2, ignoring any argument. */
s32 GetExtraLifeClassId(void)
{
    return 2;
}

/* Repoints `self->table` (`self+0x18`) at `gExtraLifeVtable`, then
 * tail-calls `DestroySpriteObj` (already matched, `moving_sprite.c`) with
 * `self` and this function's own second argument passed straight
 * through. */
void DestroyExtraLife(struct orbit_part *self, u32 flags)
{
    self->base.table = (void *)gExtraLifeVtable;
    DestroySpriteObj(&self->base, flags);
}

/* Clears the "spawned/active" gate byte `state`. */
void ResetExtraLifePickup(struct orbit_part *self)
{
    self->state = 0;
}

/* Re-initializes `self` via `InitSpriteObj` (already matched,
 * `moving_sprite.c`; its return value is discarded - same "call for
 * side effect only" shape used elsewhere in this object family),
 * repoints `self->table` at `gExtraLifeVtable`, clears the
 * "spawned/active" gate byte via `ResetExtraLifePickup`, and returns `self`. */
struct orbit_part *InitExtraLife(struct orbit_part *self)
{
    InitSpriteObj(&self->base);
    self->base.table = (void *)gExtraLifeVtable;
    ResetExtraLifePickup(self);
    return self;
}

/* If `state` (the "spawned/active" gate) is clear and the player's
 * (`gPlayer`) flags have bit 7 set, fires method table slot 13
 * (`table+0x68/0x6c`)'s trampoline (`_call_via_r1`) - the usual
 * "offset + fn pointer" pair convention already established throughout
 * this codebase (e.g. `graphics.cpp`'s own `+0x10`/`+0x14` pair). Always
 * returns 0. */
s32 CollideExtraLife(struct orbit_part *self)
{
    if (self->state == 0) {
        struct player *player = gPlayer;

        if (player->flags.all >> 7) {
            const struct vtable_slot *methods = self->base.table;
            const struct vtable_slot *entry = &methods[13];
            void *addr = (u8 *)self + entry->delta;
            void *fn = entry->fn;

            _call_via_r1(addr, fn);
        }
    }
    return 0;
}

/* Sets `x`/`y` (Q8) from the raw `x`/`y` arguments scaled by 8, and
 * mirrors both into `anchor` - seeding
 * an orbit anchor at the object's own starting position. */
void SetExtraLifePos(struct orbit_part *self, s32 x, s32 y)
{
    s32 qx, qy;

    self->base.x = INT_TO_Q8(x);
    self->base.y = INT_TO_Q8(y);
    qx = *(volatile s32 *)&self->base.x;
    qy = *(volatile s32 *)&self->base.y;
    self->anchor.x = qx;
    self->anchor.y = qy;
}

/* Sets the orbit `mode` and resets the orbit `phase` to 0. */
void SetExtraLifeHop(struct orbit_part *self, u8 mode)
{
    u8 *modePtr;
    u8 zero;

    modePtr = &self->mode;
    zero = 0;
    *modePtr = mode;
    self->phase = zero;
}

/* Unexamined byte setter, `counter` - address-adjacent to the orbit
 * mode/phase pair above but not otherwise read by any function in this
 * group. */
void SetExtraLifeCounter(struct orbit_part *self, u8 val)
{
    self->counter = val;
}

/* Per-frame player-proximity/hit-resolve step. Gated: does nothing
 * unless the orbit is active (`self+0x4a != 0`) and either its phase
 * hasn't wrapped past `0x16` yet or the player (`gPlayer`)
 * is in state `+0x88 == 3`; if the orbit is active, the phase has
 * wrapped, and the player isn't in that state, returns immediately.
 *
 * Once past that gate, proceeds only when flags bit 3
 * (the "already hit" latch `DrawExtraLife` clears) is clear and flags
 * bit 2 is set. Builds `self`'s own AABB via `GetSpriteHitbox`, then reads
 * the player's own `+0xa` state: if it's `0x13`, builds the player's
 * *primary* AABB (`GetSpriteAttackBox`) and tests it against `self`'s own via
 * `AabbOverlaps`; on overlap, sets flags bit 3, tail-calls
 * `PickUpWumpa(self, 1)`, and plays a hit SFX
 * (`PlaySfx(gAudioContext, 6, 0x80)`). Otherwise builds the
 * player's *secondary* AABB (`GetSpriteHitbox`, the same helper used for
 * `self`'s own box) and tests it the same way; on overlap, sets flags
 * bit 3 and tail-calls `PickUpWumpa(self, 0)` (no SFX on this path). */
void CheckWumpaPickup(struct orbit_part *self)
{
    struct aabb selfBox;
    struct aabb playerBox;
    struct player *player;

    if (self->mode != 0 && self->phase <= 0x16) {
        if (gPlayer->ctrlMode != 3) {
            return;
        }
    }

    {
        u8 flags = self->base.flags;
        u32 shifted = (u32)flags << 0x18;

        if ((shifted >> 0x1b) & 1) {
            return;
        }
        if (!((shifted >> 0x1a) & 1)) {
            return;
        }
    }

    selfBox = GetSpriteHitbox((struct box_part *)self);
    player = gPlayer;

    if (player->kind == 0x13) {
        GetSpriteAttackBox(&playerBox, player);
        if (AabbOverlaps(&playerBox, &selfBox)) {
            MATCH_HOLD_REG(s32, bit, r0) = 8;

            bit |= self->base.flags;
            self->base.flags = bit;
            PickUpWumpa(self, 1);
            PlaySfx(gAudioContext, SFX_WUMPA_HIT, 0x80);
        }
    } else {
        playerBox = GetSpriteHitbox((struct box_part *)player);
        if (AabbOverlaps(&playerBox, &selfBox)) {
            MATCH_HOLD_REG(s32, bit, r0) = 8;

            bit |= self->base.flags;
            self->base.flags = bit;
            PickUpWumpa(self, 0);
        }
    }
}
