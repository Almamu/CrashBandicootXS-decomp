#include "core.h"
#include "icon_manager.h"

/* GitHub issue #26: 0x0801B85C-0x0801CEE0, the whole of the former
 * asm/code_3_2_17_188d0_1b85c.s. Three objects, all gcc 2.x C++ classes
 * (a method table at +0x18/+0x10, virtual calls through the
 * sub_803AD7C/AD80/AD88 call-via-register thunks, inlined member
 * functions):
 *
 * - sub_801B85C-sub_801B980: `struct follow_child`, the 0x80-byte
 *   object (method table gStaticData_087E4ABC) sub_8017600
 *   (actor_part_17524.c) spawns for the input controller. It trails the
 *   player (gUnknown_030012D8) at a horizontal offset that eases 2 px per
 *   frame toward a clamped target, and registers itself as
 *   gUnknown_030012D4's follow target while alive.
 * - sub_801B984-sub_801BAD0: a 0x78-byte sprite subclass (method table
 *   gStaticData_087E4B34) with a factory, a player-overlap check that
 *   fires the player's method 13 (0x19), constructor and destructor.
 * - sub_801BAF0-sub_801CE60: `struct level_menu`, the paged level-select
 *   screen (docs/rom_map.md: "a paged menu/screen with a smooth
 *   horizontal page-turn animation"). sub_801BAF0 is the whole modal
 *   screen: it builds the menu (sub_801BC28), runs it (sub_801C96C)
 *   and returns the chosen level through `*arg`. Five levels per page
 *   (`arg / 5`, `arg % 5`); each level's fixed record (name text, three
 *   time-trial thresholds) is gLevelTable, its saved record a
 *   bitfield word (cleared flag, two more flags, best time). The screen
 *   keeps shadow copies of BLDCNT/BLDALPHA/BLDY/DISPCNT and commits them
 *   with the scroll registers every frame (`CommitDisplay`).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, expected/ or src/:
 * sub_801B85C, sub_801B960, sub_801B980, sub_801BAD0 (reachable only
 * through their method tables, if at all). Matched anyway.
 *
 * Everything is plain C. Details, and the two techniques that
 * closed most of the rest (`Opaque` constants, inline member helpers),
 * are in docs/matching/issue-26-level-select-menu.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS), the compiler this
 * region was originally built with; see docs/matching/old-agbcc-retry.md.
 * Under it the shadow-register blocks are plain bitfield stores; the
 * `Opaque`/register-pinned forms were current-agbcc workarounds. */

struct method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct follow_child
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u16 field_08;       // 0x08
    u8 unk_0A[2];
    u8 flags;           // 0x0C
    u8 bit0_1:2;        // 0x0D
    u8 visible:1;
    u8 bit3_7:5;
    u8 unk_0E[0x0A];
    void *vtable;       // 0x18
    u8 unk_1C[8];
    u8 unk_24;          // 0x24
    u8 unk_25[0x0D];
    u8 unk_32;          // 0x32
    u8 unk_33[0x2D];
    s32 unk_60;         // 0x60 - copied from the player every frame
    u8 unk_64[4];
    u8 unk_68;          // 0x68
    u8 unk_69[0x0F];
    s32 targetOffset;   // 0x78 - Q8 x offset from the player, 0xA00-0x3200
    s32 offset;         // 0x7C - eases toward targetOffset
};

COMPILE_TIME_ASSERT(sizeof(struct follow_child) == 0x80);

/* gUnknown_030012D8, only the fields used here. */
struct player
{
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    u8 unk_08[4];
    u8 flags;               // 0x0C - bit 7 tested by sub_801BA60
    u8 unk_0D[0x0B];
    struct method *vtable;  // 0x18
    u8 unk_1C[0x44];
    s32 unk_60;             // 0x60
};

struct follow_owner
{
    u8 unk_00[0x10];
    void *follow;           // 0x10 - the player, or a live follow_child
};

/* One 28-byte animation record, `anim_table.records[animIndex]`. */
struct anim_record
{
    u8 unk_00[0x14];
    u8 tileRecord;          // 0x14 - sub_8006DF8 record id
    u8 unk_15;
    u8 frameCount;          // 0x16
    u8 unk_17[5];
};

COMPILE_TIME_ASSERT(sizeof(struct anim_record) == 0x1C);

struct anim_table
{
    struct anim_record *records;
};

/* The 0x40-byte animated sprite part `sub_8008904` constructs, and the
 * base of the 0x78-byte `sub_801B984` object. */
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
    u8 flags28;               // 0x28
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

COMPILE_TIME_ASSERT(sizeof(struct sprite) == 0x40);

/* sub_8007B98's output box. */
struct hit_box
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
};

/* One level's fixed data (`gLevelTable`, 36-byte records,
 * indexed by level id). */
struct level_info
{
    s32 nameText;           // 0x00 - text id (sub_8026F38)
    s32 unk_04;             // 0x04 - passed to sub_801DD80
    u32 time0;              // 0x08 - time-trial thresholds, centiseconds,
    u32 time1;              // 0x0C   loosest first
    u32 time2;              // 0x10
    u8 unk_14[0x10];
};

COMPILE_TIME_ASSERT(sizeof(struct level_info) == 0x24);

/* One level's saved record word (`level_menu.save + 4 + id * 4`; byte 2
 * of the save block itself holds four more flags sub_801C608 tests). */
struct level_save
{
    u16 cleared:1;
    u16 flag1:1;
    u16 flag2:1;
    u16 time:13;        // best time, centiseconds (0 = none)
    u16 unk_16;
};

struct xy_pair
{
    s32 x;
    s32 y;
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

/* The same register viewed as its low byte, for the fade-in decrement
 * (a byte-sized test is what makes gcc narrow the `evy != 0` check to
 * an `and` of the loaded byte). */
struct bldy_byte
{
    u8 evy:5;
    u8 unk_5:3;
} __attribute__((packed));

struct dispcnt_bits
{
    u16 mode:3;
    u16 cgbMode:1;
    u16 frame:1;
    u16 hblankFree:1;
    u16 obj1d:1;
    u16 forcedBlank:1;
    u16 bg0:1;
    u16 bg1:1;
    u16 bg2:1;
    u16 bg3:1;
    u16 obj:1;
    u16 win0:1;
    u16 win1:1;
    u16 objWin:1;
};

union dispcnt
{
    u16 raw;
    struct dispcnt_bits bits;
};

struct item_vtable
{
    struct method unk_00;
    struct method m08;          // 0x08 - per-frame update (sub_801C104)
    struct method m10;          // 0x10
    struct method m18;          // 0x18
    struct method m20;          // 0x20 - draw (sub_801C51C)
    struct method m28;          // 0x28 - destructor (sub_801C040)
};

/* One level entry on the current page (sub_801DFEC, 0x14 bytes). */
struct item
{
    u8 unk_00[0x10];
    struct item_vtable *vtable; // 0x10
};

/* The level-select screen object (0xAC bytes, sub_801BC28). */
struct level_menu
{
    u8 result;                  // 0x00 - returned by sub_801BAF0
    u8 unk_01[3];
    s32 lastIndex;              // 0x04 - last valid `index` on this page
    s32 index;                  // 0x08 - cursor, 0-5
    s32 world;                  // 0x0C - page
    s32 levelId;                // 0x10 - gLevelTable index
    s32 nameText;               // 0x14 - the level name's text
    struct xy_pair *positions;  // 0x18 - cursor position per index
    void *bg1;                  // 0x1C - sub_801D7F8, BG1
    void *bg2;                  // 0x20 - sub_801D828, BG2 (icon layer)
    struct item *items[6];      // 0x24
    void *panel;                // 0x3C - sub_801E04C, the cursor panel
    struct sprite *sprites[10]; // 0x40
    char timeText[9];           // 0x68 - best time
    char recordText[9];         // 0x71 - next threshold to beat
    u8 unk_7A[2];
    u32 scroll;                 // 0x7C - BG0 auto-scroll counter
    s32 unk_80;                 // 0x80 - horizontal slide of the record panel
    s32 unk_84;                 // 0x84 - per-sprite y offsets (0 or 0x1C)
    s32 unk_88;                 // 0x88
    s32 unk_8C;                 // 0x8C
    s32 unk_90;                 // 0x90
    s32 unk_94;                 // 0x94
    s32 rank;                   // 0x98 - sub_801C608's classification, 5 = none
    u8 *save;                   // 0x9C - sub_80236EC's save block
    union blend blend;          // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;           // 0xA4 - REG_BLDY
    union dispcnt dispcnt;      // 0xA8 - REG_DISPCNT
};

COMPILE_TIME_ASSERT(sizeof(struct level_menu) == 0xAC);

struct vram_cursor
{
    u8 unk_00[8];
    u32 unk_08;
};

/* gUnknown_030012B8, only the field used here. */
struct tile_cache
{
    u8 unk_000[0x20C];
    u16 palette[16];        // 0x20C
};

/* Held keys in the low half, newly-pressed keys in the high half. */
struct held_pressed_pair
{
    u16 held;
    u16 pressed;
};

union key_state
{
    u32 all;
    struct held_pressed_pair half;
};

extern struct player *gUnknown_030012D8;
extern struct follow_owner *gUnknown_030012D4;
extern void ***gUnknown_030012D0;
extern void *gUnknown_030012F0;
extern struct tile_cache *gUnknown_030012B8;
extern void *gUnknown_030012BC;
extern void *gLevelState;
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern struct vram_cursor *gUnknown_030012FC;
extern void *gUnknown_03001300;
extern void *gUnknown_03001304;
extern struct level_menu *gUnknown_03000820;
extern u8 gUnknown_03000824;
extern union key_state gUnknown_030007E0;
extern u8 gStaticData_087E4ABC[];
extern u8 gStaticData_087E4B34[];
extern struct level_info gLevelTable[];
extern u8 gStaticData_0816C56C[];
extern u8 gStaticData_0816C484[];
extern u32 gStaticData_0816C548[];
extern u32 gStaticData_0816C558[];
extern struct xy_pair gStaticData_0816C498;
extern struct xy_pair gStaticData_0816C4A0;
extern struct xy_pair gStaticData_0816C4A8;
extern struct xy_pair gStaticData_0816C4B0;
extern struct xy_pair gStaticData_0816C4B8;
extern struct xy_pair gStaticData_0816C4C0;
extern struct xy_pair gStaticData_0816C4C8;
extern struct xy_pair gStaticData_0816C4D0;

/* Base class and runtime. */
extern void sub_8009F90(void *self);
extern void sub_8009F1C(void *self, s32 flags);
extern void sub_8009FB0(void *self);
extern void *sub_8026EDC(u32 size);
extern void sub_8026ED0(void *p);
extern s32 sub_803ADB4(s32 a, s32 b);
extern s32 sub_803AE4C(s32 a, s32 b);
extern void sub_803AD7C(void *self, void *fn);
extern s32 sub_803AD80(void *self, s32 arg, void *fn);
/* Calls the function in r4 with r0-r3 (see the `register ... asm("r4")`
 * pin at the call site). */
extern void sub_803AD88(void *self, s32 a, s32 b, s32 c);
extern s32 mem_free_bytes(s32 flags);

/* Sprite parts. */
extern struct sprite *sub_8008904(void *mem);
extern void sub_8008E94(void *manager, void *value);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, s32 arg);
extern void sub_80088D8(void *part, s32 value);
extern void sub_800737C(void *part, s32 x, s32 y);
extern void sub_8008890(void *part, s32 dx, s32 dy);
extern s32 sub_800815C(void *part);
extern void sub_8008044(void *p);
/* Really returns a u8 (src/graphics/graphics.c), but the call site
 * re-zero-extends the result, as it would through a wider return type. */
extern s32 sub_8006DF8(void *cache, u8 recordId);
extern void sub_8007B98(struct hit_box *dest, void *part);
extern u8 sub_800B37C(void *actor, struct hit_box *box);

/* Display, VRAM and sound. */
extern void sub_80006A8(void);
extern void sub_80007AC(void *p);
extern void sub_8006EA8(void *cache);
extern void sub_8006D50(void *cache, s32 arg);
extern void sub_8006DC8(void *p);
extern void sub_8006AAC(void *p);
extern void sub_8006A90(void *p);
extern void sub_8006A48(void *p);
extern void sub_803A94C(void *src, void *dst, s32 size);
extern void sub_8006C4C(struct vram_cursor *self);
extern s32 sub_8006C58(struct vram_cursor *self, s32 size);
extern void sub_8006C30(struct vram_cursor *self);
extern void sub_8006C28(struct vram_cursor *p);
extern void FlushVramDmaQueue(void);
extern void sub_801E644(void *dst, s32 a, s32 b, s32 c, s32 d);
extern void LoadGraphicsPackage(void *dst, void *pkg);
extern void sub_8001B54(void *arg0, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_8026F38(s32 id);
extern void sub_8028A30(struct icon_manager *m, u8 v);
extern void sub_8028A40(struct icon_manager *m);
extern void FormatCentiseconds(s32 value, char *buf);

/* Save data. */
extern u8 *sub_80236EC(void *p);
extern u8 sub_802336C(void *p, s32 id);
extern u8 sub_8023360(void *p, s32 id);
extern u8 sub_8023354(void *p, s32 id);
extern u8 sub_8023348(void *p, s32 id);
extern u8 sub_802333C(void *p, s32 id);

/* The level-select screen's sub-objects and siblings (0x0801CEE0 on). */
extern void *sub_801D7F8(void *mem, s32 a, s32 b);
extern void *sub_801D828(void *mem, s32 a, s32 b);
extern void *sub_801E04C(void *mem);
extern struct item *sub_801DFEC(void *mem);
extern void sub_801D638(struct level_menu *self);
extern void sub_801D5CC(struct level_menu *self);
extern void sub_801D668(struct level_menu *self);
extern void sub_801D730(struct level_menu *self);
extern void sub_801D05C(struct level_menu *self);
extern void sub_801D110(struct level_menu *self);
extern void sub_801D300(struct level_menu *self);
extern u8 sub_801D428(struct level_menu *self);
extern u8 sub_801D434(struct level_menu *self);
extern void sub_801D4C4(struct level_menu *self);
extern void sub_801D548(struct level_menu *self);
extern void sub_801D7AC(void *p);
extern void sub_801D7D4(void *p);
extern u32 sub_801D7D0(void *p);
extern void sub_801D7E0(void *p, s32 flags);
extern s32 sub_801D77C(void *p);
extern u8 sub_801D780(void *p);
extern void sub_801DA38(void *p, s32 flags);
extern void sub_801DAD8(void *p);
extern void sub_801DC28(void *p);
extern void sub_801DCBC(void *p);
extern u8 sub_801DCF8(void *p);
extern u8 sub_801DD18(void *p);
extern u8 sub_801DD28(void *p);
extern u8 sub_801DD38(void *p);
extern void sub_801DD5C(void *p);
extern void sub_801DD80(void *p, s32 arg);
extern u16 sub_801DE24(void *p);
extern u8 sub_801DE28(struct item *p);
extern s32 sub_801DE2C(struct item *it);
extern void sub_801DEA0(struct item *it, s32 arg);
extern void sub_801E190(void *p);
extern void sub_801E2BC(void *p);
extern void sub_801E408(void *p);
/* Returns a u8; the one caller that needs it tests only its low byte. */
extern s32 sub_801E464(void *p);
extern void sub_801E480(void *p, s32 x, s32 y);
extern void sub_801E524(void *p, s32 flags);
extern u16 sub_801E640(void *p);

void sub_801BAC4(struct sprite *self);
struct level_menu *sub_801BC28(struct level_menu *self, s32 arg);
void sub_801C040(struct level_menu *self, s32 flags);
void sub_801C2B0(struct level_menu *self);
void sub_801C364(struct level_menu *self);
void sub_801C3E8(struct level_menu *self, u32 time);
void sub_801C608(struct level_menu *self);
s32 sub_801C96C(struct level_menu *self);
void sub_801CDE0(struct level_menu *self);
void sub_801CE60(struct level_menu *self);

/* Returns `v` unchanged. gcc's tree folder moves a constant operand of a
 * commutative operator second, so `mask & *p` loads `*p` before building
 * the constant; this ROM consistently builds the constant first (it was
 * compiled as C++, whose front end doesn't reorder them). Routing the
 * constant through an inline call hides it from the folder - inlining
 * happens later, at RTL level - so it stays the first operand. */
static inline s32 Opaque(s32 v)
{
    return v;
}

/* sub_80087D0, inlined: select animation `idx` and restart it. */
static inline void SetAnim(struct sprite *s, s32 idx)
{
    s->animIndex = idx;
    sub_80087C0(s);
    sub_80087B4(s);
    sub_800872C(s, 0);
}

static inline struct anim_table *AnimTable(s32 offset)
{
    return (struct anim_table *)((u8 *)**gUnknown_030012D0 + offset);
}

static inline void SetIconPos(struct icon_manager *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

static inline struct item *ItemAt(struct item **items, s32 index)
{
    register s32 off asm("r0") = index * 4;

    return *(struct item **)((u8 *)items + off);
}

/* The level-select screen's per-frame register commit. */
static inline void CommitDisplay(struct level_menu *self)
{
    FlushVramDmaQueue();
    sub_801DCBC(self->bg2);
    self->scroll++;
    *(vu16 *)REG_ADDR_BG0HOFS = self->scroll >> 3;
    *(vu32 *)REG_ADDR_BG1HOFS = sub_801D7D0(self->bg1);
    *(vu16 *)REG_ADDR_BG1CNT = sub_801E640(self->bg1);
    *(vu16 *)REG_ADDR_BG2CNT = sub_801DE24(self->bg2);
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = self->blend.raw;
    *(vu16 *)REG_ADDR_BLDY = self->bldy.evy;
    *(vu16 *)REG_ADDR_DISPCNT = self->dispcnt.raw;
}

/* A virtual call through a sprite's method table (gcc 2.x lowering: take
 * the entry's address once, then read `this` adjustment and function). */
#define SPRITE_CALL(obj, idx, a)                                               \
    do                                                                         \
    {                                                                          \
        struct method *_m = &(obj)->vtable[idx];                               \
        sub_803AD80((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets `unk_32`. */
void sub_801B85C(struct follow_child *self)
{
    self->unk_32 = 1;
}

/* (Re)initializes a follow child: makes it visible, registers it as
 * gUnknown_030012D4's follow target and snaps it 0x1E00 (30 px, Q8) to
 * the player's right with both offsets reset. The `visible` bit test/
 * toggle and the stores need register pins to keep the ROM's
 * allocation (see the doc for issue 26). */
void sub_801B864(struct follow_child *self)
{
    u32 v = *((u8 *)self + 0x0D) >> 2;
    register u32 one asm("r1") = 1;

    if (!(v & one))
        self->visible = v ^ 1;
    gUnknown_030012D4->follow = self;
    {
        struct player *p = gUnknown_030012D8;
        register s32 x asm("r0") = p->x;
        register s32 y asm("r2") = p->y;
        register s32 off asm("r1") = 0x1E00;

        self->x = x + off;
        self->y = y;
        {
            register s32 f asm("r0") = 0x10;

            f |= self->flags;
            self->flags = f;
        }
        self->targetOffset = off;
        self->offset = off;
    }
}

/* Per-frame update (method table +0x18): base update, then ease `offset`
 * toward `targetOffset` by 0x200 per frame and follow the player. */
void sub_801B8BC(struct follow_child *self)
{
    s32 cur, tgt;

    sub_8009FB0(self);
    {
        s32 one = 1;
        s32 zero;
        u8 *p = &self->unk_24;

        zero = 0;
        p[0] = one;
        p[0x44] = zero;
    }
    cur = self->offset;
    tgt = self->targetOffset;
    if (cur < tgt)
    {
        cur += 0x200;
        if (cur > tgt)
            self->offset = tgt;
        else
            self->offset = cur;
    }
    else if (cur > tgt)
    {
        cur -= 0x200;
        if (cur < tgt)
            self->offset = tgt;
        else
            self->offset = cur;
    }
    {
        struct player *p = gUnknown_030012D8;
        s32 x = p->x;
        s32 y = p->y;

        self->x = x + self->offset;
        self->y = y;
    }
    self->unk_60 = gUnknown_030012D8->unk_60;
}

/* Destructor (method table +0x50): hands gUnknown_030012D4's follow
 * target back to the player. */
void sub_801B91C(struct follow_child *self, s32 flags)
{
    self->vtable = gStaticData_087E4ABC;
    gUnknown_030012D4->follow = gUnknown_030012D8;
    sub_8009F1C(self, flags);
}

/* Constructor, called from sub_8017600 (actor_part_17524.c). */
struct follow_child *sub_801B940(struct follow_child *self)
{
    sub_8009F90(self);
    self->vtable = gStaticData_087E4ABC;
    sub_801B864(self);
    return self;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the target offset, clamped to
 * 0xA00-0x3200. */
void sub_801B960(struct follow_child *self, s32 offset)
{
    if (offset > 0x3200)
        offset = 0x3200;
    else if (offset <= 0x9FF)
        offset = 0xA00;
    self->targetOffset = offset;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Returns the target offset. */
s32 sub_801B980(struct follow_child *self)
{
    return self->targetOffset;
}

/* Factory for the 0x78-byte sprite (method table gStaticData_087E4B34,
 * constructor inlined): places it at (x, y) pixels with record id `id`,
 * registers it with gUnknown_030012F0, and starts animation 0 of the
 * table at `**gUnknown_030012D0 + 0x150`. Called from sub_8021668's
 * family (graphics_loading_21668.c). The two mask constants are
 * materialized with `mov/neg` asm like graphics_loading_21668.c's
 * sub_8021668, since the compiler otherwise derives them from constants
 * already in registers. */
struct sprite *sub_801B984(u16 id, u16 x, u16 y)
{
    struct sprite *obj = sub_8026EDC(0x78);

    sub_8009F90(obj);
    obj->vtable = (struct method *)gStaticData_087E4B34;
    sub_801BAC4(obj);
    obj->id = id;
    obj->x = x << 8;
    obj->y = y << 8;
    sub_8008E94(gUnknown_030012F0, obj);
    obj->anim = (struct anim_table *)(**gUnknown_030012D0 + 0x150);
    obj->animIndex = 0;
    sub_80087C0(obj);
    sub_80087B4(obj);
    sub_800872C(obj, 0);
    {
        u8 *p28 = (u8 *)obj + 0x28;
        s32 m;
        asm("mov %0, #0x11\n\tneg %0, %0" : "=r"(m));
        m &= *p28;
        m &= -0x21;
        *p28 = m;
    }
    {
        struct anim_record *recs = obj->anim->records;
        u32 idx = obj->animIndex;
        struct anim_record *rec = &recs[idx];
        s32 pal = (u8)sub_8006DF8(gUnknown_030012B8, rec->tileRecord);
        s32 m;
        u8 *p = (u8 *)obj + 0x29;
        u8 b;
        pal &= 0xF;
        asm("mov %0, #0x10\n\tneg %0, %0" : "=r"(m));
        b = *p;
        m &= b;
        m |= pal;
        *p = m;
    }
    return obj;
}

/* Method table +0x70: if the player is active (flags bit 7) and overlaps
 * this sprite's hit box, fires the player's method 13 with (0, 0x19, 0). */
void sub_801BA60(void *self)
{
    struct hit_box box;

    if (gUnknown_030012D8->flags >> 7)
    {
        sub_8007B98(&box, self);
        if (box.unk_08 != 0 && sub_800B37C(gUnknown_030012D8, &box))
        {
            struct player *p = gUnknown_030012D8;
            struct method *m = &p->vtable[13];
            void *addr = (u8 *)p + m->thisOffset;
            register void *fn asm("r4") = *(void *volatile *)&m->fn;

            sub_803AD88(addr, 0, 0x19, 0);
            (void)fn;
        }
    }
}

/* Destructor (method table +0x50). */
void sub_801BAB0(struct sprite *self, s32 flags)
{
    self->vtable = (struct method *)gStaticData_087E4B34;
    sub_8009F1C(self, flags);
}

/* Clears flags bit 6. */
void sub_801BAC4(struct sprite *self)
{
    self->flags &= Opaque(~0x40);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan; sub_801B984 inlines it instead).
 * Constructor. */
struct sprite *sub_801BAD0(struct sprite *self)
{
    sub_8009F90(self);
    self->vtable = (struct method *)gStaticData_087E4B34;
    sub_801BAC4(self);
    return self;
}

/* The level-select screen, modal (called from game_loop55.c): resets the
 * display, palette, VRAM cursor and both text-icon managers (the same
 * setup as sub_80062A8), builds the menu for level `*arg`, runs it, stores
 * the chosen level back through `arg`, tears the menu down and returns
 * its `result` byte. The icon-manager steps are inline helpers: the ROM
 * recomputes every field address after each call instead of keeping the
 * offsets in registers, which is what separate inlined expansions give. */
static inline void IconSetup(struct icon_manager *m, u32 v)
{
    struct icon_slot *slot;

    m->field_108 = v;
    slot = &m->record->slots[6];
    sub_803AD7C((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct icon_manager **m)
{
    struct vram_cursor *c = gUnknown_030012FC;

    sub_8006C58(c, (*m)->field_12c << 5);
}

static inline void LoadMenuPalette(struct tile_cache *cache)
{
    sub_803A94C(gStaticData_0816C56C, cache->palette, 0x10);
}

u8 sub_801BAF0(s32 *arg)
{
    struct level_menu *menu;
    u8 result;
    s32 heaps = 0xC0000000;

    mem_free_bytes(heaps);
    sub_80006A8();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    sub_8006EA8(gUnknown_030012B8);
    sub_8006D50(gUnknown_030012B8, 0xF);
    LoadMenuPalette(gUnknown_030012B8);
    gUnknown_030012FC->unk_08 = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);
    IconSetup(gUnknown_030012DC, 0);
    IconReserve(&gUnknown_030012DC);
    {
        u32 v = gUnknown_030012DC->field_12c;

        IconSetup(gUnknown_030012E0, v);
    }
    IconReserve(&gUnknown_030012E0);
    sub_8006C30(gUnknown_030012FC);
    sub_8001B54(gUnknown_030012BC, 0x10);
    {
        struct level_menu **menuAddr = &gUnknown_03000820;

        *menuAddr = sub_801BC28(sub_8026EDC(0xAC), *arg);
        *arg = sub_801C96C(*menuAddr);
        menu = *menuAddr;
        result = menu->result;
        if (menu != NULL)
            sub_801C040(menu, 3);
        *menuAddr = NULL;
    }
    sub_8006EA8(gUnknown_030012B8);
    mem_free_bytes(heaps);
    return result;
}

/* Constructor: blend/display shadow registers (alpha blend, evy 16,
 * mode 1, BG0/BG1/OBJ), page/cursor from `arg` (5 levels per page, 20+
 * is the last page), the save block, both BG layers, the cursor panel,
 * six level entries and ten sprites, then the cursor position and the
 * BG registers.
 *
 * Under old_agbcc the shadow registers are plain bitfield stores (the
 * compiler chains the ORs itself); the six-entry loop needs its own
 * counter and the sprite loop's 0x80 a variable set with the counter,
 * for the ROM's register choice and hoisted constant. */
struct level_menu *sub_801BC28(struct level_menu *self, s32 arg)
{
    u8 bg0cnt[0x10];
    s32 i;
    struct sprite *s;

    self->blend.raw = 0;
    self->blend.bits.effect = 3;
    self->blend.bits.bdFirst = 1;
    self->blend.bits.bg0First = 1;
    self->blend.bits.bg1First = 1;
    self->blend.bits.bg2First = 1;
    self->blend.bits.bg3First = 1;
    self->blend.bits.objFirst = 1;
    self->bldy.evy = 16;
    *(vu32 *)REG_ADDR_BLDCNT = self->blend.raw;
    *(vu16 *)REG_ADDR_BLDY = self->bldy.evy;
    self->dispcnt.raw = 0;
    self->dispcnt.bits.obj1d = 1;
    self->dispcnt.bits.mode = 1;
    self->dispcnt.bits.bg0 = 1;
    self->dispcnt.bits.bg1 = 1;
    self->dispcnt.bits.obj = 1;
    if (arg <= 0x13)
    {
        self->world = sub_803ADB4(arg, 5);
        self->index = sub_803AE4C(arg, 5);
    }
    else
    {
        self->world = arg - 0x14;
        self->index = 5;
    }
    self->nameText = 0;
    self->save = sub_80236EC(gLevelState);
    self->result = 0;
    self->bg1 = sub_801D7F8(sub_8026EDC(0x28), 0, 0x1D);
    sub_801E644(bg0cnt, 2, 0x1E, 2, 3);
    LoadGraphicsPackage(bg0cnt, gStaticData_0816C484);
    self->scroll = 0;
    self->panel = sub_801E04C(sub_8026EDC(0x54));
    self->bg2 = sub_801D828(sub_8026EDC(0x8C), 3, 0x1F);
    {
        s32 j;

        for (j = 0; j < 6; j++)
            self->items[j] = sub_801DFEC(sub_8026EDC(0x14));
    }
    sub_801D638(self);
    sub_801D5CC(self);
    sub_801D668(self);
    {
        s32 v;

        for (i = 0, v = 0x80; i < 8; i++)
        {
            s = sub_8008904(sub_8026EDC(0x40));
            self->sprites[i] = s;
            sub_80088D8(s, 1);
            if (i > 1)
                self->sprites[i]->unk_3C = v;
        }
    }
    self->sprites[0]->anim = AnimTable(0x234);
    SetAnim(self->sprites[0], gStaticData_0816C548[self->world]);
    sub_800737C(self->sprites[0], gStaticData_0816C498.x, gStaticData_0816C498.y);
    self->sprites[1]->anim = AnimTable(0x234);
    SetAnim(self->sprites[1], 10);
    sub_800737C(self->sprites[1], gStaticData_0816C4A0.x, gStaticData_0816C4A0.y);
    self->sprites[2]->anim = AnimTable(0x1BC);
    sub_800737C(self->sprites[2], gStaticData_0816C4A8.x, gStaticData_0816C4A8.y);
    self->sprites[3]->anim = AnimTable(0x180);
    SetAnim(self->sprites[3], 1);
    sub_800737C(self->sprites[3], gStaticData_0816C4B0.x, gStaticData_0816C4B0.y);
    self->sprites[4]->anim = AnimTable(0x180);
    SetAnim(self->sprites[4], 1);
    sub_800737C(self->sprites[4], gStaticData_0816C4B0.x, gStaticData_0816C4B0.y);
    self->sprites[5]->anim = AnimTable(0x18C);
    sub_800737C(self->sprites[5], gStaticData_0816C4B8.x, gStaticData_0816C4B8.y);
    self->sprites[6]->anim = AnimTable(0x18C);
    sub_800737C(self->sprites[6], gStaticData_0816C4B8.x, gStaticData_0816C4B8.y);
    self->sprites[7]->anim = AnimTable(0x18C);
    sub_800737C(self->sprites[7], gStaticData_0816C4C0.x, gStaticData_0816C4C0.y);
    s = sub_8008904(sub_8026EDC(0x40));
    self->sprites[8] = s;
    sub_80088D8(s, 1);
    self->sprites[8]->anim = AnimTable(0x270);
    SetAnim(self->sprites[8], 1);
    sub_800737C(self->sprites[8], gStaticData_0816C4C8.x, gStaticData_0816C4C8.y);
    s = sub_8008904(sub_8026EDC(0x40));
    self->sprites[9] = s;
    sub_80088D8(s, 1);
    self->sprites[9]->anim = AnimTable(0x270);
    SetAnim(self->sprites[9], 0);
    sub_800737C(self->sprites[9], gStaticData_0816C4D0.x, gStaticData_0816C4D0.y);
    if (gUnknown_03000824 && sub_801D434(self))
    {
        sub_801E408(self->panel);
    }
    else
    {
        struct xy_pair *pos = &self->positions[self->index];

        sub_801E480(self->panel, pos->x, pos->y - 0x18);
    }
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    *(vu32 *)REG_ADDR_BG1HOFS = sub_801D7D0(self->bg1);
    *(vu16 *)REG_ADDR_BG0CNT = sub_801E640(bg0cnt);
    *(vu16 *)REG_ADDR_BG1CNT = sub_801E640(self->bg1);
    *(vu16 *)REG_ADDR_BG2CNT = sub_801DE24(self->bg2);
    return self;
}

/* Destructor: deletes the sprites, panel, BG layers and level entries
 * through their own destructors; frees itself if `flags & 1`. */
void sub_801C040(struct level_menu *self, s32 flags)
{
    struct sprite *s;
    s32 i;

    if ((s = self->sprites[9]) != NULL)
        SPRITE_CALL(s, 10, 3);
    if ((s = self->sprites[8]) != NULL)
        SPRITE_CALL(s, 10, 3);
    for (i = 0; i < 8; i++)
    {
        if ((s = self->sprites[i]) != NULL)
            SPRITE_CALL(s, 10, 3);
    }
    if (self->panel != NULL)
        sub_801E524(self->panel, 3);
    if (self->bg2 != NULL)
        sub_801DA38(self->bg2, 3);
    for (i = 0; i < 6; i++)
    {
        struct item *it = self->items[i];

        if (it != NULL)
            sub_803AD80((u8 *)it + it->vtable->m28.thisOffset, 3, it->vtable->m28.fn);
    }
    if (self->bg1 != NULL)
        sub_801D7E0(self->bg1, 3);
    if (flags & 1)
        sub_8026ED0(self);
}

/* Per-frame update: draws the selected level's name centred at the top
 * (gUnknown_030012E0) and its record panel, updates every entry, and once
 * the BG1 page has settled draws text 0x2F centred at y=0x96 (the first
 * time only, with the page arrows) and steps BG2; BG2's DISPCNT enable
 * bit follows sub_801DD28. */
void sub_801C104(struct level_menu *self)
{
    s32 i;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    sub_801E2BC(self->panel);
    if (sub_801DD18(self->bg2) && sub_801DE28(self->items[self->index]))
    {
        struct icon_slot *slot = &gUnknown_030012E0->record->slots[0];
        u32 x = (u32)(0xF0 - sub_803AD80((u8 *)gUnknown_030012E0 + slot->offset, self->nameText, slot->ptr)) >> 1;

        SetIconPos(gUnknown_030012E0, x, -self->unk_80 + 2);
        slot = &gUnknown_030012E0->record->slots[2];
        sub_803AD80((u8 *)gUnknown_030012E0 + slot->offset, self->nameText, slot->ptr);
        if (self->index <= 4)
            sub_801C364(self);
    }
    sub_801D7D4(self->bg1);
    for (i = 0; i <= self->lastIndex; i++)
    {
        struct item *it = self->items[i];
        struct method *m = &it->vtable->m08;

        sub_803AD80((u8 *)it + m->thisOffset, sub_801D77C(self->bg1), m->fn);
    }
    if (sub_801D780(self->bg1))
    {
        if (!sub_801DCF8(self->bg2))
        {
            s32 text = sub_8026F38(0x2F);
            struct icon_slot *slot = &gUnknown_030012DC->record->slots[0];
            u32 x = (u32)(0xF0 - sub_803AD80((u8 *)gUnknown_030012DC + slot->offset, text, slot->ptr)) >> 1;

            SetIconPos(gUnknown_030012DC, x, 0x96);
            sub_8028A30(gUnknown_030012DC, 0xF);
            slot = &gUnknown_030012DC->record->slots[2];
            sub_803AD80((u8 *)gUnknown_030012DC + slot->offset, text, slot->ptr);
            sub_801C2B0(self);
        }
        sub_801DC28(self->bg2);
    }
    if (sub_801DD28(self->bg2))
    {
        u8 *q = (u8 *)&self->dispcnt + 1;
        s32 m = -5;

        m &= *q;
        *q = m;
    }
    else
    {
        u8 *q = (u8 *)&self->dispcnt + 1;
        s32 m = 4;

        m |= *q;
        *q = m;
    }
    sub_8006A48(gUnknown_03001300);
}

/* Updates the two page-arrow sprites (8/9): palettes from sub_800815C,
 * frame 0/1 by whether the previous/next page is open. */
void sub_801C2B0(struct level_menu *self)
{
    register s32 lowMask asm("r5");
    register s32 highMask asm("r4");

    {
        register s32 pal asm("r0") = sub_800815C(self->sprites[8]);
        u8 *p = (u8 *)self->sprites[8] + 0x29;
        register s32 m asm("r1");
        register s32 b asm("r3");

        lowMask = 0xF;
        pal &= lowMask;
        highMask = -0x10;
        m = highMask;
        asm("" : "+r"(m));
        b = *p;
        m &= b;
        m |= pal;
        *p = m;
    }
    {
        register s32 pal asm("r0") = sub_800815C(self->sprites[9]);
        u8 *p = (u8 *)self->sprites[9] + 0x29;
        register s32 b asm("r5");

        pal &= lowMask;
        b = *p;
        highMask &= b;
        highMask |= pal;
        *p = highMask;
    }
    if (self->world <= 2)
    {
        register struct sprite *s asm("r1");
        register s32 f asm("r4");

        if (sub_801D434(self))
        {
            s = self->sprites[8];
            f = 0;
        }
        else
        {
            s = self->sprites[8];
            f = 1;
        }
        {
            register struct anim_table *a asm("r0") = s->anim;
            register u8 *pi asm("r3") = &s->animIndex;
            register struct anim_record *recs asm("r2") = a->records;
            register u32 idx asm("r5") = *pi;
            register struct anim_record *rec asm("r0") = (struct anim_record *)(idx * sizeof(struct anim_record) + (u32)recs);
            register s32 n asm("r2") = rec->frameCount;
            register struct sprite *t asm("r0") = s;

            if (f >= n)
                f = n - 1;
            t->frame = f;
            sub_8008890(t, 0, 0);
        }
    }
    if (sub_801D428(self))
    {
        register struct sprite *s asm("r3") = self->sprites[9];
        register s32 f asm("r4") = 0;
        register struct anim_table *a asm("r0") = s->anim;
        register u8 *pi asm("r2") = &s->animIndex;
        register struct anim_record *recs asm("r1") = a->records;
        register u32 idx asm("r5") = *pi;
        register struct anim_record *rec asm("r0") = (struct anim_record *)(idx * sizeof(struct anim_record) + (u32)recs);
        register s32 n asm("r0") = rec->frameCount;

        if (f >= n)
            f = n - 1;
        s->frame = f;
        sub_8008890(s, 0, 0);
    }
}

/* Draws the record panel sprites 0-4 at their per-row offsets and, if
 * the level is cleared, its time readout (sub_801C3E8). */
void sub_801C364(struct level_menu *self)
{
    sub_8008890(self->sprites[0], -self->unk_80, 0);
    sub_8008890(self->sprites[1], -self->unk_80, 0);
    sub_8008890(self->sprites[2], -self->unk_80, self->unk_84);
    sub_8008890(self->sprites[3], -self->unk_80, self->unk_88);
    if (self->rank != 5)
        sub_8008890(self->sprites[4], -self->unk_80, self->unk_8C);
    {
        u8 **ps = &self->save;
        s32 off = self->levelId * 4 + 4;
        struct level_save *sv = (struct level_save *)(*ps + off);
        s32 one = 1;
        s32 b = *(u8 *)sv;

        one &= b;
        if (one != 0)
            sub_801C3E8(self, sv->time);
    }
}

/* Draws the time readout: just the best time if it beats the tightest
 * threshold (time2), otherwise sprite 7, the next threshold to beat and
 * the best time. */
void sub_801C3E8(struct level_menu *self, u32 time)
{
    struct level_info *info;

    sub_8008890(self->sprites[5], -self->unk_80, self->unk_90);
    sub_8008890(self->sprites[6], -self->unk_80, self->unk_94);
    info = &gLevelTable[self->levelId];
    if (time != 0 && time <= info->time2)
    {
        struct icon_slot *slot;

        SetIconPos(gUnknown_030012E0, self->unk_80 + gStaticData_0816C4C0.x + 10, gStaticData_0816C4C0.y - 8);
        slot = &gUnknown_030012E0->record->slots[2];
        sub_803AD80((u8 *)gUnknown_030012E0 + slot->offset, (s32)self->timeText, slot->ptr);
    }
    else
    {
        struct icon_slot *slot;

        sub_8008890(self->sprites[7], self->unk_80, 0);
        sub_8028A30(gUnknown_030012E0, self->sprites[7]->palette);
        SetIconPos(gUnknown_030012E0, self->unk_80 + gStaticData_0816C4C0.x + 10, gStaticData_0816C4C0.y - 8);
        slot = &gUnknown_030012E0->record->slots[2];
        sub_803AD80((u8 *)gUnknown_030012E0 + slot->offset, (s32)self->recordText, slot->ptr);
        sub_8028A40(gUnknown_030012E0);
        SetIconPos(gUnknown_030012E0, self->unk_80 + gStaticData_0816C4C0.x + 10, gStaticData_0816C4C0.y + 8);
        slot = &gUnknown_030012E0->record->slots[2];
        sub_803AD80((u8 *)gUnknown_030012E0 + slot->offset, (s32)self->timeText, slot->ptr);
    }
}

/* Per-frame draw step: once the BG1 page has settled and the cursor
 * panel has arrived on a new entry, selects it and loads that level's
 * name and record (sub_801C608); then draws the six entries and record
 * sprites 2-7. */
void sub_801C51C(struct level_menu *self)
{
    struct item **items;
    struct sprite **sprites;
    s32 i;

    sub_801D7AC(self->bg1);
    sub_801E190(self->panel);
    if (!sub_801D780(self->bg1))
        return;
    {
        s32 done = sub_801E464(self->panel) << 24;
        items = self->items;
        if (!done)
            goto draw;
    }
    {
        if (!sub_801DE28(ItemAt(items, self->index)))
        {
            struct item *it = ItemAt(items, self->index);

            sub_801DEA0(it, 1);
            if (!sub_801DD18(self->bg2))
            {
                struct level_info *info;

                self->levelId = sub_801DE2C(it);
                info = &gLevelTable[self->levelId];
                sub_801DD80(self->bg2, info->unk_04);
                self->nameText = sub_8026F38(info->nameText);
            }
            self->unk_80 = 0;
            if (self->index <= 4)
            {
                sub_801C608(self);
                sub_8006EA8(gUnknown_030012B8);
                sub_801D730(self);
            }
            sub_8028A40(gUnknown_030012E0);
        }
    }
draw:
    sprites = self->sprites;
    for (i = 5; i >= 0; i--)
    {
        struct item *it = *items++;
        struct item_vtable *vt = it->vtable;

        sub_803AD7C((u8 *)it + vt->m20.thisOffset, vt->m20.fn);
    }
    sub_801DAD8(self->bg2);
    {
        struct sprite **p = sprites + 2;

        for (i = 5; i >= 0; i--)
            sub_8008044(*p++);
    }
}

/* Loads the selected level's record into the panel: its `rank` (first
 * of five save predicates that holds, 5 if none), which flag icons to
 * show (y offsets 0 or 0x1C), and the time-trial texts/sprites.
 *
 * old_agbcc, with u16 save bitfields, u32 thresholds and an int SetAnim
 * index. The ROM spills a second copy of the record pointer to sp+0 and
 * reloads it for the `time0` test: that is the separate `entry` local,
 * which `info` copies (a single local keeps it in r5 throughout). */
void sub_801C608(struct level_menu *self)
{
    s32 *rank = &self->rank;
    struct level_save *sv;

    *rank = 5;
    if (sub_802336C(gLevelState, self->levelId))
        *rank = 0;
    if (sub_8023360(gLevelState, self->levelId))
        *rank = 1;
    if (sub_8023354(gLevelState, self->levelId))
        *rank = 2;
    if (sub_8023348(gLevelState, self->levelId))
        *rank = 3;
    if (sub_802333C(gLevelState, self->levelId))
        *rank = 4;
    self->unk_84 = 0;
    self->unk_88 = 0;
    self->unk_8C = 0;
    self->unk_90 = 0;
    self->unk_94 = 0;
    sv = (struct level_save *)(self->save + (self->levelId * 4 + 4));
    if (sv->cleared)
        self->unk_84 = 0x1C;
    if (sv->flag1)
        self->unk_88 = 0x1C;
    switch (*rank)
    {
    case 0:
        if (sv->flag2)
            self->unk_8C = 0x1C;
        break;
    case 1:
        if (self->save[2] & 1)
            self->unk_8C = 0x1C;
        break;
    case 2:
        if (self->save[2] & 4)
            self->unk_8C = 0x1C;
        break;
    case 3:
        if (self->save[2] & 8)
            self->unk_8C = 0x1C;
        break;
    case 4:
        if (self->save[2] & 2)
            self->unk_8C = 0x1C;
        break;
    case 5:
        break;
    default:
        goto set_rank_icon;
    }
    if (self->rank != 5)
    {
    set_rank_icon:
        SetAnim(self->sprites[4], gStaticData_0816C558[self->rank]);
        if (self->unk_88 == self->unk_8C)
        {
            self->unk_88 -= 6;
            self->unk_8C += 6;
        }
    }
    if (sv->cleared)
    {
        struct level_info *entry = &gLevelTable[self->levelId];
        struct level_info *info = entry;

        FormatCentiseconds(info->time0, self->recordText);
        FormatCentiseconds(sv->time, self->timeText);
        SetAnim(self->sprites[5], 0);
        SetAnim(self->sprites[6], 0);
        SetAnim(self->sprites[7], 0);
        if (sv->time != 0)
        {
            if (sv->time <= info->time2)
            {
                self->unk_94 = 0x1C;
                self->unk_90 = 0x1C;
                SetAnim(self->sprites[5], 1);
                SetAnim(self->sprites[6], 1);
            }
            else if (sv->time <= info->time1)
            {
                FormatCentiseconds(info->time2, self->recordText);
                self->unk_90 = 0x1C;
                SetAnim(self->sprites[5], 2);
                SetAnim(self->sprites[6], 1);
                SetAnim(self->sprites[7], 1);
            }
            else if (sv->time <= entry->time0)
            {
                FormatCentiseconds(info->time1, self->recordText);
                self->unk_90 = 0x1C;
                SetAnim(self->sprites[5], 0);
                SetAnim(self->sprites[6], 2);
                SetAnim(self->sprites[7], 2);
            }
        }
    }
}

/* The menu loop: fades in (BLDY), then runs frames until A is pressed on
 * an open entry (sub_801D110) or Start exits (sub_801D300), dispatching
 * Up/Down page turns (sub_801D548/sub_801D4C4) and Left/Right cursor
 * moves (sub_801CDE0/sub_801CE60); returns the selected entry's level.
 *
 * Matched under old_agbcc in the near-miss polish pass
 * (docs/matching/near-miss-polish.md): the key-word copy the ROM makes
 * inside the 0x80 test comes from a statement expression holding the
 * copy plus an empty `asm` that keeps gcc from merging it. */
s32 sub_801C96C(struct level_menu *self)
{
    struct level_info *info;

    self->result = 0;
    self->levelId = sub_801DE2C(self->items[self->index]);
    info = &gLevelTable[self->levelId];
    sub_801DD80(self->bg2, info->unk_04);
    self->nameText = sub_8026F38(info->nameText);
    while (!sub_801DD18(self->bg2))
    {
        {
            struct bldy_byte *f = (struct bldy_byte *)&self->bldy;

            if (f->evy != 0)
                f->evy--;
        }
        sub_801C104(self);
        sub_80006A8();
        sub_8006DC8(gUnknown_030012B8);
        sub_8006AAC(gUnknown_03001300);
        CommitDisplay(self);
        sub_801DAD8(self->bg2);
    }
    PlaySfx(gUnknown_030012BC, 0x51, 0x100);
    self->blend.raw = 0;
    self->blend.bits.bg0Second = 1;
    self->blend.bits.bg1Second = 1;
    self->blend.bits.bg2Second = 1;
    self->blend.bits.bg3Second = 1;
    self->blend.bits.bdSecond = 1;
    self->blend.bits.eva = 0x10;
    self->blend.bits.evb = 0x10;
    if (gUnknown_03000824 && sub_801D434(self))
    {
        self->index = 0;
        sub_801D548(self);
    }
    gUnknown_03000824 = 0;
    goto loop;

check_exit:
    if (gUnknown_030007E0.half.pressed & 8)
    {
        sub_801D300(self);
        goto end;
    }
loop:
    sub_801C104(self);
    sub_80006A8();
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    CommitDisplay(self);
    sub_801C51C(self);
    if (!sub_801D780(self->bg1))
        goto loop;
    if (!(u8)sub_801E464(self->panel))
        goto loop;
    sub_80007AC(gUnknown_03001304);
    {
        union key_state keys = gUnknown_030007E0;
        union key_state k;

        if (keys.half.pressed & 0x40)
            sub_801D548(self);
        /* The ROM copies the key word between the 0x80 test's `ands`
         * and its `cmp`, and tests 0x20 on the copy; gcc merges a plain
         * copy, so the (code-free) asm keeps `k` a separate value. */
        else if (({
                     u32 hit = keys.half.pressed & 0x80;

                     k = keys;
                     asm("" : "+r"(k.all));
                     hit;
                 }))
            sub_801D4C4(self);
        else
        {
            if (k.half.pressed & 0x20)
                sub_801CDE0(self);
            else if (keys.half.pressed & 0x10)
                sub_801CE60(self);
        }
    }
    if (!(gUnknown_030007E0.half.pressed & 1))
        goto check_exit;
    if (!sub_801DE28(self->items[self->index]))
        goto check_exit;
    if (!sub_801DD18(self->bg2))
        goto check_exit;
    sub_801D110(self);
end:
    self->dispcnt.raw = 0;
    self->dispcnt.bits.obj1d = 1;
    sub_80006A8();
    sub_8006DC8(gUnknown_030012B8);
    sub_8006AAC(gUnknown_03001300);
    CommitDisplay(self);
    return sub_801DE2C(self->items[self->index]);
}

/* Deselects the current entry and runs frames until BG2 and the cursor
 * panel settle. Called by the page-turn handlers (sub_801D4C4/D548). */
void sub_801CCF8(struct level_menu *self)
{
    sub_801DEA0(self->items[self->index], 0);
    sub_801DD5C(self->bg2);
    sub_801E408(self->panel);
    while (sub_801DD38(self->bg2) || !(u8)sub_801E464(self->panel))
    {
        sub_801C104(self);
        sub_80006A8();
        sub_8006DC8(gUnknown_030012B8);
        sub_8006AAC(gUnknown_03001300);
        CommitDisplay(self);
        sub_801E190(self->panel);
        sub_801DAD8(self->bg2);
    }
    sub_8006EA8(gUnknown_030012B8);
}

/* Moves the cursor left, repeating while Left is held; sound 0x48 at the
 * first entry. */
void sub_801CDE0(struct level_menu *self)
{
    if (self->index == 0)
    {
        PlaySfx(gUnknown_030012BC, 0x48, 0x100);
        return;
    }
    sub_801DEA0(self->items[self->index], 0);
    sub_801DD5C(self->bg2);
    while (self->index != 0)
    {
        struct xy_pair *pos;

        self->index--;
        pos = &self->positions[self->index];
        sub_801E480(self->panel, pos->x, pos->y - 0x18);
        sub_801D05C(self);
        sub_80007AC(gUnknown_03001304);
        if (!(gUnknown_030007E0.all & 0x20))
            return;
    }
}

/* Moves the cursor right, repeating while Right is held; sound 0x48 at
 * the last entry. */
void sub_801CE60(struct level_menu *self)
{
    if (self->index == self->lastIndex)
    {
        PlaySfx(gUnknown_030012BC, 0x48, 0x100);
        return;
    }
    sub_801DEA0(self->items[self->index], 0);
    sub_801DD5C(self->bg2);
    while (self->index < self->lastIndex)
    {
        struct xy_pair *pos;

        self->index++;
        pos = &self->positions[self->index];
        sub_801E480(self->panel, pos->x, pos->y - 0x18);
        sub_801D05C(self);
        sub_80007AC(gUnknown_03001304);
        if (!(gUnknown_030007E0.all & 0x10))
            return;
    }
}
