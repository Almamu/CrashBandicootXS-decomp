#include "menus.hpp"
#include "level_state.hpp"

extern "C" {
#include "util.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

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

/* Animation `frame` of the icon's bank, from its start. The frame is a
 * word parameter (not u8) so the table word is loaded after the icon
 * pointer, as in the ROM. */
static inline void SetIconFrame(Sprite *s, u32 frame)
{
    s->tag = frame;
    s->ResetFrameTimer();
    s->ResetFrameIndex();
    s->SetAnimDone(0);
}

/* The crystals page: the crystal icon, the crystals found and the 20 to
 * find. */
void PauseMenu::InitCrystalsPage()
{
    crystalIcon = new UiSprite;
    SetIconBank(crystalIcon, 0xde << 1);
    SetIconPos(crystalIcon, &gPauseCrystalIconPos);
    crystalIcon->palette = crystalIcon->GetAnimPaletteSlot();
    FormatDecimal(CountCrystals(progress), crystalCount);
    FormatDecimal(0x14, crystalTotal);
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
        SetIconBank(icon, 0xe4 << 1);
        SetIconFrame(icon, gPausePowerIconFrames[i]);
        SetIconPos(powerIcons[i], &gPausePowerIconPos[i]);
        powerIcons[i]->palette = powerIcons[i]->GetAnimPaletteSlot();
    }
}

/* The gems page: the five gems' icons at half size, the clear gems and
 * colored gems found, and the 28 to find. */
void PauseMenu::InitGemsPage()
{
    s32 i;
    s32 a, b;

    for (i = 0; i <= 4; i++) {
        UiSprite *icon;

        gemIcons[i] = icon = new UiSprite;
        SetIconBank(icon, 0xc0 << 1);
        SetIconFrame(icon, gPauseGemIconFrames[i]);
        SetIconPos(gemIcons[i], &gPauseGemIconPos[i]);
        gemIcons[i]->palette = gemIcons[i]->GetAnimPaletteSlot();
        gemIcons[i]->affine = 0x80;
    }

    a = CountClearGems(progress);
    b = CountGems(progress);
    FormatDecimal(a, clearGemCount);
    FormatDecimal(b, gemCount);
    FormatDecimal(0x1c, gemTotal);
}

/* The relics page: the three relics' icons at half size, the sapphire,
 * gold and platinum relics won, their total and the 20 to win. */
void PauseMenu::InitRelicsPage()
{
    s32 i;

    for (i = 0; i <= 2; i++) {
        UiSprite *icon;

        relicIcons[i] = icon = new UiSprite;
        SetIconBank(icon, 0xc6 << 1);
        SetIconFrame(icon, gPauseRelicIconFrames[i]);
        SetIconPos(relicIcons[i], &gPauseRelicIconPos[i]);
        relicIcons[i]->palette = relicIcons[i]->GetAnimPaletteSlot();
        relicIcons[i]->affine = 0x80;
    }

    FormatDecimal(CountSapphireRelics(progress), sapphireCount);
    FormatDecimal(CountGoldRelics(progress), goldCount);
    FormatDecimal(CountPlatinumRelics(progress), platinumCount);
    FormatDecimal(CountRelics(progress), relicCount);
    FormatDecimal(0x14, relicTotal);
}

/* The time trial page (docs/rom_map.md's overlay_ui "Correction"
 * section): the level's best time (the save block's `time:13`, level_state.h)
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
    {
        /* A byte offset, not an index: keeps the ROM's `idx*4 + 4`
         * computed before the base is loaded. */
        s32 off = levelIdx * 4 + offsetof(struct game_progress, levels);

        time = (u16)*(u32 *)((u8 *)progress + off) >> 3;
    }
    FormatCentiseconds(time, timeText);
    entry = &gLevelTable[levelIdx];
    earned = 0;
    if (time != 0 && time <= entry->times[0])
        earned = 1;
    trialEarned = earned;

    trialIcon = new UiSprite;
    SetIconBank(trialIcon, 0xc6 << 1);
    SetIconPos(trialIcon, &gPauseTimeTrialIconPos);

    if (time != 0) {
        if (time <= entry->times[0])
            SetIconFrame(trialIcon, gPauseRelicIconFrames[2]);
        if (time <= entry->times[1])
            SetIconFrame(trialIcon, gPauseRelicIconFrames[1]);
        if (time <= entry->times[2])
            SetIconFrame(trialIcon, gPauseRelicIconFrames[0]);
        trialIcon->palette = trialIcon->GetAnimPaletteSlot();
    }
}
