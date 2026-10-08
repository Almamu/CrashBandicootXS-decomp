#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include "menus.h"
#include "graphics_package.h"
#include "objects.h" /* GetSpriteAnimPaletteSlot */

/* A small `struct actor`-derived on-screen icon: the first 0x1c bytes
 * are a plain `struct actor` (see actor.h), then a second keyframe-
 * table pointer at +0x20 and a frame index at +0x2d - both already
 * established by the already-matched GetSpriteAnimPaletteSlot/SpriteHitboxOverlaps
 * (src/objects/sprite_obj.cpp), which read this exact same object
 * through raw offsets. +0x29's low nibble and +0x3c are new fields this
 * chunk's functions write but don't otherwise interpret. Allocated with
 * `OperatorNew(0x40)` - bigger than plain `struct actor` (0x1c), so it
 * has more trailing fields this chunk's functions never touch. */
struct settings_icon_actor {
    struct actor base; /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **anim; /* 0x20 - the sprite bank (sprite_bank.h), see GetSpriteAnimPaletteSlot */
    u8 unused_24[0x29 - 0x24];
    u8 palette; /* 0x29 - low nibble: the OBJ palette slot (GetSpriteAnimPaletteSlot) */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex; /* 0x2d - current keyframe index, see GetSpriteAnimPaletteSlot */
    u8 unused_2e[0x38 - 0x2e];
    u8 animDone; /* 0x38 - the animation reached its end (SetSpriteAnimDone) */
    u8 unused_39[0x3c - 0x39];
    /* 0x3c - Q8 affine scale, 0 = not affine (DrawAffineSpritePieces);
     * InitPauseGemsPage/InitPauseRelicsPage draw their icons at half size (0x80) */
    u16 scale;
};

/* The pause menu (built by `RunPauseMenu`/`InitPauseMenu`, GitHub issue
 * #7). Its rows (gPauseMenuRows) are "resume", "music", "sound", "warp
 * room" and "restart trial"; the music and sound rows (types 4 and 5)
 * edit `musicVolume`/`soundVolume` in 5% steps, and PauseMenuLoop
 * returns the confirmed row's type. On the right it cycles five info
 * pages every 180 frames (`page`, AnimatePauseMenu): crystals,
 * powers, gems, relics and the level's time trial. This is the C view
 * of class PauseMenu (menus.hpp, #664 part 10d), for the pause menu's C
 * files (pause_menu_draw.c, _gems.c, _info.c, _loop.c, _pages_draw.c,
 * _powers.c and _widgets.c). Formerly `struct pause_screen_results`. This
 * reconciles three previously-separate partial views of the exact same
 * 0xd4-byte allocation (confirmed by the real call chain: RunPauseMenu
 * allocates it with `OperatorNew(0xd4)`, passes it to InitPauseMenu,
 * which passes the same pointer to InitPauseMenuInfo, which passes it to
 * the five Init*Page functions - and separately InitPauseMenu also passes
 * it to CommitPauseMenuFrame/PauseMenuLoop, which is the pause_menu_pages_draw.c/
 * pause_menu_widgets.c fields' own consumer):
 * - `struct pause_menu` (src/menus/pause_menu_pages_init.cpp,
 *   originally local to that file) - the icon-widget fields.
 * - `struct pause_screen_row_counts` (src/menus/pause_menu_widgets.c) -
 *   the per-row edit-count fields.
 * - `struct pause_screen_apply_state` (src/menus/pause_menu_pages_draw.c) -
 *   the BLDCNT/BLDY/DISPCNT apply-step fields.
 * All three agreed on their own fields' offsets with zero overlap once
 * merged - strong confirmation this is genuinely one object, not a
 * coincidence. Distinct from (and NOT reconciled with) `struct
 * save_menu` (include/save_menu.h), a smaller,
 * separately-allocated settings-sync/spinner object that happens to
 * share some byte offsets by coincidence - see that header's own
 * comment. */
struct pause_menu {
    struct bg_setup bg; /* 0x00 - BG0 (InitBgSetup) */
    /* 0x10 - the packed progress (PackSaveData(gLevelState)): passed to
     * CountClearGems/CountGems/CountRelics/etc, its per-level times read by
     * InitPauseTimeTrialPage */
    struct menu_save *progress;
    /* 0x14 - the rows (gPauseMenuRows), see DrawPauseMenuRows/PauseMenuVolumeDown */
    const struct pause_row *rows;
    s32 cursor;     /* 0x18 - currently selected/highlighted row index */
    s32 rowCount;   /* 0x1c - row count (4 or 5, from gLevelState+0x8c) */
    s32 rowSpacing; /* 0x20 - per-row Y spacing (16) */
    /* 0x24 - the info page shown (0-4: crystals, powers, gems, relics, time trial) */
    s32 page;
    s32 pageTimer;         /* 0x28 - frames to the next page (init 0xb4) */
    u8 buf2c[3];           /* InitPauseCrystalsPage */
    u8 buf2f[3];           /* InitPauseGemsPage */
    u8 buf32[3];           /* InitPauseGemsPage */
    u8 buf35[3];           /* InitPauseRelicsPage */
    u8 buf38[3];           /* InitPauseRelicsPage */
    u8 buf3b[3];           /* InitPauseRelicsPage */
    u8 buf3e[3];           /* InitPauseRelicsPage */
    u8 buf41[5];           /* InitPauseMenuInfo - itoa(value) + '%' + NUL */
    u8 buf46[3];           /* InitPauseCrystalsPage */
    u8 buf49[3];           /* InitPauseGemsPage */
    u8 buf4c[3];           /* InitPauseRelicsPage */
    u8 soundVolumeText[8]; /* " <NNN%>" scratch string, see PauseMenuVolumeDown/PauseMenuVolumeUp */
    u8 musicVolumeText[9]; /* same shape as soundVolumeText */
    s32 musicVolume;       /* 0x60 - 0-20, in 5% steps (the "music" row) */
    s32 soundVolume;       /* 0x64 - 0-20, in 5% steps (the "sound" row) */
    s32 flashTimer;        /* 0x68 - highlight-flash countdown, PauseMenuLoop */
    u8 trialEarned;        /* 0x6c - "earned" flag, InitPauseTimeTrialPage */
    u8 unused_6d[0x70 - 0x6d];
    /* 0x70 - current level's name label text ptr, InitPauseMenuInfo/DrawPauseMenu */
    void *levelName;
    /* 0x74 - secondary label text ptr (or NULL past level 0x13), InitPauseMenuInfo/DrawPauseMenu */
    void *levelLabel;
    /* small text scratch, DrawPauseGemsPage/DrawPauseRelicsPage */
    u8 buf78[4];
    /* 0x7c - InitPauseTimeTrialPage, FormatCentiseconds dest */
    u8 timeBuf[0xc];
    struct settings_icon_actor *crystalIcon; /* InitPauseCrystalsPage */
    struct settings_icon_actor *icons8c[4];  /* InitPausePowersPage */
    struct settings_icon_actor *icons9c[5];  /* InitPauseGemsPage */
    struct settings_icon_actor *iconsB0[3];  /* InitPauseRelicsPage */
    struct settings_icon_actor *trialIcon;   /* InitPauseTimeTrialPage */
    /* 0xc0 - eyelids (sprite bank 46) blinking over the background,
     * AnimatePauseMenu/DrawPauseMenu */
    struct settings_icon_actor *blinkEyes;
    s32 blinkTimer; /* 0xc4 - frames to the next blink; drawn while 0 */
    u32 bldcnt;     /* 0xc8 - REG_BLDCNT value, applied by CommitPauseMenuFrame */
    /* 0xcc - REG_BLDY value (low 5 bits); PauseMenuLoop animates this as a fade level */
    u8 bldy;
    u8 unused_cd[3];
    u16 dispcnt; /* 0xd0 - REG_DISPCNT value, applied by CommitPauseMenuFrame */
    u8 unused_d2[2];
};
COMPILE_TIME_ASSERT(pause_menu_h, sizeof(struct pause_menu) == 0xd4);

#endif /* __PAUSE_MENU_H__ */
