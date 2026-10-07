#include "core.h"
#include "level_state.h"
#include "hud.h"
#include "vtable.h"
#include "memory.h"
#include "globals.h"

/* The lives, wumpa and crate counters' slide-in timers (`struct
 * hud_counter`, include/hud.h), shared with `SetHudCrateTotal`/
 * `IncHudCrateTotal` via the `gHud` instance. Each counter has a
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
void UpdateHudSlides(struct hud_counter *self)
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

        v = self->livesSlide;
        if (v == 1) {
            goto set0;
        }
        if (v <= 1) {
            goto skip0;
        }
        if (v != 2) {
            goto skip0;
        }
        self->livesSlideTimer = 0x14;
    set0:
        self->livesSlide = 3;
    skip0:

        v = self->wumpaSlide;
        if (v == 1) {
            goto set1;
        }
        if (v <= 1) {
            goto skip1;
        }
        if (v != 2) {
            goto skip1;
        }
        self->wumpaSlideTimer = 0x14;
    set1:
        self->wumpaSlide = 3;
    skip1:;
    }

    StepHudSlide(self, &self->crateSlide, &self->crateSlideTimer, 0x78);
    StepHudSlide(self, &self->livesSlide, &self->livesSlideTimer, 0x78);
    StepHudSlide(self, &self->wumpaSlide, &self->wumpaSlideTimer, 0x78);
}

/* Trigger for the crate counter: only runs while the level state's `timeTrial`
 * flag is clear. Idle (0) or finished (3) starts
 * a fresh blink (state -> 1); already counting down the "on" phase (2)
 * instead just refreshes its timer back to the full 0x78-frame hold.
 * Already counting up (1) is left alone. */
void ShowHudCrates(struct hud_counter *self)
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = self->crateSlide;

        if (slotState == 0 || slotState == 3) {
            self->crateSlide = 1;
        } else if (slotState == 2) {
            self->crateSlideTimer = 0x78;
        }
    }
}

/* Same trigger as `ShowHudCrates`, for the lives counter. */
void ShowHudLives(struct hud_counter *self)
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = self->livesSlide;

        if (slotState == 0 || slotState == 3) {
            self->livesSlide = 1;
        } else if (slotState == 2) {
            self->livesSlideTimer = 0x78;
        }
    }
}

/* Same trigger as `ShowHudCrates`, for the wumpa counter. */
void ShowHudWumpa(struct hud_counter *self)
{
    s32 slotState;

    if (gLevelState->timeTrial == 0) {
        slotState = self->wumpaSlide;

        if (slotState == 0 || slotState == 3) {
            self->wumpaSlide = 1;
        } else if (slotState == 2) {
            self->wumpaSlideTimer = 0x78;
        }
    }
}

/* Fires all three counters' triggers at once. */
void ShowHudCounters(struct hud_counter *self)
{
    ShowHudCrates(self);
    ShowHudLives(self);
    ShowHudWumpa(self);
}

/* Generic single-counter slide advance, called once per counter by
 * `UpdateHudSlides` above. `self` is unused - forwarded through purely for
 * calling-convention parity with the trigger functions above.
 *
 * The explicit (never-reached, since callers only ever pass 0-3 and
 * state 0 is otherwise idle/inert) `case 0` below is load-bearing, not
 * dead code: without it this compiler lowers the switch as a balanced
 * comparison tree pivoting on the middle case value (2) instead of the
 * ROM's plain ascending compare chain (1, then an early-out for
 * anything <= 1, then 2, then 3) - see docs/workflow.md step 7. */
void StepHudSlide(struct hud_counter *self, s32 *state, s32 *timer, s32 threshold)
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
void SetHudCrateTotal(struct hud_counter *self, s32 val)
{
    self->crateTotal = val;
}

void IncHudCrateTotal(struct hud_counter *self)
{
    self->crateTotal += 1;
}

/* Sits right after hud_slide.c's blink-timer trio (ROM 0x08028568) and
 * before FontDrawGlyph/InitSmallFont/InitLargeFont/FontPutChar
 * (src/text/font_glyph.c) - see GitHub issue #46. Just
 * `DestroyHud` here: a `struct hud_counter`-parts destructor, unrelated
 * to the `struct bitmap_font` text/icon-glyph renderer the rest of this
 * chunk's functions operate on (see include/bitmap_font.h and the other
 * src/text/font*.c files). */

extern s32 _call_via_r2(void *arg0, s32 arg1, void *arg2);

/* Destructor for a `struct hud_counter`'s `parts` array (see
 * include/hud.h): walks the array back to front, invoking each
 * `hud_digit_part`'s own per-type descriptor's slot-10 (`table+0x50`)
 * teardown trampoline via `_call_via_r2`, frees the array itself
 * (`self->parts`, allocated with a leading element-count word per the
 * NEW_ARRAY_COUNT read below - see the same convention in src/gfx/
 * palette_cycle.cpp/actor files), then optionally frees `self` when
 * `flags` bit 0 is set (same "free-self" convention as
 * DestroyPaletteCycles/DestroyLanguageSelect elsewhere in this codebase). The
 * descriptor is a vtable (gHudPartVtable, include/vtable.h), and
 * `table+0x50` is its slot 10. */
void DestroyHud(struct hud_counter *self, s32 flags)
{
    struct hud_digit_part *parts;
    struct hud_digit_part *end;

    parts = self->parts;
    if (parts != NULL) {
        s32 count = NEW_ARRAY_COUNT(parts);

        end = (struct hud_digit_part *)((u8 *)parts + (count << 6));
        if (parts != end) {
            do {
                struct vtable_slot *slot;

                end--;
                slot = (struct vtable_slot *)end->table + 10; /* slot 10 */
                _call_via_r2((u8 *)end + slot->delta, 0, slot->fn);
            } while (self->parts != end);
        }
        OperatorDeleteArray(NEW_ARRAY_BLOCK(self->parts));
    }
    if (flags & 1) {
        OperatorDelete(self);
    }
}
