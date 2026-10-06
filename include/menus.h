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
 * The objects' own structs live in the type headers: `struct
 * pause_menu` in pause_menu.h, `struct level_menu`/`page_bg`/`zoom_bg`
 * in level_menu.h, `struct level_item` in level_select_parts.h. Those two
 * level-select headers each define their own `struct sprite`, so this
 * header only declares the tags and includes neither. The continue
 * prompt's functions at the start of src/frontend/credits.c
 * (DrawContinuePrompt..RunContinuePrompt) are here too. */

#include "core.h"
#include "vtable.h"
#include "graphics_package.h"

struct bg_package;
struct bitmap_font;
struct cursor_panel;
struct follow_child;
struct level_item;
struct level_menu;
struct menu_save;
struct page_bg;
struct pause_menu;
struct settings_icon_actor;
struct sprite;
struct twinkle;
struct zoom_bg;

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

/* The continue prompt ("Continue? Yes/No" over the Uka Uka background,
 * 0x24 bytes): InitContinuePrompt/InitContinuePromptGraphics build it,
 * ContinuePromptLoop runs it, and DrawContinuePrompt..DestroyContinuePrompt
 * (src/frontend/credits.c) draw, commit and free it. */
struct continue_prompt {
    struct bg_setup *bg1Buf; /* 0x00 - BG1 */
    struct bg_setup *bg0Buf; /* 0x04 - BG0 */
    struct bg_setup *bg2Buf; /* 0x08 - BG2 */
    u16 dispcnt;             /* 0x0c - written as one halfword to REG_DISPCNT; bytes
                              * accessed individually via ((u8 *)&dispcnt)[n] */
    u8 unused_0e[2];
    union {
        u32 word; /* 0x10 - written as one word to REG_BLDCNT/BLDALPHA */
        struct {
            u8 bldcntLo;
            u8 bldcntHi;
            u8 bldalphaLo;
            u8 bldalphaHi;
        } b;
        struct {
            u8 bldcntLo;
            u8 bldcntHi;
            u8 eva:5; /* 0x12 - ContinuePromptLoop's pulsing blend level */
            u8 evaHi:3;
            u8 bldalphaHi;
        } bits;
    } blend;
    u8 unused_14[4];
    struct bitmap_font *icons; /* 0x18 - gSmallFont */
    /* 0x1c - the selected option's blink counter (GetContinuePromptBlink) */
    s32 blinkCounter;
    s32 selection; /* 0x20 - the Yes/No cursor, 0/1 */
};

COMPILE_TIME_ASSERT(menus_h, sizeof(struct continue_prompt) == 0x24);

/* The power dialog (ShowPowerDialog, 0x2c bytes): a power's name and
 * description over a scrolling background, faded in and out through
 * BLDY. */
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

/* The level select (src/iwram/iwram_data.c). */
extern struct level_menu *gLevelSelect;
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

/* src/menus/continue_prompt_init.c */
extern struct continue_prompt *InitContinuePrompt(struct continue_prompt *self);

/* src/menus/continue_prompt.c */
extern void InitContinuePromptGraphics(struct continue_prompt *self);
extern s32 ContinuePromptLoop(struct continue_prompt *self);

/* src/frontend/credits.c */
extern void DrawContinuePrompt(struct continue_prompt *self);
extern s32 GetContinuePromptBlink(struct continue_prompt *self, s32 mode);
extern void CommitContinuePromptFrame(struct continue_prompt *self);
extern void DestroyContinuePrompt(struct continue_prompt *self, s32 mode);
extern u8 RunContinuePrompt(void);

/* src/menus/level_select.c */
extern void sub_801B85C(struct follow_child *self);
extern void ResetCameraLead(struct follow_child *self);
extern void UpdateCameraLead(struct follow_child *self);
extern void DestroyCameraLead(struct follow_child *self, s32 flags);
extern struct follow_child *CreateCameraLead(struct follow_child *self);
extern void SetCameraLeadOffset(struct follow_child *self, s32 offset);
extern s32 GetCameraLeadOffset(struct follow_child *self);
extern struct sprite *SpawnLaunchPad(u16 id, u16 x, u16 y, u16 unused);
extern void CheckLaunchPadContact(void *self);
extern void DestroyLaunchPad(struct sprite *self, s32 flags);
extern void ClearLaunchPadVulnerable(struct sprite *self);
extern struct sprite *InitLaunchPad(struct sprite *self);
extern s32 RunLevelSelect(s32 *arg);
extern struct level_menu *InitLevelSelect(struct level_menu *self, s32 arg);
extern void DestroyLevelSelect(struct level_menu *self, s32 flags);
extern void UpdateLevelSelect(struct level_menu *self);
extern void UpdateLevelSelectPageArrows(struct level_menu *self);
extern void DrawLevelSelectRecord(struct level_menu *self);
extern void DrawLevelSelectTime(struct level_menu *self, u32 time);
extern void DrawLevelSelect(struct level_menu *self);
extern void LoadLevelSelectRecord(struct level_menu *self);
extern s32 LevelSelectLoop(struct level_menu *self);
extern void SettleLevelSelectPage(struct level_menu *self);
extern void LevelSelectCursorLeft(struct level_menu *self);
extern void LevelSelectCursorRight(struct level_menu *self);

/* src/menus/level_select_pages.c */
extern void LevelSelectTurnPage(struct level_menu *self);
extern void WaitLevelSelectCursor(struct level_menu *self);
extern void LevelSelectConfirm(struct level_menu *self);
extern void LevelSelectExit(struct level_menu *self);
extern void SetNewWorldOpened(void);
extern u8 LevelSelectHasPrevWorld(struct level_menu *self);
extern u8 LevelSelectIsNextWorldOpen(struct level_menu *self);
extern void RefreshLevelSelectPage(struct level_menu *self);
extern void LevelSelectPrevWorld(struct level_menu *self);
extern void LevelSelectNextWorld(struct level_menu *self);
extern void PlaceLevelSelectEntries(struct level_menu *self);
extern void LoadLevelSelectEntries(struct level_menu *self);
extern void SetLevelSelectEntryBoxes(struct level_menu *self);
extern void CommitLevelSelectFrame(struct level_menu *self);
extern void ReloadLevelSelectPalette(struct level_menu *self);
extern s32 GetLevelSelectPageBgScroll(struct page_bg *p);
extern u8 IsLevelSelectPageBgSettled(struct page_bg *p);
extern void TurnLevelSelectPageBgBack(struct page_bg *p);
extern void TurnLevelSelectPageBgForward(struct page_bg *p);
extern void ScrollLevelSelectPageBg(struct page_bg *p);
extern u32 GetLevelSelectPageBgOffsets(struct page_bg *p);
extern void SetLevelSelectPageBgOffsets(struct page_bg *p);
extern void DestroyLevelSelectPageBg(struct page_bg *p, s32 flags);
extern struct page_bg *CreateLevelSelectPageBg(struct page_bg *self, s32 charBlock,
                                               s32 screenBlock);
extern struct zoom_bg *InitZoomBg(struct zoom_bg *self, s32 charBlock, s32 screenBlock);

/* src/menus/level_select_widgets.c */
extern void DestroyZoomBg(struct zoom_bg *self, s32 flags);
extern void UpdateZoomBg(struct zoom_bg *self);
extern void DrawZoomBg(struct zoom_bg *self);
extern void CommitZoomBg(struct zoom_bg *self);
extern u8 IsZoomBgExiting(struct zoom_bg *self);
extern u8 IsZoomBgGone(struct zoom_bg *self);
extern u8 IsZoomBgShown(struct zoom_bg *self);
extern u8 IsZoomBgWaiting(struct zoom_bg *self);
extern u8 IsZoomBgZoomingOut(struct zoom_bg *self);
extern void StartZoomBgExit(struct zoom_bg *self);
extern void ClearZoomBgPicture(struct zoom_bg *self);
extern void SetZoomBgPicture(struct zoom_bg *self, s32 image);
extern void MoveZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t);
extern void RandomizeZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t);
extern void TickZoomBgTwinkle(struct zoom_bg *self, struct twinkle *t);
extern u16 GetZoomBgControl(struct zoom_bg *self);
extern u8 IsLevelSelectEntrySelected(struct level_item *self);
extern s32 GetLevelSelectEntryLevel(struct level_item *self);
extern void AnimateLevelSelectEntry(struct level_item *self, s32 phase);
extern void SetLevelSelectEntrySelected(struct level_item *self, u8 selected);
extern void SetLevelSelectEntryLevel(struct level_item *self, s32 world, s32 index);
extern void SetLevelSelectEntryBox(struct level_item *self, s32 kind);
extern void SetLevelSelectEntryPos(struct level_item *self, s32 *pos);
extern void DrawLevelSelectEntry(void);
extern void DestroyLevelSelectEntry(struct level_item *self, s32 flags);
extern struct level_item *CreateLevelSelectEntry(struct level_item *self);
extern struct cursor_panel *CreateLevelSelectCursor(struct cursor_panel *self);
extern void UpdateLevelSelectCursor(struct cursor_panel *self);
extern void DrawLevelSelectCursor(struct cursor_panel *self);
extern void SetLevelSelectCursorMatrix(struct cursor_panel *self);
extern u8 IsLevelSelectCursorHidden(struct cursor_panel *self);
extern u8 IsLevelSelectCursorGrowing(struct cursor_panel *self);
extern void HideLevelSelectCursor(struct cursor_panel *self);
extern void ParkLevelSelectCursor(struct cursor_panel *self);
extern void GlideLevelSelectCursor(struct cursor_panel *self);
extern u8 HasLevelSelectCursorArrived(struct cursor_panel *self);
extern void MoveLevelSelectCursor(struct cursor_panel *self, s32 x, s32 y);
extern void MoveLevelSelectCursorTo(struct cursor_panel *self, struct xy_pair *pos);
extern void SetLevelSelectCursorPos(struct cursor_panel *self, s32 x, s32 y);
extern void ResetLevelSelectCursorIdleTimer(struct cursor_panel *self);
extern void DestroyLevelSelectCursor(struct cursor_panel *self, s32 flags);

/* src/menus/pause_menu.c */
extern s32 RunPauseMenu(void);
extern struct pause_menu *InitPauseMenu(struct pause_menu *self);
extern void DestroyPauseMenu(struct pause_menu *self, u32 flags);

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

/* src/menus/pause_menu_pages_init.c */
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

/* src/menus/power_dialog.c */
extern void ShowPowerDialog(s32 label1, s32 label2, s32 type);
extern struct power_dialog *InitPowerDialog(struct power_dialog *self, s32 label1, s32 label2,
                                            s32 type);

/* src/menus/power_dialog_draw.c */
extern void DrawPowerDialog(struct power_dialog *arg0);
extern void AnimatePowerDialog(struct power_dialog *arg0);
extern void CommitPowerDialogFrame(struct power_dialog *arg0);
extern void DestroyPowerDialog(struct power_dialog *arg0, u32 arg1);
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
