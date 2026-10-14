#include "menus.hpp"
#include "level_state.hpp"

extern "C" {
#include "engine/util/util.h"
#include "engine/sys/memory.h"
#include "level.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Game::Entities::Sprite;
using Game::Entities::UiSprite;
using Game::Level::GameProgress;
using Game::Ui::PauseMenu;

/* The pause menu's five info pages (PauseMenu, menus.hpp), built by
 * InitInfo (pause_menu_info.cpp): each page's icons, and its numbers as
 * text for the drawing code. Built with old_agbcc, now old_agbcp
 * (Makefile OLD_AGBCC_OBJS). */

/* Puts an icon at a fixed position. The ROM calls Entity::SetPixelPos
 * out of line here (an inline method, which g++ would inline), so this
 * calls its C name, SetEntityPixelPos, as level_select.cpp does. */
static inline void SetIconPos(Sprite *s, const struct vec2 *p)
{
    SetEntityPixelPos(s, p->x, p->y);
}

/* The crystals page: the crystal icon, the crystals found and the
 * CRYSTAL_TOTAL to find. */
void PauseMenu::InitCrystalsPage()
{
    crystalIcon = new UiSprite;
    SetIconBank(crystalIcon, 37);
    SetIconPos(crystalIcon, &gPauseCrystalIconPos);
    crystalIcon->palette = crystalIcon->GetAnimPaletteSlot();
    FormatDecimal(progress->CountCrystals(), crystalCount);
    FormatDecimal(CRYSTAL_TOTAL, crystalTotal);
}

/* The powers page: the four powers' icons. `icons[i] = icon = new
 * UiSprite` computes the slot's address before the allocation, which also
 * keeps the loop from being strength-reduced, as in the ROM. */
void PauseMenu::InitPowersPage()
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        UiSprite *icon;

        powerIcons[i] = icon = new UiSprite;
        SetIconBank(icon, 38);
        icon->StartAnim(gPausePowerIconFrames[i]);
        SetIconPos(powerIcons[i], &gPausePowerIconPos[i]);
        powerIcons[i]->palette = powerIcons[i]->GetAnimPaletteSlot();
    }
}

/* The gems page: the five gems' icons at half size, the clear gems and
 * colored gems found, and the GEM_TOTAL to find. */
void PauseMenu::InitGemsPage()
{
    s32 i;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        UiSprite *icon;

        gemIcons[i] = icon = new UiSprite;
        SetIconBank(icon, 32);
        icon->StartAnim(gPauseGemIconFrames[i]);
        SetIconPos(gemIcons[i], &gPauseGemIconPos[i]);
        gemIcons[i]->palette = gemIcons[i]->GetAnimPaletteSlot();
        gemIcons[i]->affine = Q8(0.5);
    }

    /* `a` is needed: inlined, the code is 2 instructions shorter */
    a = progress->CountClearGems();
    /* `b` is needed: inlined, the code is 4 instructions shorter */
    b = progress->CountGems();
    FormatDecimal(a, clearGemCount);
    FormatDecimal(b, gemCount);
    FormatDecimal(GEM_TOTAL, gemTotal);
}

/* The relics page: the three relics' icons at half size, the sapphire,
 * gold and platinum relics won, their total and the RELIC_TOTAL to win. */
void PauseMenu::InitRelicsPage()
{
    s32 i;

    for (i = 0; i <= 2; i++) {
        UiSprite *icon;

        relicIcons[i] = icon = new UiSprite;
        SetIconBank(icon, 33);
        icon->StartAnim(gPauseRelicIconFrames[i]);
        SetIconPos(relicIcons[i], &gPauseRelicIconPos[i]);
        relicIcons[i]->palette = relicIcons[i]->GetAnimPaletteSlot();
        relicIcons[i]->affine = Q8(0.5);
    }

    FormatDecimal(progress->CountSapphireRelics(), sapphireCount);
    FormatDecimal(progress->CountGoldRelics(), goldCount);
    FormatDecimal(progress->CountPlatinumRelics(), platinumCount);
    FormatDecimal(progress->CountRelics(), relicCount);
    FormatDecimal(RELIC_TOTAL, relicTotal);
}

/* The time trial page (docs/rom_map.md's overlay_ui "Correction"
 * section): the level's best time (the save block's `time:13`, game_progress.hpp)
 * as text, and the medal icon, the best relic the time won against the
 * level's thresholds in gLevelTable. `trialEarned` is set for a time
 * within times[0], the sapphire. */
void PauseMenu::InitTimeTrialPage()
{
    s32 levelIdx;
    u32 time;
    const struct level_info *entry;
    u8 earned;

    levelIdx = gLevelState->GetCurrentLevel();

    /* A byte offset, not an index: keeps the ROM's `idx*4 + 4`
     * computed before the base is loaded. */
    s32 off = levelIdx * 4 + offsetof(GameProgress, levels);

    time = (u16)*(u32 *)((u8 *)progress + off) >> 3;
    FormatCentiseconds(time, timeText);
    entry = &gLevelTable[levelIdx];
    earned = 0;
    if (time != 0 && time <= entry->times[0])
        earned = 1;
    trialEarned = earned;

    trialIcon = new UiSprite;
    SetIconBank(trialIcon, 33);
    SetIconPos(trialIcon, &gPauseTimeTrialIconPos);

    if (time != 0) {
        if (time <= entry->times[0])
            trialIcon->StartAnim(gPauseRelicIconFrames[2]);
        if (time <= entry->times[1])
            trialIcon->StartAnim(gPauseRelicIconFrames[1]);
        if (time <= entry->times[2])
            trialIcon->StartAnim(gPauseRelicIconFrames[0]);
        trialIcon->palette = trialIcon->GetAnimPaletteSlot();
    }
}
