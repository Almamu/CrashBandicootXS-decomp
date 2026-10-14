#ifndef GUARD_MENUS_HPP
#define GUARD_MENUS_HPP

/* The pause menu and the power dialog as C++ (#664, docs/cplusplus.md,
 * part 10d):
 *
 *   PauseMenu    0xD4  src/game/ui/pause_menu.cpp, pause_menu_draw.cpp,
 *                      _collectibles.cpp, _info.cpp, _pages_init.cpp,
 *                      _widgets.cpp, _pages_draw.cpp
 *   PowerDialog  0x2C  src/game/ui/power_dialog.cpp, power_dialog_draw.cpp
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
 * Below the includes are the menus' records and tables (the level
 * select's, the pause menu's and the continue prompt's, which src/.../data
 * defines), the data the C++ shares with them, and the menus' entry
 * points.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to emit;
 * the pragma keeps g++ from emitting out-of-line copies of the inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

#include "game/entities/sprite_obj.hpp"
#include "engine/gfx/graphics_package.hpp"
#include "game/level/game_progress.hpp"

extern "C" {
#include "core.h"
#include "engine/util/aabb.h"
#include "engine/gfx/gfx.h"
#include "engine/gfx/sprite_bank.h"
#include "game/entities/objects.h"
#include "constants/levels.h"
}

/* One row of the pause menu (gPauseMenuRows, PauseMenu::rows): a
 * GetUiText label id, then the row's type (PAUSE_ROW_TYPE_*; PauseMenu::Loop
 * returns the confirmed row's type). */
struct pause_row {
    s32 labelId;
    s32 type;
};

/* pause_row::type, and what RunPauseMenu returns (#843,
 * docs/flags/pause_menu.md). A confirms a row; on the two volume rows it
 * only plays a cue, and START returns PAUSE_ROW_TYPE_RESUME. RunRoom and
 * ActorCategory::Init turn the result into a ROOM_EXIT_* status. */
#define PAUSE_ROW_TYPE_RESUME  0 // "resume" (row 0), and START
#define PAUSE_ROW_TYPE_RESTART 1 // row 4, shown only in a time trial: ROOM_EXIT_RETRY
#define PAUSE_ROW_TYPE_QUIT    2 // row 3: ROOM_EXIT_QUIT
#define PAUSE_ROW_TYPE_UNK3    3 // no row has it; ActorCategory::Init takes it as ROOM_EXIT_CLEARED
#define PAUSE_ROW_TYPE_MUSIC   4 // row 1, the music volume (left/right)
#define PAUSE_ROW_TYPE_SOUND   5 // row 2, the sound-effect volume (left/right)

/* One level-select picture (gLevelSelectPictures): the tagged-asset
 * palette and tiles UpdateZoomBg loads with LoadTaggedAsset. */
struct image_pair {
    const u8 *palette;
    const u8 *tiles;
};

/* The level-select tables (src/game/ui/data/level_select_layout.cpp,
 * level_select_anims.cpp, level_select_picture_table.cpp, level_select_page_bg_package.cpp,
 * data/data.s). */
extern const struct vec2 gLevelSelectWorldPos;
extern const struct vec2 gLevelSelectCrashIconPos;
extern const struct vec2 gLevelSelectCrystalPos;
extern const struct vec2 gLevelSelectGemPos;
extern const struct vec2 gLevelSelectTrialIconPos;
extern const struct vec2 gLevelSelectTimePos;
extern const struct vec2 gLevelSelectNextWorldArrowPos;
extern const struct vec2 gLevelSelectPrevWorldArrowPos;
extern const struct vec2 gLevelSelectEntryPositions[LEVEL_SELECT_ENTRIES];
extern const struct vec2 gLevelSelectEntryPositionsAllCleared[LEVEL_SELECT_ENTRIES];
extern const u32 gLevelSelectWorldEntryBoxAnims[WORLD_COUNT];
extern const u32 gLevelSelectWorldAnims[WORLD_COUNT];
extern const u32 gLevelSelectRankAnims[5];
extern const u16 gLevelSelectPalette[16];
extern const struct vec2 gZoomBgSlotOffsets[4];
extern const u32 gLevelSelectEntryBoxAnims[5];
extern const u32 gLevelSelectEntryWorldAnims[WORLD_COUNT];
extern const u32 gLevelSelectCursorAnims[4];
extern const struct image_pair gLevelSelectPictures[10];
extern const struct bg_package gLevelSelectPageBg;

/* The pause menu tables (src/game/ui/data/menu_tables.cpp,
 * pause_menu_rows.cpp, pause_menu_bg.cpp). */
extern const s32 gPauseMenuPageTitles[5];
extern const struct vec2 gPauseCrystalIconPos;
extern const struct vec2 gPausePowerIconPos[4];
extern const s32 gPausePowerIconFrames[4];
extern const struct vec2 gPauseGemIconPos[5];
extern const s32 gPauseGemIconFrames[5];
extern const struct vec2 gPauseRelicIconPos[3];
extern const s32 gPauseRelicIconFrames[3];
extern const struct vec2 gPauseTimeTrialIconPos;
extern const struct pause_row gPauseMenuRows[5];
extern const u16 gPauseMenuPalette[16];
extern const struct bg_package gPauseMenuBg;

/* The continue prompt's graphics (src/game/ui/data/continue_prompt_bgs.cpp,
 * continue_prompt_palettes.cpp). */
extern const struct bg_package gContinuePromptSmokeBg;
extern const struct bg_package gContinuePromptGlowBg;
extern const struct bg_package gContinuePromptUkaUkaBg;
extern const u8 gContinuePromptCursorText[];
extern const u16 gContinuePromptPalette0[16];
extern const u16 gContinuePromptPalette1[16];
extern const u16 gContinuePromptPalette2[16];
extern const u16 gContinuePromptPalette3[17];

/* The entry points the game loop calls by their C names, so they keep C
 * linkage: the free functions' own names, and the C names of the methods
 * (cxx_symbols.txt). */

/* src/game/ui/continue_prompt.cpp (frontend.hpp: ContinuePrompt::Run) */
extern "C" u8 RunContinuePrompt(void);

/* src/game/ui/pause_menu.cpp (below: PauseMenu::Run) */
extern "C" s32 RunPauseMenu(void);

/* src/game/ui/pause_menu_widgets.cpp */
extern "C" s32 FormatDecimal(s32 value, u8 *dest);

/* src/game/ui/power_dialog_draw.cpp (the free functions around PowerDialog,
 * below) */
extern "C" void ShowTurboRunDialog(void);
extern "C" void ShowTornadoSpinDialog(void);
extern "C" void ShowDoubleJumpDialog(void);
extern "C" void ShowSuperBodySlamDialog(void);

namespace Game
{
namespace Ui
{

/* Sets an icon's sprite bank to bank `index` of the sprite bank table. `bank` is in Sprite's one-member union (sprite_obj.hpp), and gcc
 * gives every access to a union member alias set 0, so a plain store would make
 * gcc reload everything after it. Through a pointer to the member, the
 * store has the pointer's own alias set (as level_select.hpp's
 * SetBankNow). */
static inline void SetIconBank(Game::Entities::Sprite *p, s32 index)
{
    const struct sprite_bank **field = &p->bank;

    *field = gSpriteBankSet->Bank(index);
}

/* The pause menu (RunPauseMenu, called from the rooms and the 3D actors'
 * pause button): the rows on the left (gPauseMenuRows: resume, music,
 * sound, warp room, restart trial), and on the right five info pages that
 * cycle every 180 frames (crystals, powers, gems, relics, the level's
 * time trial), over a background with blinking eyes. */
class PauseMenu
{
public:
    Engine::Gfx::BgSetup bg;                 // 0x00 - BG0
    Game::Level::GameProgress *progress;     // 0x10 - PackSaveData(gLevelState)
    const struct pause_row *rows;            // 0x14 - gPauseMenuRows
    s32 cursor;                              // 0x18 - the selected row
    s32 rowCount;                            // 0x1C - 4, or 5 in a time trial
    s32 rowSpacing;                          // 0x20 - 16
    s32 page;                                // 0x24 - the info page shown (0-4)
    s32 pageTimer;                           // 0x28 - frames to the next page
    u8 crystalCount[3];                      // 0x2C - the info pages' numbers, as text
    u8 clearGemCount[3];                     // 0x2F
    u8 gemCount[3];                          // 0x32
    u8 relicCount[3];                        // 0x35
    u8 sapphireCount[3];                     // 0x38
    u8 goldCount[3];                         // 0x3B
    u8 platinumCount[3];                     // 0x3E
    u8 percentText[5];                       // 0x41 - InitPauseMenuInfo
    u8 crystalTotal[3];                      // 0x46
    u8 gemTotal[3];                          // 0x49
    u8 relicTotal[3];                        // 0x4C
    u8 soundVolumeText[8];                   // 0x4F
    u8 musicVolumeText[9];                   // 0x57
    s32 musicVolume;                         // 0x60 - 0-20, in 5% steps
    s32 soundVolume;                         // 0x64
    s32 flashTimer;                          // 0x68
    u8 trialEarned;                          // 0x6C
    const u8 *levelName;                     // 0x70
    const u8 *levelLabel;                    // 0x74
    u8 levelNumber[4];                       // 0x78
    u8 timeText[0xC];                        // 0x7C
    Game::Entities::UiSprite *crystalIcon;   // 0x88
    Game::Entities::UiSprite *powerIcons[4]; // 0x8C
    Game::Entities::UiSprite *gemIcons[5];   // 0x9C
    Game::Entities::UiSprite *relicIcons[3]; // 0xB0
    Game::Entities::UiSprite *trialIcon;     // 0xBC
    Game::Entities::UiSprite *blinkEyes;     // 0xC0 - the eyelids over the background
    s32 blinkTimer;                          // 0xC4 - frames to the next blink
    union blend blend;                       // 0xC8 - REG_BLDCNT
    struct bldy bldy;                        // 0xCC - REG_BLDY
    union dispcnt dispcnt;                   // 0xD0 - REG_DISPCNT (gfx.h)

    PauseMenu();                                         // InitPauseMenu
    ~PauseMenu();                                        // DestroyPauseMenu
    static s32 Run();                                    // RunPauseMenu
    void InitInfo();                                     // InitPauseMenuInfo
    s32 Loop();                                          // PauseMenuLoop
    void InitCrystalsPage();                             // InitPauseCrystalsPage
    void InitPowersPage();                               // InitPausePowersPage
    void InitGemsPage();                                 // InitPauseGemsPage
    void InitRelicsPage();                               // InitPauseRelicsPage
    void InitTimeTrialPage();                            // InitPauseTimeTrialPage
    void DrawPowersPage();                               // DrawPausePowersPage
    void DrawGemsPage();                                 // DrawPauseGemsPage
    void DrawRelicsPage();                               // DrawPauseRelicsPage
    void DrawFraction(const u8 *count, const u8 *total); // DrawPauseFraction
    void VolumeDown();                                   // PauseMenuVolumeDown
    void VolumeUp();                                     // PauseMenuVolumeUp
    void CursorDown();                                   // PauseMenuCursorDown
    s32 CursorUp();                                      // PauseMenuCursorUp
    void FormatVolume(s32 volume, u8 *out);              // FormatVolumePercent
    void DrawTimeTrialPage();                            // DrawPauseTimeTrialPage
    void DrawCrystalsPage();                             // DrawPauseCrystalsPage
    void DrawPageTitle();                                // DrawPauseMenuPageTitle
    void CommitFrame();                                  // CommitPauseMenuFrame
    void Animate();                                      // AnimatePauseMenu
    void Draw();                                         // DrawPauseMenu
    void DrawRows();                                     // DrawPauseMenuRows
};

COMPILE_TIME_ASSERT(menus_hpp, sizeof(PauseMenu) == 0xD4);

/* The power dialog (ShowPowerDialog, from the four Show*Dialog wrappers
 * game_frame.cpp calls when a boss gives Crash a power): the power's name
 * and description over a scrolling sky, with its icon, faded in and out
 * through BLDY. */
class PowerDialog
{
public:
    Engine::Gfx::BgSetup bg;        // 0x00 - BG0
    const u8 *titleText;            // 0x10 - GetUiText's result
    const u8 *descText;             // 0x14
    Game::Entities::UiSprite *icon; // 0x18 - the power's icon
    u32 frame;                      // 0x1C - frame counter; BG0HOFS = frame >> 3
    union blend blend;              // 0x20 - REG_BLDCNT
    struct bldy bldy;               // 0x24 - REG_BLDY, faded 16 -> 0 -> 16 by Loop
    union dispcnt dispcnt;          // 0x28 - REG_DISPCNT (gfx.h)

    PowerDialog(const u8 *titleText, const u8 *descText, s32 type); // InitPowerDialog
    ~PowerDialog();                                                 // DestroyPowerDialog
    static void Show(s32 title, s32 desc, s32 type);                // ShowPowerDialog
    void Draw();                                                    // DrawPowerDialog
    void Animate();                                                 // AnimatePowerDialog
    void CommitFrame();                                             // CommitPowerDialogFrame
    void Loop();                                                    // PowerDialogLoop
};

COMPILE_TIME_ASSERT(menus_hpp, sizeof(PowerDialog) == 0x2C);

} // namespace Ui
} // namespace Game

#endif /* GUARD_MENUS_HPP */
