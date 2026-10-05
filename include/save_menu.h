#ifndef __SAVE_MENU_H__
#define __SAVE_MENU_H__

/* A small per-row aggregate: five running totals gathered from a
 * temporary 0x70-byte scratch buffer built by ReadSaveSlot (still raw -
 * `arg0` there is some list/category handle, `arg1` a row index). Used
 * as a contiguous 4-element array at `save_menu.rowStats`
 * (see RefreshSaveSlotSummaries in src/graphics/settings_menu2.c). */
struct settings_row_stats {
    s32 percent;
    s32 gems;
    s32 relics;
    s32 lives;
    s32 crystals;
};
COMPILE_TIME_ASSERT(sizeof(struct settings_row_stats) == 0x14);

/* The save menu (`gSaveMenu`, OpenSaveMenu/InitSaveMenu, run by
 * RunSaveMenu): its main options are "load game", "load link game",
 * "save game", "delete game" and "exit" (gSaveMenuOptions). `state`
 * selects the input handler (SaveMenuInput) and the draw routine
 * (DrawSaveMenu): 0 main options, 1 load, 2 load from the link save,
 * 3 link transfer, 4 message, 5 save, 6 delete, 7 "delete?",
 * 9 "overwrite?". `field_10` is the cursor (slots 0-3, 4 = cancel).
 * `field_8c` is the cartridge save, `field_90` the save received over
 * the link cable (struct save_data, the save data).
 * `currentStats`/`rowStats` are the summaries the slot list shows
 * (SummarizeProgress). Formerly `struct pause_options_screen`; earlier
 * notes read it as a pause/options screen. */
struct save_menu {
    /* 0x00 - read by CommitSaveMenuFrame (src/graphics/settings_menu4.c), shifted
     * right by 3 and written to REG_BG0HOFS (a u16) - a saved/pending BG0
     * horizontal-scroll value, pre-shifted by the caller. Also a plain
     * per-frame counter: SaveMenuInput increments it by 1 unconditionally
     * on every call. */
    u32 field_0;
    s32 flags;          /* 0x04 - bit 2 tested via (flags>>2)&1 throughout; signed - the
                          * ROM shifts it arithmetically (asr, not lsr). SaveMenuInput also
                          * uses this same field as a wrapping 0-0xff per-frame counter
                          * ((flags+1)&0xff), which never touches bit 2. */
    u8 field_8;              /* 0x08 - RunSaveMenu/SaveMenuMainInput: "input loop should exit now" flag */
    u8 unused_09[3];
    u32 state;           /* 0x0c - DrawSaveMenu's jump-table selector */
    s32 field_10;          /* 0x10 - per-row raw value; ==4 means "maxed out"; also wrap-inc/decremented
                             * as a signed row-cursor by SaveMenuMainInput/SaveMenuMoveCursor */
    u32 field_14;            /* 0x14 - label1, passed to DrawSaveMenuMessageLines */
    u32 field_18;              /* 0x18 - label2, passed to DrawSaveMenuMessageLines */
    /* 0x1c - read by CommitSaveMenuFrame as a u16, written straight to REG_DISPCNT
     * - a saved/pending DISPCNT value, paired with field_0 above. */
    u16 field_1c;
    u8 unused_1e[2];             /* 0x1e-0x1f */
    u8 field_20;                  /* 0x20 - InitSaveMenu/RunSaveMenu: a "result ready" poll flag on the SIO-spinner instance of this struct */
    u8 unused_21[3];               /* 0x21-0x23 */
    u32 field_24;                  /* 0x24 - numeric value for the slider rows */
    /* 0x28-0x3b - a single scratch `settings_row_stats`, filled by
     * SummarizeProgress the same way each `rowStats[]` slot below is - see
     * InitSaveMenu/SaveMenuLoadInput/SaveGameToSlot. */
    struct settings_row_stats currentStats;
    struct settings_row_stats rowStats[4]; /* 0x3c-0x8b */
    void *field_8c;                    /* 0x8c - left/first icon object (struct settings_sync_buffer *, see issue #5 write-up) */
    void *field_90;                      /* 0x90 - right/second icon object (struct settings_sync_buffer *) */
    u8 unused_94[0xa8 - 0x94];             /* 0x94-0xa7 */
    void *rowObjA[5];                        /* 0xa8-0xbb */
    void *rowObjB[5];                          /* 0xbc-0xcf */
    void *rowObjC[5];                            /* 0xd0-0xe3 */
};

#endif /* __SAVE_MENU_H__ */
