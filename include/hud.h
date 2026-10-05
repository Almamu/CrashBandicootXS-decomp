#ifndef __HUD_H__
#define __HUD_H__

/* Shared shapes for the in-game HUD (`gHud`, built by `InitHud`):
 * docs/rom_map.md's "hud" investigation. `UpdateHud` (hud_stat_widget.c)
 * is the per-frame dispatcher; its widgets are `UpdateHudLives`
 * (hud_counter.c), `UpdateHudClock`/`UpdateHudWumpa` and the still
 * unnamed counters (hud_stat_widget2.c, hud_stat_widget3.c). The lives,
 * wumpa and third counters each slide in from the top of the screen
 * (`ShowHudLives`/`ShowHudWumpa`/`UpdateHudSlides`, hud_blink.c): a
 * counter's slide state is 0 hidden, 1 sliding in, 2 held, 3 sliding
 * out, and while it slides `gHudSlideOffset = slideTimer * 2 - 40`. */

struct hud_anim_record {
    u8 unknown_00[0x16];
    u8 frame_count;
    u8 unknown_17[5];
};

struct hud_anim_data {
    struct hud_anim_record *records;
};

/* A single HUD digit/icon slot. Its first 0x18 bytes plus the `table`
 * field at +0x18 match `struct actor` (include/actor.h) byte for byte -
 * sub_802710C/InitHudPart (src/gfx/palette_cycle.c) construct each
 * slot by calling the same generic `struct actor`-based table-swap
 * helpers (DestroyUiSpriteObj/InitUiSpriteObj) already used by the actor/part
 * system, treating this object as one. The rest of the fields
 * (animation state) are specific to this widget family. */
struct hud_digit_part {
    s32 x;                 /* +0x00 - position, 24.8 fixed point */
    s32 y;                 /* +0x04 */
    u8 unknown_08[0x10];
    void *table;           /* +0x18 - see `struct actor.table` */
    u8 unknown_1c[4];
    struct hud_anim_data *anim_data;
    u8 unknown_24[9];
    u8 anim_index;
    u8 unknown_2E[2];
    s32 frame_index;
    u8 unknown_34[0xC];
};

struct hud_counter {
    s32 livesSlide;          /* +0x00 - the lives counter's slide state */
    s32 livesSlideTimer;     /* +0x04 */
    s32 wumpaSlide;          /* +0x08 - the wumpa counter's slide state;
                              * UpdateHud only draws the time-trial clock
                              * while neither counter is shown. */
    u8 unknown_0c[0xC];      /* +0x0c */
    u8 icon_flag;            /* +0x18 - UpdateHud's dispatcher gate for
                               * UpdateHudPercentCounters (percentage counter); also set
                               * from ConfigureHudParts's second argument while
                               * the OAM slot array is being built. */
    u8 unknown_19[3];        /* +0x19 */
    s32 lives;               /* +0x1c */
    u8 unknown_20[0xC];      /* +0x20 */
    s32 shownMinutes;        /* +0x2c - UpdateHudClock's change-detection
                               * cache for `GetClockMinutes`'s value. */
    s32 shownSeconds;        /* +0x30 - same, for `GetClockSeconds`. */
    s32 shownTenths;        /* +0x34 - same, for `GetClockTenths`. */
    u8 unknown_38[8];        /* +0x38 */
    s32 shownLives;      /* +0x40 */
    u8 unknown_44[0x20];     /* +0x44 */
    struct hud_digit_part *parts; /* +0x64 */
};

COMPILE_TIME_ASSERT(sizeof(struct hud_anim_record) == 0x1C);
COMPILE_TIME_ASSERT(sizeof(struct hud_digit_part) == 0x40);
COMPILE_TIME_ASSERT(sizeof(struct hud_counter) == 0x68);

#endif /* !__HUD_H__ */
