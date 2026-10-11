# The HUD counters' slide states (#843)

Hud's lives, wumpa and crate counters (hud.hpp) each have a slide state,
`livesSlide` / `wumpaSlide` / `crateSlide`, and a timer. While a counter
slides, its parts are drawn `timer * 2 - 40` pixels up (gHudSlideOffset).
The names, `HUD_SLIDE_STATE_*`, sit before the class.

| Value | Name | Timer | Moves on |
|---|---|---|---|
| 0 | `HUD_SLIDE_STATE_HIDDEN` | | ShowLives/ShowWumpa/ShowCrates (not in a time trial) start it sliding in. Update* skip a hidden counter; Update draws the time-trial clock only while the lives and wumpa counters are hidden |
| 1 | `HUD_SLIDE_STATE_IN` | counts up | past 0x13: `HELD`, the timer set to StepSlide's threshold (0x78) |
| 2 | `HUD_SLIDE_STATE_HELD` | counts down; Show* refills it to 0x78 | at 0: `OUT`, the timer 0x14 |
| 3 | `HUD_SLIDE_STATE_OUT` | counts down | at 0: `HIDDEN`. Show* starts it sliding in again |

In a time trial UpdateSlides sends the lives and wumpa counters straight
to `OUT` from `IN` or `HELD` (a `HELD` one with its timer set to 0x14).

StepSlide's `*state = *timer` stores the timer's 0 as `HIDDEN`; the ROM
stores the register it just tested, so it stays that way, with a comment.
