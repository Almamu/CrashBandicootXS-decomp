#include "airship.hpp"
#include "vram_dma_queue.hpp"
#include "cell_anim.hpp"
#include "actor_bg.hpp"

extern "C" {
#include <libgcc.h>
#include "actor.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* N. Gin's airship (#664 part 11i), the all-static class Airship
 * (#772, include/airship.hpp): a bare AnimPart, `anim` (gAirship), and
 * static data members under their C names (gAirshipX, ...). See
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Position-easing helper, called from `AirshipStateFireballs`/`AirshipStateCannon`
 * (airship_states.cpp): advances the position
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

/* The register split between `&gAirshipVelX` (r6) and
 * `&gAirshipVelY` (r4) follows from how many stores each easing
 * block has before cross-jumping merges them: the X step stores its
 * `vx -+ 3` result once, the Y step stores in each branch. That makes
 * the Y address the higher-priority pseudo for global-alloc, as in the
 * ROM (docs/matching/archive/issue-58-61-naked-retry.md). */
void Airship::Steer()
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    ActorSelf *pl;

    x += velX;
    y += velY;

    pl = gActorList;
    /* `px` is needed: inlined, the instructions are reordered */
    px = pl->x;
    /* `cx` is needed: inlined, the code is 1 instruction longer */
    cx = screenX - INT_TO_Q8(18);
    dx = px - cx - (box.x + box.w / 2);
    /* `py` is needed: inlined, the code is 2 instructions longer */
    py = pl->y;
    /* `cy` is needed: inlined, the code is 3 instructions longer */
    cy = screenY + INT_TO_Q8(24);
    dy = py - cy - (box.y + box.h / 2);

    if (ABS_BRANCHLESS(dx) <= INT_TO_Q8(45) - 1) {
        s32 s = dx >> 10;
        s32 t;

        vx = velX;
        if (s >= 0) {
            velX = vx;
            if (s == 0)
                goto dx_done;
            t = vx - AIRSHIP_STEER_ACCEL_X;
        } else
            t = vx + AIRSHIP_STEER_ACCEL_X;
        velX = t;
    }
dx_done:
    if (ABS_BRANCHLESS(dy) <= INT_TO_Q8(45) - 1) {
        s32 v;

        if ((dy >> 10) >= 0) {
            v = velY;
            if ((dy >> 10) == 0)
                goto dy_done;
            velY = v - AIRSHIP_STEER_ACCEL_Y;
        } else {
            v = velY;
            velY = v + AIRSHIP_STEER_ACCEL_Y;
        }
    }
dy_done:

    if (screenX <= INT_TO_Q8(20))
        velX += AIRSHIP_EDGE_PUSH_X;
    if (screenX > INT_TO_Q8(80) - 1)
        velX -= AIRSHIP_EDGE_PUSH_X;
    if (screenY <= -INT_TO_Q8(45))
        velY += AIRSHIP_EDGE_PUSH_Y;
    if (screenY > INT_TO_Q8(20) - 1)
        velY -= AIRSHIP_EDGE_PUSH_Y;

    s32 *p = &velX;
    s32 v = *p;

    LIMIT_MAX(v, AIRSHIP_MAX_SPEED_X);
    *p = v;
    LIMIT_MIN(v, -AIRSHIP_MAX_SPEED_X);
    velX = v;
    /* scoped: its own `p`/`v` (shared, the code is 4 instructions shorter) */
    {
        s32 *p = &velY;
        s32 v = *p;

        LIMIT_MAX(v, AIRSHIP_MAX_SPEED_Y);
        *p = v;
        LIMIT_MIN(v, -AIRSHIP_MAX_SPEED_Y);
        velY = v;
    }
}

/* Sets the airship up for level `lvl`: the picture's size, its
 * animation (`new AnimPart`, an IWRAM allocation; the ROM takes
 * &gAirship before the allocation, as g++ does), state 0 (inactive) and
 * its graphics. The checkpoint count restarts unless the level was
 * entered from a checkpoint. */
void Airship::Create(s32 lvl)
{
    level = lvl;
    if (GetActorCheckpoint() == 0)
        checkpointCount = 0;
    mapCols = BOSS_PICTURE_SIZE(picture)->cols;
    mapRows = BOSS_PICTURE_SIZE(picture)->rows;
    anim = new AnimPart(keyframes, mapFrames, 1);
    SetState(AIRSHIP_STATE_INACTIVE, 0);
    LoadGraphics();
    bg2PageFlip = 0;
}

/* A large "spawn/arm this weapon-kind instance" setup routine: resets
 * the ramp/velocity globals, enters state 1 (approach) with
 * animation 0, seeds the position accumulators
 * (`gAirshipX`/`gAirshipY`/`gAirshipZ`) from
 * its own three arguments, looks up a per-kind keyframe-table record
 * (`gAirshipAttacks`, indexed by both `gAirshipLevel` - the
 * level index `CreateAirship` stashed - and this function's own first
 * argument) and copies several of its fields into
 * `gAirshipFireTimer`/`gAirshipHp`, resets the DMA-refresh/
 * palette-strip counters, recomputes the BG2 zoom scale/offset via
 * `CellAnim::GetDistance`/`__divsi3`/`ActorBg::SetLayerDepth`, blits the airship's
 * current frame via `DrawAirshipMap`, sets DISPCNT's bit10,
 * recomputes the BG2 affine matrix (`UpdateAirshipBg2`), and finally queues
 * a palette-strip DMA transfer (`VramDmaQueue::Add`).
 *
 * Matching notes: the zoom divide is an explicit `__divsi3` call
 * (the ROM reloads `gAirshipDistance` after it, which `/`'s const
 * libcall wouldn't force) and the record lookup is written `a - -b` (see
 * below). `gAirshipLevel` is the level index `CreateAirship` caches,
 * not an object pointer. */

void Airship::Spawn(s32 kind, s32 sx, s32 sy, s32 sz)
{
    s32 scale;
    AnimPart *self;

    velZ = AIRSHIP_APPROACH_SPEED_Z;
    SetState(AIRSHIP_STATE_APPROACH, 0);
    x = sx * 5;
    y = sy * 2;
    z = sz + INT_TO_Q8(160);
    /* `a - -b` rather than `a + b`: the latter lets fold reassociate the
     * constant table base out of `&table[kind]`, while the ROM adds the
     * level offset to the finished record address. */
    attack = (const struct airship_attack *)(level * (s32)sizeof(struct airship_attack) -
                                             -(s32)&attacks[kind]);
    fireTimer = attack->fireballBurstDelay;
    hp = attack->hp;
    volleyCount = 0;
    bg2PageFlip = 1;
    bg2Page = 0;
    distance = z - INT_TO_Q8(CellAnim::GetDistance());
    scale = __divsi3(ACTOR_PROJECTION, distance);
    screenX = Q12_MUL(x, scale);
    screenY = Q12_MUL(scale, y);
    ActorBg::SetLayerDepth(distance);
    self = anim;

    s32 t = Q8_TO_INT(self->animTime);
    DrawMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    REG_DISPCNT |= DISPCNT_BG2_ON;
    UpdateBg2();
    hitFlashTimer = 0;
    VramDmaQueue::Add(hitFlashPalettes, BG_PALETTE(1), PALETTE_SIZE_16, 0x10);
}

/* The airship's frame: the state function (gAirshipStateFuncs, a plain
 * function table), the hit flash, and, once it is active, its animation
 * step, the BG2 zoom from its distance, and the picture's map again when
 * the animation moved on to another frame.
 *
 * Matching notes: the zoom divide is a plain call to `__divsi3`
 * (not `/`, whose libcall the compiler would treat as not clobbering
 * memory - the ROM reloads `gAirshipDistance` after it), and the
 * re-blit tail reads the airship through a fresh local (a separate
 * pseudo from the head's own `self`). */
void Airship::Update()
{
    /* `prev` is needed: inlined, the code is 25 instructions shorter */
    s32 prev = Q8_TO_INT(anim->animTime);
    AnimPart *self;

    stateFuncs[state]();
    AnimatePalette();
    stateTimer++;
    if (state != AIRSHIP_STATE_INACTIVE) {
        s32 scale;

        self = anim;
        self->animTime += self->animTimer;
        self->animDone = 0;
        if (self->GetAnimFrameBaseOffset() >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        distance = z - INT_TO_Q8(CellAnim::GetDistance());
        scale = __divsi3(ACTOR_PROJECTION, distance);
        screenX = Q12_MUL(x, scale);
        screenY = Q12_MUL(scale, y);
        ActorBg::SetLayerDepth(distance);

        AnimPart *cur = anim;
        s32 t = Q8_TO_INT(cur->animTime);
        if (prev != t) {
            DrawMap((u16 *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
            bg2PageFlip = 1;
        }
    }
}

/* Same boss-weapon subsystem as airship_fireball.cpp/airship_states.cpp - see
 * airship_fireball.cpp's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md. */

/* If `gAirshipBg2PageFlip` (an "apply now" latch) is set, toggles
 * `BG2CNT` between two palette/priority presets (tracked by
 * `gAirshipBg2Page`) and clears the latch. Either way, recomputes the
 * BG2 affine matrix (a uniform `scale` from `gAirshipDistance` via
 * `__divsi3`, offset by the screen-projection helpers
 * `ActorBg::GetCenterX`/`ActorBg::GetCenterY`) so the effect stays centered while
 * zooming. */
void Airship::UpdateBg2()
{
    if (bg2PageFlip != 0) {
        if (bg2Page == 0) {
            REG_BG2CNT =
                BGCNT_PRIORITY(1) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(24) | BGCNT_AFF256x256;
        } else {
            REG_BG2CNT =
                BGCNT_PRIORITY(1) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(25) | BGCNT_AFF256x256;
        }
        bg2PageFlip = 0;
        bg2Page ^= 1;
    }

    /* `distance << 8`: Q8_DIV(distance, INT_TO_Q8(60))'s numerator, as
     * an explicit __divsi3 call (see Spawn) */
    s32 scale = __divsi3(distance << 8, INT_TO_Q8(60));
    /* `dy` is needed: inlined, the code is 1 instruction longer */
    s32 dy = screenX + ActorBg::GetCenterX();
    /* `dx` is needed: inlined, the code is 2 instructions longer */
    s32 dx = screenY + ActorBg::GetCenterY();

    REG_BG2X = 0x8000 - Q8_MUL(dy, scale);
    REG_BG2Y = 0x8000 - Q8_MUL(dx, scale);

    REG_BG2PA = scale;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = scale;
}
