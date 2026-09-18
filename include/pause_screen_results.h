#ifndef __PAUSE_SCREEN_RESULTS_H__
#define __PAUSE_SCREEN_RESULTS_H__
/* (blank line above left intentionally to keep this header's
 * COMPILE_TIME_ASSERT off whatever line number icon_manager.h's own
 * assert happens to sit on - COMPILE_TIME_ASSERT's generated symbol
 * name is line-number-based, not per-file, so two same-numbered
 * asserts collide with a "redefinition" error when both headers end
 * up in the same translation unit, as they do here.) */

/* A small `struct actor`-derived on-screen icon: the first 0x1c bytes
 * are a plain `struct actor` (see actor.h), then a second keyframe-
 * table pointer at +0x20 and a frame index at +0x2d - both already
 * established by the already-matched sub_800815C/sub_80080C0
 * (src/graphics/actor_part4.c), which read this exact same object
 * through raw offsets. +0x29's low nibble and +0x3c are new fields this
 * chunk's functions write but don't otherwise interpret. Allocated with
 * `sub_8026EDC(0x40)` - bigger than plain `struct actor` (0x1c), so it
 * has more trailing fields this chunk's functions never touch. */
struct settings_icon_actor {
    struct actor base;    /* 0x00-0x1b */
    u8 unused_1c[0x20 - 0x1c];
    void **field_20;        /* 0x20 - keyframe-table pointer, see sub_800815C */
    u8 unused_24[0x29 - 0x24];
    u8 field_29;               /* 0x29 - low nibble set from sub_800815C's result */
    u8 unused_2a[0x2d - 0x2a];
    u8 frameIndex;                /* 0x2d - current keyframe index, see sub_800815C */
    u8 unused_2e[0x38 - 0x2e];
    u8 field_38;                    /* 0x38 - sub_8005304: "currently highlighted/armed" flag */
    u8 unused_39[0x3c - 0x39];
    u16 field_3c;                   /* 0x3c - sub_8005B80/sub_8005C58 only, set to 0x80 */
};

/* A fixed {x, y} screen-position pair, as consumed by sub_800737C. */
struct icon_pos {
    s32 x;
    s32 y;
};

/* The composite pause/options screen's top-level object (built by
 * `sub_8004D74`/`sub_8004EC0`, GitHub issue #7 - see docs/rom_map.md's
 * "overlay_ui" section, "one composite pause/options screen"). This
 * reconciles three previously-separate partial views of the exact same
 * 0xd4-byte allocation (confirmed by the real call chain: sub_8004D74
 * allocates it with `sub_8026EDC(0xd4)`, passes it to sub_8004EC0,
 * which passes the same pointer to sub_800599C, which passes it to
 * sub_8005A78/AE8/B80/C58/D44 - and separately sub_8004EC0 also passes
 * it to sub_8006250/sub_8005100, which is the settings_menu12.c/
 * settings_menu7.c fields' own consumer):
 * - `struct pause_screen_results` (src/graphics/settings_menu6.c,
 *   originally local to that file) - the icon-widget fields.
 * - `struct pause_screen_row_counts` (src/graphics/settings_menu7.c) -
 *   the per-row edit-count fields.
 * - `struct pause_screen_apply_state` (src/graphics/settings_menu12.c) -
 *   the BLDCNT/BLDY/DISPCNT apply-step fields.
 * All three agreed on their own fields' offsets with zero overlap once
 * merged - strong confirmation this is genuinely one object, not a
 * coincidence. Distinct from (and NOT reconciled with) `struct
 * pause_options_screen` (include/pause_options_screen.h), a smaller,
 * separately-allocated settings-sync/spinner object that happens to
 * share some byte offsets by coincidence - see that header's own
 * comment. */
struct pause_screen_results {
    u8 unused_00[0x10];
    void *field_10;             /* 0x10 - a row-stats handle, passed to sub_8006920/64/68A8/etc and read via gUnknown_030012C0's per-level index table in sub_8005D44 */
    void *field_14;               /* 0x14 - base of an 8-byte-stride per-row record array (gStaticData_0816B298), see sub_800556C/sub_8005EF4 */
    s32 field_18;                   /* 0x18 - currently selected/highlighted row index */
    s32 field_1c;                     /* 0x1c - row count (4 or 5, from gUnknown_030012C0+0x8c) */
    s32 field_20;                       /* 0x20 - per-row Y spacing (16) */
    s32 field_24;                         /* 0x24 - sub_8005304's jump-table state (0-4) */
    s32 field_28;                           /* 0x28 - sub_8005304's countdown (init 0xb4) */
    u8 buf2c[3];    /* sub_8005A78 */
    u8 buf2f[3];    /* sub_8005B80 */
    u8 buf32[3];    /* sub_8005B80 */
    u8 buf35[3];    /* sub_8005C58 */
    u8 buf38[3];    /* sub_8005C58 */
    u8 buf3b[3];    /* sub_8005C58 */
    u8 buf3e[3];    /* sub_8005C58 */
    u8 buf41[5];     /* sub_800599C - itoa(value) + '%' + NUL */
    u8 buf46[3];    /* sub_8005A78 */
    u8 buf49[3];    /* sub_8005B80 */
    u8 buf4c[3];    /* sub_8005C58 */
    u8 buf4f[8];         /* " <NNN%>" scratch string, see sub_8005EF4/FBC */
    u8 buf57[9];           /* same shape as buf4f */
    s32 field_60;             /* 0x60 - a 0-0x13 item count */
    s32 field_64;               /* 0x64 - a 0-0x13 item count */
    s32 field_68;                 /* 0x68 - highlight-flash countdown, sub_8005100 */
    u8 field_6c;                    /* 0x6c - "earned" flag, sub_8005D44 */
    u8 unused_6d[0x70 - 0x6d];
    void *field_70;                   /* 0x70 - current level's name label text ptr, sub_800599C/sub_80053F4 */
    void *field_74;                     /* 0x74 - secondary label text ptr (or NULL past level 0x13), sub_800599C/sub_80053F4 */
    u8 buf78[4];                       /* small text scratch, sub_80057E0/sub_80058C0 */
    u8 timeBuf[0xc];         /* 0x7c - sub_8005D44, FormatCentiseconds dest */
    struct settings_icon_actor *field_88;      /* sub_8005A78 */
    struct settings_icon_actor *icons8c[4];      /* sub_8005AE8 */
    struct settings_icon_actor *icons9c[5];        /* sub_8005B80 */
    struct settings_icon_actor *iconsB0[3];          /* sub_8005C58 */
    struct settings_icon_actor *field_bc;              /* sub_8005D44 */
    struct settings_icon_actor *field_c0;                /* 0xc0 - currently-highlighted row's icon, sub_8005304/sub_8005100 */
    s32 field_c4;                                          /* 0xc4 - field_c0's blink/reveal countdown */
    u32 field_c8;                                            /* 0xc8 - REG_BLDCNT value, applied by sub_8006250 */
    u8 field_cc;                                              /* 0xcc - REG_BLDY value (low 5 bits); sub_8005100 animates this as a fade level */
    u8 unused_cd[3];
    u16 field_d0;                                              /* 0xd0 - REG_DISPCNT value, applied by sub_8006250 */
    u8 unused_d2[2];
};
COMPILE_TIME_ASSERT(sizeof(struct pause_screen_results) == 0xd4);

extern s32 sub_800815C(struct actor *part);

/* Sets `field_29`'s low nibble from sub_800815C's result, keeping the
 * high nibble - the recurring last step of every icon constructor that
 * touches a `struct settings_icon_actor` (see src/graphics/
 * settings_menu6.c and src/graphics/settings_menu13.c). Written with
 * explicit register pins (matching the SUB_8006600_* macros in
 * src/graphics/oam_count.c) because gcc's constant-propagation
 * otherwise folds the ROM's two-instruction "movs r1,#0x10 / rsbs
 * r1,r1,#0" -0x10 load into a single `sub` relative to the just-used
 * 0xf mask, which the ROM never does. */
#define UPDATE_ICON_FRAME_NIBBLE(iconExpr) \
    do { \
        register s32 _ret asm("r0") = sub_800815C(&(iconExpr)->base); \
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

#endif /* __PAUSE_SCREEN_RESULTS_H__ */
