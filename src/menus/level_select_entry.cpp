/* LevelSelectEntry (gLevelSelectEntryVtable: 1 Animate, the bob;
 * 2 SetLevel; 3 SetPos; 4 Draw; 5 the destructor; include/level_select.hpp),
 * a level-select page entry. Its key method is here, so g++ emits its
 * vtable here (ldscript.txt places it).
 *
 * GitHub issues #28 and #29 (the constructor); split from
 * level_select_widgets.cpp (#767), same flags (old_agbcc). */

#include "level_select.hpp"
#include "audio.hpp"

extern "C" {
#include <agb_syscall.h>
#include "math_util.h"
}

u8 LevelSelectEntry::IsSelected()
{
    return selected;
}

s32 LevelSelectEntry::GetLevel()
{
    return id;
}

/* Slot 1: bobs the entry by `phase` (a byte; 0xA0-0xFF wrap to a small
 * upward offset), the icon 2 px lower while selected, and shows the box's
 * selected/unselected frame. */
void LevelSelectEntry::Animate(s32 phase)
{
    s32 dy;

    phase &= 0xFF;
    if (phase <= 0x9F)
        dy = -phase;
    else
        dy = 0x100 - phase;
    if (selected)
        icon->DrawWithOffset(0, dy + 2);
    else
        icon->DrawWithOffset(0, dy);
    if (selected)
        ShowFrame(frame, 1);
    else
        ShowFrame(frame, 0);
    frame->DrawWithOffset(0, dy);
}

void LevelSelectEntry::SetSelected(u8 value)
{
    selected = value;
}

/* Slot 2: entry `index` of world `world`. Indices 0-4 are levels (icon
 * frame = level id); anything past that is the world's extra entry (level
 * id 0x14 + world, its own icon animation). */
void LevelSelectEntry::SetLevel(s32 world, s32 index)
{
    if (index <= 4) {
        id = world * LEVELS_PER_WORLD + index;
        ShowFrame(icon, id);
    } else {
        id = world + LEVEL_FIRST_BOSS;
        icon->StartAnim(gLevelSelectEntryWorldAnims[world]);
    }
}

/* Sets the box's animation (gLevelSelectEntryBoxAnims[kind]) and refreshes
 * both parts' palettes. */
void LevelSelectEntry::SetBox(s32 kind)
{
    frame->StartAnim(gLevelSelectEntryBoxAnims[kind]);
    frame->palette = frame->GetAnimPaletteSlot();
    icon->palette = icon->GetAnimPaletteSlot();
}

/* Slot 3: places the entry at pixel `pos`, the icon 3 px higher. The
 * frame's position is SetEntityPixelPos out of line, as in the ROM. */
void LevelSelectEntry::SetPos(const struct vec2 *pos)
{
    icon->SetPixelPos(pos->x, pos->y - 3);
    SetEntityPixelPos(frame, pos->x, pos->y);
}

/* Slot 4: the per-frame draw LevelSelect::Draw calls on every entry. Empty
 * for this class. */
void LevelSelectEntry::Draw()
{
}

/* Slot 5: the destructor. */
LevelSelectEntry::~LevelSelectEntry()
{
    delete icon;
    delete frame;
}

LevelSelectEntry::LevelSelectEntry()
{
    selected = 0;
    frame = new UiSprite;
    SetBankNow(frame, AnimTable(0x24C));
    frame->SetPriority(1);
    icon = new UiSprite;
    SetBankNow(icon, AnimTable(0x264));
    icon->SetPriority(1);
}
