#ifndef GUARD_HUD_HPP
#define GUARD_HUD_HPP

/* The in-game HUD as C++ (#664 cleanup): class Hud, gHud (globals.h
 * declares it as a `Hud *` to C++), built by `new Hud` in
 * src/level/game_frame.c (InitHud(OperatorNew(0x68))) and deleted there
 * (DestroyHud(gHud, 3)). It has no vtable. Its 35 parts are HudParts
 * (part_list.hpp), allocated with `new HudPart[35]` by the constructor
 * and freed with `delete[]` by the destructor.
 *
 *   src/hud/hud_init.cpp      constructor, ConfigureParts
 *   src/hud/hud.cpp           Update
 *   src/hud/hud_boss_clock.cpp UpdateBoss, UpdateClock
 *   src/hud/hud_lives.cpp     UpdateLives
 *   src/hud/hud_counters.cpp  UpdateCrates, UpdateWumpa, UpdatePercentCounters
 *   src/hud/hud_slide.cpp     the slides, the crate total, destructor
 *
 * hud.h's struct hud_counter stays the C view, for the C files (the
 * level and actor code); the C prototypes there keep the methods' C
 * names, which cxx_symbols.txt maps them to. */

#include "part_list.hpp"

extern "C" {
#include "core.h"
#include "hud.h"
}

/* Sets a part's sprite bank. `bank` is in Sprite's anonymous union with
 * `anim`, and gcc gives every access to a union member alias set 0, so a
 * plain store would make gcc reload `parts` after it. Through a pointer
 * to the member the store has the pointer's own alias set (as menus.hpp's
 * SetIconBank and level_select.hpp's SetBankNow); written in place rather
 * than through an inline's local, so the store keeps the part's register
 * as its base (`str rN, [part, #0x20]`), as the ROM has it. */
#define SET_PART_BANK(part, b) (*(const struct sprite_bank **)&(part)->bank = (b))

/* Sets the part's frame, clamped to its animation's last one (the C's
 * HUD_CLAMP_FRAME, hud.h). The part pointer is bound before the frame
 * value, which is what gives old_agbcp's clamp sequence. */
#define HUDPART_CLAMP_FRAME(part, index, step)                                 \
    {                                                                          \
        HudPart *_p = (part);                                                  \
        s32 _f = (step);                                                       \
        s32 _n = _p->bank->anims[index].frameCount;                            \
        CLAMP_INDEX(_f, _n);                                                   \
        _p->frame = _f;                                                        \
    }

class Hud
{
public:
    s32 livesSlide;            // 0x00 - the lives counter's slide state (hud.h)
    s32 livesSlideTimer;       // 0x04
    s32 wumpaSlide;            // 0x08
    s32 wumpaSlideTimer;       // 0x0C
    s32 crateSlide;            // 0x10
    s32 crateSlideTimer;       // 0x14
    u8 icon_flag;              // 0x18 - the jetpack categories' percentage counters
    u8 unknown_19[3];          // 0x19
    s32 lives;                 // 0x1C
    s32 wumpa;                 // 0x20
    s32 crateCount;            // 0x24
    s32 crateTotal;            // 0x28
    s32 shownMinutes;          // 0x2C
    s32 shownSeconds;          // 0x30
    s32 shownTenths;           // 0x34
    s32 playerHpPercent;       // 0x38
    s32 airshipHpPercent;      // 0x3C
    s32 shownLives;            // 0x40 - ConfigureParts fills +0x40..+0x63 with -1
    s32 shownWumpa;            // 0x44
    s32 shownCrateCount;       // 0x48
    s32 shownCrateTotal;       // 0x4C
    u8 unknown_50[0xC];        // 0x50
    s32 shownPlayerHpPercent;  // 0x5C
    s32 shownAirshipHpPercent; // 0x60
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

COMPILE_TIME_ASSERT(hud_hpp, sizeof(Hud) == sizeof(struct hud_counter));

#endif /* !GUARD_HUD_HPP */
