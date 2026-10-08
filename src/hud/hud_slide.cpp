#include "hud.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "globals.h"
}

/* Hud's (#664 cleanup, include/hud.hpp) slide-in timers for the lives,
 * wumpa and crate counters, the crate total's setters and the
 * destructor. Each counter has a
 * {state, timer} pair: state 0 idle, 1 counting up to a threshold then
 * -> 2, 2 counting down 0x14 frames then -> 3, 3 counting down its own
 * timer then back to 0. The lives counter slides in with
 * `ShowHudLives`, the wumpa counter with `ShowHudWumpa` (`CollectWumpa`),
 * and the crate counter with `ShowHudCrates`, whenever the level state's
 * `crateCount` advances (`AddBrokenCrate`). `crateTotal` receives the
 * level state's `crateTotal` (`EndBonusRound`). */

/* Per-frame tick, gated on the level state's `timeTrial` flag:
 * force-advances the lives and wumpa counters out of a stuck 1/2 state
 * (state 1 -> 3 directly; state 2 -> 3, refreshing its timer to 0x14
 * first), then runs the generic `StepHudSlide` advance on all three
 * counters unconditionally. The crate counter doesn't get the manual
 * force-advance the other two do - reproduced as-is. */
void Hud::UpdateSlides()
{
    /* Pointer arithmetic is kept inline (not cached into locals) at each
     * use site, matching the ROM's own redundant recomputation - a
     * plain cached-pointer version pulls the four field addresses into
     * extra callee-saved registers up front (see docs/workflow.md
     * step 7). Each of the two "force out of state 1/2" checks below
     * also needs the same explicit `goto`-forced extra `<=`
     * range-check branch as `StepHudSlide`'s dispatch (see its own
     * comment) to reproduce the ROM's exact compare chain - a plain
     * `if (v==1) {...} else if (v==2) {...}` collapses that redundant
     * middle branch away. */
    if (gLevelState->timeTrial != 0) {
        s32 v;

        v = livesSlide;
        if (v == 1) {
            goto set0;
        }
        if (v <= 1) {
            goto skip0;
        }
        if (v != 2) {
            goto skip0;
        }
        livesSlideTimer = 0x14;
    set0:
        livesSlide = 3;
    skip0:

        v = wumpaSlide;
        if (v == 1) {
            goto set1;
        }
        if (v <= 1) {
            goto skip1;
        }
        if (v != 2) {
            goto skip1;
        }
        wumpaSlideTimer = 0x14;
    set1:
        wumpaSlide = 3;
    skip1:;
    }

    StepSlide(&crateSlide, &crateSlideTimer, 0x78);
    StepSlide(&livesSlide, &livesSlideTimer, 0x78);
    StepSlide(&wumpaSlide, &wumpaSlideTimer, 0x78);
}

/* Trigger for the crate counter: only runs while the level state's `timeTrial`
 * flag is clear. Idle (0) or finished (3) starts
 * a fresh blink (state -> 1); already counting down the "on" phase (2)
 * instead just refreshes its timer back to the full 0x78-frame hold.
 * Already counting up (1) is left alone. */
void Hud::ShowCrates()
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = crateSlide;

        if (slotState == 0 || slotState == 3) {
            crateSlide = 1;
        } else if (slotState == 2) {
            crateSlideTimer = 0x78;
        }
    }
}

/* Same trigger as `ShowHudCrates`, for the lives counter. */
void Hud::ShowLives()
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = livesSlide;

        if (slotState == 0 || slotState == 3) {
            livesSlide = 1;
        } else if (slotState == 2) {
            livesSlideTimer = 0x78;
        }
    }
}

/* Same trigger as `ShowHudCrates`, for the wumpa counter. */
void Hud::ShowWumpa()
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = wumpaSlide;

        if (slotState == 0 || slotState == 3) {
            wumpaSlide = 1;
        } else if (slotState == 2) {
            wumpaSlideTimer = 0x78;
        }
    }
}

/* Fires all three counters' triggers at once. */
void Hud::ShowCounters()
{
    ShowCrates();
    ShowLives();
    ShowWumpa();
}

/* Generic single-counter slide advance, called once per counter by
 * `UpdateSlides` above. A method, though it doesn't use `this`.
 *
 * The explicit (never-reached, since callers only ever pass 0-3 and
 * state 0 is otherwise idle/inert) `case 0` below is load-bearing, not
 * dead code: without it this compiler lowers the switch as a balanced
 * comparison tree pivoting on the middle case value (2) instead of the
 * ROM's plain ascending compare chain (1, then an early-out for
 * anything <= 1, then 2, then 3) - see docs/workflow.md step 7. */
void Hud::StepSlide(s32 *state, s32 *timer, s32 threshold)
{
    switch (*state) {
    case 1:
        *timer += 1;
        if (*timer > 0x13) {
            *state = 2;
            *timer = threshold;
        }
        break;
    case 0:
        break;
    case 3:
        *timer -= 1;
        if (*timer == 0) {
            *state = *timer;
        }
        break;
    case 2:
        *timer -= 1;
        if (*timer == 0) {
            *state = 3;
            *timer = 0x14;
        }
        break;
    }
}

/* Setter/increment pair for `crateTotal` (see the file doc comment
 * above) - GitHub issue #46. */
void Hud::SetCrateTotal(s32 val)
{
    crateTotal = val;
}

void Hud::IncCrateTotal()
{
    crateTotal += 1;
}

/* Sits right after hud_slide.cpp's blink-timer trio (ROM 0x08028568) and
 * before FontDrawGlyph/InitSmallFont/InitLargeFont/FontPutChar
 * (src/text/font_glyph.cpp) - see GitHub issue #46. */

/* DestroyHud: deletes the parts (each one's virtual destructor, slot 10
 * of gHudPartVtable, from the last to the first, then the array with its
 * count word); game_frame.cpp deletes gHud itself (`flags` 3). */
Hud::~Hud()
{
    delete[] parts;
}
