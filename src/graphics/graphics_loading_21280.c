#include "core.h"
#include "text_popup.h"

/* Text-popup variants with their own header constructors, ROM
 * 0x08021280-0x08021668. Built with old_agbcc; see include/text_popup.h. */

extern void *gLevelState;
extern void *gUnknown_030012F4;

extern struct enemy_ctrl *CreateDingodile(void *block, u16 arg1, u16 arg2);
extern void sub_8023318(void *self, struct enemy_ctrl *hdr);
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
extern void *gUnknown_030012E8;
extern u8 IsInGemPath(void *self);
extern u8 IsInBonusRound(void *self);
extern s32 sub_8023324(void *self);
extern s32 GetCurrentLevel(void *self);
extern struct spawn_part *sub_80071E4(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80070EC(struct spawn_part *part, s32 w, s32 h);
extern s32 *CreatePlatform(u16 x, u16 y, u16 w, u16 h, s32 id);
extern void SetCrateGemPos(void *self, s32 *point);

/* Three-way spawner. While the level controller reports nothing pending
 * and the current level's table entry has no guard, spawns a 0x64x0x64
 * sub_80071E4 part tagged 0x12. Otherwise, unless gPlayer's
 * +0x88 flag is set, hands SetCrateGemPos a point just above-left of a
 * CreatePlatform probe; with the flag set it spawns a 0x28x0x28 part.
 * The ROM computes the point's x/y into fresh registers
 * (`subs r2, r1, #2`; `adds r3, r0, #0; subs r3, #30`) where plain C
 * reuses their inputs (9 halfwords off), the same gap as sub_802209C
 * (graphics_loading_21d80.c). Pinning the four temporaries plus an empty
 * `asm("" : "+r" (x))` - which stops combine folding `x` back into its
 * input before `y` is loaded - reproduces it. */
void sub_8021280(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (!IsInGemPath(gLevelState) && !IsInBonusRound(gLevelState)
        && !sub_8023324(gLevelState)
        && gLevelTable[GetCurrentLevel(gLevelState)].guard == 0)
    {
        struct spawn_part *part = sub_80071E4(arg0, arg1, arg2, arg3);

        sub_80070EC(part, 0x64, 0x64);
        part->field_0A = 0x12;
        sub_8008E94(gUnknown_030012E8, part);
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
        struct spawn_part *part = sub_80071E4(arg0, arg1, arg2, arg3);

        sub_80070EC(part, 0x28, 0x28);
        part->field_0A = 0x12;
        sub_8008E94(gUnknown_030012E8, part);
    }
}

/* "Two-line text popup" variant with its own header: instead of
 * CreateEnemyCtrl it builds one with CreateDingodile in a fresh 0x30-byte block
 * (from arg1/arg2), attaches the part to it once, and registers the
 * header with the level controller via sub_8023318. */
void sub_8021388(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x288);
    part->frameNibble = sub_800815C(part);
    part->base.flags |= 0x10;
    SetPartField0A(part, 1);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    sub_8008E94(gUnknown_030012F0, part);
    hdr = CreateDingodile(sub_8026EDC(0x30), arg1, arg2);
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    sub_8023318(gLevelState, hdr);
}

/* "Two-line text popup" variant whose header comes from CreateTiny
 * (after a 0x4c-byte sub_8026EDC reservation). Attaches the part once,
 * packs the collected bits, sets flag bit 4, and registers the part and
 * the header with the manager and the level controller. */
void sub_8021480(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x294);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x4c);
    hdr = CreateTiny();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012F0, part);
    sub_8023318(gLevelState, hdr);
}

/* "Two-line text popup" variant with the OAM-trio setup: animation 1 at
 * +0x27c, header from CreateCortexBoss (after a 0x24-byte sub_8026EDC
 * reservation), collected bits and flag bit 4, then registration with
 * gUnknown_030012F4's manager and the level controller. */
void SpawnCortexBoss(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;
    struct level_record *rec;

    part->anim = POPUP_ANIM(0x27c);
    SetPartTag(part, 1);
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    sub_8026EDC(0x24);
    hdr = CreateCortexBoss();
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    rec = LEVEL_RECORD(arg3);
    part->flipX = (rec->flags >> 1 ^ 1) & 1;
    part->unk_28_5 = rec->flags >> 2 & 1;
    part->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012F4, part);
    part->unk_2A[2] = 0;
    sub_8023318(gLevelState, hdr);
}
