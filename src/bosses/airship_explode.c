#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "util.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "bosses.h"
#include "level_state.h"
#include "globals.h"
#include "math_util.h"

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_states.c - see
 * airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Large weapon-kind projectile spawner: advances the position
 * accumulators (`gAirshipX`/`gAirshipY`/
 * `gAirshipZ`), clears `gAirshipHitFlashTimer`'s DMA-refresh
 * counter, and derives two base screen coordinates from a fixed
 * keyframe-table box (`gAirshipBox`, `>>8`) offset by the
 * accumulators. Dispatches on `gAirshipStateTimer` (a frame/flags
 * counter, the same one `LoadAirshipGraphics`'s palette fade reads) through
 * five weapon-kind cases (0xa/0x32/0x50/0x6e/0xaa), each clearing one
 * BG palette bank-1 slot then spawning 1-3 sub-projectiles via
 * `RandRange` (a per-axis jitter/randomizer) and `CreateJetpackExplosion` (the
 * actual spawn call, `(x, y, z)`); the 0xaa case instead
 * fires the state-5/table-index-1 transition on the tracker object,
 * plays a sound, and - gated by a lock byte
 * (`gLevelState+0x8c`) and a spawn-budget counter
 * (`gAirshipCheckpointCount`) - spawns a homing/seek effect via
 * `SetJetpackCheckpoint`/`CreateJetpackCheckpointText`.
 *
 * Matching notes: the box table is `const` (so its jitter ranges stay
 * CSE'd in registers across the spawn calls), the palette base pointer
 * is assigned right where the ROM materializes it (declared-and-
 * initialized at the top it gets hoisted into a callee-saved register),
 * the RNG `RandRange` is read back as a `u16` here (the ROM zero-
 * extends its result), and the seek spawn takes `&gActorList`
 * before the last lock check, as the ROM loads that address early. */

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

/* One sub-projectile, jittered around (x, y) by the box's own +-range. */
#define SPAWN(x, y) CreateJetpackExplosion((x) + RandRange(INT_TO_Q8(gAirshipBox.w)), \
                                (y) + RandRange(INT_TO_Q8(gAirshipBox.h)), \
                                gAirshipZ - 0x100)

void AirshipStateExplode(void)
{
    s32 x, y;
    u16 *pal;

    gAirshipX += gAirshipVelX;
    gAirshipY += gAirshipVelY;
    gAirshipZ += gAirshipVelZ;
    gAirshipHitFlashTimer = 0;
    pal = (u16 *)(BG_PLTT + 0x20);
    x = gAirshipX + INT_TO_Q8(gAirshipBox.x);
    y = gAirshipY + INT_TO_Q8(gAirshipBox.y);

    if (gAirshipStateTimer == 0xa) {
        pal[15] = 0;
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x32) {
        pal[1] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x50) {
        pal[4] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0x6e) {
        pal[8] = 0;
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
        SPAWN(x, y);
    } else if (gAirshipStateTimer == 0xaa) {
        ResumeActorSpawns();
        BossSetState(5, 1);
        PlaySfx(gAudioContext, SFX_UNKNOWN_42, 0x100);
        gAirshipVelZ = 0x9d;
        if (gLevelState->timeTrial == 0 && gAirshipCheckpointCount <= 1) {
            struct actor_self **pl = &gActorList;
            if (gJetpackPlayerInactive == 0) {
                SetJetpackCheckpoint(*pl);
                CreateJetpackCheckpointText();
                gAirshipCheckpointCount++;
            }
        }
    }
}
