#include "spawners.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "level_state.h"
#include "level.h"
#include "globals.h"
}

/* The player's mirror bit stored from a parameter: a general bitfield
 * insert, as in the ROM. */
static inline void SetFlipX(Player *p, u32 v)
{
    p->mirrorFlags.mirrorX = v;
}

/* Entity type 0x00, the player start of the normal rooms (#664,
 * include/spawners.hpp). Two halves:
 *
 * 1. If the level state says to spawn at the start (GetSpawnAtStart),
 *    the player faces the way bit 1 of the entity's parameter flags
 *    says and moves to the entity's position (as SpawnPlayerPosition,
 *    spawn_pickups.cpp, does).
 * 2. Unless in a time trial: once the player has died
 *    GetMaskAssistDeaths times, or with no lives left outside the bonus
 *    round and no mask, the player gets an Aku Aku mask
 *    (EVENT_MASK_GAIN) and its sound plays.
 *
 * Old_agbcp, like the old_agbcc C before. That C was a NAKED asm
 * transcription for a long time (docs/matching/archive/naked-sub_801e990-matched.md),
 * then 17 pins and 2 `asm` statements, a hand-written slot-13 call and
 * gotos; the C++ needs none. */
void SpawnStartMarker(u32 arg0, u16 x, u16 y, u16 z)
{
    if (GetSpawnAtStart(gLevelState)) {
        const struct entity_params *rec = EntityParams(z);

        SetFlipX(gPlayer, (rec->flags >> 1) & 1);
        gPlayer->x = INT_TO_Q8(x);
        gPlayer->y = INT_TO_Q8(y);
    }
    if (gLevelState->timeTrial != 0)
        return;
    if (GetDeaths(gLevelState) >= GetMaskAssistDeaths(gLevelState) ||
        (GetLives(gLevelState) == 0 && !IsInBonusRound(gLevelState) &&
         gLevelState->maskLevel == MASK_LEVEL_NONE)) {
        gPlayer->HandleEvent(0, EVENT_MASK_GAIN, 0);
        gAudioContext->PlaySfx(SFX_AKU_AKU_GAIN, 0x100);
    }
}
