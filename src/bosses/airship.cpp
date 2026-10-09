#include "boss_actors.hpp"

extern "C" {
#include <libgcc.h>
#include "actor.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* N. Gin's airship (#664 part 11i): a bare AnimPart, gAirship, and its
 * globals (include/boss_actors.hpp). See
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
void SteerAirship(void)
{
    s32 vx;
    s32 dx, dy, cx, cy, px, py;
    ActorSelf *pl;

    gAirshipX += gAirshipVelX;
    gAirshipY += gAirshipVelY;

    pl = gActorList;
    px = pl->x;
    cx = gAirshipScreenX - 0x1200;
    dx = px - cx - (gAirshipBox.x + gAirshipBox.w / 2);
    py = pl->y;
    cy = gAirshipScreenY + 0x1800;
    dy = py - cy - (gAirshipBox.y + gAirshipBox.h / 2);

    if (ABS_BRANCHLESS(dx) <= 0x2CFF) {
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
    if (ABS_BRANCHLESS(dy) <= 0x2CFF) {
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

/* Sets the airship up for level `level`: the picture's size, its
 * animation (`new AnimPart`, an IWRAM allocation; the ROM takes
 * &gAirship before the allocation, as g++ does), state 0 (inactive) and
 * its graphics. The checkpoint count restarts unless the level was
 * entered from a checkpoint. */
void CreateAirship(s32 level)
{
    gAirshipLevel = level;
    if (GetActorCheckpoint() == 0)
        gAirshipCheckpointCount = 0;
    gAirshipMapCols = BOSS_PICTURE_SIZE(gAirshipPicture)->cols;
    gAirshipMapRows = BOSS_PICTURE_SIZE(gAirshipPicture)->rows;
    gAirship = new AnimPart((struct anim_frame_record *)gAirshipKeyframes, gAirshipMapFrames, 1);
    SetAirshipState(0, 0);
    LoadAirshipGraphics();
    gAirshipBg2PageFlip = 0;
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
 * `GetCellAnimDistance`/`__divsi3`/`SetActorBgLayerDepth`, blits the airship's
 * current frame via `DrawAirshipMap`, sets DISPCNT's bit10,
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
    AnimPart *self;

    gAirshipVelZ = 0x66;
    SetAirshipState(1, 0);
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
    gAirshipScreenX = Q12_MUL(gAirshipX, scale);
    gAirshipScreenY = Q12_MUL(scale, gAirshipY);
    SetActorBgLayerDepth(gAirshipDistance);
    self = gAirship;
    {
        s32 t = Q8_TO_INT(self->animTime);
        DrawAirshipMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
    }
    REG_DISPCNT |= DISPCNT_BG2_ON;
    UpdateAirshipBg2();
    gAirshipHitFlashTimer = 0;
    QueueVramDmaTransfer((void *)gAirshipHitFlashPalettes, (void *)(BG_PLTT + 0x20), 0x20, 0x10);
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
void UpdateAirship(void)
{
    s32 prev = Q8_TO_INT(gAirship->animTime);
    AnimPart *self;

    gAirshipStateFuncs[gAirshipState]();
    AnimateAirshipPalette();
    gAirshipStateTimer++;
    if (gAirshipState != 0) {
        s32 scale;

        self = gAirship;
        self->animTime += (s16)self->animTimer;
        self->animDone = 0;
        if (self->GetAnimFrameBaseOffset() >= self->anims[self->animIndex].loopThreshold) {
            ANIM_REWIND(self->animTime, self->anims[self->animIndex]);
            self->animDone = 1;
        }
        gAirshipDistance = gAirshipZ - INT_TO_Q8(GetCellAnimDistance());
        scale = __divsi3(0x1C00000, gAirshipDistance);
        gAirshipScreenX = Q12_MUL(gAirshipX, scale);
        gAirshipScreenY = Q12_MUL(scale, gAirshipY);
        SetActorBgLayerDepth(gAirshipDistance);
        {
            AnimPart *cur = gAirship;
            s32 t = Q8_TO_INT(cur->animTime);
            if (prev != t) {
                DrawAirshipMap((u16 *)cur->frameOffsets[cur->anims[cur->animIndex].frameIndex + t]);
                gAirshipBg2PageFlip = 1;
            }
        }
    }
}

/* Same boss-weapon subsystem as airship_fireball.cpp/airship_fall.cpp - see
 * airship_fireball.cpp's header comment and
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
