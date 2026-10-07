#include "core.h"
#include "hud.h"
#include <libgcc.h>
#include "bosses.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* The object `UpdateHudPercentCounters` reads its percentage from, called through a
 * gcc 2.x pointer-to-member (delta + function) slot in its vtable. */
extern s32 _call_via_r1(void *self, void *fn);

static inline void SetPartPos(s32 x, s32 y, struct hud_digit_part *part)
{
    part->x = x << 8;
    part->y = y << 8;
}

/* Sets the part's desired frame, clamped to its animation's last one. */
#define CLAMP_FRAME(part, index, frame)                                   \
    {                                                                     \
        struct hud_digit_part *_p = (part);                               \
        s32 _f = (frame);                                                 \
        s32 _n = _p->anim_data->records[index].frame_count;               \
        if (_f >= _n)                                                     \
            _f = _n - 1;                                                  \
        _p->frame_index = _f;                                             \
    }

/* The remaining three callees of the HUD stat-widget dispatcher
 * (`UpdateHud`, `hud.c`) - see `docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md` for the family's full
 * background. Built with old_agbcc, like `hud_boss_clock.c`.
 *
 * These were parked as NAKED on the belief that a second "r7 wrong-value
 * miscompile" broke every clamp site; under old_agbcc the plain clamp
 * (`CLAMP_FRAME`, part pointer bound before the frame value) reproduces
 * the ROM's `ldrb r7; ...; adds rN, r7, #0` sequence exactly. The other
 * things that mattered: a literal `-1` frame lets the compiler fold the
 * clamp into a bare store (the ROM does that in some branches), while a
 * `-1` held in a local keeps the compare; the "100%" branches read
 * `self->parts` into their own block-local so its register differs from
 * the digit branches (the ROM does not cross-jump them); and
 * `UpdateHudCrates`'s second counter reads value, cached value, then
 * `self->parts`, in that order. */

/* The two-digit score-style counter: two independent 3-digit displays
 * (`self->crateCount`/`self->shownCrateCount` change-detection pair at slots
 * 0xc0/0x80/0xa0, `self->crateTotal`/`self->shownCrateTotal` pair at slots
 * 0xc0*2/0xe0/0x80*4), each branching 3-digit vs. 2-digit vs. 1-digit
 * (hiding the unused leading slot(s) via a desired frame of -1, exactly
 * like `UpdateHudLives`'s own single-digit case), plus one more icon
 * (`gHudPartPositions`-positioned, slot at `self->parts + 0xa0*4`)
 * whose x/y table index is picked from a 3-way digit-count check on the
 * first counter's value. */
void UpdateHudCrates(struct hud_counter *self)
{
    struct hud_digit_part *parts;
    s32 v;
    s32 digits;
    s32 w;
    s32 off;

    if (self->crateSlide == 0)
        return;
    self->crateCount = GetCrateCount(gLevelState);
    if (self->crateSlide == 1 || self->crateSlide == 3)
        gHudSlideOffset = self->crateSlideTimer * 2 - 0x28;
    else
        gHudSlideOffset = 0;

    v = self->crateCount;
    if (v != self->shownCrateCount) {
        if (v > 99) {
            s32 f = __divsi3(v, 100);

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, f);
            f = __modsi3(__divsi3(self->crateCount, 10), 10);
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            f = __modsi3(self->crateCount, 10);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, f);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, f);
            f = __modsi3(self->crateCount, 10);
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, -1);
        } else {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[3], parts[3].anim_index, v);
            f = -1;
            CLAMP_FRAME(&parts[4], parts[4].anim_index, f);
            CLAMP_FRAME(&parts[5], parts[5].anim_index, f);
        }
    }
    DrawHudPart(&self->parts[3], 0, 0);
    DrawHudPart(&self->parts[4], 0, 0);
    DrawHudPart(&self->parts[5], 0, 0);

    if (self->crateCount > 99)
        digits = 2;
    else if (self->crateCount > 9)
        digits = 1;
    else
        digits = 0;
    off = digits * 15;

    v = self->crateTotal;
    w = self->shownCrateTotal;
    parts = self->parts;
    if (v != w) {
        if (v > 99) {
            s32 f = __divsi3(v, 100);

            CLAMP_FRAME(&parts[6], parts[6].anim_index, f);
            f = __modsi3(__divsi3(self->crateTotal, 10), 10);
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            f = __modsi3(self->crateTotal, 10);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, f);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            CLAMP_FRAME(&parts[6], parts[6].anim_index, f);
            f = __modsi3(self->crateTotal, 10);
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, -1);
        } else {
            s32 f;

            CLAMP_FRAME(&parts[6], parts[6].anim_index, v);
            f = -1;
            CLAMP_FRAME(&parts[7], parts[7].anim_index, f);
            CLAMP_FRAME(&parts[8], parts[8].anim_index, f);
        }
    }
    DrawHudPart(&parts[6], off, 0);
    DrawHudPart(&self->parts[7], off, 0);
    DrawHudPart(&self->parts[8], off, 0);

    {
        struct hud_digit_part *part;

        SetPartPos(gHudPartPositions[10].x + off, gHudPartPositions[10].y,
                   (part = &self->parts[10]));
        CLAMP_FRAME(part, self->parts[10].anim_index, 0);
        DrawHudPart(part, 0, 0);
    }
    DrawHudPart(&self->parts[9], 0, 0);
    self->shownCrateTotal = self->crateTotal;
    self->shownCrateCount = self->crateCount;
}

/* A smaller sibling of `UpdateHudCrates` above: one 2-digit display
 * (`self->wumpa`/`self->shownWumpa` change-detection pair, slots
 * `0xb0*4`/`0xc0*4`), sourced from `GetWumpa` (`UpdateHudCrates` used
 * `GetCrateCount` for its own primary counter) rather than a mode/layout
 * pair like the dispatcher's other callees - always refreshes one more
 * slot (`self->parts + 0xd0*4`) up front via `AdvanceSpriteAnim`/
 * `DrawHudPart` regardless of whether the value changed. */
void UpdateHudWumpa(struct hud_counter *self)
{
    struct hud_digit_part *parts;
    s32 v;

    if (self->wumpaSlide == 0)
        return;
    if (self->wumpaSlide == 1 || self->wumpaSlide == 3)
        gHudSlideOffset = self->wumpaSlideTimer * 2 - 0x28;
    else
        gHudSlideOffset = 0;
    self->wumpa = GetWumpa(gLevelState);
    AdvanceSpriteAnim((struct box_part *)&self->parts[13]);
    DrawHudPart(&self->parts[13], 0, 0);

    v = self->wumpa;
    if (v != self->shownWumpa) {
        if (v > 9) {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[11], parts[11].anim_index, f);
            f = __modsi3(self->wumpa, 10);
            CLAMP_FRAME(&parts[12], parts[12].anim_index, f);
        } else {
            parts = self->parts;
            CLAMP_FRAME(&parts[11], parts[11].anim_index, v);
            CLAMP_FRAME(&parts[12], parts[12].anim_index, -1);
        }
    }
    DrawHudPart(&self->parts[11], 0, 0);
    DrawHudPart(&self->parts[12], 0, 0);
    self->shownWumpa = self->wumpa;
}

/* The percentage-counter widget (`docs/rom_map.md`'s "fx" investigation
 * already named it this way from the `cmp r1, #0x64` special case
 * below): a 3-digit-or-percent display of the player's HP. UpdateHud
 * only runs it in the jetpack categories (`icon_flag`, set from the
 * category type in SetupActorVramPool), where gActorList's root is the
 * jetpack player, and it calls that actor's `getHp` method (vtable slot
 * 6, GetJetpackPlayerHpPercent) through `_call_via_r1`.
 *
 * A value of exactly 100 (`0x64`) skips the digit split entirely and
 * shows a single dedicated "100%" icon (slot `0xc8*8`, tens/ones slots
 * still get the fixed values `1`/`-1` to hide them). Otherwise the usual
 * 2-digit-vs-1-digit split runs (slots `0xc8*8`/`0xd0*8`/`0xd8*8`,
 * hiding the leading digit via `-1` past `0xe0*8` when unused). Runs a
 * second, independent instance of the same shape right after (guarded
 * by its own `GetAirshipHpPercent`/`self->airshipHpPercent`/`self->shownAirshipHpPercent`
 * change-detection triple, slots `0xe8*8` fixed-icon plus
 * `0xf0*8`/`0xf8*8`/`0x80<<4`/`0x84<<4` digit slots) - two independent
 * percent-style readouts sharing one function body. */
void UpdateHudPercentCounters(struct hud_counter *self)
{
    struct hud_digit_part *parts;
    struct hud_digit_part *part;
    s32 v;

    gHudSlideOffset = 0;
    SetPartPos(gHudPartPositions[24].x, gHudPartPositions[24].y, (part = &self->parts[24]));
    CLAMP_FRAME(part, self->parts[24].anim_index, 0);
    DrawHudPart(part, 0, 0);

    {
        struct actor_self *src = gActorList;
        struct actor_vtable *vt = src->vtable;

        v = _call_via_r1((u8 *)src + vt->getHp.thisOffset, vt->getHp.fn);
    }
    self->playerHpPercent = v;
    if (v != self->shownPlayerHpPercent) {
        if (v == 100) {
            struct hud_digit_part *p = self->parts;

            CLAMP_FRAME(&p[25], p[25].anim_index, 1);
            CLAMP_FRAME(&p[26], p[26].anim_index, 0);
            CLAMP_FRAME(&p[27], p[27].anim_index, 0);
            CLAMP_FRAME(&p[28], p[28].anim_index, 10);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[25], parts[25].anim_index, f);
            f = __modsi3(self->playerHpPercent, 10);
            CLAMP_FRAME(&parts[26], parts[26].anim_index, f);
            CLAMP_FRAME(&parts[27], parts[27].anim_index, 10);
            CLAMP_FRAME(&parts[28], parts[28].anim_index, -1);
        } else {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[25], parts[25].anim_index, v);
            CLAMP_FRAME(&parts[26], parts[26].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[27], parts[27].anim_index, f);
            CLAMP_FRAME(&parts[28], parts[28].anim_index, f);
        }
    }
    DrawHudPart(&self->parts[25], 0, 0);
    DrawHudPart(&self->parts[26], 0, 0);
    DrawHudPart(&self->parts[27], 0, 0);
    DrawHudPart(&self->parts[28], 0, 0);
    self->shownPlayerHpPercent = self->playerHpPercent;

    if (GetBossIndex(gLevelState) != -1)
        return;
    if ((self->airshipHpPercent = GetAirshipHpPercent()) == -1)
        return;

    SetPartPos(gHudPartPositions[29].x, gHudPartPositions[29].y, (part = &self->parts[29]));
    CLAMP_FRAME(part, self->parts[29].anim_index, 0);
    DrawHudPart(part, 0, 0);

    v = self->airshipHpPercent;
    if (v != self->shownAirshipHpPercent) {
        if (v == 100) {
            struct hud_digit_part *p = self->parts;

            CLAMP_FRAME(&p[30], p[30].anim_index, 1);
            CLAMP_FRAME(&p[31], p[31].anim_index, 0);
            CLAMP_FRAME(&p[32], p[32].anim_index, 0);
            CLAMP_FRAME(&p[33], p[33].anim_index, 10);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            parts = self->parts;
            CLAMP_FRAME(&parts[30], parts[30].anim_index, f);
            f = __modsi3(self->airshipHpPercent, 10);
            CLAMP_FRAME(&parts[31], parts[31].anim_index, f);
            CLAMP_FRAME(&parts[32], parts[32].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[33], parts[33].anim_index, f);
        } else {
            s32 f;

            parts = self->parts;
            CLAMP_FRAME(&parts[30], parts[30].anim_index, v);
            CLAMP_FRAME(&parts[31], parts[31].anim_index, 10);
            f = -1;
            CLAMP_FRAME(&parts[32], parts[32].anim_index, f);
            CLAMP_FRAME(&parts[33], parts[33].anim_index, f);
        }
    }
    DrawHudPart(&self->parts[30], 0, 0);
    DrawHudPart(&self->parts[31], 0, 0);
    DrawHudPart(&self->parts[32], 0, 0);
    DrawHudPart(&self->parts[33], 0, 0);
    self->shownAirshipHpPercent = self->airshipHpPercent;
}
