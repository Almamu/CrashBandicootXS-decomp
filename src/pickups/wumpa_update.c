#include "core.h"
#include "match.h"
#include "actor.h"
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

/* GitHub issue #12/#14 Phase 2, second parallel slice: the tail 6
 * functions of the still-large 24-function chunk past AddCollisionCandidate
 * (asm/code_3_2_17_e560_10d54.s) - see
 * docs/matching/archive/issue-14-0x08010d54-physics-apply.md's Phase 2 planning
 * section for the full function/size list. This file carves out only
 * PickUpWumpa-UpdateWumpaHop (the chunk's last 6 functions, contiguous
 * through to the already-matched src/pickups/wumpa.c at
 * 0x080119A8) - a clean single trim point at the tail of the asm file,
 * chosen specifically because CreateExtraLife and the UpdateExtraLifeHop-
 * CheckWumpaPickup "no cross-reference" accessor cluster in between this
 * file's own functions and the ones a sibling parallel session is
 * working on are interleaved with several *other* individually-
 * characterized functions (CheckExtraLifePickup/PickUpExtraLife/UpdateExtraLife/
 * SendExtraLifeToHud) this pass also read and verified in isolation but did
 * NOT integrate here - splitting the asm file at more than one point to
 * reach them would need a second new C file, which this session's
 * parallel-agent convention reserves collision-avoidance for a single
 * name (wumpa_update.c) - see the issue doc's own follow-up note. */

/* Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15
 * NAKED retry: PickUpWumpa and UpdateWumpaHop match only under it, and the
 * rest of the file compiles identically under either compiler. */

/* `frame = min(0, frameCount - 1)` against the part's current animation
 * record - the clamp every spawner/launcher in this family repeats. */
static inline void OrbitClampFrame(struct orbit_part *self)
{
    s32 frame = 0;
    s32 count = self->bank->records[self->tag].frameCount;

    if (frame >= count)
        frame = count - 1;
    self->frame = frame;
}

/* PickUpWumpa: "randomized-position spawn/despawn picker" (docs/rom_map.md),
 * called as `PickUpWumpa(entry, 1)`/`(other, 1)` from time_trial.c/
 * crate_break.c for despawn. PlaySfx(gAudioContext, 8, 0x100), then
 * either derives a randomized (dx,dy) offset pair from rand() (arg1
 * nonzero - self->0x48 = 2, self->0x49 tags which of three rand()-driven
 * bands was picked) or uses a fixed (0x1000,0x1000) offset and fires
 * ShowHudWumpa(gHud) (self->0x48 = 1). Either way: self->0x3c
 * = 0xa0, self->0x30 clamped from a self->0x20 table lookup at
 * self->0x2d*0x1c+0x16 (same "table[tag]->field 0x16, clamp against a
 * zero floor" idiom UpdateExtraLife/SendWumpaToHud also use), self->0x25 = 1,
 * self->0xc |= 0x10, then calls WorldToScreen(self, self->x>>8, self->y>>8,
 * &outX, &outY) and re-derives self->x/self->y plus self->0x40/self->0x44
 * (a "distance to travel" pair, via -FixedDiv(newPos<<8 - offset,
 * 0x1400)) from the results - the exact same tail shape PickUpExtraLife/
 * SendExtraLifeToHud/SendWumpaToHud all share in this subsystem.
 *
 * The `self->0x25 = 1` store goes through a `u8` local so old_agbcc
 * materializes the 1 before the field address, as the ROM does. (Earlier
 * notes blamed an r7 allocation gap; under old_agbcc the tag lands in r7
 * on its own.) */
void PickUpWumpa(struct orbit_part *self, u8 randomize)
{
    s32 dx, dy;
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gAudioContext, 8, 0x100);
    if (randomize) {
        u32 rv = (u16)rand();
        u8 lowbit = rv & 1;

        self->counter = lowbit;
        if (lowbit) {
            if (rv & 2)
                dx = ((rv & 0x3f) + 5) << 8;
            else
                dx = (0xeb - (rv & 0x3f)) << 8;
        } else {
            dx = ((rv & 0x7f) + 0x24) << 8;
        }
        dy = ((rv & 0x1f) + 0x10) << 8;
        self->state = 2;
    } else {
        dx = dy = 0x1000;
        self->state = 1;
        ShowHudWumpa(gHud);
    }
    self->timer = 0xa0;
    OrbitClampFrame(self);
    {
        u8 one = 1;

        self->screenSpace = one;
    }
    self->base.flags |= 0x10;

    WorldToScreen(self, self->base.x >> 8, self->base.y >> 8, &outX, &outY);

    newX = outX << 8;
    self->base.x = newX;
    self->velX = -FixedDiv(newX - dx, 0x1400);
    newY = outY << 8;
    self->base.y = newY;
    self->velY = -FixedDiv(newY - dy, 0x1400);
}

/* UpdateWumpa: "entity-vtable-dispatched velocity integrator" (docs/
 * rom_map.md). Dispatches on self->0x48 (0-3):
 *  - mode 1: integrates self->x/self->y by self->0x40/self->0x44 (the
 *    "distance to travel" pair PickUpWumpa/PickUpExtraLife/etc. compute),
 *    wraps self->0x3c by +/-4 (mode-gated by self->0x49) each frame in
 *    [0,0x140], and once self->x>>8/self->y>>8 both fall within a small
 *    box (|x|<=0x10, |y|<=0x10) fires PlaySfx(gAudioContext,0xe,
 *    0x100), calls CollectWumpa(gLevelState) (a scoring/counter
 *    candidate per docs/rom_map.md), sets self->0xc bit 0, and - unless
 *    self->8 == 0xffff - sets self->8's bit in the gEntityFlags+
 *    0x108 collision bitmap (the same inline idiom MarkEntityGone/
 *    DropExtraLife use on a struct actor).
 *  - mode 2: same integrate step, then wraps self->0x3c similarly but
 *    with different thresholds/direction, and on wrap-triggered falls
 *    into the same "set self->0xc bit 0 + collision-bitmap" tail as
 *    mode 1.
 *  - mode 3: increments self->0x49 each frame; every 11th frame resets
 *    it and calls DropWumpa(gEntitySpawner, self->x>>8, self->y>>8,
 *    0, 1, 0) (a NAKED part-object spawner already matched in
 *    entity_spawner.c) - then increments self->0x4b every frame too; every
 *    10th frame falls into the same collision-bitmap tail as modes 1/2.
 *  - mode 0 (default, self->0x4a-gated): increments self->0x49 or
 *    self->0x4b depending on self->0x4a, wrapping self->0x4a's own
 *    gate off after 32 self->0x4b ticks.
 * All four modes converge on a shared tail: if self->0x48 == 0, reads
 * self->0x4a again - if clear, computes a velocity step via
 * gSineTable[self->0x49 & 0x7f] and FixedMul, added to
 * self->0x50 and stored into self->y (a "rotate self->y around a fixed
 * center by a table-driven step" idiom, same table/shape as
 * UpdateExtraLife's own default-mode branch); if set, calls UpdateWumpaHop
 * (the small self->0x4b/self->0x4a-driven table helper above) instead.
 * If self->0x48 == 3 specifically, self->x/self->y are instead reset to
 * gPlayer's own position minus a small fixed offset
 * (0xFFFFFC00/0xFFFFF200, i.e. -0x400/-0xe00 in Q8). Every path ends
 * with a tail call to UpdateSpriteObj(self) (already matched elsewhere,
 * src/objects/sprite_obj.c).
 *
 * Matched (old_agbcc) over three passes, see
 * docs/matching/archive/big-naked-retry-3.md and
 * docs/matching/archive/mix-naked-retry-5.md. The three "flags |= 1, set the id
 * bit" tails are merged by cross-jumping as in the ROM: the spawn's byte
 * argument is a plain `*(volatile u8 *)` store of a QImode 1 that cse
 * reuses for mode 3's `flags |= 1` but not for the SImode `1 << bit`;
 * the phase test spells out the zero-extension as shifts; the timer
 * re-reads go through an `s32` inline so the compare stays the ROM's
 * signed `ble`; the state is re-read for each test. The integrate step
 * (ORBIT_STEP), the spawn argument's address and the state-3 tail's
 * locals settle the last register and order differences. */
typedef struct actor *(*OrbitSpawn4)(void *pool, s32 x, s32 y, u8 p3);

/* flags |= 1 and, unless the id is 0xffff, the id's bit in the
 * collision bitmap. The three copies are merged by cross-jumping. */
#define ORBIT_MARK_GONE(self, one)                                             \
    if (1)                                                                     \
    {                                                                          \
        (self)->base.flags |= (one);                                           \
        if ((self)->base.field_08 != 0xffff) {                                 \
            ORBIT_SET_ID_BIT((self)->base.field_08, 1);                        \
        }                                                                      \
    } else (void)0

#define ORBIT_SET_ID_BIT(idExpr, one)                                          \
    if (1)                                                                     \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = (u8 *)gEntityFlags;                                  \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= (one) << (_id - _word * 32);                                 \
    } else (void)0

/* An `s32` view of the timer: keeps the compare signed after the
 * ROM's fresh `ldrh`. */
static inline s32 OrbitTimer(struct orbit_part *self)
{
    return self->timer;
}

/* pos += vel. The two empty asms each add a reference to the velocity
 * (brief item 8), which raises its allocation priority so that it gets
 * r0 and the position r1, as in the ROM. */
#define ORBIT_STEP(pos, vel)                                                   \
    {                                                                          \
        s32 _p = (pos);                                                        \
        s32 _v = (vel);                                                        \
                                                                               \
        asm("" : : "r"(_v));                                                   \
        asm("" : : "r"(_v));                                                   \
        (pos) = _p + _v;                                                       \
    }

void UpdateWumpa(struct orbit_part *self)
{
    s32 argP4;
    u32 argP5;

    if (self->state == 1) {
        ORBIT_STEP(self->base.x, self->velX);
        ORBIT_STEP(self->base.y, self->velY);
        if (self->timer != 0) {
            self->timer += 4;
            if (OrbitTimer(self) > 0x100)
                self->timer = 0;
        }
        if (self->base.x >> 8 <= 0x10 && self->base.y >> 8 <= 0x10) {
            PlaySfx(gAudioContext, 0xe, 0x100);
            CollectWumpa(gLevelState);
            ORBIT_MARK_GONE(self, 1);
        }
    } else if (self->state == 2) {
        s32 fire;

        ORBIT_STEP(self->base.x, self->velX);
        ORBIT_STEP(self->base.y, self->velY);
        fire = 0;
        if (self->counter == 0) {
            s32 t = self->timer - 4;

            self->timer = t;
            if (t < 0x40)
                fire = 1;
        } else {
            self->timer += 0xc;
            if (OrbitTimer(self) > 0x1b0)
                fire = 1;
        }
        if (fire) {
            ORBIT_MARK_GONE(self, 1);
        }
    } else if (self->state == 3) {
        if (++self->counter > 10) {
            self->counter = 0;
            {
                s32 sx = self->base.x >> 8;
                s32 sy = self->base.y >> 8;

                volatile u8 *q;

                /* The empty asm takes `&argP5` into a register as its own
                 * insn, so its `add r3, sp, #4` comes before the `movs r5,
                 * #1` (as an address reload of the store, it came after). */
                ((OrbitSpawn4)DropWumpa)(gEntitySpawner, sx, sy,
                    (*(volatile s32 *)&argP4 = 0,
                     ({ MATCH_CONST(q, &argP5); 0; }),
                     *q = 1, 0));
            }
            {
                u32 ph = self->phase + 1;

                self->phase = ph;
                if ((ph << 24) >> 24 > 9) {
                    ORBIT_MARK_GONE(self, 1);
                }
            }
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
            UpdateWumpaHop(self);
        }
    } else if (self->state == 3) {
        struct player *p = gPlayer;
        s32 px = p->x, py = p->y;
        s32 nx = px - 0x400, ny = py - 0xe00;

        self->base.x = nx;
        self->base.y = ny;
    }
    UpdateSpriteObj(&self->base);
}

/* CreateWumpa: the wumpa spawner, called by SpawnWumpa
 * (src/level/spawn_pickups.c) with its spawn-table slot's arguments and by
 * DropWumpa (entity_spawner.c, NAKED, already matched): `CreateWumpa(id,
 * x, y, special)` where `special` is `0xFFFF` or `0` selecting which of
 * two dual_array_manager lists (`gUnknown_030012F4` vs `gUnknown_030012EC`)
 * the newly spawned part joins. Allocates a new 0x54-byte object
 * (`OperatorNew`), re-initializes it (`InitSpriteObj`), points its vtable
 * at `gWumpaVtable`, re-initializes via `ResetWumpaPickup` (wumpa.c,
 * already matched), stores `id` at `+8` and `x`/`y` (Q8-shifted) at `+0`/
 * `+4` - mirrored into `+0x4c`/`+0x50` as a "home position" pair the same
 * way UpdateWumpa's mode-3 branch reads it back - joins the
 * `special`-selected list, points `+0x20` at `gSpriteBankSet`'s shared
 * resource table (fixed slot `0xd2*2`, per the same `DropExtraLife`/
 * `DropWumpa` convention), tags `+0x2d = 1`, builds the OAM/keyframe
 * trio (`ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone`), derives `+0x30` from
 * the same `table[tag]->+0x16` clamp idiom as PickUpWumpa/SendWumpaToHud,
 * clears bits 0/5 of `+0x28`, clears `+0x49`, tags `+0x4a`/`+0x4b` both 0
 * (always - `r7`/`r6` are hardcoded 0 locals, not passed through from any
 * argument), and - since that tag is always 0, never 0xff - never fires
 * the `StartWumpaPayout` special-case call the ROM's own dead `cmp r7,#0xff`
 * still checks for. Finishes with the same `+0x29` nibble-from-
 * `GetPaletteSlot` bitfield combine `DropExtraLife`/`DropWumpa` already use,
 * then returns the new part.
 *
 * Matched (old_agbcc) with three nudges:
 * - `self` is pinned to r4 for the spawn/list-join part. That keeps
 *   local-alloc from handing r4 to the truncated u16 parameters (it puts
 *   them in r5/r6/r8/sb, skipping r7 as it always does), which leaves r7
 *   for the global `mode`, as in the ROM. After the list `if`/`else` the
 *   code uses an unpinned copy `p`, so CSE can keep `&p->tag` in r5
 *   across the anim-setup calls the way the ROM does (a hard-register
 *   pointer loses that).
 * - `mode` is a plain 0 set before the first call. CSE loses it at the
 *   list `if`/`else` join, so the ROM's dead `cmp r7,#0xff` stays.
 * - `phase` is 0 opaqued by an empty asm. The clamp compares it against the
 *   frame count (`cmp r6,r0`) while the stored frame is a fresh 0, and
 *   +0x4B stores it where +0x49 gets its own fresh zero. Writing the tag
 *   through `t` with the byte `one` places the `mov r6,#0` between the
 *   tag address and its `strb`. */

struct orbit_part *CreateWumpa(u16 id, u16 x, u16 y, u16 special)
{
    register struct orbit_part *self asm("r4");
    struct orbit_part *p;
    u8 mode = 0;
    u8 phase;

    self = OperatorNew(0x54);
    InitSpriteObj(&self->base);
    self->base.table = (void *)gWumpaVtable;
    ResetWumpaPickup(self);
    self->base.field_08 = id;
    self->base.x = x << 8;
    self->base.y = y << 8;
    self->anchor = ORBIT_POS(self);
    if (special == 0xffff)
        AddToPartList(gUnknown_030012F4, self);
    else
        AddToPartList(gUnknown_030012EC, self);
    p = self;
    p->bank = (struct act_anim_bank *)(SPRITE_BANK_BASE + 0xd2 * 2);
    {
        u8 one = 1;
        u8 *t = &p->tag;

        phase = 0;
        /* opaque 0: keeps the clamp's `cmp r6,r0` and the +0x4B store
         * from being folded to constants */
        asm("" : "+r"(phase));
        *t = one;
    }
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
    {
        s32 frame = 0;
        s32 count = p->bank->records[p->tag].frameCount;

        if (phase >= count)
            frame = count - 1;
        p->frame = frame;
    }
    p->flipX = 0;
    p->flipY = 0;
    p->counter = 0;
    p->mode = mode;
    p->phase = phase;
    if (mode == 0xff)
        StartWumpaPayout(p);
    p->slotNibble = GetPaletteSlot(gPaletteCache, p->bank->records->paletteId);
    return p;
}

/* SendWumpaToHud: alternative to SendExtraLifeToHud (drop_extra_life.c), called from
 * entity_spawner.c "instead of SendExtraLifeToHud" per that file's own doc
 * comment. Same shape as SendExtraLifeToHud/PickUpWumpa's tail: PlaySfx(
 * gAudioContext, 8, 0x100), self->0x48 = 1, self->x -= self->0x4a<<8
 * (a fixed-offset nudge), self->0x3c = 0xa0, self->0x30 clamped from the
 * same self->0x20/self->0x2d table-lookup idiom, self->0x25 = 1, calls
 * WorldToScreen(self, x>>8, y>>8, &outX, &outY) and re-derives self->x/
 * self->y plus self->0x40/self->0x44 the same way, with a fixed
 * 0xFFFFF000 (-0x1000) offset on both axes instead of a randomized one -
 * then, unlike PickUpWumpa/SendExtraLifeToHud, finishes with
 * ShowHudWumpa(gHud) instead of ShowHudLives.
 *
 * The fixed -0x1000 offsets go through `OrbitOffset` (an inline taking
 * the offset as a parameter): that is what makes old_agbcc reload the
 * 0xFFFFF000 constant from the pool for each axis instead of keeping one
 * copy across the call, as the ROM does. (The earlier "r7 hazard" note
 * was a symptom of compiling with the wrong compiler.) */
static inline s32 OrbitOffset(s32 pos, s32 off)
{
    return pos - off;
}

void SendWumpaToHud(struct orbit_part *self)
{
    s32 outX, outY;
    s32 newX, newY;

    PlaySfx(gAudioContext, 8, 0x100);
    self->state = 1;
    self->base.x -= self->mode << 8;
    self->timer = 0xa0;
    OrbitClampFrame(self);
    self->screenSpace = 1;

    WorldToScreen(self, self->base.x >> 8, self->base.y >> 8, &outX, &outY);

    newX = outX << 8;
    self->base.x = newX;
    self->velX = -FixedDiv(OrbitOffset(newX, 0x1000), 0x1400);
    newY = outY << 8;
    self->base.y = newY;
    self->velY = -FixedDiv(OrbitOffset(newY, 0x1000), 0x1400);
    ShowHudWumpa(gHud);
}

/* StartWumpaPayout: sibling of SendWumpaToHud above - sets `state` = 3
 * and `counter` = 0xa (a fixed countdown), no other side effects. */
void StartWumpaPayout(struct orbit_part *self)
{
    self->state = 3;
    self->counter = 0xa;
}

/* UpdateWumpaHop: address-adjacent to StartWumpaPayout, a small self->0x4b/
 * self->0x4a-driven table helper - copies a fixed 3-word table
 * (gWumpaHopWidths) onto the stack, computes self->y from a
 * gSineTable (shared trig-ish table, see UpdateExtraLife's own doc
 * comment) lookup at self->0x4b*4 scaled by FixedMul(...,0x3000)
 * against self->0x50 (the "home Y" CreateWumpa/UpdateWumpa both write),
 * then computes self->x from a second gSineTable lookup at
 * self->0x4b*2 scaled by FixedMul against the stack copy indexed by
 * self->0x4a-1, added to or subtracted from self->0x4c (the "home X")
 * depending on whether self->0x4a is 1, 2, or anything else (unchanged).
 * Called from UpdateWumpa's own default-mode tail above when
 * self->0x4a is nonzero.
 *
 * Same shape as UpdateExtraLifeHop (extra_life.c) with a 0x3000 y-scale: the
 * sine sample goes through one reused local, which old_agbcc keeps in r2
 * across both calls exactly like the ROM. */
void UpdateWumpaHop(struct orbit_part *self)
{
    struct three_words scales = *(const struct three_words *)gWumpaHopWidths;
    s32 dy;
    s32 sn;

    sn = gSineTable[self->phase * 4];
    dy = FixedMul(sn, 0x3000);
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

/* The ROM pads this function to the next word with zeros, not a nop. */
asm(".align 2, 0");
