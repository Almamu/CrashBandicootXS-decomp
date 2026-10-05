#include "core.h"
#include "actor.h"
#include "hud.h"

extern void *gLevelState;

extern void AdvanceSpriteAnim(struct actor *part);
extern s32 GetBossIndex(void *self);
extern u8 IsInBonusRound(void *self);

/* The HUD stat-widget family's dispatcher - see docs/rom_map.md's "full
 * HUD stat-widget family" section. `self` is the same `struct
 * hud_counter` passed straight through to every callee here (including
 * `UpdateHudLives`, matched separately in hud_lives.c) - `sself->parts`
 * is the 35-slot OAM array `InitHud` builds. Runs the percentage
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
void UpdateHud(struct hud_counter *self)
{
    register struct hud_counter *sself asm("r5") = self;

    gHudSlideOffset = 0;

    if (sself->icon_flag) {
        UpdateHudPercentCounters(sself);
    }

    UpdateHudLives(sself);

    if (GetBossIndex(gLevelState) != -1) {
        UpdateHudBoss(sself);
        return;
    }

    if (IsInBonusRound(gLevelState)) {
        gHudSlideOffset = 0;
        AdvanceSpriteAnim((struct actor *)&sself->parts[34]);
        DrawHudPart(&sself->parts[34], 0, 0);
    }

    if (*((u8 *)gLevelState + 0x8c) != 0 && sself->livesSlide == 0 && sself->wumpaSlide == 0) {
        UpdateHudClock(sself);
    }

    UpdateHudCrates(sself);
    UpdateHudWumpa(sself);
}
