#include "font.hpp"
#include "level_state.hpp"

extern "C" {
#include "util.h"
#include "memory.h"
}

/* Small methods of unrelated classes collected after libgcc in the ROM
 * (0x0803AFDC-0x0803B058; nothing can move across libgcc): the AABB
 * set-size and set-position primitives, LevelState::GetLives and the two
 * fonts' destructors. Likely the original's out-of-line copies of inline
 * or implicit functions, gathered in one place, hence the name
 * (src/util/aabb_setup.cpp until #770). */

/* Set-size primitive - already referenced by name from several other
 * files (sprite.cpp/power_dialog_draw.c's DrawPowerDialog) as the
 * shared `SetAabbPos`(set-position)/`SetAabbSize`(set-size) pair. */
void SetAabbSize(struct aabb *dest, s32 w, s32 h)
{
    dest->w = w;
    dest->h = h;
}

/* Set-position primitive, see SetAabbSize above. */
void SetAabbPos(struct aabb *dest, s32 x, s32 y)
{
    dest->x = x;
    dest->y = y;
}

/* Lives getter of the level state (`gLevelState`; read by
 * game_frame.cpp, hud_lives.cpp, actor_category_init.cpp and
 * spawn_start_marker.cpp). */
s32 LevelState::GetLives()
{
    return lives;
}

/* The two fonts' destructors (include/font.hpp), their classes' key
 * methods: g++ emits gLargeFontVtable and gSmallFontVtable here. Each
 * stores its own vtable and then, expanding Font's inline destructor,
 * gFontVtable: the ROM's two stores to `+0x130` in a row (the C needed an
 * asm address anchor for each to keep the dead first one). */
LargeFont::~LargeFont()
{
}

SmallFont::~SmallFont()
{
}
