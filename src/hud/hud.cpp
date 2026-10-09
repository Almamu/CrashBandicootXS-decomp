#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "level.h"
#include "globals.h"
}

/* The HUD stat-widget family's dispatcher - see docs/rom_map.md's "full
 * HUD stat-widget family" section. Hud::Update (UpdateHud)
 * calls the other widgets on the same HUD (UpdateLives is in
 * hud_counters.cpp) - `parts` is the 35-slot array the constructor builds. Runs the percentage
 * counter (`UpdateHudPercentCounters`) when `icon_flag` is set, then the score
 * counter (`UpdateHudLives`) unconditionally, then branches on
 * `GetBossIndex`'s level-type/game-mode result: a non-"none" mode
 * (!= -1) hands off entirely to the icon-indicator widget
 * (`UpdateHudBoss`) and returns early, skipping the rest of the family;
 * otherwise it refreshes the last OAM slot's animation state whenever
 * `IsInBonusRound` says the mode changed, conditionally runs
 * `UpdateHudClock` while a "paused"-style central-state flag is set and
 * `mode`/`field_08` are both still zero, then unconditionally runs the
 * two remaining digit counters (`UpdateHudCrates`, `UpdateHudWumpa`). */
void Hud::Update()
{
    gHudSlideOffset = 0;

    if (icon_flag) {
        UpdatePercentCounters();
    }

    UpdateLives();

    if (gLevelState->GetBossIndex() != BOSS_NONE) {
        UpdateBoss();
        return;
    }

    if (gLevelState->IsInBonusRound()) {
        gHudSlideOffset = 0;
        parts[34].AdvanceAnim();
        parts[34].Draw(0, 0);
    }

    if (gLevelState->timeTrial != 0 && livesSlide == 0 && wumpaSlide == 0) {
        UpdateClock();
    }

    UpdateCrates();
    UpdateWumpa();
}
