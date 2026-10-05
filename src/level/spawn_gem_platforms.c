#include "core.h"
#include "gfx_part.h"
#include "objects.h"

/* 0x08020E84-0x08021280 (GitHub issue #31): four of the "trigger effect
 * type N" spawners reached through the 15-slot dispatch table at
 * gStaticData_0816C7D8 (docs/rom_map.md, "A family of 'trigger effect
 * type N' functions") - the same family as SpawnCrystal-SpawnYellowGem
 * (src/level/spawn_gems.c).
 *
 * Each one tests one "collected" bit of the level progress record
 * (gLevelState + 2). If it is set, the effect only plays a sound:
 * CreatePlatform with a per-slot id, or the shared id 0xC when
 * IsGemPathDone says so or the record's +0x8C byte is set, handed to
 * SetGemPlatform. Otherwise it spawns the full visual effect: a
 * CreateSpriteObj part on anim bank offset 0x180, with a per-slot tag,
 * built through the ResetSpriteFrameTimer/ResetSpriteFrameIndex/SetSpriteAnimDone OAM trio, and
 * registered with the gUnknown_030012EC manager.
 *
 *   function     bit  sound  tag
 *   SpawnRedGemPlatform   1    0xB    7
 *   SpawnYellowGemPlatform   2    0x3    5
 *   SpawnGreenGemPlatform   4    0xA    6
 *   SpawnBlueGemPlatform   8    0x9    8
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM materializes
 * the bit mask before loading the byte it is ANDed with, old_agbcc's
 * tell. Under it all four are plain C with no pins; they had been
 * parked as NAKED after drafts under the current agbcc stalled on
 * register allocation (see docs/matching/issue-31-trigger-effect-type-n.md,
 * "Old-compiler pass"). The two CreatePlatform calls are written
 * separately, one per sound id: the ROM repeats the a0 truncation in
 * both arms and shares the rest of the call, which is gcc's
 * cross-jumping of two call sites (a single call with an `id` variable
 * truncates a0 once, after the join). */

struct level_progress
{
    u8 unk_00[2];
    u8 collected;   // 0x02 - one bit per trigger effect slot
    u8 unk_03[0x89];
    u8 timeTrial;      // 0x8C
};

extern struct level_progress *gLevelState;
extern u8 ***gSpriteBankSet;
extern void *gUnknown_030012EC;

extern u8 IsGemPathDone(struct level_progress *self);
extern void SetGemPlatform(struct level_progress *self, void *handle);

/* The `tag` locals are set before the CreateSpriteObj call on purpose: the
 * ROM loads the constant into a callee-saved register up front and
 * stores it from there afterwards. */
void SpawnRedGemPlatform(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gLevelState->collected & 1;

    if (bit)
    {
        void *snd;

        if (IsGemPathDone(gLevelState) || gLevelState->timeTrial)
            snd = CreatePlatform(a0, a1, a2, a3, 0xC);
        else
            snd = CreatePlatform(a0, a1, a2, a3, 0xB);
        SetGemPlatform(gLevelState, snd);
    }
    else
    {
        u8 tag = 7;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gSpriteBankSet + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = bit;
        AddToPartList(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

void SpawnYellowGemPlatform(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gLevelState->collected & 2;

    if (bit)
    {
        void *snd;

        if (IsGemPathDone(gLevelState) || gLevelState->timeTrial)
            snd = CreatePlatform(a0, a1, a2, a3, 0xC);
        else
            snd = CreatePlatform(a0, a1, a2, a3, 0x3);
        SetGemPlatform(gLevelState, snd);
    }
    else
    {
        u8 tag = 5;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gSpriteBankSet + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = bit;
        AddToPartList(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

void SpawnGreenGemPlatform(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gLevelState->collected & 4;

    if (bit)
    {
        void *snd;

        if (IsGemPathDone(gLevelState) || gLevelState->timeTrial)
            snd = CreatePlatform(a0, a1, a2, a3, 0xC);
        else
            snd = CreatePlatform(a0, a1, a2, a3, 0xA);
        SetGemPlatform(gLevelState, snd);
    }
    else
    {
        u8 tag = 6;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gSpriteBankSet + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = bit;
        AddToPartList(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}

/* Mask and tag are the same constant here, so the ROM keeps one copy of
 * it in a callee-saved register for both uses. */
void SpawnBlueGemPlatform(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = gLevelState->collected & 8;

    if (bit)
    {
        void *snd;

        if (IsGemPathDone(gLevelState) || gLevelState->timeTrial)
            snd = CreatePlatform(a0, a1, a2, a3, 0xC);
        else
            snd = CreatePlatform(a0, a1, a2, a3, 0x9);
        SetGemPlatform(gLevelState, snd);
    }
    else
    {
        u8 tag = 8;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(**gSpriteBankSet + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = bit;
        AddToPartList(gUnknown_030012EC, part);
        part->hidden = 0;
    }
}
