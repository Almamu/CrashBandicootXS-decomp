#ifndef GUARD_LEVEL_MENU_H
#define GUARD_LEVEL_MENU_H

/* The level-select screen (`struct level_menu`) and its two background
 * layers, shared by src/menus/level_select_pages.c (GitHub issue #27).
 *
 * The layouts are the ones src/menus/level_select.c (issue #26)
 * worked out for the same objects; that file still carries its own
 * copies of these definitions and can switch to this header.
 *
 * All of it is gcc 2.x C++: method tables of {s16 this-adjust; pad; fn}
 * entries, called through the _call_via_r1/AD80/AD84/AD88 "call via
 * r1/r2/r3/r4" thunks. */

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

/* One 28-byte animation record, `anim_table.records[animIndex]`. */
struct anim_record
{
    u8 unk_00[0x14];
    u8 tileRecord;          // 0x14 - GetPaletteSlot/LockPalette record id
    u8 unk_15;
    u8 frameCount;          // 0x16
    u8 unk_17[5];
};

struct anim_table
{
    struct anim_record *records;
};

/* Bits of a sprite's `+0x28` byte (see struct part_f28 in
 * dingodile.c). */
struct sprite_f28
{
    u8 mode:2;
    u8 unk_2:2;
    u8 flipX:1;
    u8 flipY:1;
    u8 unk_6:2;
} __attribute__((packed));

/* The 0x40-byte animated sprite part `InitUiSpriteObj` constructs. */
struct sprite
{
    s32 x;                    // 0x00
    s32 y;                    // 0x04
    u16 id;                   // 0x08
    u8 unk_0A[2];
    u8 flags;                 // 0x0C
    u8 unk_0D[0x0B];
    struct method *vtable;    // 0x18
    u8 unk_1C[4];
    struct anim_table *anim;  // 0x20
    u8 unk_24[4];
    struct sprite_f28 f28;    // 0x28
    u8 palette:4;             // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 animIndex;             // 0x2D
    u8 unk_2E[2];
    s32 frame;                // 0x30
    u8 unk_34[8];
    u16 unk_3C;               // 0x3C
    u8 unk_3E[2];
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct sprite) == 0x40);

struct xy_pair
{
    s32 x;
    s32 y;
};

/* One level's saved record word (`level_menu.save + 4 + id * 4`). */
struct level_save
{
    u32 cleared:1;
    u32 flag1:1;
    u32 flag2:1;
    u32 time:13;        // best time, centiseconds (0 = none)
    u32 unk_16:16;
};

/* The same word, read a byte at a time. */
struct level_save_b
{
    u8 cleared:1;
    u8 unk_0_1:7;
    u8 unk_1[3];
};

union level_record
{
    struct level_save w;
    struct level_save_b b;
};

/* The save block PackSaveData returns, as far as the menu reads it. */
struct menu_save
{
    u8 unk_00[2];
    u8 open;            // 0x02 - bit 5/7/6: pages 1/2/3 reachable
    u8 unk_03;
    union level_record levels[1];  // 0x04, five per page
};

/* Shadow copies of the blend/display registers, committed every frame. */
struct blend_bits
{
    u32 bg0First:1;     // BLDCNT 1st target
    u32 bg1First:1;
    u32 bg2First:1;
    u32 bg3First:1;
    u32 objFirst:1;
    u32 bdFirst:1;
    u32 effect:2;
    u32 bg0Second:1;    // BLDCNT 2nd target
    u32 bg1Second:1;
    u32 bg2Second:1;
    u32 bg3Second:1;
    u32 objSecond:1;
    u32 bdSecond:1;
    u32 unk_14:2;
    u32 eva:5;          // BLDALPHA
    u32 unk_21:3;
    u32 evb:5;
    u32 unk_29:3;
};

union blend
{
    u32 raw;
    struct blend_bits bits;
};

struct bldy
{
    u32 evy:5;
    u32 unk_5:27;
};

union dispcnt
{
    u16 raw;
};

/* Method table of a page entry (`struct item`, CreateLevelSelectEntry). */
struct item_vtable
{
    struct method unk_00;
    struct method m08;          // 0x08 - per-frame update
    struct method m10;          // 0x10 - (world, slot): load the entry
    struct method m18;          // 0x18 - (struct xy_pair *): place it
    struct method m20;          // 0x20 - draw
    struct method m28;          // 0x28 - destructor
};

/* One level entry on the current page (CreateLevelSelectEntry, 0x14 bytes). */
struct item
{
    u8 unk_00[0x10];
    struct item_vtable *vtable; // 0x10
};

/* BG1, the page strip (CreateLevelSelectPageBg). The first 0x10 bytes are the
 * InitBgSetup background descriptor (BGxCNT at +0x0C, GetBgSetupControl). */
struct page_bg
{
    u8 desc[0x10];      // 0x00 - InitBgSetup
    s32 scroll;         // 0x10 - current page scroll, Q8 (0x100 = a page)
    s32 target;         // 0x14 - scroll `scroll` eases toward
    u8 unk_18[0x0C];
    u16 hofs;           // 0x24 - BG1HOFS (GetLevelSelectPageBgOffsets returns hofs|vofs)
    u16 vofs;           // 0x26 - BG1VOFS
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct page_bg) == 0x28);

/* One twinkle sprite at the picture's corners, handed to
 * RandomizeZoomBgTwinkle (`struct twinkle` in level_select_widgets.c: the
 * first 8 bytes are its timer and blink window). */
struct twinkle
{
    u8 unk_00[8];
    struct sprite *part;        // 0x08
};

/* BG2, the zooming level picture (InitZoomBg, BG2CNT through
 * GetZoomBgControl; `struct zoom_bg` in level_select_widgets.c). */
struct zoom_bg
{
    u8 unk_00[0x0C];
    s32 state;                  // 0x0C - see UpdateZoomBg
    s32 image;                  // 0x10 - gLevelSelectPictures index, 11 = none
    s32 scale;                  // 0x14 - zoom, 0x100 = 1:1, 8 = smallest
    s32 charBlock;              // 0x18
    s32 screenBlock;            // 0x1C
    s32 x;                      // 0x20
    s32 y;                      // 0x24
    s32 dx;                     // 0x28 - wobble offset
    s32 dy;                     // 0x2C
    s32 phase;                  // 0x30 - wobble phase, 0-0xFF
    union
    {
        u16 raw;
        struct
        {
            u8 priority:2;      // 0x34 - BG2CNT shadow
            u8 charBase:2;
            u8 unk_34_4:3;
            u8 color256:1;
            u8 screenBase:5;    // 0x35
            u8 unk_35_5:1;
            u8 screenSize:2;
        } __attribute__((packed)) bits;
    } __attribute__((packed)) bgcnt;
    u8 unk_36[2];
    s32 texX;                   // 0x38 - BgAffineSet source
    s32 texY;                   // 0x3C
    u16 x16;                    // 0x40
    u16 y16;                    // 0x42
    u8 unk_44[4];
    u16 alpha;                  // 0x48 - rotation
    u8 unk_4A[0x12];
    struct twinkle twinkles[4]; // 0x5C
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct zoom_bg) == 0x8C);

/* The level-select screen object (0xAC bytes, InitLevelSelect). */
struct level_menu
{
    u8 result;                  // 0x00 - returned by RunLevelSelect
    u8 unk_01[3];
    s32 lastIndex;              // 0x04 - last valid `index` on this page
    s32 index;                  // 0x08 - cursor, 0-5
    s32 world;                  // 0x0C - page
    s32 levelId;                // 0x10 - gLevelTable index
    s32 nameText;               // 0x14 - the level name's text
    struct xy_pair *positions;  // 0x18 - cursor position per index
    struct page_bg *bg1;        // 0x1C - CreateLevelSelectPageBg, BG1
    struct zoom_bg *bg2;        // 0x20 - InitZoomBg, BG2 (the level picture)
    struct item *items[6];      // 0x24
    void *panel;                // 0x3C - CreateLevelSelectCursor, the cursor panel
    struct sprite *sprites[10]; // 0x40
    char timeText[9];           // 0x68 - best time
    char recordText[9];         // 0x71 - next threshold to beat
    u8 unk_7A[2];
    u32 scroll;                 // 0x7C - BG0 auto-scroll counter
    s32 panelSlideX;            // 0x80 - x offset of the record panel
    s32 clearedIconY;           // 0x84 - sprite 2's y offset (0 or 0x1C), see LoadLevelSelectRecord
    s32 flag1IconY;             // 0x88 - sprite 3's
    s32 gemIconY;               // 0x8C - sprite 4's (the `rank` gem icon)
    s32 trialIconY;             // 0x90 - sprite 5's (time-trial icons)
    s32 trialIcon2Y;            // 0x94 - sprite 6's
    s32 rank;                   // 0x98
    struct menu_save *save;     // 0x9C - PackSaveData's save block
    union blend blend;          // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;           // 0xA4 - REG_BLDY
    union dispcnt dispcnt;      // 0xA8 - REG_DISPCNT
};

COMPILE_TIME_ASSERT(level_menu_h, sizeof(struct level_menu) == 0xAC);

#endif /* GUARD_LEVEL_MENU_H */
