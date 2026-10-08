#ifndef __HUD_H__
#define __HUD_H__

/* The in-game HUD (`gHud`): class Hud (include/hud.hpp; all of its code
 * is C++, src/hud/*.cpp), built and deleted by game_frame.cpp.
 * docs/rom_map.md's "hud" investigation. Hud::Update (UpdateHud) is the
 * per-frame dispatcher; its widgets are UpdateLives, UpdateClock,
 * UpdateWumpa, UpdateCrates, UpdateBoss and UpdatePercentCounters. The
 * lives, wumpa and crate counters each slide in from the top of the
 * screen (ShowLives/ShowWumpa/ShowCrates/UpdateSlides, hud_slide.cpp): a
 * counter's slide state is 0 hidden, 1 sliding in, 2 held, 3 sliding out,
 * and while it slides `gHudSlideOffset = slideTimer * 2 - 40`. `gHud`
 * itself is declared in globals.h. */

#include "core.h"
#include "math_util.h"
#include "aabb.h"

/* The HUD object (`gHud`, class Hud, include/hud.hpp: 0x68 bytes) as the
 * C side sees it: a tag (globals.h). */
struct hud_counter;

/* The vertical offset HudPart::Draw adds to every part, set by the
 * counters while they slide (src/iwram/iwram_data.cpp). */
extern s32 gHudSlideOffset;

/* Each of the 35 parts' starting animation and position
 * (src/data/hud_fonts_174be0.c). */
extern const u32 gHudPartAnims[35];
extern const struct vec2 gHudPartPositions[35];

#endif /* !__HUD_H__ */
