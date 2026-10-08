#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include <libgcc.h>
#include "level.h"
#include "globals.h"
}

/* Icon-indicator widget (lives display) - see
 * docs/matching/archive/issue-45-hud-stat-widget-dispatcher.md for the full
 * semantic account: positions and clamps the primary icon slot
 * (parts[22]) unconditionally, then a second icon (parts[23]) only when
 * GetBossHealth's count exceeds 1.
 *
 * Built with old_agbcp, as the C was with old_agbcc. The earlier
 * NAKED note blamed an "r7 wrong-value miscompile" at the clamp sites;
 * under old_agbcc plain code reproduces the ROM's `ldrb r7; ...; adds rN,
 * r7, #0` clamp sequence exactly. The position helper takes the part
 * pointer last so the table symbol is loaded before `parts`. */

static inline void SetPartPos(s32 x, s32 y, HudPart *part)
{
    part->x = INT_TO_Q8(x);
    part->y = INT_TO_Q8(y);
}

void Hud::UpdateBoss()
{
    HudPart *part;
    s32 count;

    gHudSlideOffset = 0;
    SetPartPos(gHudPartPositions[22].x, gHudPartPositions[22].y, (part = &parts[22]));
    HUDPART_CLAMP_FRAME(part, parts[22].tag, 0);
    part->Draw(0, 0);

    count = gLevelState->GetBossHealth();
    if (count > 0) {
        HudPart *second = &parts[23];

        SetPartPos(gHudPartPositions[23].x, gHudPartPositions[23].y, second);
        HUDPART_CLAMP_FRAME(second, parts[23].tag, count - 1);
        second->Draw(0, 0);
    }
}

/* Three more digit/icon widgets, gated by their own change-detection
 * caches (`sync_value_a`/`b`/`c`, `include/hud.h`) against
 * `GetClockMinutes`/`GetClockSeconds`/`GetClockTenths`. The first two split their
 * value into tens/ones cur (`__udivsi3`/`__umodsi3`, div/mod by
 * 10) across a slot pair each (14/15, 17/18); the third does not split
 * at all - slot 20 gets the raw value as its desired frame, slot 21
 * always gets a fixed desired frame of 0 (a single-frame icon, not a
 * digit). All six slots get redrawn unconditionally afterward via
 * `Draw` - slot 21 appears twice in that list, matching the ROM
 * exactly. Old_agbcp, like `UpdateBoss`; `HUDPART_CLAMP_FRAME` binds the
 * part pointer before the frame value, which is the order the ROM
 * computes them in. */
void Hud::UpdateClock()
{
    HudPart *cur;

    gHudSlideOffset = 0;
    if (shownMinutes != gLevelState->GetClockMinutes()) {
        s32 f;

        shownMinutes = gLevelState->GetClockMinutes();
        f = __udivsi3(shownMinutes, 10);
        cur = parts;
        HUDPART_CLAMP_FRAME(&cur[14], cur[14].tag, f);
        f = __umodsi3(shownMinutes, 10);
        HUDPART_CLAMP_FRAME(&cur[15], cur[15].tag, f);
    }
    if (shownSeconds != gLevelState->GetClockSeconds()) {
        s32 f;

        shownSeconds = gLevelState->GetClockSeconds();
        f = __udivsi3(shownSeconds, 10);
        cur = parts;
        HUDPART_CLAMP_FRAME(&cur[17], cur[17].tag, f);
        f = __umodsi3(shownSeconds, 10);
        HUDPART_CLAMP_FRAME(&cur[18], cur[18].tag, f);
    }
    if (shownTenths != gLevelState->GetClockTenths()) {
        s32 f;

        shownTenths = f = gLevelState->GetClockTenths();
        cur = parts;
        HUDPART_CLAMP_FRAME(&cur[20], cur[20].tag, f);
        HUDPART_CLAMP_FRAME(&cur[21], cur[21].tag, 0);
    }
    parts[14].Draw(0, 0);
    parts[15].Draw(0, 0);
    parts[17].Draw(0, 0);
    parts[18].Draw(0, 0);
    parts[20].Draw(0, 0);
    parts[21].Draw(0, 0);
    parts[16].Draw(0, 0);
    parts[19].Draw(0, 0);
    parts[21].Draw(0, 0);
}
