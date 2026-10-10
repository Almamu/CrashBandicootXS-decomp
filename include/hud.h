#ifndef __HUD_H__
#define __HUD_H__

/* The in-game HUD's C-linkage data: the slide offset (defined in
 * src/iwram/iwram_data.cpp) and the parts' tables, which
 * src/data/hud_fonts_174be0.c defines. The HUD itself, `gHud` (globals.h), is
 * class Hud (include/hud.hpp; all of its code is C++, src/hud/*.cpp). */

#include "core.h"
#include "math_util.h"
#include "aabb.h"

/* The vertical offset HudPart::Draw adds to every part, set by the
 * counters while they slide (src/iwram/iwram_data.cpp). */
extern s32 gHudSlideOffset;

/* Each of the 35 parts' starting animation and position
 * (src/data/hud_fonts_174be0.c). */
extern const u32 gHudPartAnims[35];
extern const struct vec2 gHudPartPositions[35];

#endif /* !__HUD_H__ */
