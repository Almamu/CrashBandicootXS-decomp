/* The HUD part (HudPart, gHudPartVtable; #664, part 7c;
 * include/game/entities/part_list.hpp): its Draw, the key method, so g++ emits the
 * vtable here (games/xs/ldscript.txt places it), and its empty destructor and
 * constructor. Split from gfx/palette_cycle.cpp (#767), same flags
 * (old_agbcc). */

#include "game/ui/hud.hpp"

extern "C" {
#include "engine/sys/memory.h"
#include "globals.h"
}

using Game::Ui::HudPart;

/* Draws the part, unless it is hidden (`frame` -1), `gHudSlideOffset`
 * lower. */
void HudPart::Draw(screen_dx_t dx, screen_dy_t dy)
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
