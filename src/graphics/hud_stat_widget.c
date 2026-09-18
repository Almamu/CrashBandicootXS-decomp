#include "core.h"
#include "actor.h"
#include "hud.h"

extern void *gUnknown_030012C0;
extern s32 gUnknown_0300086C;

extern void sub_80270E0(struct hud_digit_part *part, s32 x, s32 y);
extern void sub_8008044(struct actor *part);
extern s32 sub_80233B4(void *self);
extern u8 sub_80232B8(void *self);
extern void sub_8027838(struct hud_counter *counter);
extern void sub_8027E88(struct hud_counter *self);
extern void sub_802757C(struct hud_counter *self);
extern void sub_802763C(struct hud_counter *self);
extern void sub_8027940(struct hud_counter *self);
extern void sub_8027D5C(struct hud_counter *self);

/* The HUD stat-widget family's dispatcher - see docs/rom_map.md's "full
 * HUD stat-widget family" section. `self` is the same `struct
 * hud_counter` passed straight through to every callee here (including
 * `sub_8027838`, matched separately in hud_counter.c) - `sself->parts`
 * is the 35-slot OAM array `sub_8027138` builds. Runs the percentage
 * counter (`sub_8027E88`) when `icon_flag` is set, then the score
 * counter (`sub_8027838`) unconditionally, then branches on
 * `sub_80233B4`'s level-type/game-mode result: a non-"none" mode
 * (!= -1) hands off entirely to the icon-indicator widget
 * (`sub_802757C`) and returns early, skipping the rest of the family;
 * otherwise it refreshes the last OAM slot's animation state whenever
 * `sub_80232B8` says the mode changed, conditionally runs
 * `sub_802763C` while a "paused"-style central-state flag is set and
 * `mode`/`field_08` are both still zero, then unconditionally runs the
 * two remaining digit counters (`sub_8027940`, `sub_8027D5C`). */
void sub_80274EC(struct hud_counter *self)
{
    register struct hud_counter *sself asm("r5") = self;

    gUnknown_0300086C = 0;

    if (sself->icon_flag) {
        sub_8027E88(sself);
    }

    sub_8027838(sself);

    if (sub_80233B4(gUnknown_030012C0) != -1) {
        sub_802757C(sself);
        return;
    }

    if (sub_80232B8(gUnknown_030012C0)) {
        gUnknown_0300086C = 0;
        sub_8008044((struct actor *)((u8 *)sself->parts + 0x880));
        sub_80270E0((struct hud_digit_part *)((u8 *)sself->parts + 0x880), 0, 0);
    }

    if (*((u8 *)gUnknown_030012C0 + 0x8c) != 0 && sself->mode == 0 && sself->field_08 == 0) {
        sub_802763C(sself);
    }

    sub_8027940(sself);
    sub_8027D5C(sself);
}
