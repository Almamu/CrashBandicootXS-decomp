#include "core.h"
#include "match.h"
#include "bitmap_font.h"
#include <agb_syscall.h>
#include "text.h"
#include "util.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "menus.h"
#include "level_menu.h"
#include "camera_lead.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"

/* GitHub issue #26: 0x0801B85C-0x0801CEE0, the whole of the former
 * asm/code_3_2_17_188d0_1b85c.s. Three objects, all gcc 2.x C++ classes
 * (a method table at +0x18/+0x10, virtual calls through the
 * _call_via_r1/AD80/AD88 call-via-register thunks, inlined member
 * functions):
 *
 * - sub_801B85C-GetCameraLeadOffset: `struct follow_child`, the 0x80-byte
 *   object (method table gCameraLeadVtable) InputCtrlStateStart
 *   (input_ctrl.cpp) spawns for the input controller. It trails the
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
 * are in docs/matching/archive/issue-26-level-select-menu.md.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS), the compiler this
 * region was originally built with; see docs/matching/archive/old-agbcc-retry.md.
 * Under it the shadow-register blocks are plain bitfield stores; the
 * `Opaque`/register-pinned forms were current-agbcc workarounds. */

/* `struct follow_child` is camera_lead.h's. */
COMPILE_TIME_ASSERT(level_select_c, sizeof(struct follow_child) == 0x80);

/* The BLDY shadow (`level_menu.bldy`, gfx.h's `struct bldy`) viewed as
 * its low byte, for the fade-in decrement
 * (a byte-sized test is what makes gcc narrow the `evy != 0` check to
 * an `and` of the loaded byte). */
struct bldy_byte {
    u8 evy:5;
    u8 unk_5:3;
} __attribute__((packed));

/* `struct sprite`, `struct level_item`, `struct level_menu` and the rest
 * of the screen's types are level_menu.h's (this file had its own copies,
 * merged in #574 batch 9e). */

/* Held keys in the low half, newly-pressed keys in the high half. */


/* codegen: gLevelSelectGemPos and gLevelSelectTrialIconPos are const
 * (menus.h), but InitLevelSelect reads each one twice, across calls,
 * and the ROM loads it again each time: through the const object gcc
 * keeps the first loads in registers. docs/headers_plan.md */
extern struct xy_pair gLevelSelectGemPos_rw asm("gLevelSelectGemPos");
extern struct xy_pair gLevelSelectTrialIconPos_rw asm("gLevelSelectTrialIconPos");

/* Base class and runtime. */
extern void _call_via_r1(void *self, void *fn);
extern s32 _call_via_r2(void *self, s32 arg, void *fn);
/* Calls the function in r4 with r0-r3 (see the `MATCH_HOLD_REG(..., r4)`
 * pin at the call site). */
extern void _call_via_r4(void *self, s32 a, s32 b, s32 c);

/* Save data. */

/* The level-select screen's sub-objects and siblings (0x0801CEE0 on). */

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

static inline const struct sprite_bank *AnimTable(s32 offset)
{
    return (const struct sprite_bank *)(SPRITE_BANK_BASE + offset);
}

static inline void SetIconPos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

static inline struct level_item *ItemAt(struct level_item **items, s32 index)
{
    MATCH_HOLD_REG(s32, off, r0) = index * 4;

    return *(struct level_item **)((u8 *)items + off);
}

/* The level-select screen's per-frame register commit. */
static inline void CommitDisplay(struct level_menu *self)
{
    FlushVramDmaQueue();
    CommitZoomBg(self->bg2);
    self->scroll++;
    *(vu16 *)REG_ADDR_BG0HOFS = self->scroll >> 3;
    *(vu32 *)REG_ADDR_BG1HOFS = GetLevelSelectPageBgOffsets(self->bg1);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(&self->bg1->bg);
    *(vu16 *)REG_ADDR_BG2CNT = GetZoomBgControl(self->bg2);
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = self->blend.raw;
    *(vu16 *)REG_ADDR_BLDY = self->bldy.evy;
    *(vu16 *)REG_ADDR_DISPCNT = self->dispcnt.raw;
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan). Sets `unk_32`, a byte nothing else
 * reads or writes, so its meaning (and this function's name) is open. */
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
    u32 v = self->state.all >> 2;
    MATCH_HOLD_REG(u32, one, r1) = 1;

    if (!(v & one))
        self->state.bits.visible = v ^ 1;
    gCamera->target = (struct camera_target *)self;
    {
        struct player *p = gPlayer;
        MATCH_HOLD_REG(s32, x, r0) = p->x;
        MATCH_HOLD_REG(s32, y, r2) = p->y;
        MATCH_HOLD_REG(s32, off, r1) = 0x1E00;

        self->x = x + off;
        self->y = y;
        {
            MATCH_HOLD_REG(s32, f, r0) = 0x10;

            f |= self->flags.all;
            self->flags.all = f;
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

    UpdateMovingSprite((struct actor *)self);
    {
        s32 one = 1;
        s32 zero;
        u8 *p = &self->moveAxes;

        zero = 0;
        p[0] = one;
        p[0x44] = zero;
    }
    cur = self->offset;
    tgt = self->targetOffset;
    if (cur < tgt) {
        cur += 0x200;
        if (cur > tgt)
            self->offset = tgt;
        else
            self->offset = cur;
    } else if (cur > tgt) {
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
    self->speedX = gPlayer->speedX;
}

/* Destructor (method table +0x50): hands gCamera's follow
 * target back to the player. */
void DestroyCameraLead(struct follow_child *self, s32 flags)
{
    self->vtable = gCameraLeadVtable;
    gCamera->target = (struct camera_target *)gPlayer;
    DestroyMovingSprite((struct actor *)self, flags);
}

/* Constructor, called from InputCtrlStateStart (input_ctrl.cpp). */
struct follow_child *CreateCameraLead(struct follow_child *self)
{
    InitMovingSprite((struct actor *)self);
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
struct sprite *SpawnLaunchPad(u16 id, u16 x, u16 y, u16 unused)
{
    struct sprite *obj = OperatorNew(0x78);

    InitMovingSprite((struct actor *)obj);
    obj->vtable = (struct sprite_vtable *)gLaunchPadVtable;
    ClearLaunchPadVulnerable(obj);
    obj->id = id;
    obj->x = INT_TO_Q8(x);
    obj->y = INT_TO_Q8(y);
    AddToPartList(gCollidableList, obj);
    obj->anim = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x150);
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
        const struct sprite_anim *recs = obj->anim->anims;
        u32 idx = obj->animIndex;
        const struct sprite_anim *rec = &recs[idx];
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

    if (gPlayer->flags.all >> 7) {
        box = GetSpriteHitbox(self);
        if (box.w != 0 && PlayerTouchesBox(gPlayer, &box)) {
            struct player *p = gPlayer;
            const struct actor_method *m = &p->vtable->handleEvent;
            void *addr = (u8 *)p + m->thisOffset;
            MATCH_HOLD_REG(void *, fn, r4) = *(void *const volatile *)&m->fn;

            _call_via_r4(addr, 0, EVENT_LAUNCH_PAD, 0);
            (void)fn;
        }
    }
}

/* Destructor (method table +0x50). */
void DestroyLaunchPad(struct sprite *self, s32 flags)
{
    self->vtable = (struct sprite_vtable *)gLaunchPadVtable;
    DestroyMovingSprite((struct actor *)self, flags);
}

/* Clears flags bit 6, the sprite objects' "vulnerable" bit (the launch
 * pad's own out-of-line copy of sprite_obj.c's ClearSpriteObjVulnerable;
 * the constructor calls it). */
void ClearLaunchPadVulnerable(struct sprite *self)
{
    self->flags &= Opaque(~0x40);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, data/, src/ and a
 * whole-ROM Thumb-pointer scan; SpawnLaunchPad inlines it instead).
 * Constructor. */
struct sprite *InitLaunchPad(struct sprite *self)
{
    InitMovingSprite((struct actor *)self);
    self->vtable = (struct sprite_vtable *)gLaunchPadVtable;
    ClearLaunchPadVulnerable(self);
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
    struct vram_upload_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

static inline void LoadMenuPalette(struct palette_cache *cache)
{
    CpuSet(gLevelSelectPalette, cache->slots[15], 0x10);
}

s32 RunLevelSelect(s32 *arg)
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
    PlaySong(gAudioContext, SONG_WARP_ROOM);
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
    struct bg_setup bg0cnt;
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
    self->dispcnt.bits.objMap1D = 1;
    self->dispcnt.bits.mode = 1;
    self->dispcnt.bits.bg0 = 1;
    self->dispcnt.bits.bg1 = 1;
    self->dispcnt.bits.obj = 1;
    if (arg <= LEVEL_LAST_NUMBERED) {
        self->world = __divsi3(arg, LEVELS_PER_WORLD);
        self->index = __modsi3(arg, LEVELS_PER_WORLD);
    } else {
        self->world = arg - LEVEL_FIRST_BOSS;
        self->index = 5;
    }
    self->nameText = 0;
    self->save = PackSaveData(gLevelState);
    self->result = 0;
    self->bg1 = CreateLevelSelectPageBg(OperatorNew(0x28), 0, 0x1D);
    InitBgSetup(&bg0cnt, 2, 0x1E, 2, 3);
    LoadGraphicsPackage(&bg0cnt, &gMenuSkyBg);
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

        for (i = 0, v = 0x80; i < 8; i++) {
            s = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
            self->sprites[i] = s;
            SetSpritePriority(s, 1);
            if (i > 1)
                self->sprites[i]->scale = v;
        }
    }
    self->sprites[0]->anim = AnimTable(0x234);
    SetAnim(self->sprites[0], gLevelSelectWorldAnims[self->world]);
    SetEntityPixelPos((struct actor *)self->sprites[0], gLevelSelectWorldPos.x,
                      gLevelSelectWorldPos.y);
    self->sprites[1]->anim = AnimTable(0x234);
    SetAnim(self->sprites[1], 10);
    SetEntityPixelPos((struct actor *)self->sprites[1], gLevelSelectCrashIconPos.x,
                      gLevelSelectCrashIconPos.y);
    self->sprites[2]->anim = AnimTable(0x1BC);
    SetEntityPixelPos((struct actor *)self->sprites[2], gLevelSelectCrystalPos.x,
                      gLevelSelectCrystalPos.y);
    self->sprites[3]->anim = AnimTable(0x180);
    SetAnim(self->sprites[3], 1);
    SetEntityPixelPos((struct actor *)self->sprites[3], gLevelSelectGemPos_rw.x,
                      gLevelSelectGemPos_rw.y);
    self->sprites[4]->anim = AnimTable(0x180);
    SetAnim(self->sprites[4], 1);
    SetEntityPixelPos((struct actor *)self->sprites[4], gLevelSelectGemPos_rw.x,
                      gLevelSelectGemPos_rw.y);
    self->sprites[5]->anim = AnimTable(0x18C);
    SetEntityPixelPos((struct actor *)self->sprites[5], gLevelSelectTrialIconPos_rw.x,
                      gLevelSelectTrialIconPos_rw.y);
    self->sprites[6]->anim = AnimTable(0x18C);
    SetEntityPixelPos((struct actor *)self->sprites[6], gLevelSelectTrialIconPos_rw.x,
                      gLevelSelectTrialIconPos_rw.y);
    self->sprites[7]->anim = AnimTable(0x18C);
    SetEntityPixelPos((struct actor *)self->sprites[7], gLevelSelectTimePos.x,
                      gLevelSelectTimePos.y);
    s = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
    self->sprites[8] = s;
    SetSpritePriority(s, 1);
    self->sprites[8]->anim = AnimTable(0x270);
    SetAnim(self->sprites[8], 1);
    SetEntityPixelPos((struct actor *)self->sprites[8], gLevelSelectNextWorldArrowPos.x,
                      gLevelSelectNextWorldArrowPos.y);
    s = (struct sprite *)InitUiSpriteObj(OperatorNew(0x40));
    self->sprites[9] = s;
    SetSpritePriority(s, 1);
    self->sprites[9]->anim = AnimTable(0x270);
    SetAnim(self->sprites[9], 0);
    SetEntityPixelPos((struct actor *)self->sprites[9], gLevelSelectPrevWorldArrowPos.x,
                      gLevelSelectPrevWorldArrowPos.y);
    if (gNewWorldOpened && LevelSelectIsNextWorldOpen(self)) {
        ParkLevelSelectCursor(self->panel);
    } else {
        const struct xy_pair *pos = &self->positions[self->index];

        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
    }
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    *(vu32 *)REG_ADDR_BG1HOFS = GetLevelSelectPageBgOffsets(self->bg1);
    *(vu16 *)REG_ADDR_BG0CNT = GetBgSetupControl(&bg0cnt);
    *(vu16 *)REG_ADDR_BG1CNT = GetBgSetupControl(&self->bg1->bg);
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
        VTABLE_CALL2(s, m50, 3);
    if ((s = self->sprites[8]) != NULL)
        VTABLE_CALL2(s, m50, 3);
    for (i = 0; i < 8; i++) {
        if ((s = self->sprites[i]) != NULL)
            VTABLE_CALL2(s, m50, 3);
    }
    if (self->panel != NULL)
        DestroyLevelSelectCursor(self->panel, 3);
    if (self->bg2 != NULL)
        DestroyZoomBg(self->bg2, 3);
    for (i = 0; i < 6; i++) {
        struct level_item *it = self->items[i];

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
    if (IsZoomBgShown(self->bg2) && IsLevelSelectEntrySelected(self->items[self->index])) {
        struct icon_slot *slot = &gLargeFont->record->slots[0];
        // clang-format off
        u32 x = (u32)(0xF0 - _call_via_r2((u8 *)gLargeFont + slot->offset, self->nameText,
                                          slot->ptr)) >> 1;
        // clang-format on

        SetIconPos(gLargeFont, x, -self->panelSlideX + 2);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, self->nameText, slot->ptr);
        if (self->index <= 4)
            DrawLevelSelectRecord(self);
    }
    SetLevelSelectPageBgOffsets(self->bg1);
    for (i = 0; i <= self->lastIndex; i++) {
        struct level_item *it = self->items[i];
        struct actor_method *m = &it->vtable->m08;

        _call_via_r2((u8 *)it + m->thisOffset, GetLevelSelectPageBgScroll(self->bg1), m->fn);
    }
    if (IsLevelSelectPageBgSettled(self->bg1)) {
        if (!IsZoomBgExiting(self->bg2)) {
            s32 text = GetUiText(0x2F);
            struct icon_slot *slot = &gSmallFont->record->slots[0];
            u32 x =
                (u32)(0xF0 - _call_via_r2((u8 *)gSmallFont + slot->offset, text, slot->ptr)) >> 1;

            SetIconPos(gSmallFont, x, 0x96);
            FontSetPalette(gSmallFont, 0xF);
            slot = &gSmallFont->record->slots[2];
            _call_via_r2((u8 *)gSmallFont + slot->offset, text, slot->ptr);
            UpdateLevelSelectPageArrows(self);
        }
        DrawZoomBg(self->bg2);
    }
    if (IsZoomBgWaiting(self->bg2)) {
        u8 *q = (u8 *)&self->dispcnt + 1;
        s32 m = -5;

        m &= *q;
        *q = m;
    } else {
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
    MATCH_HOLD_REG(s32, lowMask, r5);
    MATCH_HOLD_REG(s32, highMask, r4);

    {
        MATCH_HOLD_REG(s32, pal, r0) = GetSpriteAnimPaletteSlot((struct actor *)self->sprites[8]);
        u8 *p = (u8 *)self->sprites[8] + 0x29;
        MATCH_HOLD_REG(s32, m, r1);
        MATCH_HOLD_REG(s32, b, r3);

        lowMask = 0xF;
        pal &= lowMask;
        highMask = -0x10;
        m = highMask;
        MATCH_KEEP(m);
        b = *p;
        m &= b;
        m |= pal;
        *p = m;
    }
    {
        MATCH_HOLD_REG(s32, pal, r0) = GetSpriteAnimPaletteSlot((struct actor *)self->sprites[9]);
        u8 *p = (u8 *)self->sprites[9] + 0x29;
        MATCH_HOLD_REG(s32, b, r5);

        pal &= lowMask;
        b = *p;
        highMask &= b;
        highMask |= pal;
        *p = highMask;
    }
    if (self->world <= 2) {
        MATCH_HOLD_REG(struct sprite *, s, r1);
        MATCH_HOLD_REG(s32, f, r4);

        if (LevelSelectIsNextWorldOpen(self)) {
            s = self->sprites[8];
            f = 0;
        } else {
            s = self->sprites[8];
            f = 1;
        }
        {
            MATCH_HOLD_REG(const struct sprite_bank *, a, r0) = s->anim;
            MATCH_HOLD_REG(u8 *, pi, r3) = &s->animIndex;
            MATCH_HOLD_REG(const struct sprite_anim *, recs, r2) = a->anims;
            MATCH_HOLD_REG(u32, idx, r5) = *pi;
            MATCH_HOLD_REG(const struct sprite_anim *, rec, r0) =
                (const struct sprite_anim *)(idx * sizeof(struct sprite_anim) + (u32)recs);
            MATCH_HOLD_REG(s32, n, r2) = rec->frameCount;
            MATCH_HOLD_REG(struct sprite *, t, r0) = s;

            CLAMP_INDEX(f, n);
            t->frame = f;
            DrawSpriteWithOffset((struct actor *)t, 0, 0);
        }
    }
    if (LevelSelectHasPrevWorld(self)) {
        MATCH_HOLD_REG(struct sprite *, s, r3) = self->sprites[9];
        MATCH_HOLD_REG(s32, f, r4) = 0;
        MATCH_HOLD_REG(const struct sprite_bank *, a, r0) = s->anim;
        MATCH_HOLD_REG(u8 *, pi, r2) = &s->animIndex;
        MATCH_HOLD_REG(const struct sprite_anim *, recs, r1) = a->anims;
        MATCH_HOLD_REG(u32, idx, r5) = *pi;
        MATCH_HOLD_REG(const struct sprite_anim *, rec, r0) =
            (const struct sprite_anim *)(idx * sizeof(struct sprite_anim) + (u32)recs);
        MATCH_HOLD_REG(s32, n, r0) = rec->frameCount;

        CLAMP_INDEX(f, n);
        s->frame = f;
        DrawSpriteWithOffset((struct actor *)s, 0, 0);
    }
}

/* Draws the record panel sprites 0-4 at their per-row offsets and, if
 * the level is cleared, its time readout (DrawLevelSelectTime). */
void DrawLevelSelectRecord(struct level_menu *self)
{
    DrawSpriteWithOffset((struct actor *)self->sprites[0], -self->panelSlideX, 0);
    DrawSpriteWithOffset((struct actor *)self->sprites[1], -self->panelSlideX, 0);
    DrawSpriteWithOffset((struct actor *)self->sprites[2], -self->panelSlideX, self->clearedIconY);
    DrawSpriteWithOffset((struct actor *)self->sprites[3], -self->panelSlideX, self->flag1IconY);
    if (self->rank != 5)
        DrawSpriteWithOffset((struct actor *)self->sprites[4], -self->panelSlideX, self->gemIconY);
    {
        u8 **ps = (u8 **)&self->save;
        s32 off = self->levelId * 4 + 4;
        struct level_save_h *sv = (struct level_save_h *)(*ps + off);
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
    const struct level_info *info;

    DrawSpriteWithOffset((struct actor *)self->sprites[5], -self->panelSlideX, self->trialIconY);
    DrawSpriteWithOffset((struct actor *)self->sprites[6], -self->panelSlideX, self->trialIcon2Y);
    info = &gLevelTable[self->levelId];
    if (time != 0 && time <= info->times[2]) {
        struct icon_slot *slot;

        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10,
                   gLevelSelectTimePos.y - 8);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, (s32)self->timeText, slot->ptr);
    } else {
        struct icon_slot *slot;

        DrawSpriteWithOffset((struct actor *)self->sprites[7], self->panelSlideX, 0);
        FontSetPalette(gLargeFont, self->sprites[7]->palette);
        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10,
                   gLevelSelectTimePos.y - 8);
        slot = &gLargeFont->record->slots[2];
        _call_via_r2((u8 *)gLargeFont + slot->offset, (s32)self->recordText, slot->ptr);
        FontResetPalette(gLargeFont);
        SetIconPos(gLargeFont, self->panelSlideX + gLevelSelectTimePos.x + 10,
                   gLevelSelectTimePos.y + 8);
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
    struct level_item **items;
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
        if (!IsLevelSelectEntrySelected(ItemAt(items, self->index))) {
            struct level_item *it = ItemAt(items, self->index);

            SetLevelSelectEntrySelected(it, 1);
            if (!IsZoomBgShown(self->bg2)) {
                const struct level_info *info;

                self->levelId = GetLevelSelectEntryLevel(it);
                info = &gLevelTable[self->levelId];
                SetZoomBgPicture(self->bg2, info->theme);
                self->nameText = GetUiText(info->nameText);
            }
            self->panelSlideX = 0;
            if (self->index <= 4) {
                LoadLevelSelectRecord(self);
                FreeUnlockedPaletteSlots(gPaletteCache);
                ReloadLevelSelectPalette(self);
            }
            FontResetPalette(gLargeFont);
        }
    }
draw:
    sprites = self->sprites;
    for (i = 5; i >= 0; i--) {
        struct level_item *it = *items++;
        struct item_vtable *vt = it->vtable;

        _call_via_r1((u8 *)it + vt->m20.thisOffset, vt->m20.fn);
    }
    UpdateZoomBg(self->bg2);
    {
        struct sprite **p = sprites + 2;

        for (i = 5; i >= 0; i--)
            AdvanceSpriteAnim((struct box_part *)*p++);
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
    struct level_save_h *sv;

    *rank = 5;
    if ((u8)LevelHasGemPathGem(gLevelState, self->levelId))
        *rank = 0;
    if ((u8)LevelHasRedGem(gLevelState, self->levelId))
        *rank = 1;
    if ((u8)LevelHasGreenGem(gLevelState, self->levelId))
        *rank = 2;
    if ((u8)LevelHasBlueGem(gLevelState, self->levelId))
        *rank = 3;
    if ((u8)LevelHasYellowGem(gLevelState, self->levelId))
        *rank = 4;
    self->clearedIconY = 0;
    self->flag1IconY = 0;
    self->gemIconY = 0;
    self->trialIconY = 0;
    self->trialIcon2Y = 0;
    sv = (struct level_save_h *)((u8 *)self->save + (self->levelId * 4 + 4));
    if (sv->cleared)
        self->clearedIconY = 0x1C;
    if (sv->flag1)
        self->flag1IconY = 0x1C;
    switch (*rank) {
    case 0:
        if (sv->flag2)
            self->gemIconY = 0x1C;
        break;
    case 1:
        if (self->save->flags & 1)
            self->gemIconY = 0x1C;
        break;
    case 2:
        if (self->save->flags & 4)
            self->gemIconY = 0x1C;
        break;
    case 3:
        if (self->save->flags & 8)
            self->gemIconY = 0x1C;
        break;
    case 4:
        if (self->save->flags & 2)
            self->gemIconY = 0x1C;
        break;
    case 5:
        break;
    default:
        goto set_rank_icon;
    }
    if (self->rank != 5) {
    set_rank_icon:
        SetAnim(self->sprites[4], gLevelSelectRankAnims[self->rank]);
        if (self->flag1IconY == self->gemIconY) {
            self->flag1IconY -= 6;
            self->gemIconY += 6;
        }
    }
    if (sv->cleared) {
        const struct level_info *entry = &gLevelTable[self->levelId];
        const struct level_info *info = entry;

        FormatCentiseconds(info->times[0], self->recordText);
        FormatCentiseconds(sv->time, self->timeText);
        SetAnim(self->sprites[5], 0);
        SetAnim(self->sprites[6], 0);
        SetAnim(self->sprites[7], 0);
        if (sv->time != 0) {
            if (sv->time <= info->times[2]) {
                self->trialIcon2Y = 0x1C;
                self->trialIconY = 0x1C;
                SetAnim(self->sprites[5], 1);
                SetAnim(self->sprites[6], 1);
            } else if (sv->time <= info->times[1]) {
                FormatCentiseconds(info->times[2], self->recordText);
                self->trialIconY = 0x1C;
                SetAnim(self->sprites[5], 2);
                SetAnim(self->sprites[6], 1);
                SetAnim(self->sprites[7], 1);
            } else if (sv->time <= entry->times[0]) {
                FormatCentiseconds(info->times[1], self->recordText);
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
 * (docs/matching/archive/near-miss-polish.md): the key-word copy the ROM makes
 * inside the 0x80 test comes from a statement expression holding the
 * copy plus an empty `asm` that keeps gcc from merging it. */
s32 LevelSelectLoop(struct level_menu *self)
{
    const struct level_info *info;

    self->result = 0;
    self->levelId = GetLevelSelectEntryLevel(self->items[self->index]);
    info = &gLevelTable[self->levelId];
    SetZoomBgPicture(self->bg2, info->theme);
    self->nameText = GetUiText(info->nameText);
    while (!IsZoomBgShown(self->bg2)) {
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
    PlaySfx(gAudioContext, SFX_ZOOM_BG_SHOWN, 0x100);
    self->blend.raw = 0;
    self->blend.bits.bg0Second = 1;
    self->blend.bits.bg1Second = 1;
    self->blend.bits.bg2Second = 1;
    self->blend.bits.bg3Second = 1;
    self->blend.bits.bdSecond = 1;
    self->blend.bits.eva = 0x10;
    self->blend.bits.evb = 0x10;
    if (gNewWorldOpened && LevelSelectIsNextWorldOpen(self)) {
        self->index = 0;
        LevelSelectNextWorld(self);
    }
    gNewWorldOpened = 0;
    goto loop;

check_exit:
    if (gKeys.half.pressed & START_BUTTON) {
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

        if (keys.half.pressed & DPAD_UP)
            LevelSelectNextWorld(self);
        /* The ROM copies the key word between the 0x80 test's `ands`
         * and its `cmp`, and tests 0x20 on the copy; gcc merges a plain
         * copy, so the (code-free) asm keeps `k` a separate value. */
        else if (({
                     u32 hit = keys.half.pressed & DPAD_DOWN;

                     k = keys;
                     MATCH_KEEP(k.all);
                     hit;
                 }))
            LevelSelectPrevWorld(self);
        else {
            if (k.half.pressed & DPAD_LEFT)
                LevelSelectCursorLeft(self);
            else if (keys.half.pressed & DPAD_RIGHT)
                LevelSelectCursorRight(self);
        }
    }
    if (!(gKeys.half.pressed & A_BUTTON))
        goto check_exit;
    if (!IsLevelSelectEntrySelected(self->items[self->index]))
        goto check_exit;
    if (!IsZoomBgShown(self->bg2))
        goto check_exit;
    LevelSelectConfirm(self);
end:
    self->dispcnt.raw = 0;
    self->dispcnt.bits.objMap1D = 1;
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
    while (IsZoomBgZoomingOut(self->bg2) || !(u8)HasLevelSelectCursorArrived(self->panel)) {
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
    if (self->index == 0) {
        PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
        return;
    }
    SetLevelSelectEntrySelected(self->items[self->index], 0);
    ClearZoomBgPicture(self->bg2);
    while (self->index != 0) {
        const struct xy_pair *pos;

        self->index--;
        pos = &self->positions[self->index];
        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
        WaitLevelSelectCursor(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_LEFT))
            return;
    }
}

/* Moves the cursor right, repeating while Right is held; sound 0x48 at
 * the last entry. */
void LevelSelectCursorRight(struct level_menu *self)
{
    if (self->index == self->lastIndex) {
        PlaySfx(gAudioContext, SFX_MENU_ERROR, 0x100);
        return;
    }
    SetLevelSelectEntrySelected(self->items[self->index], 0);
    ClearZoomBgPicture(self->bg2);
    while (self->index < self->lastIndex) {
        const struct xy_pair *pos;

        self->index++;
        pos = &self->positions[self->index];
        MoveLevelSelectCursor(self->panel, pos->x, pos->y - 0x18);
        WaitLevelSelectCursor(self);
        UpdateKeys(gInput);
        if (!(gKeys.all & DPAD_RIGHT))
            return;
    }
}
