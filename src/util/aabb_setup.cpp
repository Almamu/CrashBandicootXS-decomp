#include "font.hpp"

extern "C" {
#include "level_state.h"
#include "util.h"
#include "memory.h"
}

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
s32 GetLives(struct level_state *self)
{
    return self->lives;
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
