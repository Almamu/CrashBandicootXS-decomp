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
 * continue prompt in frontend.hpp. The prototypes below are the C names
 * (cxx_symbols.txt) of the methods that a C file or a vtable still uses,
 * and the free functions. The C files left (the pause menu's drawing and
 * input, power_dialog_loop.c) see their objects through C structs:
 * `struct pause_menu` in pause_menu.h, `struct power_dialog` below;
 * `struct sprite` and the save block are in level_menu.h, which this
 * header doesn't include (it only declares the tags). */

#include "core.h"
#include "vtable.h"
#include "graphics_package.h"
#include "constants/levels.h"

struct bg_package;
struct follow_child;
struct level_item;
struct level_menu;
struct pause_menu;
struct settings_icon_actor;
struct sprite;

/* A screen position, as the level-select tables store them. */
struct xy_pair {
    s32 x;
    s32 y;
};

/* A fixed {x, y} screen-position pair, as consumed by SetEntityPixelPos
 * (the pause menu's icon positions). */
struct icon_pos {
    s32 x;
    s32 y;
};

/* One row of the pause menu (gPauseMenuRows, `pause_menu.field_14`): a
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

/* The power dialog (ShowPowerDialog, 0x2c bytes): a power's name and
 * description over a scrolling background, faded in and out through
 * BLDY. The C view of class PowerDialog (menus.hpp), for
 * power_dialog_loop.c. */
struct power_dialog {
    struct bg_setup bg;               /* 0x00 - BG0 (InitBgSetup) */
    s32 titleText;                    /* 0x10 - the title's text */
    void *descText;                   /* 0x14 - the description's text */
    struct settings_icon_actor *icon; /* 0x18 - the power's icon */
    u32 frame;                        /* 0x1c - frame counter, BG0HOFS = frame >> 3 */
    u32 bldcnt;                       /* 0x20 - REG_BLDCNT */
    union {
        u8 raw;
        struct {
            u8 level:5; /* REG_BLDY, faded 0x10 -> 0 -> 0x10 by PowerDialogLoop */
            u8 rest:3;
        } __attribute__((packed)) bits;
    } __attribute__((packed)) bldy;
    u8 unused_25[3];
    /* REG_DISPCNT: cleared as a halfword, then bit 6 of its low byte
     * set (PowerDialogLoop). */
    union {
        u16 all;
        struct {
            u8 flags;
            u8 hi;
        } b;
    } dispcnt;
};

COMPILE_TIME_ASSERT(menus_h, sizeof(struct power_dialog) == 0x2c);

/* The method tables (src/data/entity_vtables_7e3bec.c). */
extern const struct vtable_slot gCameraLeadVtable[15];
extern const struct vtable_slot gLaunchPadVtable[15];
extern const struct vtable_slot gLevelSelectEntryVtable[6];

/* The level select (src/iwram/iwram_data.c): the screen while
 * RunLevelSelect runs. The C++ files see it as its class, LevelSelect
 * (level_select.hpp); for C it is an opaque `struct level_menu`. */
#ifdef __cplusplus
extern class LevelSelect *gLevelSelect;
#else
extern struct level_menu *gLevelSelect;
#endif
extern u8 gNewWorldOpened;

/* The level-select tables (src/data/map_tables_16c498.c,
 * map_tables_16c5f0.c, image_table_16c5a0.c, bg_package_16c58c.c,
 * data/data.s). */
extern const struct xy_pair gLevelSelectWorldPos;
extern const struct xy_pair gLevelSelectCrashIconPos;
extern const struct xy_pair gLevelSelectCrystalPos;
extern const struct xy_pair gLevelSelectGemPos;
extern const struct xy_pair gLevelSelectTrialIconPos;
extern const struct xy_pair gLevelSelectTimePos;
extern const struct xy_pair gLevelSelectNextWorldArrowPos;
extern const struct xy_pair gLevelSelectPrevWorldArrowPos;
extern const struct xy_pair gLevelSelectEntryPositions[6];
extern const struct xy_pair gLevelSelectEntryPositionsAllCleared[6];
extern const u32 gLevelSelectWorldEntryBoxAnims[4];
extern const u32 gLevelSelectWorldAnims[4];
extern const u32 gLevelSelectRankAnims[5];
extern const u16 gLevelSelectPalette[16];
extern const struct xy_pair gZoomBgSlotOffsets[4];
extern const u32 gLevelSelectEntryBoxAnims[5];
extern const u32 gLevelSelectEntryWorldAnims[4];
extern const u32 gLevelSelectCursorAnims[4];
extern const struct image_pair gLevelSelectPictures[10];
extern const struct bg_package gLevelSelectPageBg;
extern const u8 gLevelSelectCursorZoomTiles[];

/* The pause menu tables (src/data/menu_tables_16b138.c,
 * pause_rows_16b298.c, bg_package_16b284.c). */
extern const s32 gPauseMenuPageTitles[5];
extern const struct icon_pos gPauseCrystalIconPos;
extern const struct icon_pos gPausePowerIconPos[4];
extern const s32 gPausePowerIconFrames[4];
extern const struct icon_pos gPauseGemIconPos[5];
extern const s32 gPauseGemIconFrames[5];
extern const struct icon_pos gPauseRelicIconPos[3];
extern const s32 gPauseRelicIconFrames[3];
extern const struct icon_pos gPauseTimeTrialIconPos;
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

/* src/frontend/credits.cpp (C++, frontend.hpp: ContinuePrompt's methods) */
extern u8 RunContinuePrompt(void);

/* src/menus/level_select.cpp */
extern void UpdateCameraLead(struct follow_child *self);
extern void DestroyCameraLead(struct follow_child *self, s32 flags);
extern struct sprite *SpawnLaunchPad(u16 id, u16 x, u16 y, u16 unused);
extern void CheckLaunchPadContact(void *self);
extern void DestroyLaunchPad(struct sprite *self, s32 flags);
extern s32 RunLevelSelect(s32 *arg);

/* src/menus/level_select_pages.cpp */
extern void SetNewWorldOpened(void);

/* src/menus/level_select_widgets.cpp */
extern void AnimateLevelSelectEntry(struct level_item *self, s32 phase);
extern void SetLevelSelectEntryLevel(struct level_item *self, s32 world, s32 index);
extern void SetLevelSelectEntryPos(struct level_item *self, s32 *pos);
extern void DrawLevelSelectEntry(void);
extern void DestroyLevelSelectEntry(struct level_item *self, s32 flags);

/* src/menus/pause_menu.cpp (C++, menus.hpp: PauseMenu::Run) */
extern s32 RunPauseMenu(void);

/* src/menus/pause_menu_draw.c */
extern void AnimatePauseMenu(struct pause_menu *self);
extern void DrawPauseMenu(struct pause_menu *self);
extern void DrawPauseMenuRows(struct pause_menu *self);

/* src/menus/pause_menu_gems.c */
extern void DrawPauseGemsPage(struct pause_menu *self);
extern void DrawPauseRelicsPage(struct pause_menu *self);

/* src/menus/pause_menu_info.c */
extern void InitPauseMenuInfo(struct pause_menu *self);

/* src/menus/pause_menu_loop.c */
extern s32 PauseMenuLoop(struct pause_menu *self);

/* src/menus/pause_menu_pages_draw.c */
extern void DrawPauseTimeTrialPage(struct pause_menu *self);
extern void DrawPauseCrystalsPage(struct pause_menu *self);
extern void DrawPauseMenuPageTitle(struct pause_menu *self);
extern void CommitPauseMenuFrame(struct pause_menu *self);

/* src/menus/pause_menu_pages_init.cpp (C++, menus.hpp: PauseMenu's methods) */
extern void InitPauseCrystalsPage(struct pause_menu *self);
extern void InitPausePowersPage(struct pause_menu *self);
extern void InitPauseGemsPage(struct pause_menu *self);
extern void InitPauseRelicsPage(struct pause_menu *self);
extern void InitPauseTimeTrialPage(struct pause_menu *self);

/* src/menus/pause_menu_powers.c */
extern void DrawPausePowersPage(struct pause_menu *self);

/* src/menus/pause_menu_widgets.c */
extern void DrawPauseFraction(struct pause_menu *self, void *label1, void *label2);
extern void PauseMenuVolumeDown(struct pause_menu *self);
extern void PauseMenuVolumeUp(struct pause_menu *self);
extern void PauseMenuCursorDown(struct pause_menu *self);
extern s32 PauseMenuCursorUp(struct pause_menu *self);
extern s32 FormatDecimal(s32 value, u8 *dest);
extern void FormatVolumePercent(s32 arg0, s32 arg1, u8 *out);

/* src/menus/power_dialog_draw.cpp (C++, menus.hpp: PowerDialog's methods and
 * the free functions) */
extern void DrawPowerDialog(struct power_dialog *arg0);
extern void AnimatePowerDialog(struct power_dialog *arg0);
extern void CommitPowerDialogFrame(struct power_dialog *arg0);
extern void ShowTurboRunDialog(void);
extern void ShowTornadoSpinDialog(void);
extern void ShowDoubleJumpDialog(void);
extern void ShowSuperBodySlamDialog(void);
extern s32 GetProgressLives(void *arg0);
extern s32 CountPlatinumRelics(void *arg0);
extern s32 CountGoldRelics(void *arg0);
extern s32 CountSapphireRelics(void *arg0);
extern s32 CountRelics(void *arg0);
extern s32 CountGems(void *arg0);
extern s32 CountClearGems(void *arg0);
extern s32 CountCrystals(void *arg0);

/* src/menus/power_dialog_loop.c */
extern void PowerDialogLoop(struct power_dialog *self);

#endif /* GUARD_MENUS_H */
