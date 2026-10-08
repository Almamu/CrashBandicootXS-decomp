#include "hud.hpp"
#include "actor_self.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include <libgcc.h>
#include "bosses.h"
#include "level.h"
#include "globals.h"
}

static inline void SetPartPos(s32 x, s32 y, HudPart *part)
{
    part->x = INT_TO_Q8(x);
    part->y = INT_TO_Q8(y);
}

/* The remaining three callees of the HUD stat-widget dispatcher
 * (Hud::Update, `hud.cpp`) - see `docs/matching/
 * issue-45-hud-stat-widget-dispatcher.md` for the family's full
 * background. Built with old_agbcp, like `hud_boss_clock.cpp` (the C
 * was old_agbcc).
 *
 * These were parked as NAKED on the belief that a second "r7 wrong-value
 * miscompile" broke every clamp site; under old_agbcc the plain clamp
 * (`HUDPART_CLAMP_FRAME`, part pointer bound before the frame value) reproduces
 * the ROM's `ldrb r7; ...; adds rN, r7, #0` sequence exactly. The other
 * things that mattered: a literal `-1` frame lets the compiler fold the
 * clamp into a bare store (the ROM does that in some branches), while a
 * `-1` held in a local keeps the compare; the "100%" branches read
 * `parts` into their own block-local so its register differs from
 * the digit branches (the ROM does not cross-jump them); and
 * `UpdateHudCrates`'s second counter reads value, cached value, then
 * `parts`, in that order. */

/* The two-digit score-style counter: two independent 3-digit displays
 * (`crateCount`/`shownCrateCount` change-detection pair at slots
 * 0xc0/0x80/0xa0, `crateTotal`/`shownCrateTotal` pair at slots
 * 0xc0*2/0xe0/0x80*4), each branching 3-digit vs. 2-digit vs. 1-digit
 * (hiding the unused leading slot(s) via a desired frame of -1, exactly
 * like `UpdateHudLives`'s own single-digit case), plus one more icon
 * (`gHudPartPositions`-positioned, slot at `parts + 0xa0*4`)
 * whose x/y table index is picked from a 3-way digit-count check on the
 * first counter's value. */
void Hud::UpdateCrates()
{
    HudPart *cur;
    s32 v;
    s32 digits;
    s32 w;
    s32 off;

    if (crateSlide == 0)
        return;
    crateCount = gLevelState->GetCrateCount();
    if (crateSlide == 1 || crateSlide == 3)
        gHudSlideOffset = crateSlideTimer * 2 - 0x28;
    else
        gHudSlideOffset = 0;

    v = crateCount;
    if (v != shownCrateCount) {
        if (v > 99) {
            s32 f = __divsi3(v, 100);

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[3], cur[3].tag, f);
            f = __modsi3(__divsi3(crateCount, 10), 10);
            HUDPART_CLAMP_FRAME(&cur[4], cur[4].tag, f);
            f = __modsi3(crateCount, 10);
            HUDPART_CLAMP_FRAME(&cur[5], cur[5].tag, f);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[3], cur[3].tag, f);
            f = __modsi3(crateCount, 10);
            HUDPART_CLAMP_FRAME(&cur[4], cur[4].tag, f);
            HUDPART_CLAMP_FRAME(&cur[5], cur[5].tag, -1);
        } else {
            s32 f;

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[3], cur[3].tag, v);
            f = -1;
            HUDPART_CLAMP_FRAME(&cur[4], cur[4].tag, f);
            HUDPART_CLAMP_FRAME(&cur[5], cur[5].tag, f);
        }
    }
    parts[3].Draw(0, 0);
    parts[4].Draw(0, 0);
    parts[5].Draw(0, 0);

    if (crateCount > 99)
        digits = 2;
    else if (crateCount > 9)
        digits = 1;
    else
        digits = 0;
    off = digits * 15;

    v = crateTotal;
    w = shownCrateTotal;
    cur = parts;
    if (v != w) {
        if (v > 99) {
            s32 f = __divsi3(v, 100);

            HUDPART_CLAMP_FRAME(&cur[6], cur[6].tag, f);
            f = __modsi3(__divsi3(crateTotal, 10), 10);
            HUDPART_CLAMP_FRAME(&cur[7], cur[7].tag, f);
            f = __modsi3(crateTotal, 10);
            HUDPART_CLAMP_FRAME(&cur[8], cur[8].tag, f);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            HUDPART_CLAMP_FRAME(&cur[6], cur[6].tag, f);
            f = __modsi3(crateTotal, 10);
            HUDPART_CLAMP_FRAME(&cur[7], cur[7].tag, f);
            HUDPART_CLAMP_FRAME(&cur[8], cur[8].tag, -1);
        } else {
            s32 f;

            HUDPART_CLAMP_FRAME(&cur[6], cur[6].tag, v);
            f = -1;
            HUDPART_CLAMP_FRAME(&cur[7], cur[7].tag, f);
            HUDPART_CLAMP_FRAME(&cur[8], cur[8].tag, f);
        }
    }
    cur[6].Draw(off, 0);
    parts[7].Draw(off, 0);
    parts[8].Draw(off, 0);

    {
        HudPart *part;

        SetPartPos(gHudPartPositions[10].x + off, gHudPartPositions[10].y, (part = &parts[10]));
        HUDPART_CLAMP_FRAME(part, parts[10].tag, 0);
        part->Draw(0, 0);
    }
    parts[9].Draw(0, 0);
    shownCrateTotal = crateTotal;
    shownCrateCount = crateCount;
}

/* A smaller sibling of `UpdateHudCrates` above: one 2-digit display
 * (`wumpa`/`shownWumpa` change-detection pair, slots
 * `0xb0*4`/`0xc0*4`), sourced from `GetWumpa` (`UpdateHudCrates` used
 * `GetCrateCount` for its own primary counter) rather than a mode/layout
 * pair like the dispatcher's other callees - always refreshes one more
 * slot (`parts + 0xd0*4`) up front via `AdvanceSpriteAnim`/
 * `Draw` regardless of whether the value changed. */
void Hud::UpdateWumpa()
{
    HudPart *cur;
    s32 v;

    if (wumpaSlide == 0)
        return;
    if (wumpaSlide == 1 || wumpaSlide == 3)
        gHudSlideOffset = wumpaSlideTimer * 2 - 0x28;
    else
        gHudSlideOffset = 0;
    wumpa = gLevelState->GetWumpa();
    parts[13].AdvanceAnim();
    parts[13].Draw(0, 0);

    v = wumpa;
    if (v != shownWumpa) {
        if (v > 9) {
            s32 f = __divsi3(v, 10);

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[11], cur[11].tag, f);
            f = __modsi3(wumpa, 10);
            HUDPART_CLAMP_FRAME(&cur[12], cur[12].tag, f);
        } else {
            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[11], cur[11].tag, v);
            HUDPART_CLAMP_FRAME(&cur[12], cur[12].tag, -1);
        }
    }
    parts[11].Draw(0, 0);
    parts[12].Draw(0, 0);
    shownWumpa = wumpa;
}

/* The percentage-counter widget (`docs/rom_map.md`'s "fx" investigation
 * already named it this way from the `cmp r1, #0x64` special case
 * below): a 3-digit-or-percent display of the player's HP. UpdateHud
 * only runs it in the jetpack categories (`icon_flag`, set from the
 * category type in SetupActorVramPool), where gActorList's root is the
 * jetpack player, and it calls that actor's HpActor::GetHp (virtual,
 * slot 6: GetJetpackPlayerHpPercent).
 *
 * A value of exactly 100 (`0x64`) skips the digit split entirely and
 * shows a single dedicated "100%" icon (slot `0xc8*8`, tens/ones slots
 * still get the fixed values `1`/`-1` to hide them). Otherwise the usual
 * 2-digit-vs-1-digit split runs (slots `0xc8*8`/`0xd0*8`/`0xd8*8`,
 * hiding the leading digit via `-1` past `0xe0*8` when unused). Runs a
 * second, independent instance of the same shape right after (guarded
 * by its own `GetAirshipHpPercent`/`airshipHpPercent`/`shownAirshipHpPercent`
 * change-detection triple, slots `0xe8*8` fixed-icon plus
 * `0xf0*8`/`0xf8*8`/`0x80<<4`/`0x84<<4` digit slots) - two independent
 * percent-style readouts sharing one function body. */
void Hud::UpdatePercentCounters()
{
    HudPart *cur;
    HudPart *part;
    s32 v;

    gHudSlideOffset = 0;
    SetPartPos(gHudPartPositions[24].x, gHudPartPositions[24].y, (part = &parts[24]));
    HUDPART_CLAMP_FRAME(part, parts[24].tag, 0);
    part->Draw(0, 0);

    v = static_cast<HpActor *>(gActorList)->GetHp();
    playerHpPercent = v;
    if (v != shownPlayerHpPercent) {
        if (v == 100) {
            HudPart *p = parts;

            HUDPART_CLAMP_FRAME(&p[25], p[25].tag, 1);
            HUDPART_CLAMP_FRAME(&p[26], p[26].tag, 0);
            HUDPART_CLAMP_FRAME(&p[27], p[27].tag, 0);
            HUDPART_CLAMP_FRAME(&p[28], p[28].tag, 10);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[25], cur[25].tag, f);
            f = __modsi3(playerHpPercent, 10);
            HUDPART_CLAMP_FRAME(&cur[26], cur[26].tag, f);
            HUDPART_CLAMP_FRAME(&cur[27], cur[27].tag, 10);
            HUDPART_CLAMP_FRAME(&cur[28], cur[28].tag, -1);
        } else {
            s32 f;

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[25], cur[25].tag, v);
            HUDPART_CLAMP_FRAME(&cur[26], cur[26].tag, 10);
            f = -1;
            HUDPART_CLAMP_FRAME(&cur[27], cur[27].tag, f);
            HUDPART_CLAMP_FRAME(&cur[28], cur[28].tag, f);
        }
    }
    parts[25].Draw(0, 0);
    parts[26].Draw(0, 0);
    parts[27].Draw(0, 0);
    parts[28].Draw(0, 0);
    shownPlayerHpPercent = playerHpPercent;

    if (gLevelState->GetBossIndex() != BOSS_NONE)
        return;
    if ((airshipHpPercent = GetAirshipHpPercent()) == -1)
        return;

    SetPartPos(gHudPartPositions[29].x, gHudPartPositions[29].y, (part = &parts[29]));
    HUDPART_CLAMP_FRAME(part, parts[29].tag, 0);
    part->Draw(0, 0);

    v = airshipHpPercent;
    if (v != shownAirshipHpPercent) {
        if (v == 100) {
            HudPart *p = parts;

            HUDPART_CLAMP_FRAME(&p[30], p[30].tag, 1);
            HUDPART_CLAMP_FRAME(&p[31], p[31].tag, 0);
            HUDPART_CLAMP_FRAME(&p[32], p[32].tag, 0);
            HUDPART_CLAMP_FRAME(&p[33], p[33].tag, 10);
        } else if (v > 9) {
            s32 f = __divsi3(v, 10);

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[30], cur[30].tag, f);
            f = __modsi3(airshipHpPercent, 10);
            HUDPART_CLAMP_FRAME(&cur[31], cur[31].tag, f);
            HUDPART_CLAMP_FRAME(&cur[32], cur[32].tag, 10);
            f = -1;
            HUDPART_CLAMP_FRAME(&cur[33], cur[33].tag, f);
        } else {
            s32 f;

            cur = parts;
            HUDPART_CLAMP_FRAME(&cur[30], cur[30].tag, v);
            HUDPART_CLAMP_FRAME(&cur[31], cur[31].tag, 10);
            f = -1;
            HUDPART_CLAMP_FRAME(&cur[32], cur[32].tag, f);
            HUDPART_CLAMP_FRAME(&cur[33], cur[33].tag, f);
        }
    }
    parts[30].Draw(0, 0);
    parts[31].Draw(0, 0);
    parts[32].Draw(0, 0);
    parts[33].Draw(0, 0);
    shownAirshipHpPercent = airshipHpPercent;
}
