#ifndef __HUD_H__
#define __HUD_H__

/* Shared shapes for the in-game HUD (`gHud`, built by `InitHud`):
 * docs/rom_map.md's "hud" investigation. `UpdateHud` (hud.c)
 * is the per-frame dispatcher; its widgets are `UpdateHudLives`
 * (hud_lives.c), `UpdateHudClock`/`UpdateHudWumpa` and the still
 * unnamed counters (hud_boss_clock.c, hud_counters.c). The lives,
 * wumpa and crate counters each slide in from the top of the screen
 * (`ShowHudLives`/`ShowHudWumpa`/`ShowHudCrates`/`UpdateHudSlides`,
 * hud_slide.c): a counter's slide state is 0 hidden, 1 sliding in,
 * 2 held, 3 sliding out, and while it slides
 * `gHudSlideOffset = slideTimer * 2 - 40`.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). `gHud` itself is a shared global, declared in
 * globals.h. */

#include "core.h"
#include "math_util.h"

/* One HUD part's per-animation record (`anim_data->records[anim_index]`). */
struct hud_anim_record {
    u8 unknown_00[0x14];
    u8 tile_record; /* +0x14 - palette record id, for GetPaletteSlot */
    u8 unknown_15;
    u8 frame_count; /* +0x16 */
    u8 unknown_17[5];
};

struct hud_anim_data {
    struct hud_anim_record *records;
};

/* A single HUD digit/icon slot: the C view of HudPart
 * (include/part_list.hpp), a UiSprite with its own vtable
 * (gHudPartVtable), whose constructor and destructor are InitHudPart and
 * DestroyHudPart (src/gfx/palette_cycle.cpp). The class checks the
 * size. */
struct hud_digit_part {
    s32 x; /* +0x00 - position, 24.8 fixed point */
    s32 y; /* +0x04 */
    u8 unknown_08[0x10];
    void *table; /* +0x18 - see `struct actor.table` */
    u8 unknown_1c[4];
    struct hud_anim_data *anim_data; /* +0x20 */
    u8 unknown_24[5];
    u8 palette:4; /* +0x29 - OAM palette slot (low nibble) */
    u8 unknown_29_4:4;
    u8 unknown_2A[3];
    u8 anim_index; /* +0x2D */
    u8 unknown_2E[2];
    s32 frame_index; /* +0x30 */
    u8 unknown_34[0xC];
};

/* The HUD object (`gHud`, 0x68 bytes, built by InitHud). The lives,
 * wumpa and crate counters each have a {slide state, slide timer} pair
 * (see above), stepped by StepHudSlide. Each counter keeps the value it
 * is showing (`shown*`), so it only updates its digits when the value
 * changes. */
struct hud_counter {
    s32 livesSlide;               /* +0x00 - the lives counter's slide state */
    s32 livesSlideTimer;          /* +0x04 */
    s32 wumpaSlide;               /* +0x08 - the wumpa counter's slide state;
                                   * UpdateHud only draws the time-trial clock
                                   * while neither counter is shown. */
    s32 wumpaSlideTimer;          /* +0x0c */
    s32 crateSlide;               /* +0x10 - the crate counter's slide state */
    s32 crateSlideTimer;          /* +0x14 */
    u8 icon_flag;                 /* +0x18 - UpdateHud's dispatcher gate for
                                   * UpdateHudPercentCounters (percentage counter); also set
                                   * from ConfigureHudParts's second argument while
                                   * the OAM slot array is being built. */
    u8 unknown_19[3];             /* +0x19 */
    s32 lives;                    /* +0x1c */
    s32 wumpa;                    /* +0x20 */
    s32 crateCount;               /* +0x24 */
    s32 crateTotal;               /* +0x28 - SetHudCrateTotal/IncHudCrateTotal */
    s32 shownMinutes;             /* +0x2c - UpdateHudClock's change-detection
                                   * cache for `GetClockMinutes`'s value. */
    s32 shownSeconds;             /* +0x30 - same, for `GetClockSeconds`. */
    s32 shownTenths;              /* +0x34 - same, for `GetClockTenths`. */
    s32 playerHpPercent;          /* +0x38 - UpdateHudPercentCounters' first percentage: the
                                   * jetpack player's HP (gActorList's getHp method) */
    s32 airshipHpPercent;         /* +0x3c - its second one (GetAirshipHpPercent) */
    s32 shownLives;               /* +0x40 - ConfigureHudParts fills +0x40..+0x63 with -1 */
    s32 shownWumpa;               /* +0x44 */
    s32 shownCrateCount;          /* +0x48 */
    s32 shownCrateTotal;          /* +0x4c */
    u8 unknown_50[0xC];           /* +0x50 */
    s32 shownPlayerHpPercent;     /* +0x5c - cache for `playerHpPercent` */
    s32 shownAirshipHpPercent;    /* +0x60 - cache for `airshipHpPercent` */
    struct hud_digit_part *parts; /* +0x64 - the 35 slots InitHud builds */
};

/* A HUD part's position, in pixels. */
struct hud_pos {
    s32 x;
    s32 y;
};

COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_anim_record) == 0x1C);
COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_digit_part) == 0x40);
COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_counter) == 0x68);
COMPILE_TIME_ASSERT(hud_h, sizeof(struct hud_pos) == 0x8);

/* The vertical offset DrawHudPart adds to every part, set by the
 * counters while they slide (src/iwram/iwram_data.c). */
extern s32 gHudSlideOffset;

/* Each of the 35 parts' starting animation and position
 * (src/data/hud_fonts_174be0.c). */
extern const u32 gHudPartAnims[35];
extern const struct hud_pos gHudPartPositions[35];

/* src/hud/hud.c */
extern void UpdateHud(struct hud_counter *self);

/* src/hud/hud_boss_clock.c */
extern void UpdateHudBoss(struct hud_counter *self);
extern void UpdateHudClock(struct hud_counter *self);

/* src/hud/hud_counters.c */
extern void UpdateHudCrates(struct hud_counter *self);
extern void UpdateHudWumpa(struct hud_counter *self);
extern void UpdateHudPercentCounters(struct hud_counter *self);

/* src/hud/hud_init.c */
extern struct hud_counter *InitHud(struct hud_counter *self);
extern void ConfigureHudParts(struct hud_counter *self, u8 iconFlag);

/* src/hud/hud_lives.c */
extern void UpdateHudLives(struct hud_counter *counter);

/* src/hud/hud_slide.c */
extern void UpdateHudSlides(struct hud_counter *self);
extern void ShowHudCrates(struct hud_counter *self);
extern void ShowHudLives(struct hud_counter *self);
extern void ShowHudWumpa(struct hud_counter *self);
extern void ShowHudCounters(struct hud_counter *self);
extern void StepHudSlide(struct hud_counter *self, s32 *state, s32 *timer, s32 threshold);
extern void SetHudCrateTotal(struct hud_counter *self, s32 val);
extern void IncHudCrateTotal(struct hud_counter *self);
extern void DestroyHud(struct hud_counter *self, s32 flags);

/* The HUD parts' draw and constructor. They are defined in
 * src/gfx/palette_cycle.cpp, which holds the ROM range they sit in. */
extern void DrawHudPart(struct hud_digit_part *part, s32 x, s32 y);
extern struct hud_digit_part *InitHudPart(struct hud_digit_part *part);

/* Sets the part's desired frame, clamped to its animation's last one
 * (hud_counters.c, hud_boss_clock.c). The part pointer is bound before
 * the frame value, which is what gives old_agbcc's clamp sequence. */
#define HUD_CLAMP_FRAME(part, index, frame)                                    \
    {                                                                          \
        struct hud_digit_part *_p = (part);                                    \
        s32 _f = (frame);                                                      \
        s32 _n = _p->anim_data->records[index].frame_count;                    \
        CLAMP_INDEX(_f, _n);                                                   \
        _p->frame_index = _f;                                                  \
    }

#endif /* !__HUD_H__ */
