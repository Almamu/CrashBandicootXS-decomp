/* The save block's statistics (struct game_progress): GetProgressLives,
 * the relic, gem and crystal counts, and GetCompletionPercent. Split from
 * the end of menus/power_dialog_draw.cpp and the start of gfx/graphics.cpp
 * (#767); both were old_agbcc, as this file is. */

extern "C" {
#include "core.h"
#include <libgcc.h>
#include "menus.h"
#include "level.h"
#include "level_state.h"
#include "globals.h"
}

/* gLevelTable's time-trial thresholds (`level_info.times`, level.h):
 * CountSapphireRelics/CountGoldRelics/CountPlatinumRelics each count how
 * many of a caller's 20 records fall between two adjacent thresholds
 * (times[0]/[1] for one function, times[1]/[2] for the next, and just
 * times[2] alone for the simplest one).
 * CountSapphireRelics/CountGoldRelics read through inline asm rather than plain
 * struct field access on purpose: gcc's CSE otherwise shares the
 * "table[i]" address between the two threshold reads even though the
 * ROM recomputes it fresh for each one (see docs/matching.md, "Matching
 * decompilation"). */

s32 GetProgressLives(const struct game_progress *save)
{
    return save->lives;
}

/* The save block's counts (the pause menu's pages, the save menu's rows,
 * the game-over screen's totals). The relics are the time-trial medals:
 * a platinum relic for a time within the level's times[2], gold within
 * times[1], sapphire within times[0]. */
s32 CountPlatinumRelics(const struct game_progress *save)
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountGoldRelics(const struct game_progress *save)
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[1] && val > gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountSapphireRelics(const struct game_progress *save)
{
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[0] && val > gLevelTable[i].times[1]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountRelics(const struct game_progress *save)
{
    s32 total;
    s32 b;
    s32 c;

    total = CountSapphireRelics(save);
    b = CountGoldRelics(save);
    c = CountPlatinumRelics(save);
    total += b;
    total += c;
    return total;
}

s32 CountGems(const struct game_progress *save)
{
    s32 total;
    s32 i;
    s32 result;
    u8 flags;

    total = 0;
    for (i = 0; i < 20; i++)
        total += save->levels[i].b.flag1 + save->levels[i].b.flag2;
    total += save->levels[24].b.flag1 + save->levels[24].b.flag2;
    flags = save->flags;
    result = total + (((u32)flags << 31) >> 31);
    result += ((u32)flags << 29) >> 31;
    result += ((u32)flags << 28) >> 31;
    result += ((u32)flags << 30) >> 31;
    return result;
}

s32 CountClearGems(const struct game_progress *save)
{
    s32 total;
    s32 i;

    total = 0;
    for (i = 0; i < 20; i++)
        total += save->levels[i].b.flag1 + save->levels[i].b.flag2;
    total += save->levels[24].b.flag1 + save->levels[24].b.flag2;
    return total;
}

s32 CountCrystals(const struct game_progress *save)
{
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 20; i++)
        count += save->levels[i].b.cleared;
    return count;
}

/* The save's completion percentage: the crystals, gems, relics (a
 * sapphire counts half) and the four secret flags, out of 72. */
s32 GetCompletionPercent(const struct game_progress *self)
{
    s32 total = CountCrystals(self);
    s32 gems = CountGems(self);
    s32 sapphires = CountSapphireRelics(self);
    s32 golds = CountGoldRelics(self);
    s32 platinums = CountPlatinumRelics(self);
    u8 flags;

    total += gems;
    total += sapphires / 2;
    total += golds;
    total += platinums;
    flags = self->flags;
    total += flags >> 7;
    total += ((u32)flags << 26) >> 31;
    total += ((u32)flags << 25) >> 31;
    total += ((u32)flags << 27) >> 31;
    return __divsi3(total * 100, 0x48);
}
