#ifndef GUARD_MENUS_HPP
#define GUARD_MENUS_HPP

/* The pause menu and the power dialog as C++ (#664, docs/cplusplus.md,
 * part 10d):
 *
 *   PauseMenu    0xD4  src/menus/pause_menu.cpp, pause_menu_pages_init.cpp,
 *                      pause_menu_info.cpp, pause_menu_draw.cpp, _gems.cpp,
 *                      _loop.cpp, _pages_draw.cpp, _powers.cpp, _widgets.cpp
 *   PowerDialog  0x2C  src/menus/power_dialog.cpp, power_dialog_draw.cpp,
 *                      power_dialog_loop.cpp
 *
 * The sizes are the ROM's (RunPauseMenu's and ShowPowerDialog's `new`s).
 * Neither has a vtable: they are plain classes with a constructor and a
 * destructor (`delete` calls the destructor with 3, and it frees the
 * object when bit 0 is set). Their icons are UiSprites.
 *
 * All of their code is C++ (the pause menu's and the power dialog's last
 * C files since the #664 cleanup); cxx_symbols.txt maps the methods to
 * their C names. The continue prompt's class, ContinuePrompt, is in
 * frontend.hpp.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to emit;
 * the pragma keeps g++ from emitting out-of-line copies of the inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

#include "sprite_obj.hpp"

extern "C" {
#include "core.h"
#include "gfx.h"
#include "graphics_package.h"
#include "menus.h"
#include "level_menu.h"
#include "pause_menu.h"
}

/* REG_DISPCNT's shadow as a halfword or its bitfields. */
union MenuDispcnt {
    u16 raw;
    struct dispcnt_bits bits;
};

/* Sets an icon's sprite bank to the one at `offset` in the sprite bank
 * data. `bank` is in Sprite's anonymous union with `anim`, and gcc gives
 * every access to a union member alias set 0, so a plain store would make
 * gcc reload everything after it. Through a pointer to the member, the
 * store has the pointer's own alias set (as level_select.hpp's
 * SetBankNow). */
static inline void SetIconBank(Sprite *p, s32 offset)
{
    const struct sprite_bank **field = &p->bank;

    *field = (const struct sprite_bank *)(SPRITE_BANK_BASE + offset);
}

/* The pause menu (RunPauseMenu, called from the rooms and the 3D actors'
 * pause button): the rows on the left (gPauseMenuRows: resume, music,
 * sound, warp room, restart trial), and on the right five info pages that
 * cycle every 180 frames (crystals, powers, gems, relics, the level's
 * time trial), over a background with blinking eyes. */
class PauseMenu
{
public:
    struct bg_setup bg;             // 0x00 - BG0
    struct game_progress *progress; // 0x10 - PackSaveData(gLevelState)
    const struct pause_row *rows;   // 0x14 - gPauseMenuRows
    s32 cursor;                     // 0x18 - the selected row
    s32 rowCount;                   // 0x1C - 4, or 5 in a time trial
    s32 rowSpacing;                 // 0x20 - 16
    s32 page;                       // 0x24 - the info page shown (0-4)
    s32 pageTimer;                  // 0x28 - frames to the next page
    u8 crystalCount[3];             // 0x2C - the info pages' numbers, as text
    u8 clearGemCount[3];            // 0x2F
    u8 gemCount[3];                 // 0x32
    u8 relicCount[3];               // 0x35
    u8 sapphireCount[3];            // 0x38
    u8 goldCount[3];                // 0x3B
    u8 platinumCount[3];            // 0x3E
    u8 percentText[5];              // 0x41 - InitPauseMenuInfo
    u8 crystalTotal[3];             // 0x46
    u8 gemTotal[3];                 // 0x49
    u8 relicTotal[3];               // 0x4C
    u8 soundVolumeText[8];          // 0x4F
    u8 musicVolumeText[9];          // 0x57
    s32 musicVolume;                // 0x60 - 0-20, in 5% steps
    s32 soundVolume;                // 0x64
    s32 flashTimer;                 // 0x68
    u8 trialEarned;                 // 0x6C
    u8 unused_6d[3];                // 0x6D
    void *levelName;                // 0x70
    void *levelLabel;               // 0x74
    u8 levelNumber[4];              // 0x78
    u8 timeText[0xC];               // 0x7C
    UiSprite *crystalIcon;          // 0x88
    UiSprite *powerIcons[4];        // 0x8C
    UiSprite *gemIcons[5];          // 0x9C
    UiSprite *relicIcons[3];        // 0xB0
    UiSprite *trialIcon;            // 0xBC
    UiSprite *blinkEyes;            // 0xC0 - the eyelids over the background
    s32 blinkTimer;                 // 0xC4 - frames to the next blink
    union blend blend;              // 0xC8 - REG_BLDCNT
    struct bldy bldy;               // 0xCC - REG_BLDY
    union MenuDispcnt dispcnt;      // 0xD0 - REG_DISPCNT (a word: a struct is 4-aligned)

    PauseMenu();                                 // InitPauseMenu
    ~PauseMenu();                                // DestroyPauseMenu
    static s32 Run();                            // RunPauseMenu
    void InitInfo();                             // InitPauseMenuInfo
    s32 Loop();                                  // PauseMenuLoop
    void InitCrystalsPage();                     // InitPauseCrystalsPage
    void InitPowersPage();                       // InitPausePowersPage
    void InitGemsPage();                         // InitPauseGemsPage
    void InitRelicsPage();                       // InitPauseRelicsPage
    void InitTimeTrialPage();                    // InitPauseTimeTrialPage
    void DrawPowersPage();                       // DrawPausePowersPage
    void DrawGemsPage();                         // DrawPauseGemsPage
    void DrawRelicsPage();                       // DrawPauseRelicsPage
    void DrawFraction(void *count, void *total); // DrawPauseFraction
    void VolumeDown();                           // PauseMenuVolumeDown
    void VolumeUp();                             // PauseMenuVolumeUp
    void CursorDown();                           // PauseMenuCursorDown
    s32 CursorUp();                              // PauseMenuCursorUp
    void FormatVolume(s32 volume, u8 *out);      // FormatVolumePercent
    void DrawTimeTrialPage();                    // DrawPauseTimeTrialPage
    void DrawCrystalsPage();                     // DrawPauseCrystalsPage
    void DrawPageTitle();                        // DrawPauseMenuPageTitle
    void CommitFrame();                          // CommitPauseMenuFrame
    void Animate();                              // AnimatePauseMenu
    void Draw();                                 // DrawPauseMenu
    void DrawRows();                             // DrawPauseMenuRows
};

COMPILE_TIME_ASSERT(menus_hpp, sizeof(PauseMenu) == 0xD4);

/* The power dialog (ShowPowerDialog, from the four Show*Dialog wrappers
 * game_frame.cpp calls when a boss gives Crash a power): the power's name
 * and description over a scrolling sky, with its icon, faded in and out
 * through BLDY. */
class PowerDialog
{
public:
    struct bg_setup bg;        // 0x00 - BG0
    s32 titleText;             // 0x10 - GetUiText's result
    s32 descText;              // 0x14
    UiSprite *icon;            // 0x18 - the power's icon
    u32 frame;                 // 0x1C - frame counter; BG0HOFS = frame >> 3
    union blend blend;         // 0x20 - REG_BLDCNT
    struct bldy bldy;          // 0x24 - REG_BLDY, faded 16 -> 0 -> 16 by Loop
    union MenuDispcnt dispcnt; // 0x28 - REG_DISPCNT (a word: a struct is 4-aligned)

    PowerDialog(s32 titleText, s32 descText, s32 type); // InitPowerDialog
    ~PowerDialog();                                     // DestroyPowerDialog
    static void Show(s32 title, s32 desc, s32 type);    // ShowPowerDialog
    void Draw();                                        // DrawPowerDialog
    void Animate();                                     // AnimatePowerDialog
    void CommitFrame();                                 // CommitPowerDialogFrame
    void Loop();                                        // PowerDialogLoop
};

COMPILE_TIME_ASSERT(menus_hpp, sizeof(PowerDialog) == 0x2C);

#endif /* GUARD_MENUS_HPP */
