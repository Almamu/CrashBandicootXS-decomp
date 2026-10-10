/* The save block's statistics, GameProgress's methods (#766,
 * include/game_progress.hpp): GetLives (GetProgressLives), the relic, gem
 * and crystal counts, and GetCompletionPercent. Split from the end of
 * menus/power_dialog_draw.cpp and the start of gfx/graphics.cpp (#767);
 * both were old_agbcc, as this file is. */

#include "game_progress.hpp"

extern "C" {
#include "core.h"
#include <libgcc.h>
#include "menus.h"
#include "level.h"
#include "globals.h"
}

/* gLevelTable's time-trial thresholds (`level_info.times`, level.h):
 * CountSapphireRelics/CountGoldRelics/CountPlatinumRelics each count how
 * many of a caller's 20 records fall between two adjacent thresholds
 * (times[0]/[1] for one function, times[1]/[2] for the next, and just
 * times[2] alone for the simplest one). */

s32 GameProgress::GetLives() const
{
    return lives;
}

/* The save block's counts (the pause menu's pages, the save menu's rows,
 * the game-over screen's totals). The relics are the time-trial medals:
 * a platinum relic for a time within the level's times[2], gold within
 * times[1], sapphire within times[0]. */
s32 GameProgress::CountPlatinumRelics() const
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 GameProgress::CountGoldRelics() const
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[1] && val > gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 GameProgress::CountSapphireRelics() const
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[0] && val > gLevelTable[i].times[1]) {
                count++;
            }
        }
    }
    return count;
}

s32 GameProgress::CountRelics() const
{
    s32 total;
    s32 b;
    s32 c;

    total = CountSapphireRelics();
    b = CountGoldRelics();
    c = CountPlatinumRelics();
    total += b;
    total += c;
    return total;
}

s32 GameProgress::CountGems() const
{
    s32 total;
    s32 i;
    s32 result;
    u8 bits;

    total = 0;
    for (i = 0; i < 20; i++)
        total += levels[i].b.flag1 + levels[i].b.flag2;
    total += levels[24].b.flag1 + levels[24].b.flag2;
    bits = flags;
    result = total + (((u32)bits << 31) >> 31);
    result += ((u32)bits << 29) >> 31;
    result += ((u32)bits << 28) >> 31;
    result += ((u32)bits << 30) >> 31;
    return result;
}

s32 GameProgress::CountClearGems() const
{
    s32 total;
    s32 i;

    total = 0;
    for (i = 0; i < 20; i++)
        total += levels[i].b.flag1 + levels[i].b.flag2;
    total += levels[24].b.flag1 + levels[24].b.flag2;
    return total;
}

s32 GameProgress::CountCrystals() const
{
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 20; i++)
        count += levels[i].b.cleared;
    return count;
}

/* The save's completion percentage: the crystals, gems, relics (a
 * sapphire counts half) and the four secret flags, out of 72. */
s32 GameProgress::GetCompletionPercent() const
{
    s32 total = CountCrystals();
    s32 gems = CountGems();
    s32 sapphires = CountSapphireRelics();
    s32 golds = CountGoldRelics();
    s32 platinums = CountPlatinumRelics();
    u8 bits;

    total += gems;
    total += sapphires / 2;
    total += golds;
    total += platinums;
    bits = flags;
    total += bits >> 7;
    total += ((u32)bits << 26) >> 31;
    total += ((u32)bits << 25) >> 31;
    total += ((u32)bits << 27) >> 31;
    return __divsi3(total * 100, 0x48);
}
