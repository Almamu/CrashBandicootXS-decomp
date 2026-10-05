#include "core.h"
#include "bitmap_font.h"
#include <agb_syscall.h>
#include "text.h"
#include "util.h"
#include <libgcc.h>

/* GitHub issue #26: 0x0801B85C-0x0801CEE0, the whole of the former
 * asm/code_3_2_17_188d0_1b85c.s. Three objects, all gcc 2.x C++ classes
 * (a method table at +0x18/+0x10, virtual calls through the
 * _call_via_r1/AD80/AD88 call-via-register thunks, inlined member
 * functions):
 *
 * - sub_801B85C-GetCameraLeadOffset: `struct follow_child`, the 0x80-byte
 *   object (method table gCameraLeadVtable) InputCtrlStateStart
 *   (input_ctrl.c) spawns for the input controller. It trails the
 *   player (gPlayer) at a horizontal offset that eases 2 px per
 *   frame toward a clamped target, and registers itself as
 *   gCamera's follow target while alive.
 * - SpawnLaunchPad-InitLaunchPad: a 0x78-byte sprite subclass (method table
 *   gLaunchPadVtable) with a factory, a player-overlap check that
 *   fires the player's method 13 (0x19), constructor and destructor:
 *   the green pad of entity type 0x3D (sprite bank 28). Player event
 *   0x19 makes the action controller launch Crash upward in an air
 *   spin with a full tornado charge (Y motion 0x13, or 0x14 with A held).
 * - RunLevelSelect-LevelSelectCursorRight: `struct level_menu`, the paged level-select
 *   screen (docs/rom_map.md: "a paged menu/screen with a smooth
 *   horizontal page-turn animation"). RunLevelSelect is the whole modal
 *   screen: it builds the menu (InitLevelSelect), runs it (LevelSelectLoop)
 *   and returns the chosen level through `*arg`. Five levels per page
 *   (`arg / 5`, `arg % 5`); each level's fixed record (name text, three
 *   time-trial thresholds) is gLevelTable, its saved record a
 *   bitfield word (cleared flag, two more flags, best time). The screen
 *   keeps shadow copies of BLDCNT/BLDALPHA/BLDY/DISPCNT and commits them
 *   with the scroll registers every frame (`CommitDisplay`).
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, expected/ or src/:
 * sub_801B85C, SetCameraLeadOffset, GetCameraLeadOffset, InitLaunchPad (reachable only
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

COMPILE_TIME_ASSERT(level_select_c, sizeof(struct follow_child) == 0x80);

/* gPlayer, only the fields used here. */
struct player
{
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    u8 unk_08[4];
    u8 flags;               // 0x0C - bit 7 tested by CheckLaunchPadContact
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
    u8 paletteId;          // 0x14 - GetPaletteSlot record id
    u8 unk_15;
    u8 frameCount;          // 0x16
    u8 unk_17[5];
};

COMPILE_TIME_ASSERT(level_select_c, sizeof(struct anim_record) == 0x1C);

struct anim_table
{
    struct anim_record *records;
};

/* The 0x40-byte animated sprite part `InitUiSpriteObj` constructs, and the
 * base of the 0x78-byte `SpawnLaunchPad` object. */
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

COMPILE_TIME_ASSERT(level_select_c, sizeof(struct sprite) == 0x40);

/* One level's fixed data (`gLevelTable`, 36-byte records,
 * indexed by level id). */
struct level_info
{
    s32 nameText;           // 0x00 - text id (GetUiText)
    s32 theme;              // 0x04 - also the SetZoomBgPicture image
    u32 time0;              // 0x08 - time-trial thresholds, centiseconds,
    u32 time1;              // 0x0C   loosest first
    u32 time2;              // 0x10
    u8 unk_14[0x10];
};

COMPILE_TIME_ASSERT(level_select_c, sizeof(struct level_info) == 0x24);

/* One level's saved record word (`level_menu.save + 4 + id * 4`; byte 2
 * of the save block itself holds four more flags LoadLevelSelectRecord tests). */
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
    struct method m08;          // 0x08 - per-frame update (UpdateLevelSelect)
    struct method m10;          // 0x10
    struct method m18;          // 0x18
    struct method m20;          // 0x20 - draw (DrawLevelSelect)
    struct method m28;          // 0x28 - destructor (DestroyLevelSelect)
};

/* One level entry on the current page (CreateLevelSelectEntry, 0x14 bytes). */
struct item
{
    u8 unk_00[0x10];
    struct item_vtable *vtable; // 0x10
};

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
    void *bg1;                  // 0x1C - CreateLevelSelectPageBg, BG1
    void *bg2;                  // 0x20 - InitZoomBg, BG2 (the level picture)
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
    s32 rank;                   // 0x98 - LoadLevelSelectRecord's classification, 5 = none
    u8 *save;                   // 0x9C - PackSaveData's save block
    union blend blend;          // 0xA0 - REG_BLDCNT + REG_BLDALPHA
    struct bldy bldy;           // 0xA4 - REG_BLDY
    union dispcnt dispcnt;      // 0xA8 - REG_DISPCNT
};

COMPILE_TIME_ASSERT(level_select_c, sizeof(struct level_menu) == 0xAC);

struct vram_cursor
{
    u8 unk_00[8];
    u32 baseTile;
};

/* gPaletteCache, only the field used here. */
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

extern struct player *gPlayer;
extern struct follow_owner *gCamera;
extern void ***gSpriteBankSet;
extern void *gCollidableList;
extern struct tile_cache *gPaletteCache;
extern void *gAudioContext;
extern void *gLevelState;
extern struct vram_cursor *gObjVramCursor;
extern void *gOamBuffer;
extern void *gInput;
extern struct level_menu *gLevelSelect;
extern u8 gNewWorldOpened;
extern union key_state gKeys;
extern u8 gCameraLeadVtable[];
extern u8 gLaunchPadVtable[];
extern struct level_info gLevelTable[];
extern u8 gLevelSelectPalette[];
extern u8 gMenuSkyBg[];
extern u32 gLevelSelectWorldAnims[];
extern u32 gLevelSelectRankAnims[];
extern struct xy_pair gLevelSelectWorldPos;
extern struct xy_pair gStaticData_0816C4A0;
extern struct xy_pair gLevelSelectCrystalPos;
extern struct xy_pair gLevelSelectGemPos;
extern struct xy_pair gLevelSelectTrialIconPos;
extern struct xy_pair gLevelSelectTimePos;
extern struct xy_pair gStaticData_0816C4C8;
extern struct xy_pair gStaticData_0816C4D0;

/* Base class and runtime. */
extern void InitMovingSprite(void *self);
extern void DestroyMovingSprite(void *self, s32 flags);
extern void UpdateMovingSprite(void *self);
extern void *OperatorNew(u32 size);
extern void OperatorDelete(void *p);
extern void _call_via_r1(void *self, void *fn);
extern s32 _call_via_r2(void *self, s32 arg, void *fn);
/* Calls the function in r4 with r0-r3 (see the `register ... asm("r4")`
 * pin at the call site). */
extern void _call_via_r4(void *self, s32 a, s32 b, s32 c);
extern s32 mem_free_bytes(s32 flags);

/* Sprite parts. */
extern struct sprite *InitUiSpriteObj(void *mem);
extern void AddToPartList(void *manager, void *value);
extern void ResetSpriteFrameTimer(void *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteAnimDone(void *part, s32 arg);
extern void SetSpritePriority(void *part, s32 value);
extern void SetEntityPixelPos(void *part, s32 x, s32 y);
extern void DrawSpriteWithOffset(void *part, s32 dx, s32 dy);
extern s32 GetSpriteAnimPaletteSlot(void *part);
extern void AdvanceSpriteAnim(void *p);
/* Really returns a u8 (src/gfx/graphics.c), but the call site
 * re-zero-extends the result, as it would through a wider return type. */
extern s32 GetPaletteSlot(void *cache, u8 recordId);
extern void GetSpriteHitbox(struct aabb *dest, void *part);
extern u8 PlayerTouchesBox(void *actor, struct aabb *box);

/* Display, VRAM and sound. */
extern void WaitForVBlank(void);
extern void UpdateKeys(void *p);
extern void FreeUnlockedPaletteSlots(void *cache);
extern void ClaimPaletteSlot(void *cache, s32 arg);
extern void UploadPaletteCache(void *p);
extern void CommitOamBuffer(void *p);
extern void ResetOamBuffer(void *p);
extern void HideUnusedOamEntries(void *p);
extern void ResetObjVram(struct vram_cursor *self);
extern s32 ReserveObjVram(struct vram_cursor *self, s32 size);
extern void MarkObjVram(struct vram_cursor *self);
extern void RewindObjVram(struct vram_cursor *p);
extern void FlushVramDmaQueue(void);
extern void InitBgSetup(void *dst, s32 a, s32 b, s32 c, s32 d);
extern void LoadGraphicsPackage(void *dst, void *pkg);
extern void PlaySong(void *arg0, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 GetUiText(s32 id);

/* Save data. */
extern u8 *PackSaveData(void *p);
extern u8 LevelHasGemPathGem(void *p, s32 id);
extern u8 LevelHasRedGem(void *p, s32 id);
extern u8 LevelHasGreenGem(void *p, s32 id);
extern u8 LevelHasBlueGem(void *p, s32 id);
extern u8 LevelHasYellowGem(void *p, s32 id);

/* The level-select screen's sub-objects and siblings (0x0801CEE0 on). */
extern void *CreateLevelSelectPageBg(void *mem, s32 a, s32 b);
extern void *InitZoomBg(void *mem, s32 a, s32 b);
extern void *CreateLevelSelectCursor(void *mem);
extern struct item *CreateLevelSelectEntry(void *mem);
extern void LoadLevelSelectEntries(struct level_menu *self);
extern void PlaceLevelSelectEntries(struct level_menu *self);
extern void SetLevelSelectEntryBoxes(struct level_menu *self);
extern void ReloadLevelSelectPalette(struct level_menu *self);
extern void WaitLevelSelectCursor(struct level_menu *self);
extern void LevelSelectConfirm(struct level_menu *self);
extern void LevelSelectExit(struct level_menu *self);
extern u8 LevelSelectHasPrevWorld(struct level_menu *self);
extern u8 LevelSelectIsNextWorldOpen(struct level_menu *self);
extern void LevelSelectPrevWorld(struct level_menu *self);
extern void LevelSelectNextWorld(struct level_menu *self);
extern void ScrollLevelSelectPageBg(void *p);
extern void SetLevelSelectPageBgOffsets(void *p);
extern u32 GetLevelSelectPageBgOffsets(void *p);
extern void DestroyLevelSelectPageBg(void *p, s32 flags);
extern s32 GetLevelSelectPageBgScroll(void *p);
extern u8 IsLevelSelectPageBgSettled(void *p);
extern void DestroyZoomBg(void *p, s32 flags);
extern void UpdateZoomBg(void *p);
extern void DrawZoomBg(void *p);
extern void CommitZoomBg(void *p);
extern u8 IsZoomBgExiting(void *p);
extern u8 IsZoomBgShown(void *p);
extern u8 IsZoomBgWaiting(void *p);
extern u8 IsZoomBgZoomingOut(void *p);
extern void ClearZoomBgPicture(void *p);
extern void SetZoomBgPicture(void *p, s32 arg);
extern u16 GetZoomBgControl(void *p);
extern u8 IsLevelSelectEntrySelected(struct item *p);
extern s32 GetLevelSelectEntryLevel(struct item *it);
extern void SetLevelSelectEntrySelected(struct item *it, s32 arg);
extern void UpdateLevelSelectCursor(void *p);
extern void DrawLevelSelectCursor(void *p);
extern void ParkLevelSelectCursor(void *p);
/* Returns a u8; the one caller that needs it tests only its low byte. */
extern s32 HasLevelSelectCursorArrived(void *p);
extern void MoveLevelSelectCursor(void *p, s32 x, s32 y);
extern void DestroyLevelSelectCursor(void *p, s32 flags);
extern u16 GetBgSetupControl(void *p);

void sub_801BAC4(struct sprite *self);
struct level_menu *InitLevelSelect(struct level_menu *self, s32 arg);
void DestroyLevelSelect(struct level_menu *self, s32 flags);
void UpdateLevelSelectPageArrows(struct level_menu *self);
void DrawLevelSelectRecord(struct level_menu *self);
void DrawLevelSelectTime(struct level_menu *self, u32 time);
void LoadLevelSelectRecord(struct level_menu *self);
s32 LevelSelectLoop(struct level_menu *self);
void LevelSelectCursorLeft(struct level_menu *self);
void LevelSelectCursorRight(struct level_menu *self);

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

/* SetSpriteAnim, inlined: select animation `idx` and restart it. */
static inline void SetAnim(struct sprite *s, s32 idx)
{
    s->animIndex = idx;
    ResetSpriteFrameTimer(s);
    ResetSpriteFrameIndex(s);
    SetSpriteAnimDone(s, 0);
}

static inline struct anim_table *AnimTable(s32 offset)
{
    return (struct anim_table *)((u8 *)**gSpriteBankSet + offset);
}

static inline void SetIconPos(struct bitmap_font *m, u32 x, u32 y)
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
    CommitZoomBg(self->bg2);
    self->scroll++;
    *(vu16 *)REG_ADDR_BG0HOFS = self->scroll >> 3;
    *(vu32 *)REG_ADDR_BG1HOFS = GetLevelSelectPageBgOffsets(self->bg1);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(self->bg1);
    *(vu16 *)REG_ADDR_BG2CNT = GetZoomBgControl(self->bg2);
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
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } while (0)

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets `unk_32`. */
void sub_801B85C(struct follow_child *self)
{
    self->unk_32 = 1;
}

/* (Re)initializes a follow child: makes it visible, registers it as
 * gCamera's follow target and snaps it 0x1E00 (30 px, Q8) to
 * the player's right with both offsets reset. The `visible` bit test/
 * toggle and the stores need register pins to keep the ROM's
 * allocation (see the doc for issue 26). */
void ResetCameraLead(struct follow_child *self)
{
    u32 v = *((u8 *)self + 0x0D) >> 2;
    register u32 one asm("r1") = 1;

    if (!(v & one))
        self->visible = v ^ 1;
    gCamera->follow = self;
    {
        struct player *p = gPlayer;
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
void UpdateCameraLead(struct follow_child *self)
{
    s32 cur, tgt;

    UpdateMovingSprite(self);
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
        struct player *p = gPlayer;
        s32 x = p->x;
        s32 y = p->y;

        self->x = x + self->offset;
        self->y = y;
    }
    self->unk_60 = gPlayer->unk_60;
}

/* Destructor (method table +0x50): hands gCamera's follow
 * target back to the player. */
void DestroyCameraLead(struct follow_child *self, s32 flags)
{
    self->vtable = gCameraLeadVtable;
    gCamera->follow = gPlayer;
    DestroyMovingSprite(self, flags);
}

/* Constructor, called from InputCtrlStateStart (input_ctrl.c). */
struct follow_child *CreateCameraLead(struct follow_child *self)
{
    InitMovingSprite(self);
    self->vtable = gCameraLeadVtable;
    ResetCameraLead(self);
    return self;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets the target offset, clamped to
 * 0xA00-0x3200. */
void SetCameraLeadOffset(struct follow_child *self, s32 offset)
{
    if (offset > 0x3200)
        offset = 0x3200;
    else if (offset <= 0x9FF)
        offset = 0xA00;
    self->targetOffset = offset;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Returns the target offset. */
s32 GetCameraLeadOffset(struct follow_child *self)
{
    return self->targetOffset;
}

/* Factory for the 0x78-byte sprite (method table gLaunchPadVtable,
 * constructor inlined): places it at (x, y) pixels with record id `id`,
 * registers it with gCollidableList, and starts animation 0 of the
 * table at `**gSpriteBankSet + 0x150`. Called from SpawnMegaMix's
 * family (spawn_objects.c). The two mask constants are
 * materialized with `mov/neg` asm like spawn_objects.c's
 * SpawnMegaMix, since the compiler otherwise derives them from constants
 * already in registers. */
struct sprite *SpawnLaunchPad(u16 id, u16 x, u16 y)
{
    struct sprite *obj = OperatorNew(0x78);

    InitMovingSprite(obj);
    obj->vtable = (struct method *)gLaunchPadVtable;
    sub_801BAC4(obj);
    obj->id = id;
    obj->x = x << 8;
    obj->y = y << 8;
    AddToPartList(gCollidableList, obj);
    obj->anim = (struct anim_table *)(**gSpriteBankSet + 0x150);
    obj->animIndex = 0;
    ResetSpriteFrameTimer(obj);
    ResetSpriteFrameIndex(obj);
    SetSpriteAnimDone(obj, 0);
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
        s32 pal = (u8)GetPaletteSlot(gPaletteCache, rec->paletteId);
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
void CheckLaunchPadContact(void *self)
{
    struct aabb box;

    if (gPlayer->flags >> 7)
    {
        GetSpriteHitbox(&box, self);
        if (box.w != 0 && PlayerTouchesBox(gPlayer, &box))
        {
            struct player *p = gPlayer;
            struct method *m = &p->vtable[13];
            void *addr = (u8 *)p + m->thisOffset;
            register void *fn asm("r4") = *(void *volatile *)&m->fn;

            _call_via_r4(addr, 0, 0x19, 0);
            (void)fn;
        }
    }
}

/* Destructor (method table +0x50). */
void DestroyLaunchPad(struct sprite *self, s32 flags)
{
    self->vtable = (struct method *)gLaunchPadVtable;
    DestroyMovingSprite(self, flags);
}

/* Clears flags bit 6. */
void sub_801BAC4(struct sprite *self)
{
    self->flags &= Opaque(~0x40);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan; SpawnLaunchPad inlines it instead).
 * Constructor. */
struct sprite *InitLaunchPad(struct sprite *self)
{
    InitMovingSprite(self);
    self->vtable = (struct method *)gLaunchPadVtable;
    sub_801BAC4(self);
    return self;
}

/* The level-select screen, modal (called from game_frame.c): resets the
 * display, palette, VRAM cursor and both text-icon managers (the same
 * setup as ShowPowerDialog), builds the menu for level `*arg`, runs it, stores
 * the chosen level back through `arg`, tears the menu down and returns
 * its `result` byte. The icon-manager steps are inline helpers: the ROM
 * recomputes every field address after each call instead of keeping the
 * offsets in registers, which is what separate inlined expansions give. */
static inline void IconSetup(struct bitmap_font *m, u32 v)
{
    struct icon_slot *slot;

    m->tileBase = v;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserve(struct bitmap_font **m)
{
    struct vram_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

static inline void LoadMenuPalette(struct tile_cache *cache)
{
    CpuSet(gLevelSelectPalette, cache->palette, 0x10);
}

u8 RunLevelSelect(s32 *arg)
{
    struct level_menu *menu;
    u8 result;
    s32 heaps = 0xC0000000;

    mem_free_bytes(heaps);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;
    FreeUnlockedPaletteSlots(gPaletteCache);
    ClaimPaletteSlot(gPaletteCache, 0xF);
    LoadMenuPalette(gPaletteCache);
    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    IconSetup(gSmallFont, 0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        IconSetup(gLargeFont, v);
    }
    IconReserve(&gLargeFont);
    MarkObjVram(gObjVramCursor);
    PlaySong(gAudioContext, 0x10);
    {
        struct level_menu **menuAddr = &gLevelSelect;

        *menuAddr = InitLevelSelect(OperatorNew(0xAC), *arg);
        *arg = LevelSelectLoop(*menuAddr);
        menu = *menuAddr;
        result = menu->result;
        if (menu != NULL)
            DestroyLevelSelect(menu, 3);
        *menuAddr = NULL;
    }
    FreeUnlockedPaletteSlots(gPaletteCache);
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
struct level_menu *InitLevelSelect(struct level_menu *self, s32 arg)
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
        self->world = __divsi3(arg, 5);
        self->index = __modsi3(arg, 5);
    }
    else
    {
        self->world = arg - 0x14;
        self->index = 5;
    }
    self->nameText = 0;
    self->save = PackSaveData(gLevelState);
    self->result = 0;
    self->bg1 = CreateLevelSelectPageBg(OperatorNew(0x28), 0, 0x1D);
    InitBgSetup(bg0cnt, 2, 0x1E, 2, 3);
    LoadGraphicsPackage(bg0cnt, gMenuSkyBg);
    self->scroll = 0;
    self->panel = CreateLevelSelectCursor(OperatorNew(0x54));
    self->bg2 = InitZoomBg(OperatorNew(0x8C), 3, 0x1F);
    {
        s32 j;

        for (j = 0; j < 6; j++)
            self->items[j] = CreateLevelSelectEntry(OperatorNew(0x14));
    }
    LoadLevelSelectEntries(self);
    PlaceLevelSelectEntries(self);
    SetLevelSelectEntryBoxes(self);
    {
        s32 v;

        for (i = 0, v = 0x80; i < 8; i++)
        {
            s = InitUiSpriteObj(OperatorNew(0x40));
            self->sprites[i] = s;
            SetSpritePriority(s, 1);
            if (i > 1)
                self->sprites[i]->unk_3C = v;
        }
    }
    self->sprites[0]->anim = AnimTable(0x234);
    SetAnim(self->sprites[0], gLevelSelectWorldAnims[self->world]);
    SetEntityPixelPos(self->sprites[0], gLevelSelectWorldPos.x, gLevelSelectWorldPos.y);
    self->sprites[1]->anim = AnimTable(0x234);
    SetAnim(self->sprites[1], 10);
    SetEntityPixelPos(self->sprites[1], gStaticData_0816C4A0.x, gStaticData_0816C4A0.y);
    self->sprites[2]->anim = AnimTable(0x1BC);
    SetEntityPixelPos(self->sprites[2], gLevelSelectCrystalPos.x, gLevelSelectCrystalPos.y);
    self->sprites[3]->anim = AnimTable(0x180);
    SetAnim(self->sprites[3], 1);
    SetEntityPixelPos(self->sprites[3], gLevelSelectGemPos.x, gLevelSelectGemPos.y);
    self->sprites[4]->anim = AnimTable(0x180);
    SetAnim(self->sprites[4], 1);
    SetEntityPixelPos(self->sprites[4], gLevelSelectGemPos.x, gLevelSelectGemPos.y);
    self->sprites[5]->anim = AnimTable(0x18C);
    SetEntityPixelPos(self->sprites[5], gLevelSelectTrialIconPos.x, gLevelSelectTrialIconPos.y);
    self->sprites[6]->anim = AnimTable(0x18C);
    SetEntityPixelPos(self->sprites[6], gLevelSelectTrialIconPos.x, gLevelSelectTrialIconPos.y);
    self->sprites[7]->anim = AnimTable(0x18C);
    SetEntityPixelPos(self->sprites[7], gLevelSelectTimePos.x, gLevelSelectTimePos.y);
    s = InitUiSpriteObj(OperatorNew(0x40));
    self->sprites[8] = s;
    SetSpritePriority(s, 1);
    self->sprites[8]->anim = AnimTable(0x270);
    SetAnim(self->sprites[8], 1);
    SetEntityPixelPos(self->sprites[8], gStaticData_0816C4C8.x, gStaticData_0816C4C8.y);
    s = InitUiSpriteObj(OperatorNew(0x40));
    self->sprites[9] = s;
    SetSpritePriority(s, 1);
    self->sprites[9]->anim = AnimTable(0x270);
    SetAnim(self->sprites[9], 0);
    SetEntityPixelPos(self->sprites[9], gStaticData_0816C4D0.x, gStaticData_0816C4D0.y);
    if (gNewWorldOpened && LevelSelectIsNextWorldOpen(self))
    {
        ParkLevelSelectCursor(self->panel);
    }
    else
    {
        struct xy_pair *pos = &self->positions[self->index];

        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
    }
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    *(vu32 *)REG_ADDR_BG1HOFS = GetLevelSelectPageBgOffsets(self->bg1);
    *(vu16 *)REG_ADDR_BG0CNT = GetBgSetupControl(bg0cnt);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(self->bg1);
    *(vu16 *)REG_ADDR_BG2CNT = GetZoomBgControl(self->bg2);
    return self;
}

/* Destructor: deletes the sprites, panel, BG layers and level entries
 * through their own destructors; frees itself if `flags & 1`. */
void DestroyLevelSelect(struct level_menu *self, s32 flags)
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
        DestroyLevelSelectCursor(self->panel, 3);
    if (self->bg2 != NULL)
        DestroyZoomBg(self->bg2, 3);
    for (i = 0; i < 6; i++)
    {
        struct item *it = self->items[i];

        if (it != NULL)
            _call_via_r2((u8 *)it + it->vtable->m28.thisOffset, 3, it->vtable->m28.fn);
    }
    if (self->bg1 != NULL)
        DestroyLevelSelectPageBg(self->bg1, 3);
    if (flags & 1)
        OperatorDelete(self);
}

/* Per-frame update: draws the selected level's name centred at the top
 * (gLargeFont) and its record panel, updates every entry, and once
 * the BG1 page has settled draws text 0x2F centred at y=0x96 (the first
 * time only, with the page arrows) and steps BG2; BG2's DISPCNT enable
 * bit follows IsZoomBgWaiting. */
void UpdateLevelSelect(struct level_menu *self)
{
    s32 i;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    DrawLevelSelectCursor(self->panel);
    if (IsZoomBgShown(self->bg2) && IsLevelSelectEntrySelected(self->items[self->index]))
    {
        struct icon_slot *slot = &gLargeFont->record->slots[0];
        u32 x = (u32)(0xF0 - _call_via_r2((u8 *)gLargeFont + slot->offset, self->nameText, slot->ptr)) >> 1;

        SetIconPos(gLargeFont, x, -self->panelSlideX + 2);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, self->nameText, slot->ptr);
        if (self->index <= 4)
            DrawLevelSelectRecord(self);
    }
    SetLevelSelectPageBgOffsets(self->bg1);
    for (i = 0; i <= self->lastIndex; i++)
    {
        struct item *it = self->items[i];
        struct method *m = &it->vtable->m08;

        _call_via_r2((u8 *)it + m->thisOffset, GetLevelSelectPageBgScroll(self->bg1), m->fn);
    }
    if (IsLevelSelectPageBgSettled(self->bg1))
    {
        if (!IsZoomBgExiting(self->bg2))
        {
            s32 text = GetUiText(0x2F);
            struct icon_slot *slot = &gSmallFont->record->slots[0];
            u32 x = (u32)(0xF0 - _call_via_r2((u8 *)gSmallFont + slot->offset, text, slot->ptr)) >> 1;

            SetIconPos(gSmallFont, x, 0x96);
            FontSetPalette(gSmallFont, 0xF);
            slot = &gSmallFont->record->slots[2];
            _call_via_r2((u8 *)gSmallFont + slot->offset, text, slot->ptr);
            UpdateLevelSelectPageArrows(self);
        }
        DrawZoomBg(self->bg2);
    }
    if (IsZoomBgWaiting(self->bg2))
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
    HideUnusedOamEntries(gOamBuffer);
}

/* Updates the two page-arrow sprites (8/9): palettes from GetSpriteAnimPaletteSlot,
 * frame 0/1 by whether the previous/next page is open. */
void UpdateLevelSelectPageArrows(struct level_menu *self)
{
    register s32 lowMask asm("r5");
    register s32 highMask asm("r4");

    {
        register s32 pal asm("r0") = GetSpriteAnimPaletteSlot(self->sprites[8]);
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
        register s32 pal asm("r0") = GetSpriteAnimPaletteSlot(self->sprites[9]);
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

        if (LevelSelectIsNextWorldOpen(self))
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
            DrawSpriteWithOffset(t, 0, 0);
        }
    }
    if (LevelSelectHasPrevWorld(self))
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
        DrawSpriteWithOffset(s, 0, 0);
    }
}

/* Draws the record panel sprites 0-4 at their per-row offsets and, if
 * the level is cleared, its time readout (DrawLevelSelectTime). */
void DrawLevelSelectRecord(struct level_menu *self)
{
    DrawSpriteWithOffset(self->sprites[0], -self->panelSlideX, 0);
    DrawSpriteWithOffset(self->sprites[1], -self->panelSlideX, 0);
    DrawSpriteWithOffset(self->sprites[2], -self->panelSlideX, self->clearedIconY);
    DrawSpriteWithOffset(self->sprites[3], -self->panelSlideX, self->flag1IconY);
    if (self->rank != 5)
        DrawSpriteWithOffset(self->sprites[4], -self->panelSlideX, self->gemIconY);
    {
        u8 **ps = &self->save;
        s32 off = self->levelId * 4 + 4;
        struct level_save *sv = (struct level_save *)(*ps + off);
        s32 one = 1;
        s32 b = *(u8 *)sv;

        one &= b;
        if (one != 0)
            DrawLevelSelectTime(self, sv->time);
    }
}

/* Draws the time readout: just the best time if it beats the tightest
 * threshold (time2), otherwise sprite 7, the next threshold to beat and
 * the best time. */
void DrawLevelSelectTime(struct level_menu *self, u32 time)
{
    struct level_info *info;

    DrawSpriteWithOffset(self->sprites[5], -self->panelSlideX, self->trialIconY);
    DrawSpriteWithOffset(self->sprites[6], -self->panelSlideX, self->trialIcon2Y);
    info = &gLevelTable[self->levelId];
    if (time != 0 && time <= info->time2)
    {
        struct icon_slot *slot;

        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, (s32)self->timeText, slot->ptr);
    }
    else
    {
        struct icon_slot *slot;

        DrawSpriteWithOffset(self->sprites[7], self->panelSlideX, 0);
        FontSetPalette(gLargeFont, self->sprites[7]->palette);
        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y - 8);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, (s32)self->recordText, slot->ptr);
        FontResetPalette(gLargeFont);
        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10, gLevelSelectTimePos.y + 8);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, (s32)self->timeText, slot->ptr);
    }
}

/* Per-frame draw step: once the BG1 page has settled and the cursor
 * panel has arrived on a new entry, selects it and loads that level's
 * name and record (LoadLevelSelectRecord); then draws the six entries and record
 * sprites 2-7. */
void DrawLevelSelect(struct level_menu *self)
{
    struct item **items;
    struct sprite **sprites;
    s32 i;

    ScrollLevelSelectPageBg(self->bg1);
    UpdateLevelSelectCursor(self->panel);
    if (!IsLevelSelectPageBgSettled(self->bg1))
        return;
    {
        s32 done = HasLevelSelectCursorArrived(self->panel) << 24;
        items = self->items;
        if (!done)
            goto draw;
    }
    {
        if (!IsLevelSelectEntrySelected(ItemAt(items, self->index)))
        {
            struct item *it = ItemAt(items, self->index);

            SetLevelSelectEntrySelected(it, 1);
            if (!IsZoomBgShown(self->bg2))
            {
                struct level_info *info;

                self->levelId = GetLevelSelectEntryLevel(it);
                info = &gLevelTable[self->levelId];
                SetZoomBgPicture(self->bg2, info->theme);
                self->nameText = GetUiText(info->nameText);
            }
            self->panelSlideX = 0;
            if (self->index <= 4)
            {
                LoadLevelSelectRecord(self);
                FreeUnlockedPaletteSlots(gPaletteCache);
                ReloadLevelSelectPalette(self);
            }
            FontResetPalette(gLargeFont);
        }
    }
draw:
    sprites = self->sprites;
    for (i = 5; i >= 0; i--)
    {
        struct item *it = *items++;
        struct item_vtable *vt = it->vtable;

        _call_via_r1((u8 *)it + vt->m20.thisOffset, vt->m20.fn);
    }
    UpdateZoomBg(self->bg2);
    {
        struct sprite **p = sprites + 2;

        for (i = 5; i >= 0; i--)
            AdvanceSpriteAnim(*p++);
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
void LoadLevelSelectRecord(struct level_menu *self)
{
    s32 *rank = &self->rank;
    struct level_save *sv;

    *rank = 5;
    if (LevelHasGemPathGem(gLevelState, self->levelId))
        *rank = 0;
    if (LevelHasRedGem(gLevelState, self->levelId))
        *rank = 1;
    if (LevelHasGreenGem(gLevelState, self->levelId))
        *rank = 2;
    if (LevelHasBlueGem(gLevelState, self->levelId))
        *rank = 3;
    if (LevelHasYellowGem(gLevelState, self->levelId))
        *rank = 4;
    self->clearedIconY = 0;
    self->flag1IconY = 0;
    self->gemIconY = 0;
    self->trialIconY = 0;
    self->trialIcon2Y = 0;
    sv = (struct level_save *)(self->save + (self->levelId * 4 + 4));
    if (sv->cleared)
        self->clearedIconY = 0x1C;
    if (sv->flag1)
        self->flag1IconY = 0x1C;
    switch (*rank)
    {
    case 0:
        if (sv->flag2)
            self->gemIconY = 0x1C;
        break;
    case 1:
        if (self->save[2] & 1)
            self->gemIconY = 0x1C;
        break;
    case 2:
        if (self->save[2] & 4)
            self->gemIconY = 0x1C;
        break;
    case 3:
        if (self->save[2] & 8)
            self->gemIconY = 0x1C;
        break;
    case 4:
        if (self->save[2] & 2)
            self->gemIconY = 0x1C;
        break;
    case 5:
        break;
    default:
        goto set_rank_icon;
    }
    if (self->rank != 5)
    {
    set_rank_icon:
        SetAnim(self->sprites[4], gLevelSelectRankAnims[self->rank]);
        if (self->flag1IconY == self->gemIconY)
        {
            self->flag1IconY -= 6;
            self->gemIconY += 6;
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
                self->trialIcon2Y = 0x1C;
                self->trialIconY = 0x1C;
                SetAnim(self->sprites[5], 1);
                SetAnim(self->sprites[6], 1);
            }
            else if (sv->time <= info->time1)
            {
                FormatCentiseconds(info->time2, self->recordText);
                self->trialIconY = 0x1C;
                SetAnim(self->sprites[5], 2);
                SetAnim(self->sprites[6], 1);
                SetAnim(self->sprites[7], 1);
            }
            else if (sv->time <= entry->time0)
            {
                FormatCentiseconds(info->time1, self->recordText);
                self->trialIconY = 0x1C;
                SetAnim(self->sprites[5], 0);
                SetAnim(self->sprites[6], 2);
                SetAnim(self->sprites[7], 2);
            }
        }
    }
}

/* The menu loop: fades in (BLDY), then runs frames until A is pressed on
 * an open entry (LevelSelectConfirm) or Start exits (LevelSelectExit), dispatching
 * Up/Down page turns (LevelSelectNextWorld/LevelSelectPrevWorld) and Left/Right cursor
 * moves (LevelSelectCursorLeft/LevelSelectCursorRight); returns the selected entry's level.
 *
 * Matched under old_agbcc in the near-miss polish pass
 * (docs/matching/near-miss-polish.md): the key-word copy the ROM makes
 * inside the 0x80 test comes from a statement expression holding the
 * copy plus an empty `asm` that keeps gcc from merging it. */
s32 LevelSelectLoop(struct level_menu *self)
{
    struct level_info *info;

    self->result = 0;
    self->levelId = GetLevelSelectEntryLevel(self->items[self->index]);
    info = &gLevelTable[self->levelId];
    SetZoomBgPicture(self->bg2, info->theme);
    self->nameText = GetUiText(info->nameText);
    while (!IsZoomBgShown(self->bg2))
    {
        {
            struct bldy_byte *f = (struct bldy_byte *)&self->bldy;

            if (f->evy != 0)
                f->evy--;
        }
        UpdateLevelSelect(self);
        WaitForVBlank();
        UploadPaletteCache(gPaletteCache);
        CommitOamBuffer(gOamBuffer);
        CommitDisplay(self);
        UpdateZoomBg(self->bg2);
    }
    PlaySfx(gAudioContext, 0x51, 0x100);
    self->blend.raw = 0;
    self->blend.bits.bg0Second = 1;
    self->blend.bits.bg1Second = 1;
    self->blend.bits.bg2Second = 1;
    self->blend.bits.bg3Second = 1;
    self->blend.bits.bdSecond = 1;
    self->blend.bits.eva = 0x10;
    self->blend.bits.evb = 0x10;
    if (gNewWorldOpened && LevelSelectIsNextWorldOpen(self))
    {
        self->index = 0;
        LevelSelectNextWorld(self);
    }
    gNewWorldOpened = 0;
    goto loop;

check_exit:
    if (gKeys.half.pressed & 8)
    {
        LevelSelectExit(self);
        goto end;
    }
loop:
    UpdateLevelSelect(self);
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    CommitDisplay(self);
    DrawLevelSelect(self);
    if (!IsLevelSelectPageBgSettled(self->bg1))
        goto loop;
    if (!(u8)HasLevelSelectCursorArrived(self->panel))
        goto loop;
    UpdateKeys(gInput);
    {
        union key_state keys = gKeys;
        union key_state k;

        if (keys.half.pressed & 0x40)
            LevelSelectNextWorld(self);
        /* The ROM copies the key word between the 0x80 test's `ands`
         * and its `cmp`, and tests 0x20 on the copy; gcc merges a plain
         * copy, so the (code-free) asm keeps `k` a separate value. */
        else if (({
                     u32 hit = keys.half.pressed & 0x80;

                     k = keys;
                     asm("" : "+r"(k.all));
                     hit;
                 }))
            LevelSelectPrevWorld(self);
        else
        {
            if (k.half.pressed & 0x20)
                LevelSelectCursorLeft(self);
            else if (keys.half.pressed & 0x10)
                LevelSelectCursorRight(self);
        }
    }
    if (!(gKeys.half.pressed & 1))
        goto check_exit;
    if (!IsLevelSelectEntrySelected(self->items[self->index]))
        goto check_exit;
    if (!IsZoomBgShown(self->bg2))
        goto check_exit;
    LevelSelectConfirm(self);
end:
    self->dispcnt.raw = 0;
    self->dispcnt.bits.obj1d = 1;
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    CommitDisplay(self);
    return GetLevelSelectEntryLevel(self->items[self->index]);
}

/* Deselects the current entry and runs frames until BG2 and the cursor
 * panel settle. Called by the page-turn handlers (LevelSelectPrevWorld/D548). */
void SettleLevelSelectPage(struct level_menu *self)
{
    SetLevelSelectEntrySelected(self->items[self->index], 0);
    ClearZoomBgPicture(self->bg2);
    ParkLevelSelectCursor(self->panel);
    while (IsZoomBgZoomingOut(self->bg2) || !(u8)HasLevelSelectCursorArrived(self->panel))
    {
        UpdateLevelSelect(self);
        WaitForVBlank();
        UploadPaletteCache(gPaletteCache);
        CommitOamBuffer(gOamBuffer);
        CommitDisplay(self);
        UpdateLevelSelectCursor(self->panel);
        UpdateZoomBg(self->bg2);
    }
    FreeUnlockedPaletteSlots(gPaletteCache);
}

/* Moves the cursor left, repeating while Left is held; sound 0x48 at the
 * first entry. */
void LevelSelectCursorLeft(struct level_menu *self)
{
    if (self->index == 0)
    {
        PlaySfx(gAudioContext, 0x48, 0x100);
        return;
    }
    SetLevelSelectEntrySelected(self->items[self->index], 0);
    ClearZoomBgPicture(self->bg2);
    while (self->index != 0)
    {
        struct xy_pair *pos;

        self->index--;
        pos = &self->positions[self->index];
        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
        WaitLevelSelectCursor(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & 0x20))
            return;
    }
}

/* Moves the cursor right, repeating while Right is held; sound 0x48 at
 * the last entry. */
void LevelSelectCursorRight(struct level_menu *self)
{
    if (self->index == self->lastIndex)
    {
        PlaySfx(gAudioContext, 0x48, 0x100);
        return;
    }
    SetLevelSelectEntrySelected(self->items[self->index], 0);
    ClearZoomBgPicture(self->bg2);
    while (self->index < self->lastIndex)
    {
        struct xy_pair *pos;

        self->index++;
        pos = &self->positions[self->index];
        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
        WaitLevelSelectCursor(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & 0x10))
            return;
    }
}
