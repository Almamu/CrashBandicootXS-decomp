#ifndef __PAUSE_OPTIONS_SCREEN_H__
#define __PAUSE_OPTIONS_SCREEN_H__

/* A small per-row aggregate: five running totals gathered from a
 * temporary 0x70-byte scratch buffer built by sub_8002C14 (still raw -
 * `arg0` there is some list/category handle, `arg1` a row index). Used
 * as a contiguous 4-element array at `pause_options_screen.rowStats`
 * (see sub_8004860 in src/graphics/settings_menu2.c). */
struct settings_row_stats {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
    s32 field_10;
};
COMPILE_TIME_ASSERT(sizeof(struct settings_row_stats) == 0x14);

/* The composite pause/options screen widget (see docs/rom_map.md's
 * overlay_ui section - "one composite pause/options screen"). Only the
 * fields actually touched by functions matched so far
 * (0x08002C84-0x08003A60, this chunk's issue #5 write-up; plus
 * 0x08003B40-0x08004CB4 and sub_8004CB4/sub_8004CE8 at
 * 0x08004CB4-0x08004D74, src/graphics/settings_menu4.c) are named; the
 * real constructor (sub_8004D74/sub_8004EC0, still raw) would pin down
 * the rest. Also reused (same 0xe4 size, same field_8c/field_90 shape)
 * by sub_800306C for the unrelated "connecting..." spinner dialog
 * object - see docs/matching/issue-5-overlay-ui-sync-buffer.md. */
struct pause_options_screen {
    /* 0x00 - read by sub_8004CE8 (src/graphics/settings_menu4.c), shifted
     * right by 3 and written to REG_BG0HOFS (a u16) - a saved/pending BG0
     * horizontal-scroll value, pre-shifted by the caller. Also a plain
     * per-frame counter: sub_80031E4 increments it by 1 unconditionally
     * on every call. */
    u32 field_0;
    s32 flags;          /* 0x04 - bit 2 tested via (flags>>2)&1 throughout; signed - the
                          * ROM shifts it arithmetically (asr, not lsr). sub_80031E4 also
                          * uses this same field as a wrapping 0-0xff per-frame counter
                          * ((flags+1)&0xff), which never touches bit 2. */
    u8 field_8;              /* 0x08 - sub_800300C/sub_80032E8: "input loop should exit now" flag */
    u8 unused_09[3];
    u32 state;           /* 0x0c - sub_8004BD0's jump-table selector */
    s32 field_10;          /* 0x10 - per-row raw value; ==4 means "maxed out"; also wrap-inc/decremented
                             * as a signed row-cursor by sub_80032E8/sub_80033E8 */
    u32 field_14;            /* 0x14 - label1, passed to sub_8003BDC */
    u32 field_18;              /* 0x18 - label2, passed to sub_8003BDC */
    /* 0x1c - read by sub_8004CE8 as a u16, written straight to REG_DISPCNT
     * - a saved/pending DISPCNT value, paired with field_0 above. */
    u16 field_1c;
    u8 unused_1e[2];             /* 0x1e-0x1f */
    u8 field_20;                  /* 0x20 - sub_800306C/sub_800300C: a "result ready" poll flag on the SIO-spinner instance of this struct */
    u8 unused_21[3];               /* 0x21-0x23 */
    u32 field_24;                  /* 0x24 - numeric value for the slider rows */
    /* 0x28-0x3b - a single scratch `settings_row_stats`, filled by
     * sub_80048E0 the same way each `rowStats[]` slot below is - see
     * sub_800306C/sub_80034BC/sub_8003698. */
    struct settings_row_stats currentStats;
    struct settings_row_stats rowStats[4]; /* 0x3c-0x8b */
    void *field_8c;                    /* 0x8c - left/first icon object (struct settings_sync_buffer *, see issue #5 write-up) */
    void *field_90;                      /* 0x90 - right/second icon object (struct settings_sync_buffer *) */
    u8 unused_94[0xa8 - 0x94];             /* 0x94-0xa7 */
    void *rowObjA[5];                        /* 0xa8-0xbb */
    void *rowObjB[5];                          /* 0xbc-0xcf */
    void *rowObjC[5];                            /* 0xd0-0xe3 */
};

#endif /* __PAUSE_OPTIONS_SCREEN_H__ */
