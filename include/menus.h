#ifndef GUARD_MENUS_H
#define GUARD_MENUS_H

/* The menus subsystem (src/menus/): the continue prompt, the level
 * select (with its camera lead and launch pad), the pause menu and the
 * power dialog.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The menus are C++ classes (#664): the level select's in
 * level_select.hpp, the pause menu and the power dialog in menus.hpp, the
 * continue prompt in frontend.hpp, with no C views. The prototypes below
 * are the C names (cxx_symbols.txt) of the methods that a vtable in
 * src/data/ still uses, and the free functions. The save block is
 * game_progress.hpp's class GameProgress. */

#include "core.h"
#include "aabb.h"
#include "vtable.h"
#include "graphics_package.h"
#include "constants/levels.h"

struct bg_package;

/* One row of the pause menu (gPauseMenuRows, PauseMenu::rows): a
 * GetUiText label id, then the row's type (4/5 are the music/sound
 * volume rows; PauseMenuLoop returns the confirmed row's type). */
struct pause_row {
    s32 labelId;
    s32 type;
};

/* One level-select picture (gLevelSelectPictures): the tagged-asset
 * palette and tiles UpdateZoomBg loads with LoadTaggedAsset. */
struct image_pair {
    const u8 *palette;
    const u8 *tiles;
};

/* The level select (src/iwram/iwram_data.cpp): the screen while
 * RunLevelSelect runs, a LevelSelect (level_select.hpp). No C file uses
 * it. */
#ifdef __cplusplus
extern class LevelSelect *gLevelSelect;
#endif
extern u8 gNewWorldOpened;

/* The level-select tables (src/data/map_tables_16c498.c,
 * map_tables_16c5f0.c, image_table_16c5a0.c, bg_package_16c58c.c,
 * data/data.s). */
extern const struct vec2 gLevelSelectWorldPos;
extern const struct vec2 gLevelSelectCrashIconPos;
extern const struct vec2 gLevelSelectCrystalPos;
extern const struct vec2 gLevelSelectGemPos;
extern const struct vec2 gLevelSelectTrialIconPos;
extern const struct vec2 gLevelSelectTimePos;
extern const struct vec2 gLevelSelectNextWorldArrowPos;
extern const struct vec2 gLevelSelectPrevWorldArrowPos;
extern const struct vec2 gLevelSelectEntryPositions[6];
extern const struct vec2 gLevelSelectEntryPositionsAllCleared[6];
extern const u32 gLevelSelectWorldEntryBoxAnims[4];
extern const u32 gLevelSelectWorldAnims[4];
extern const u32 gLevelSelectRankAnims[5];
extern const u16 gLevelSelectPalette[16];
extern const struct vec2 gZoomBgSlotOffsets[4];
extern const u32 gLevelSelectEntryBoxAnims[5];
extern const u32 gLevelSelectEntryWorldAnims[4];
extern const u32 gLevelSelectCursorAnims[4];
extern const struct image_pair gLevelSelectPictures[10];
extern const struct bg_package gLevelSelectPageBg;
extern const u8 gLevelSelectCursorZoomTiles[];

/* The pause menu tables (src/data/menu_tables_16b138.c,
 * pause_rows_16b298.c, bg_package_16b284.c). */
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

/* The continue prompt's graphics (src/data/bg_package_17c594.c,
 * hud_palettes_17c510.c). */
extern const struct bg_package gContinuePromptSmokeBg;
extern const struct bg_package gContinuePromptGlowBg;
extern const struct bg_package gContinuePromptUkaUkaBg;
extern const u8 gContinuePromptCursorText[];
extern const u16 gContinuePromptPalette0[16];
extern const u16 gContinuePromptPalette1[16];
extern const u16 gContinuePromptPalette2[16];
extern const u16 gContinuePromptPalette3[17];

/* src/menus/continue_prompt.cpp (C++, frontend.hpp: ContinuePrompt's methods) */
extern u8 RunContinuePrompt(void);

/* src/menus/level_select.cpp */
extern s32 RunLevelSelect(s32 *arg);

/* src/menus/level_select_pages.cpp */
extern void SetNewWorldOpened(void);

/* src/menus/pause_menu.cpp (C++, menus.hpp: PauseMenu::Run) */
extern s32 RunPauseMenu(void);

/* src/menus/pause_menu_widgets.cpp (C linkage) */
extern s32 FormatDecimal(s32 value, u8 *dest);

/* src/menus/power_dialog_draw.cpp (C++, menus.hpp: PowerDialog's methods and
 * the free functions) */
extern void ShowTurboRunDialog(void);
extern void ShowTornadoSpinDialog(void);
extern void ShowDoubleJumpDialog(void);
extern void ShowSuperBodySlamDialog(void);

#endif /* GUARD_MENUS_H */
