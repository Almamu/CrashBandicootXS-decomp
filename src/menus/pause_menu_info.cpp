#include "menus.hpp"
#include "audio.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* PauseMenu::InitInfo (menus.hpp; C++ since the #664 cleanup), the pause
 * menu's info constructor: the current level's name and number (" N"
 * for the numbered levels, none past them), the completion percentage,
 * the music and sound volumes (GetMusicVolume/GetSfxVolume scaled
 * `(v + 0xc) * 20 / 256` to 5% steps, then " <NN%>"-formatted), then the
 * five info pages (pause_menu_pages_init.cpp). */
void PauseMenu::InitInfo()
{
    s32 levelIdx = GetCurrentLevel(gLevelState);
    u32 labelId = *(u32 *)((u8 *)gLevelTable + levelIdx * 0x24);

    levelName = (void *)GetUiText(labelId);

    if (levelIdx <= LEVEL_LAST_NUMBERED) {
        levelLabel = (void *)GetUiText(0);
        levelNumber[0] = ' ';
        FormatDecimal(levelIdx + 1, &levelNumber[1]);
    } else {
        levelLabel = 0;
    }

    {
        s32 count = FormatDecimal(GetCompletionPercent(progress), percentText);
        percentText[count] = '%';
        percentText[count + 1] = 0;
    }

    musicVolume = (gAudioContext->GetMusicVolume() + 0xc) * 20 / 256;
    soundVolume = (gAudioContext->GetSfxVolume() + 0xc) * 20 / 256;

    FormatVolume(musicVolume, musicVolumeText);
    FormatVolume(soundVolume, soundVolumeText);

    InitCrystalsPage();
    InitPowersPage();
    InitGemsPage();
    InitRelicsPage();
    InitTimeTrialPage();
}
