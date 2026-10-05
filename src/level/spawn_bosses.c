#include "core.h"
#include "text_popup.h"

/* Text-popup variants with their own header constructors, ROM
 * 0x08021280-0x08021668. Built with old_agbcc; see include/text_popup.h. */

extern void *gLevelState;
extern void *gUnknown_030012F4;

extern struct enemy_ctrl *CreateDingodile(void *block, u16 arg1, u16 arg2);
extern void SetLevelBoss(void *self, struct enemy_ctrl *hdr);
extern struct enemy_ctrl *CreateTiny(void);
extern struct enemy_ctrl *CreateCortexBoss(void);

struct level_guard
{
    s32 unk_00;
    s32 guard;
    u8 unk_08[0x1C];
};

struct spawn_part
{
    u8 unk_00[0xA];
    u8 field_0A;
};

extern struct level_guard gLevelTable[];
extern u8 *gPlayer;
extern void *gUpdateOnlyPartList;
extern u8 IsInGemPath(void *self);
extern u8 IsInBonusRound(void *self);
extern s32 sub_8023324(void *self);
extern s32 GetCurrentLevel(void *self);
extern struct spawn_part *CreateEntity(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void SetEntitySize(struct spawn_part *part, s32 w, s32 h);
extern s32 *CreatePlatform(u16 x, u16 y, u16 w, u16 h, s32 id);
extern void SetCrateGemPos(void *self, s32 *point);

/* Entity type 0x55, the room's exit. Every room but room 16 and the boss
 * rooms places exactly one. The tag-0x12 zone is collision class 0x12,
 * the player event that PlayerHandleEvent answers with RequestRoomExit;
 * the kind-4 platform is bank 39's glowing exit pad.
 *
 * Three-way spawner. While the level controller reports nothing pending
 * and the current level's table entry has no guard, spawns a 0x64x0x64
 * CreateEntity part tagged 0x12. Otherwise, unless gPlayer's
 * +0x88 flag is set, hands SetCrateGemPos a point just above-left of a
 * CreatePlatform probe; with the flag set it spawns a 0x28x0x28 part.
 * The ROM computes the point's x/y into fresh registers
 * (`subs r2, r1, #2`; `adds r3, r0, #0; subs r3, #30`) where plain C
 * reuses their inputs (9 halfwords off), the same gap as SpawnCrateGemMarker
 * (graphics_loading_21d80.c). Pinning the four temporaries plus an empty
 * `asm("" : "+r" (x))` - which stops combine folding `x` back into its
 * input before `y` is loaded - reproduces it. */
void SpawnRoomExit(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (!IsInGemPath(gLevelState) && !IsInBonusRound(gLevelState)
        && !sub_8023324(gLevelState)
        && gLevelTable[GetCurrentLevel(gLevelState)].guard == 0)
    {
        struct spawn_part *part = CreateEntity(arg0, arg1, arg2, arg3);

        SetEntitySize(part, 0x64, 0x64);
        part->field_0A = 0x12;
        AddToPartList(gUpdateOnlyPartList, part);
    }
    else if (gPlayer[0x88] == 0)
    {
        s32 *pos = CreatePlatform(arg0, arg1, arg2, arg3, 4);
        register s32 px asm("r1") = pos[0] >> 8;
        register s32 x asm("r2") = px - 2;
        register s32 py asm("r0");
        register s32 y asm("r3");
        s32 point[2];

        asm("" : "+r" (x));
        py = pos[1] >> 8;
        y = py - 0x1E;
        point[0] = x;
        point[1] = y;
        SetCrateGemPos(gLevelState, point);
    }
    else
    {
        struct spawn_part *part = CreateEntity(arg0, arg1, arg2, arg3);

        SetEntitySize(part, 0x28, 0x28);
        part->field_0A = 0x12;
        AddToPartList(gUpdateOnlyPartList, part);
    }
}

/* "Two-line text popup" variant with its own header: instead of
 * CreateEnemyCtrl it builds one with CreateDingodile in a fresh 0x30-byte block
 * (from arg1/arg2), attaches the part to it once, and registers the
 * header with the level controller via SetLevelBoss. */
void SpawnDingodile(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x288);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    part->base.flags |= 0x10;
    SetPartField0A(part, 1);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    AddToPartList(gCollidableList, part);
    hdr = CreateDingodile(OperatorNew(0x30), arg1, arg2);
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    SetLevelBoss(gLevelState, hdr);
}

/* "Two-line text popup" variant whose header comes from CreateTiny
 * (after a 0x4c-byte OperatorNew reservation). Attaches the part once,
 * packs the collected bits, sets flag bit 4, and registers the part and
 * the header with the manager and the level controller. */
void SpawnTiny(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x294);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x4c);
    hdr = CreateTiny();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    AddToPartList(gCollidableList, part);
    SetLevelBoss(gLevelState, hdr);
}

/* "Two-line text popup" variant with the OAM-trio setup: animation 1 at
 * +0x27c, header from CreateCortexBoss (after a 0x24-byte OperatorNew
 * reservation), collected bits and flag bit 4, then registration with
 * gUnknown_030012F4's manager and the level controller. */
void SpawnCortexBoss(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x27c);
    SetPartTag(part, 1);
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    part->frameNibble = GetSpriteAnimPaletteSlot(part);
    OperatorNew(0x24);
    hdr = CreateCortexBoss();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->flipY = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    AddToPartList(gUnknown_030012F4, part);
    part->animating = 0;
    SetLevelBoss(gLevelState, hdr);
}
