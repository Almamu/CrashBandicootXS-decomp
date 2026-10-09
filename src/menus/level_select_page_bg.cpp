/* LevelSelectPageBg (include/level_select.hpp): the level-select screen's
 * BG1, the page strip whose vertical scroll eases 8 per frame toward a Q8
 * target (0x100 = one page). GitHub issue #27; split from
 * level_select_pages.cpp (#767), same flags (old_agbcc). */

#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
}

s32 LevelSelectPageBg::GetScroll()
{
    return scroll;
}

u8 LevelSelectPageBg::IsSettled()
{
    u8 r = 0;

    if (scroll == target)
        r = 1;
    return r;
}

void LevelSelectPageBg::TurnBack()
{
    target += 0x100;
}

void LevelSelectPageBg::TurnForward()
{
    target -= 0x100;
}

/* Eases `scroll` 8 per frame toward `target` and scrolls BG1 with it. */
void LevelSelectPageBg::Scroll()
{
    if (scroll < target)
        scroll += 8;
    if (scroll > target)
        scroll -= 8;
    vofs = scroll;
}

/* BG1HOFS and BG1VOFS as one word. */
u32 LevelSelectPageBg::GetOffsets()
{
    return *(u32 *)&hofs;
}

void LevelSelectPageBg::SetOffsets()
{
    hofs = 8;
    vofs = scroll + 0x30;
}

LevelSelectPageBg::~LevelSelectPageBg()
{
}

LevelSelectPageBg::LevelSelectPageBg(s32 charBlock, s32 screenBlock)
    : bg(charBlock, screenBlock, 0, 2)
{
    scroll = target = 0x300;
    bg.Load(&gLevelSelectPageBg);
}
