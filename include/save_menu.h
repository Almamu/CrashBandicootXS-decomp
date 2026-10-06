#ifndef __SAVE_MENU_H__
#define __SAVE_MENU_H__

/* A small per-row aggregate: five running totals gathered from a
 * temporary 0x70-byte scratch buffer built by ReadSaveSlot (still raw -
 * `arg0` there is some list/category handle, `arg1` a row index). Used
 * as a contiguous 4-element array at `save_menu.rowStats`
 * (see RefreshSaveSlotSummaries in src/save/save_menu_ui.c). */
struct settings_row_stats {
    s32 percent;
    s32 gems;
    s32 relics;
    s32 lives;
    s32 crystals;
};
COMPILE_TIME_ASSERT(save_menu_h, sizeof(struct settings_row_stats) == 0x14);

/* The save menu (`gSaveMenu`, OpenSaveMenu/InitSaveMenu, run by
 * RunSaveMenu): its main options are "load game", "load link game",
 * "save game", "delete game" and "exit" (gSaveMenuOptions). `state`
 * selects the input handler (SaveMenuInput) and the draw routine
 * (DrawSaveMenu): 0 main options, 1 load, 2 load from the link save,
 * 3 link transfer, 4 message, 5 save, 6 delete, 7 "delete?",
 * 9 "overwrite?". `cursor` is the slot cursor (slots 0-3, 4 = cancel).
 * `cartSave` is the cartridge save, `linkSave` the save received over
 * the link cable (struct save_data, the save data).
 * `currentStats`/`rowStats` are the summaries the slot list shows
 * (SummarizeProgress). Formerly `struct pause_options_screen`; earlier
 * notes read it as a pause/options screen. */
struct save_menu {
    /* 0x00 - frame counter (SaveMenuInput adds 1 every call);
     * CommitSaveMenuFrame scrolls the background by it (BG0HOFS = frame >> 3) */
    u32 frame;
    s32 flags; /* 0x04 - bit 2 tested via (flags>>2)&1 throughout; signed - the
                * ROM shifts it arithmetically (asr, not lsr). SaveMenuInput also
                * uses this same field as a wrapping 0-0xff per-frame counter
                * ((flags+1)&0xff), which never touches bit 2. */
    u8 done;   /* 0x08 - RunSaveMenu/SaveMenuMainInput: "input loop should exit now" flag */
    u8 unused_09[3];
    u32 state; /* 0x0c - DrawSaveMenu's jump-table selector */
    /* 0x10 - the cursor: a main option, or a slot (0-3, 4 = cancel); wrapped by
     * SaveMenuMainInput/SaveMenuMoveCursor, set from RunSaveMenu's argument */
    s32 cursor;
    u32 messageLine1; /* 0x14 - state 4's first text line (DrawSaveMenuMessageLines) */
    u32 messageLine2; /* 0x18 - its second line */
    /* 0x1c - read by CommitSaveMenuFrame as a u16, written straight to REG_DISPCNT
     * - the DISPCNT shadow. */
    u16 dispcnt;
    u8 unused_1e[2]; /* 0x1e-0x1f */
    /* 0x20 - set when a game was loaded (SaveMenuLoadInput); RunSaveMenu
     * returns it */
    u8 gameLoaded;
    u8 unused_21[3]; /* 0x21-0x23 */
    /* 0x24 - the slot the "delete?"/"overwrite?" prompt (states 7, 9) acts on */
    u32 pendingSlot;
    /* 0x28-0x3b - a single scratch `settings_row_stats`, filled by
     * SummarizeProgress the same way each `rowStats[]` slot below is - see
     * InitSaveMenu/SaveMenuLoadInput/SaveGameToSlot. */
    struct settings_row_stats currentStats;
    struct settings_row_stats rowStats[4]; /* 0x3c-0x8b */
    void *cartSave;                        /* 0x8c - the cartridge's save data (struct save_data) */
    void *linkSave;                        /* 0x90 - the save data received over the link cable */
    u8 unused_94[0xa8 - 0x94];             /* 0x94-0xa7 */
    void *rowObjA[5];                      /* 0xa8-0xbb */
    void *rowObjB[5];                      /* 0xbc-0xcf */
    void *rowObjC[5];                      /* 0xd0-0xe3 */
};

#endif /* __SAVE_MENU_H__ */
