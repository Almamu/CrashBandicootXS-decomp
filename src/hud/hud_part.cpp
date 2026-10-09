/* The HUD part (HudPart, gHudPartVtable; #664, part 7c;
 * include/part_list.hpp): its Draw, the key method, so g++ emits the
 * vtable here (ldscript.txt places it), and its empty destructor and
 * constructor. Split from gfx/palette_cycle.cpp (#767), same flags
 * (old_agbcc). */

#include "part_list.hpp"

extern "C" {
#include "memory.h"
#include "globals.h"
}

/* Draws the part, unless it is hidden (`frame` -1), `gHudSlideOffset`
 * lower. */
void HudPart::Draw(s32 dx, s32 dy)
{
    if (frame != -1)
        DrawWithOffset(dx, dy + gHudSlideOffset);
}

HudPart::~HudPart()
{
}

HudPart::HudPart()
{
}
