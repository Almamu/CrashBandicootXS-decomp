#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "memory.h"
#include <libgcc.h>
#include "actor.h"
#include "bosses.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Position-easing helper, called from `AirshipStateFireballs`/`AirshipStateCannon`
 * (airship_states.c): advances the position
 * accumulators (`gAirshipX`/`gAirshipY`) by their
 * per-frame deltas (`gAirshipVelX`/`gAirshipVelY`), then
 * computes the player's (`gActorList`) signed distance from a
 * fixed keyframe-table-relative target point on each axis
 * (`self+0x1c`/`0x20` against `gAirshipScreenX`/`gAirshipScreenY`
 * offset by `gAirshipBox`'s box) and, per axis, nudges a
 * "shake"/camera-offset accumulator (`gAirshipScreenX`/
 * `gAirshipScreenY`, via `ip`/`r8`) toward the target in small
 * discrete steps once the distance exceeds a `0x2CFF` threshold and a
 * finer `>>10` sub-threshold. Also clamps both accumulators against a
 * set of fixed ranges/bias points (`0xa000`/`0x4FFF`, `0xFFFFD300`/
 * `0x13FF`) and a final `0x180`/`-0x180`, `0x100`/`-0x100` hard clamp.
 */

static inline s32 Abs(s32 x)
{
    s32 s = x >> 31;

    return (x ^ s) - s;
}

/* The register split between `&gAirshipVelX` (r6) and
 * `&gAirshipVelY` (r4) follows from how many stores each easing
 * block has before cross-jumping merges them: the X step stores its
 * `vx -+ 3` result once, the Y step stores in each branch. That makes
 * the Y address the higher-priority pseudo for global-alloc, as in the
 * ROM (docs/matching/archive/issue-58-61-naked-retry.md). */
void SteerAirship(void)
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    struct actor_self *pl;

    gAirshipX += gAirshipVelX;
    gAirshipY += gAirshipVelY;

    pl = gActorList;
    px = pl->x;
    cx = gAirshipScreenX - 0x1200;
    dx = px - cx - (gAirshipBox.x + gAirshipBox.w / 2);
    py = pl->y;
    cy = gAirshipScreenY + 0x1800;
    dy = py - cy - (gAirshipBox.y + gAirshipBox.h / 2);

    if (Abs(dx) <= 0x2CFF) {
        s32 s = dx >> 10;
        s32 t;

        vx = gAirshipVelX;
        if (s >= 0) {
            gAirshipVelX = vx;
            if (s == 0)
                goto dx_done;
            t = vx - 3;
        } else
            t = vx + 3;
        gAirshipVelX = t;
    }
dx_done:
    if (Abs(dy) <= 0x2CFF) {
        s32 v;

        if ((dy >> 10) >= 0) {
            v = gAirshipVelY;
            if ((dy >> 10) == 0)
                goto dy_done;
            gAirshipVelY = v - 2;
        } else {
            v = gAirshipVelY;
            gAirshipVelY = v + 2;
        }
    }
dy_done:

    if (gAirshipScreenX <= 0x1400)
        gAirshipVelX += 6;
    if (gAirshipScreenX > 0x4FFF)
        gAirshipVelX -= 6;
    if (gAirshipScreenY <= -0x2D00)
        gAirshipVelY += 3;
    if (gAirshipScreenY > 0x13FF)
        gAirshipVelY -= 3;

    {
        s32 *p = &gAirshipVelX;
        s32 v = *p;

        LIMIT_MAX(v, 0x180);
        *p = v;
        LIMIT_MIN(v, -0x180);
        gAirshipVelX = v;
    }
    {
        s32 *p = &gAirshipVelY;
        s32 v = *p;

        LIMIT_MAX(v, 0x100);
        *p = v;
        LIMIT_MIN(v, -0x100);
        gAirshipVelY = v;
    }
}

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Constructor for the small tracker object (`gAirship`):
 * stashes its level-index argument into `gAirshipLevel`, and - if
 * `GetActorCheckpoint` (a level-index/mode query) returns zero - clears
 * `gAirshipCheckpointCount`'s spawn-budget counter. Seeds the row/column
 * dimensions (`gAirshipMapCols`/`gAirshipMapRows`) from
 * `gAirshipPicture`'s first two halfwords, allocates the 0x1c-byte
 * tracker object, wires its event table (`gAirshipKeyframes`) and
 * part table (`gAirshipMapFrames`) pointers plus a fixed `+0x18` flag,
 * registers it via `SetActorAnim`, and stores it into
 * `gAirship`. Resets both boss-weapon state globals
 * (`gAirshipState`/`gAirshipStateTimer`) and fires the tracker's own
 * state-0/table-index-0 transition (anim frame from its own part-table
 * pointer at `+0`). Finishes by running `LoadAirshipGraphics` once (the DMA/
 * tile-cache setup + palette fade, airship_load_graphics.c) and clearing
 * `gAirshipBg2PageFlip`'s "apply now" latch.
 *
 * Matched as the inlined C++ `gAirship = new Tracker(...)`:
 * the destination's address is taken before the allocation, the size
 * goes through an `operator new`-style inline wrapper (materialized
 * before the heap flags), and the part-table setup is an inlined
 * constructor taking its values as arguments (all loaded before the
 * stores). */

static inline void BossSetState(s32 st, s32 idx)
{
    struct actor_self *self;
    gAirshipState = st;
    gAirshipStateTimer = 0;
    self = gAirship;
    self->animIndex = idx;
    self->animTimer = self->anims[idx].duration;
    self->animDone = 0;
    if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold)
        self->animTime = 0;
}

static inline struct actor_self *AllocActor(u32 size)
{
    return (struct actor_self *)mem_alloc(size, MEM_HEAP_IWRAM);
}

static inline void InitAnimPart(struct actor_self *self, struct anim_frame_record *anims,
                                u32 *offsets, s32 flag)
{
    self->anims = anims;
    self->frameOffsets = offsets;
    self->palette = flag;
    SetActorAnim(self, 0);
}

void CreateAirship(s32 level)
{
    struct actor_self *t;
    struct actor_self **slot;

    gAirshipLevel = level;
    if (GetActorCheckpoint() == 0)
        gAirshipCheckpointCount = 0;
    gAirshipMapCols = BOSS_PICTURE_SIZE(gAirshipPicture)->cols;
    gAirshipMapRows = BOSS_PICTURE_SIZE(gAirshipPicture)->rows;
    slot = &gAirship;
    t = AllocActor(0x1c);
    InitAnimPart(t, (struct anim_frame_record *)gAirshipKeyframes, (u32 *)gAirshipMapFrames, 1);
    *slot = t;
    BossSetState(0, 0);
    LoadAirshipGraphics();
    gAirshipBg2PageFlip = 0;
}

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * A large "spawn/arm this weapon-kind instance" setup routine: resets
 * the ramp/velocity globals, fires the tracker object's state-1/
 * table-index-0 transition, seeds the position accumulators
 * (`gAirshipX`/`gAirshipY`/`gAirshipZ`) from
 * its own three arguments, looks up a per-kind keyframe-table record
 * (`gAirshipAttacks`, indexed by both `gAirshipLevel` - the
 * level index `CreateAirship` stashed - and this function's own first
 * argument) and copies several of its fields into
 * `gAirshipFireTimer`/`gAirshipHp`, resets the DMA-refresh/
 * palette-strip counters, recomputes the BG2 zoom scale/offset via
 * `GetCellAnimDistance`/`__divsi3`/`SetActorBgLayerDepth`, blits the tracker's
 * current keyframe-table box via `DrawAirshipMap`, sets DISPCNT's bit10,
 * recomputes the BG2 affine matrix (`UpdateAirshipBg2`), and finally queues
 * a palette-strip DMA transfer (`QueueVramDmaTransfer`).
 *
 * Matching notes: the zoom divide is an explicit `__divsi3` call
 * (the ROM reloads `gAirshipDistance` after it, which `/`'s const
 * libcall wouldn't force) and the record lookup is written `a - -b` (see
 * below). `gAirshipLevel` is the level index `CreateAirship` caches,
 * not an object pointer. */

void SpawnAirship(s32 kind, s32 x, s32 y, s32 z)
{
    s32 scale;
    struct actor_self *self;

    gAirshipVelZ = 0x66;
    BossSetState(1, 0);
    gAirshipX = x * 5;
    gAirshipY = y * 2;
    gAirshipZ = z + 0xA000;
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    gAirshipAttack =
        (const struct airship_attack *)(gAirshipLevel * (s32)sizeof(struct airship_attack) -
                                        -(s32)&gAirshipAttacks[kind]);
    gAirshipFireTimer = gAirshipAttack->fireballBurstDelay;
    gAirshipHp = gAirshipAttack->hp;
    gAirshipVolleyCount = 0;
    gAirshipBg2PageFlip = 1;
    gAirshipBg2Page = 0;
    gAirshipDistance = gAirshipZ - INT_TO_Q8(GetCellAnimDistance());
    scale = __divsi3(0x1C00000, gAirshipDistance);
    gAirshipScreenX = Q12_TO_INT(gAirshipX * scale);
    gAirshipScreenY = Q12_TO_INT(scale * gAirshipY);
    SetActorBgLayerDepth(gAirshipDistance);
    self = gAirship;
    {
        s32 t = Q8_TO_INT(self->animTime);
        DrawAirshipMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    REG_DISPCNT |= DISPCNT_BG2_ON;
    UpdateAirshipBg2();
    gAirshipHitFlashTimer = 0;
    QueueVramDmaTransfer(gAirshipHitFlashPalettes, (void *)(BG_PLTT + 0x20), 0x20, 0x10);
}

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * A large per-frame "advance this weapon-kind instance" driver: fires
 * a stride-4 trampoline (`gAirshipStateFuncs`, indexed by the
 * tracker's own state global `gAirshipState`) via `_call_via_r0`,
 * refreshes the palette-strip animation (`AnimateAirshipPalette`), and advances
 * `gAirshipStateTimer`'s frame counter. While the tracker's state is
 * nonzero: advances its own anim-frame accumulator (`+8`, by its part-
 * table's `+0x10` halfword) and, once `GetAnimFrameBaseOffset` crosses
 * the current keyframe-table entry's threshold, both re-arms the
 * accumulator against the *next* entry's own delta and sets the "loop"
 * flag (`+0x12`). Always recomputes the BG2 zoom scale/offset the same
 * way `SpawnAirship` (above) does (`GetCellAnimDistance`/
 * `__divsi3`/`SetActorBgLayerDepth`), and - only when the tracker's
 * accumulator (`+8`, `>>8`) actually crossed to a new keyframe-table
 * index this frame - re-blits its box via `DrawAirshipMap` and re-arms
 * the "apply now" latch (`gAirshipBg2PageFlip`).
 *
 * Matching notes: the zoom divide is a plain call to `__divsi3`
 * (not `/`, whose libcall the compiler would treat as not clobbering
 * memory - the ROM reloads `gAirshipDistance` after it), and the
 * re-blit tail reads the tracker through a fresh local (a separate
 * pseudo from the head's own `self`). */
extern s32 _call_via_r0(void *fn);

void UpdateAirship(void)
{
    s32 prev = Q8_TO_INT(gAirship->animTime);
    struct actor_self *self;

    _call_via_r0(gAirshipStateFuncs[gAirshipState]);
    AnimateAirshipPalette();
    gAirshipStateTimer++;
    if (gAirshipState != 0) {
        s32 scale;

        self = gAirship;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (GetAnimFrameBaseOffset(self) >= self->anims[self->animIndex].loopThreshold) {
            // clang-format off
            self->animTime -= INT_TO_Q8(self->anims[self->animIndex].loopThreshold -
                                         self->anims[self->animIndex].loopBase);
            // clang-format on
            self->animDone = 1;
        }
        gAirshipDistance = gAirshipZ - INT_TO_Q8(GetCellAnimDistance());
        scale = __divsi3(0x1C00000, gAirshipDistance);
        gAirshipScreenX = Q12_TO_INT(gAirshipX * scale);
        gAirshipScreenY = Q12_TO_INT(scale * gAirshipY);
        SetActorBgLayerDepth(gAirshipDistance);
        {
            struct actor_self *cur = gAirship;
            s32 t = Q8_TO_INT(cur->animTime);
            if (prev != t) {
                DrawAirshipMap((u16 *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                gAirshipBg2PageFlip = 1;
            }
        }
    }
}

/* Same boss-weapon subsystem as airship_fireball.c/airship_fall.c - see
 * airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md. */

/* If `gAirshipBg2PageFlip` (an "apply now" latch) is set, toggles
 * `BG2CNT` between two palette/priority presets (tracked by
 * `gAirshipBg2Page`) and clears the latch. Either way, recomputes the
 * BG2 affine matrix (a uniform `scale` from `gAirshipDistance` via
 * `__divsi3`, offset by the screen-projection helpers
 * `GetActorBgCenterX`/`GetActorBgCenterY`) so the effect stays centered while
 * zooming. */
void UpdateAirshipBg2(void)
{
    if (gAirshipBg2PageFlip != 0) {
        if (gAirshipBg2Page == 0) {
            REG_BG2CNT = 0x5809;
        } else {
            REG_BG2CNT = 0x5909;
        }
        gAirshipBg2PageFlip = 0;
        gAirshipBg2Page ^= 1;
    }

    {
        s32 scale = __divsi3(gAirshipDistance << 8, 0x3c00);
        s32 dy = gAirshipScreenX + GetActorBgCenterX();
        s32 dx = gAirshipScreenY + GetActorBgCenterY();

        REG_BG2X = 0x8000 - Q8_MUL(dy, scale);
        REG_BG2Y = 0x8000 - Q8_MUL(dx, scale);

        REG_BG2PA = scale;
        REG_BG2PB = 0;
        REG_BG2PC = 0;
        REG_BG2PD = scale;
    }
}
