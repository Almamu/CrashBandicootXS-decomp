#ifndef GUARD_HUD_HPP
#define GUARD_HUD_HPP

/* The in-game HUD as C++ (#664 cleanup): class Hud, gHud (globals.h
 * declares it as a `Hud *` to C++), built by `new Hud` in
 * engine/src/game/level/game_frame.cpp and deleted there (`delete
 * gHud`). It has no vtable. Its 35 parts are HudParts
 * (part_list.hpp), allocated with `new HudPart[35]` by the constructor
 * and freed with `delete[]` by the destructor.
 *
 *   engine/src/game/ui/hud_init.cpp      constructor, ConfigureParts
 *   engine/src/game/ui/hud.cpp           Update
 *   engine/src/game/ui/hud_counters.cpp  UpdateBoss, UpdateClock, UpdateLives,
 *                             UpdateCrates, UpdateWumpa, UpdatePercentCounters
 *   engine/src/game/ui/hud_slide.cpp     the slides, the crate total, destructor
 *
 * Update (UpdateHud) is the per-frame dispatcher; its widgets are
 * UpdateLives, UpdateClock, UpdateWumpa, UpdateCrates, UpdateBoss and
 * UpdatePercentCounters (docs/rom_map.md's "hud" investigation). The
 * lives, wumpa and crate counters each slide in from the top of the
 * screen (ShowLives/ShowWumpa/ShowCrates/UpdateSlides, hud_slide.cpp): a
 * counter's slide state is 0 hidden, 1 sliding in, 2 held, 3 sliding out
 * (HUD_SLIDE_STATE_*), and while it slides
 * `gHudSlideOffset = slideTimer * 2 - 40`.
 *
 * Hud has no C view; cxx_symbols.txt maps the methods to their C names. */

#include "game/entities/part_list.hpp"

extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
#include "engine/util/aabb.h"
}

/* The vertical offset HudPart::Draw adds to every part, set by the
 * counters while they slide (engine/src/engine/sys/iwram_data.cpp): -40 to 0, from
 * above the top row into place. */
extern screen_dy_t gHudSlideOffset;

/* Each of the 35 parts' starting animation and position
 * (engine/src/game/ui/data/hud_font_tables.cpp). */
extern const u32 gHudPartAnims[35];
extern const struct vec2 gHudPartPositions[35];

/* Sets a part's sprite bank. `bank` is in Sprite's one-member union (sprite_obj.hpp),
 * and gcc gives every access to a union member alias set 0, so a
 * plain store would make gcc reload `parts` after it. Through a pointer
 * to the member the store has the pointer's own alias set (as menus.hpp's
 * SetIconBank and level_select.hpp's SetBankNow); written in place rather
 * than through an inline's local, so the store keeps the part's register
 * as its base (`str rN, [part, #0x20]`), as the ROM has it. */
#define SET_PART_BANK(part, b) (*(const struct sprite_bank **)&(part)->bank = (b))

/* Sets the part's frame, clamped to its animation's last one (the C's
 * HUD_CLAMP_FRAME before it). The part pointer is bound before the frame
 * value, which is what gives old_agbcp's clamp sequence. */
#define HUDPART_CLAMP_FRAME(part, index, step)                                 \
    {                                                                          \
        Game::Ui::HudPart *_p = (part);                                        \
        s32 _f = (step);                                                       \
        s32 _n = _p->bank->anims[index].frameCount;                            \
        CLAMP_INDEX(_f, _n);                                                   \
        _p->frame = _f;                                                        \
    }

/* A counter's slide state, Hud's livesSlide/wumpaSlide/crateSlide
 * (#843, docs/flags/hud_slide.md). Show* starts a hidden or sliding-out
 * counter sliding in, or refreshes a held one's timer; StepSlide moves
 * each on when its timer runs out. */
#define HUD_SLIDE_STATE_HIDDEN 0 // not drawn
#define HUD_SLIDE_STATE_IN     1 // the timer counts up to 0x14: sliding down into view
#define HUD_SLIDE_STATE_HELD   2 // shown; the timer counts down from StepSlide's threshold (0x78)
#define HUD_SLIDE_STATE_OUT    3 // the timer counts down from 0x14: sliding back up

namespace Game
{
namespace Ui
{

/* The HUD object (`gHud`, 0x68 bytes). The lives, wumpa and crate
 * counters each have a {slide state, slide timer} pair, stepped by
 * StepSlide. Each counter keeps the value it is showing (`shown*`), so it
 * only updates its digits when the value changes. */
class Hud
{
public:
    s32 livesSlide;       // 0x00 - the lives counter's slide state
    s32 livesSlideTimer;  // 0x04
    s32 wumpaSlide;       // 0x08 - the wumpa counter's; Update only draws the time-trial
                          //        clock while neither counter is shown
    s32 wumpaSlideTimer;  // 0x0C
    s32 crateSlide;       // 0x10 - the crate counter's
    s32 crateSlideTimer;  // 0x14
    u8 icon_flag;         // 0x18 - the jetpack categories' percentage counters: Update's
                          //        gate for UpdatePercentCounters (ConfigureParts' argument)
    s32 lives;            // 0x1C
    s32 wumpa;            // 0x20
    s32 crateCount;       // 0x24
    s32 crateTotal;       // 0x28 - SetCrateTotal/IncCrateTotal
    s32 shownMinutes;     // 0x2C - UpdateClock's caches of GetClockMinutes/Seconds/Tenths
    s32 shownSeconds;     // 0x30
    s32 shownTenths;      // 0x34
    s32 playerHpPercent;  // 0x38 - UpdatePercentCounters' first percentage: the jetpack
                          //        player's HP
    s32 airshipHpPercent; // 0x3C - its second (GetAirshipHpPercent)
    s32 shownLives;       // 0x40 - ConfigureParts fills +0x40..+0x63 with -1
    s32 shownWumpa;       // 0x44
    s32 shownCrateCount;  // 0x48
    s32 shownCrateTotal;  // 0x4C
    // 0x50 - three more counter caches ConfigureParts sets to -1 with the
    // others; nothing else touches them
    s32 unknown_50[3];
    s32 shownPlayerHpPercent;  // 0x5C - the cache of `playerHpPercent`
    s32 shownAirshipHpPercent; // 0x60 - of `airshipHpPercent`
    HudPart *parts;            // 0x64 - the 35 parts

    Hud();                                                 // InitHud
    ~Hud();                                                // DestroyHud
    void ConfigureParts(u8 iconFlag);                      // ConfigureHudParts
    void Update();                                         // UpdateHud
    void UpdateBoss();                                     // UpdateHudBoss
    void UpdateClock();                                    // UpdateHudClock
    void UpdateLives();                                    // UpdateHudLives
    void UpdateCrates();                                   // UpdateHudCrates
    void UpdateWumpa();                                    // UpdateHudWumpa
    void UpdatePercentCounters();                          // UpdateHudPercentCounters
    void UpdateSlides();                                   // UpdateHudSlides
    void ShowCrates();                                     // ShowHudCrates
    void ShowLives();                                      // ShowHudLives
    void ShowWumpa();                                      // ShowHudWumpa
    void ShowCounters();                                   // ShowHudCounters
    void StepSlide(s32 *state, s32 *timer, s32 threshold); // StepHudSlide
    void SetCrateTotal(s32 val);                           // SetHudCrateTotal
    void IncCrateTotal();                                  // IncHudCrateTotal
};

LAYOUT_ASSERT(hud_hpp, sizeof(Hud) == 0x68);

} // namespace Ui
} // namespace Game

#endif /* !GUARD_HUD_HPP */
