#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__
/* (blank line above left intentionally to keep this header's
 * COMPILE_TIME_ASSERT off whatever line number bitmap_font.h's own
 * assert happens to sit on - COMPILE_TIME_ASSERT's generated symbol
 * name is line-number-based, not per-file, so two same-numbered
 * asserts collide with a "redefinition" error when both headers end
 * up in the same translation unit, as they do here.) */

/* A small `struct actor`-derived on-screen icon: the first 0x1c bytes
 * are a plain `struct actor` (see actor.h), then a second keyframe-
 * table pointer at +0x20 and a frame index at +0x2d - both already
 * established by the already-matched GetSpriteAnimPaletteSlot/SpriteHitboxOverlaps
 * (src/objects/sprite_obj.c), which read this exact same object
 * through raw offsets. +0x29's low nibble and +0x3c are new fields this
 * chunk's functions write but don't otherwise interpret. Allocated with
 * `OperatorNew(0x40)` - bigger than plain `struct actor` (0x1c), so it
 * has more trailing fields this chunk's functions never touch. */
struct settings_icon_actor {
    struct actor base;    /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **field_20;        /* 0x20 - keyframe-table pointer, see GetSpriteAnimPaletteSlot */
    u8 unused_24[0x29 - 0x24];
    u8 field_29;               /* 0x29 - low nibble set from GetSpriteAnimPaletteSlot's result */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex;                /* 0x2d - current keyframe index, see GetSpriteAnimPaletteSlot */
    u8 unused_2e[0x38 - 0x2e];
    u8 field_38;                    /* 0x38 - AnimatePauseMenu: "currently highlighted/armed" flag */
    u8 unused_39[0x3c - 0x39];
    u16 field_3c;                   /* 0x3c - InitPauseGemsPage/InitPauseRelicsPage only, set to 0x80 */
};

/* A fixed {x, y} screen-position pair, as consumed by SetEntityPixelPos. */
struct icon_pos {
    s32 x;
    s32 y;
};

/* The pause menu (built by `RunPauseMenu`/`InitPauseMenu`, GitHub issue
 * #7). Its rows (gPauseMenuRows) are "resume", "music", "sound", "warp
 * room" and "restart trial"; the music and sound rows (types 4 and 5)
 * edit `musicVolume`/`soundVolume` in 5% steps, and PauseMenuLoop
 * returns the confirmed row's type. On the right it cycles five info
 * pages every 180 frames (`field_24`, AnimatePauseMenu): crystals,
 * powers, gems, relics and the level's time trial. Formerly
 * `struct pause_screen_results`. This
 * reconciles three previously-separate partial views of the exact same
 * 0xd4-byte allocation (confirmed by the real call chain: RunPauseMenu
 * allocates it with `OperatorNew(0xd4)`, passes it to InitPauseMenu,
 * which passes the same pointer to InitPauseMenuInfo, which passes it to
 * the five Init*Page functions - and separately InitPauseMenu also passes
 * it to CommitPauseMenuFrame/PauseMenuLoop, which is the pause_menu_pages_draw.c/
 * pause_menu_widgets.c fields' own consumer):
 * - `struct pause_menu` (src/menus/pause_menu_pages_init.c,
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
    u8 unused_00[0x10];
    void *field_10;             /* 0x10 - a row-stats handle, passed to CountClearGems/CountGems/CountRelics/etc and read via gLevelState's per-level index table in InitPauseTimeTrialPage */
    void *field_14;               /* 0x14 - base of an 8-byte-stride per-row record array (gPauseMenuRows), see DrawPauseMenuRows/PauseMenuVolumeDown */
    s32 field_18;                   /* 0x18 - currently selected/highlighted row index */
    s32 field_1c;                     /* 0x1c - row count (4 or 5, from gLevelState+0x8c) */
    s32 field_20;                       /* 0x20 - per-row Y spacing (16) */
    s32 field_24;                         /* 0x24 - AnimatePauseMenu's jump-table state (0-4) */
    s32 field_28;                           /* 0x28 - AnimatePauseMenu's countdown (init 0xb4) */
    u8 buf2c[3];    /* InitPauseCrystalsPage */
    u8 buf2f[3];    /* InitPauseGemsPage */
    u8 buf32[3];    /* InitPauseGemsPage */
    u8 buf35[3];    /* InitPauseRelicsPage */
    u8 buf38[3];    /* InitPauseRelicsPage */
    u8 buf3b[3];    /* InitPauseRelicsPage */
    u8 buf3e[3];    /* InitPauseRelicsPage */
    u8 buf41[5];     /* InitPauseMenuInfo - itoa(value) + '%' + NUL */
    u8 buf46[3];    /* InitPauseCrystalsPage */
    u8 buf49[3];    /* InitPauseGemsPage */
    u8 buf4c[3];    /* InitPauseRelicsPage */
    u8 soundVolumeText[8];         /* " <NNN%>" scratch string, see PauseMenuVolumeDown/PauseMenuVolumeUp */
    u8 musicVolumeText[9];           /* same shape as soundVolumeText */
    s32 musicVolume;             /* 0x60 - 0-20, in 5% steps (the "music" row) */
    s32 soundVolume;               /* 0x64 - 0-20, in 5% steps (the "sound" row) */
    s32 field_68;                 /* 0x68 - highlight-flash countdown, PauseMenuLoop */
    u8 field_6c;                    /* 0x6c - "earned" flag, InitPauseTimeTrialPage */
    u8 unused_6d[0x70 - 0x6d];
    void *field_70;                   /* 0x70 - current level's name label text ptr, InitPauseMenuInfo/DrawPauseMenu */
    void *field_74;                     /* 0x74 - secondary label text ptr (or NULL past level 0x13), InitPauseMenuInfo/DrawPauseMenu */
    u8 buf78[4];                       /* small text scratch, DrawPauseGemsPage/DrawPauseRelicsPage */
    u8 timeBuf[0xc];         /* 0x7c - InitPauseTimeTrialPage, FormatCentiseconds dest */
    struct settings_icon_actor *field_88;      /* InitPauseCrystalsPage */
    struct settings_icon_actor *icons8c[4];      /* InitPausePowersPage */
    struct settings_icon_actor *icons9c[5];        /* InitPauseGemsPage */
    struct settings_icon_actor *iconsB0[3];          /* InitPauseRelicsPage */
    struct settings_icon_actor *field_bc;              /* InitPauseTimeTrialPage */
    struct settings_icon_actor *field_c0;                /* 0xc0 - currently-highlighted row's icon, AnimatePauseMenu/PauseMenuLoop */
    s32 field_c4;                                          /* 0xc4 - field_c0's blink/reveal countdown */
    u32 field_c8;                                            /* 0xc8 - REG_BLDCNT value, applied by CommitPauseMenuFrame */
    u8 field_cc;                                              /* 0xcc - REG_BLDY value (low 5 bits); PauseMenuLoop animates this as a fade level */
    u8 unused_cd[3];
    u16 field_d0;                                              /* 0xd0 - REG_DISPCNT value, applied by CommitPauseMenuFrame */
    u8 unused_d2[2];
};
COMPILE_TIME_ASSERT(sizeof(struct pause_menu) == 0xd4);

extern s32 GetSpriteAnimPaletteSlot(struct actor *part);

/* Sets `field_29`'s low nibble from GetSpriteAnimPaletteSlot's result, keeping the
 * high nibble - the recurring last step of every icon constructor that
 * touches a `struct settings_icon_actor` (see src/menus/
 * pause_menu_pages_init.c and src/menus/power_dialog.c). Written with
 * explicit register pins (matching the SUB_8006600_* macros in
 * src/menus/power_dialog_draw.c) because gcc's constant-propagation
 * otherwise folds the ROM's two-instruction "movs r1,#0x10 / rsbs
 * r1,r1,#0" -0x10 load into a single `sub` relative to the just-used
 * 0xf mask, which the ROM never does. */
#define UPDATE_ICON_FRAME_NIBBLE(iconExpr) \
    do { \
        register s32 _ret asm("r0") = GetSpriteAnimPaletteSlot(&(iconExpr)->base); \
        register u8 *_addr asm("r2") = &(iconExpr)->field_29; \
        register s32 _mask asm("r1"); \
        register u8 _byte asm("r3"); \
        _mask = 0xf; \
        _ret &= _mask; \
        asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r" (_mask)); \
        _byte = *_addr; \
        _mask &= _byte; \
        _mask |= _ret; \
        *_addr = _mask; \
    } while (0)

#endif /* __PAUSE_MENU_H__ */
