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
 * and while it slides `gHudSlideOffset = slideTimer * 2 - 40`.
 *
 * The C files (bonus_round.c, actor_category_init.c, actor_vram_pool.c)
 * see gHud as a `struct hud_counter *` (a tag: they don't read its
 * fields), and the prototypes are the C names of the Hud methods they
 * call (cxx_symbols.txt). `gHud` itself is declared in globals.h. */

#include "core.h"
#include "math_util.h"

/* The HUD object (`gHud`, class Hud, include/hud.hpp: 0x68 bytes). No C
 * file reads its fields: the C callers of its methods (below) only pass
 * the pointer, so the C side has the tag alone. */
struct hud_counter;

/* A HUD part's position, in pixels. */
struct hud_pos {
    s32 x;
    s32 y;
};

COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_pos) == 0x8);

/* The vertical offset HudPart::Draw adds to every part, set by the
 * counters while they slide (src/iwram/iwram_data.c). */
extern s32 gHudSlideOffset;

/* Each of the 35 parts' starting animation and position
 * (src/data/hud_fonts_174be0.c). */
extern const u32 gHudPartAnims[35];
extern const struct hud_pos gHudPartPositions[35];

/* src/hud/hud.cpp */
extern void UpdateHud(struct hud_counter *self);

/* src/hud/hud_init.cpp */
extern void ConfigureHudParts(struct hud_counter *self, u8 iconFlag);

/* src/hud/hud_slide.cpp */
extern void UpdateHudSlides(struct hud_counter *self);
extern void ShowHudCounters(struct hud_counter *self);
extern void SetHudCrateTotal(struct hud_counter *self, s32 val);

#endif /* !__HUD_H__ */
