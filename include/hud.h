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
 * struct hud_counter below is Hud's C view, for the C files
 * (bonus_round.c, actor_category_init.c, actor_vram_pool.c), and the
 * prototypes are the C names of the Hud methods they call
 * (cxx_symbols.txt). `gHud` itself is declared in globals.h. */

#include "core.h"
#include "math_util.h"

/* The HUD object (`gHud`, 0x68 bytes, built by InitHud). The lives,
 * wumpa and crate counters each have a {slide state, slide timer} pair
 * (see above), stepped by Hud::StepSlide. Each counter keeps the value it
 * is showing (`shown*`), so it only updates its digits when the value
 * changes. */
struct hud_counter {
    s32 livesSlide;            /* +0x00 - the lives counter's slide state */
    s32 livesSlideTimer;       /* +0x04 */
    s32 wumpaSlide;            /* +0x08 - the wumpa counter's slide state;
                                * UpdateHud only draws the time-trial clock
                                * while neither counter is shown. */
    s32 wumpaSlideTimer;       /* +0x0c */
    s32 crateSlide;            /* +0x10 - the crate counter's slide state */
    s32 crateSlideTimer;       /* +0x14 */
    u8 icon_flag;              /* +0x18 - UpdateHud's dispatcher gate for
                                * UpdateHudPercentCounters (percentage counter);
                                * also set from ConfigureHudParts's second
                                * argument while the parts are being built. */
    u8 unknown_19[3];          /* +0x19 */
    s32 lives;                 /* +0x1c */
    s32 wumpa;                 /* +0x20 */
    s32 crateCount;            /* +0x24 */
    s32 crateTotal;            /* +0x28 - SetHudCrateTotal/IncHudCrateTotal */
    s32 shownMinutes;          /* +0x2c - UpdateHudClock's change-detection
                                * cache for `GetClockMinutes`'s value. */
    s32 shownSeconds;          /* +0x30 - same, for `GetClockSeconds`. */
    s32 shownTenths;           /* +0x34 - same, for `GetClockTenths`. */
    s32 playerHpPercent;       /* +0x38 - UpdateHudPercentCounters' first
                                * percentage: the jetpack player's HP */
    s32 airshipHpPercent;      /* +0x3c - its second one (GetAirshipHpPercent) */
    s32 shownLives;            /* +0x40 - ConfigureHudParts fills +0x40..+0x63 with -1 */
    s32 shownWumpa;            /* +0x44 */
    s32 shownCrateCount;       /* +0x48 */
    s32 shownCrateTotal;       /* +0x4c */
    u8 unknown_50[0xC];        /* +0x50 */
    s32 shownPlayerHpPercent;  /* +0x5c - cache for `playerHpPercent` */
    s32 shownAirshipHpPercent; /* +0x60 - cache for `airshipHpPercent` */
    void *parts;               /* +0x64 - the 35 HudParts InitHud builds */
};

/* A HUD part's position, in pixels. */
struct hud_pos {
    s32 x;
    s32 y;
};

COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_counter) == 0x68);
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
