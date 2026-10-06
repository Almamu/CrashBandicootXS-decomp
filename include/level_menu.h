#ifndef GUARD_LEVEL_MENU_H
#define GUARD_LEVEL_MENU_H

/* The level-select screen (`struct level_menu`, src/menus/level_select.c,
 * GitHub issue #26), its two background layers (level_select_pages.c,
 * issue #27), its page entries and its UI sprite parts (shared with
 * level_select_widgets.c through level_select_parts.h, issues #28/#29).
 *
 * level_select.c, level_select_widgets.c and level_select_parts.h used to
 * carry their own copies of these types (two different `struct sprite`s,
 * `anim_record`/`anim_table`, `item`/`level_item`, `twinkle`/`zoom_bg`);
 * they were merged here (#574, batch 9e).
 *
 * All of it is gcc 2.x C++: method tables of {s16 this-adjust; pad; fn}
 * entries (`struct actor_method`), called through the
 * _call_via_r1/AD80/AD84/AD88 "call via r1/r2/r3/r4" thunks. */

#include "menus.h"
#include "gfx.h"
#include "actor_self.h"
#include "sprite_bank.h"

/* Bits of a sprite's `+0x28` byte (see struct part_f28 in
 * dingodile.c). */
struct sprite_f28 {
    u8 mode:2;
    u8 unk_2:2;
    u8 flipX:1;
    u8 flipY:1;
    u8 unk_6:2;
} __attribute__((packed));

/* The sprite part's method table (at part+0x18); +0x50 (slot 10) is the
 * destructor. */
struct sprite_vtable {
    u8 unk_00[0x50];
    struct actor_method m50; // 0x50
};

/* The 0x40-byte animated sprite part `InitUiSpriteObj` constructs, and
 * the base of level_select.c's 0x78-byte `SpawnLaunchPad` object. Its
 * animation set is a sprite bank (sprite_bank.h; the records were
 * `struct anim_record`/`anim_table`, whose `tileRecord`/`paletteId` is
 * `sprite_anim.paletteId`). */
struct sprite {
    s32 x;  // 0x00 - Q8
    s32 y;  // 0x04 - Q8
    u16 id; // 0x08
    u8 unk_0A[2];
    u8 flags; // 0x0C
    u8 unk_0D[0x0B];
    struct sprite_vtable *vtable; // 0x18
    u8 unk_1C[4];
    const struct sprite_bank *anim; // 0x20
    u8 unk_24[4];
    struct sprite_f28 f28; // 0x28
    u8 palette:4;          // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 animIndex; // 0x2D
    u8 unk_2E[2];
    s32 frame; // 0x30
    u8 unk_34[4];
    u8 animDone; // 0x38
    u8 unk_39[3];
    u16 scale; // 0x3C - Q8 affine scale, 0 = not affine (DrawAffineSpritePieces)
    u8 unk_3E[2];
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct sprite) == 0x40);

/* One level's saved record word (`level_menu.save + 4 + id * 4`). */
struct level_save {
    u32 cleared:1;
    u32 flag1:1;
    u32 flag2:1;
    u32 time:13; // best time, centiseconds (0 = none)
    u32 unk_16:16;
};

/* The same word with halfword bitfields: level_select.c's reads load it
 * with `ldrh` (byte 2 of the save block itself holds four more flags
 * LoadLevelSelectRecord tests). */
struct level_save_h {
    u16 cleared:1;
    u16 flag1:1;
    u16 flag2:1;
    u16 time:13; // best time, centiseconds (0 = none)
    u16 unk_16;
};

/* The same word, read a byte at a time. */
struct level_save_b {
    u8 cleared:1;
    u8 unk_0_1:7;
    u8 unk_1[3];
};

union level_record {
    struct level_save w;
    struct level_save_h h;
    struct level_save_b b;
};

/* The save block PackSaveData returns, as far as the menu reads it. */
struct menu_save {
    u8 unk_00[2];
    u8 open; // 0x02 - bit 5/7/6: pages 1/2/3 reachable
    u8 unk_03;
    union level_record levels[1]; // 0x04, five per page
};

union dispcnt {
    u16 raw;
    struct dispcnt_bits bits;
};

/* Method table of a page entry (`struct level_item`, gLevelSelectEntryVtable). */
struct item_vtable {
    struct actor_method unk_00;
    struct actor_method m08; // 0x08 - per-frame update (AnimateLevelSelectEntry)
    struct actor_method m10; // 0x10 - (world, slot): load the entry (SetLevelSelectEntryLevel)
    struct actor_method m18; // 0x18 - (struct xy_pair *): place it (SetLevelSelectEntryPos)
    struct actor_method m20; // 0x20 - draw
    struct actor_method m28; // 0x28 - destructor (DestroyLevelSelectEntry)
};

/* One level entry on the level-select page (0x14 bytes, constructor
 * CreateLevelSelectEntry, destructor DestroyLevelSelectEntry, method table
 * gLevelSelectEntryVtable). `icon` shows the level's picture (or, past
 * index 4, a per-world animation), `frame` the surrounding box.
 * level_menu.h's `struct item` and level_select.c's `struct level_item`
 * were its method-table views. */
struct level_item {
    s32 id;      // 0x00 - level id (GetLevelSelectEntryLevel)
    u8 selected; // 0x04
    u8 unk_05[3];
    struct sprite *icon;        // 0x08
    struct sprite *frame;       // 0x0C
    struct item_vtable *vtable; // 0x10 - gLevelSelectEntryVtable
};

/* BG1, the page strip (CreateLevelSelectPageBg). The first 0x10 bytes are the
 * InitBgSetup background descriptor (BGxCNT at +0x0C, GetBgSetupControl). */
struct page_bg {
    struct bg_setup bg; // 0x00 - BG1 (InitBgSetup)
    s32 scroll;         // 0x10 - current page scroll, Q8 (0x100 = a page)
    s32 target;         // 0x14 - scroll `scroll` eases toward
    u8 unk_18[0x0C];
    u16 hofs; // 0x24 - BG1HOFS (GetLevelSelectPageBgOffsets returns hofs|vofs)
    u16 vofs; // 0x26 - BG1VOFS
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct page_bg) == 0x28);

/* One twinkle sprite at the picture's corners (RandomizeZoomBgTwinkle,
 * TickZoomBgTwinkle). */
struct twinkle {
    s32 timer;           // 0x00 - frames until a new frame is picked
    s32 blink;           // 0x04 - blink window, counts down with timer
    struct sprite *part; // 0x08
};

/* REG_BG2CNT's layout in halfword bitfields (GetZoomBgControl's side). */
struct bgcnt_bits16 {
    u16 priority:2;
    u16 charBase:2;
    u16 unk_4:2;
    u16 mosaic:1;
    u16 colors256:1;
    u16 screenBase:5;
    u16 wrap:1;
    u16 size:2;
} __attribute__((packed));

/* BG2, the zooming level picture (InitZoomBg, 0x8C bytes). */
struct zoom_bg {
    u8 unk_00[0x0C];
    s32 state;       // 0x0C - see UpdateZoomBg
    s32 image;       // 0x10 - gLevelSelectPictures index, 11 = none
    s32 scale;       // 0x14 - zoom, 0x100 = 1:1, 8 = smallest
    s32 charBlock;   // 0x18
    s32 screenBlock; // 0x1C
    s32 x;           // 0x20 - screen centre
    s32 y;           // 0x24
    s32 dx;          // 0x28 - wobble offset
    s32 dy;          // 0x2C
    u32 phase;       // 0x30 - wobble phase, 0-0xFF
    /* The BG2CNT shadow (GetZoomBgControl). InitZoomBg sets it through
     * byte bitfields (`bits`), UpdateZoomBg through halfword ones
     * (`bits16`); `packed` keeps the union 2 bytes. */
    union {
        u16 raw;
        struct {
            u8 priority:2; // 0x34
            u8 charBase:2;
            u8 unk_34_4:3;
            u8 color256:1;
            u8 screenBase:5; // 0x35
            u8 unk_35_5:1;
            u8 screenSize:2;
        } __attribute__((packed)) bits;
        struct bgcnt_bits16 bits16;
    } __attribute__((packed)) bgcnt;
    u8 unk_36[2];
    /* BgAffineSet source, 0x38-0x49 */
    s32 texX;  // 0x38
    s32 texY;  // 0x3C
    s16 x16;   // 0x40 - screen centre
    s16 y16;   // 0x42
    s16 sx;    // 0x44
    s16 sy;    // 0x46
    u16 alpha; // 0x48 - rotation
    u8 unk_4A[2];
    /* BgAffineSet destination, committed by CommitZoomBg */
    s16 pa;                     // 0x4C
    s16 pb;                     // 0x4E
    s16 pc;                     // 0x50
    s16 pd;                     // 0x52
    s32 bgx;                    // 0x54
    s32 bgy;                    // 0x58
    struct twinkle twinkles[4]; // 0x5C
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct zoom_bg) == 0x8C);

/* The level-select screen object (0xAC bytes, InitLevelSelect). */
struct level_menu {
    u8 result; // 0x00 - returned by RunLevelSelect
    u8 unk_01[3];
    s32 lastIndex;                   // 0x04 - last valid `index` on this page
    s32 index;                       // 0x08 - cursor, 0-5
    s32 world;                       // 0x0C - page
    s32 levelId;                     // 0x10 - gLevelTable index
    s32 nameText;                    // 0x14 - the level name's text
    const struct xy_pair *positions; // 0x18 - cursor position per index
    struct page_bg *bg1;             // 0x1C - CreateLevelSelectPageBg, BG1
    struct zoom_bg *bg2;             // 0x20 - InitZoomBg, BG2 (the level picture)
    struct level_item *items[6];     // 0x24
    void *panel;                     // 0x3C - CreateLevelSelectCursor, the cursor panel
    struct sprite *sprites[10];      // 0x40
    char timeText[9];                // 0x68 - best time
    char recordText[9];              // 0x71 - next threshold to beat
    u8 unk_7A[2];
    u32 scroll;             // 0x7C - BG0 auto-scroll counter
    s32 panelSlideX;        // 0x80 - x offset of the record panel
    s32 clearedIconY;       // 0x84 - sprite 2's y offset (0 or 0x1C), see LoadLevelSelectRecord
    s32 flag1IconY;         // 0x88 - sprite 3's
    s32 gemIconY;           // 0x8C - sprite 4's (the `rank` gem icon)
    s32 trialIconY;         // 0x90 - sprite 5's (time-trial icons)
    s32 trialIcon2Y;        // 0x94 - sprite 6's
    s32 rank;               // 0x98 - LoadLevelSelectRecord's classification, 5 = none
    struct menu_save *save; // 0x9C - PackSaveData's save block
    union blend blend;      // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;       // 0xA4 - REG_BLDY
    union dispcnt dispcnt;  // 0xA8 - REG_DISPCNT
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct level_menu) == 0xAC);

#endif /* GUARD_LEVEL_MENU_H */
