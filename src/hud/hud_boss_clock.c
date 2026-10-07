#include "core.h"
#include "gba/defines.h"
#include "hud.h"
#include <libgcc.h>
#include "level.h"
#include "globals.h"

/* Icon-indicator widget (lives display) - see
 * docs/matching/archive/issue-45-hud-stat-widget-dispatcher.md for the full
 * semantic account: positions and clamps the primary icon slot
 * (parts[22]) unconditionally, then a second icon (parts[23]) only when
 * GetBossHealth's count exceeds 1.
 *
 * Built with old_agbcc (the whole file matches under it). The earlier
 * NAKED note blamed an "r7 wrong-value miscompile" at the clamp sites;
 * under old_agbcc plain C reproduces the ROM's `ldrb r7; ...; adds rN,
 * r7, #0` clamp sequence exactly. The position helper takes the part
 * pointer last so the table symbol is loaded before `self->parts`. */

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

void UpdateHudBoss(struct hud_counter *self)
{
    struct hud_digit_part *part;
    s32 count;

    gHudSlideOffset = 0;
    SetPartPos(gHudPartPositions[22].x, gHudPartPositions[22].y, (part = &self->parts[22]));
    CLAMP_FRAME(part, self->parts[22].anim_index, 0);
    DrawHudPart(part, 0, 0);

    count = GetBossHealth(gLevelState);
    if (count > 0) {
        struct hud_digit_part *second = &self->parts[23];

        SetPartPos(gHudPartPositions[23].x, gHudPartPositions[23].y, second);
        CLAMP_FRAME(second, self->parts[23].anim_index, count - 1);
        DrawHudPart(second, 0, 0);
    }
}

/* Three more digit/icon widgets, gated by their own change-detection
 * caches (`sync_value_a`/`b`/`c`, `include/hud.h`) against
 * `GetClockMinutes`/`GetClockSeconds`/`GetClockTenths`. The first two split their
 * value into tens/ones digits (`__udivsi3`/`__umodsi3`, div/mod by
 * 10) across a slot pair each (14/15, 17/18); the third does not split
 * at all - slot 20 gets the raw value as its desired frame, slot 21
 * always gets a fixed desired frame of 0 (a single-frame icon, not a
 * digit). All six slots get redrawn unconditionally afterward via
 * `DrawHudPart` - slot 21 appears twice in that list, matching the ROM
 * exactly. Old_agbcc, like `UpdateHudBoss`; `CLAMP_FRAME` binds the
 * part pointer before the frame value, which is the order the ROM
 * computes them in. */
void UpdateHudClock(struct hud_counter *self)
{
    struct hud_digit_part *parts;

    gHudSlideOffset = 0;
    if (self->shownMinutes != GetClockMinutes(gLevelState)) {
        s32 f;

        self->shownMinutes = GetClockMinutes(gLevelState);
        f = __udivsi3(self->shownMinutes, 10);
        parts = self->parts;
        CLAMP_FRAME(&parts[14], parts[14].anim_index, f);
        f = __umodsi3(self->shownMinutes, 10);
        CLAMP_FRAME(&parts[15], parts[15].anim_index, f);
    }
    if (self->shownSeconds != GetClockSeconds(gLevelState)) {
        s32 f;

        self->shownSeconds = GetClockSeconds(gLevelState);
        f = __udivsi3(self->shownSeconds, 10);
        parts = self->parts;
        CLAMP_FRAME(&parts[17], parts[17].anim_index, f);
        f = __umodsi3(self->shownSeconds, 10);
        CLAMP_FRAME(&parts[18], parts[18].anim_index, f);
    }
    if (self->shownTenths != GetClockTenths(gLevelState)) {
        s32 f;

        self->shownTenths = f = GetClockTenths(gLevelState);
        parts = self->parts;
        CLAMP_FRAME(&parts[20], parts[20].anim_index, f);
        CLAMP_FRAME(&parts[21], parts[21].anim_index, 0);
    }
    DrawHudPart(&self->parts[14], 0, 0);
    DrawHudPart(&self->parts[15], 0, 0);
    DrawHudPart(&self->parts[17], 0, 0);
    DrawHudPart(&self->parts[18], 0, 0);
    DrawHudPart(&self->parts[20], 0, 0);
    DrawHudPart(&self->parts[21], 0, 0);
    DrawHudPart(&self->parts[16], 0, 0);
    DrawHudPart(&self->parts[19], 0, 0);
    DrawHudPart(&self->parts[21], 0, 0);
}
