#include "core.h"
#include "gfx_part.h"
#include "bosses.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* 0x0801EA5C-0x0801EF0C (GitHub issue #30), formerly
 * asm/code_3_2_17_1e990.s: six of the "trigger effect type N" spawners
 * reached through the trigger dispatch table at gStaticData_0816C6C0
 * (SpawnCrateGem is also called directly by level_state.c). Each one
 * spawns a CreateSpriteObj part with a fixed bank offset, tag and type byte
 * (+0x0A) and registers it with the gUnknown_030012EC manager, unless
 * the level's "already collected" bit for it is set:
 *
 * - SpawnCrystal/SpawnCrateGem/SpawnGemPathGem test bits 0/1/2 of the byte
 *   GetCurrentLevelFlags(gLevelState) points at; SpawnCrateGem also spawns a
 *   second (0x2B) effect through SpawnEffectPart.
 * - SpawnRedGem/SpawnGreenGem/SpawnYellowGem first ask GetBossIndex whether
 *   the level is in mode 1, and if so hand over to SpawnCortexBossGem
 *   (cortex.c) with kind 0/1/2 instead; otherwise they test
 *   bits 0/2/1 of gLevelState+2.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS): the ROM materializes
 * each bit mask before loading the byte it is ANDed with, old_agbcc's
 * tell. Under it all six are plain C with no pins (the current agbcc
 * misses all six - likely also what parked the spawn_gem_platforms.c
 * siblings, issue #31). See docs/matching/issue-30-graphics-loading.md,
 * "Tenth pass". */

/* The `tag`/`type` locals are not just naming: the ROM loads both
 * constants into callee-saved registers before the CreateSpriteObj call and
 * stores them from there afterwards, which is how this compiler treats a
 * variable set before the call (a literal would be loaded at the store). */
void SpawnCrystal(u32 a0, u16 a1, u16 a2, u16 a3)
{
    s32 bit = *GetCurrentLevelFlags(gLevelState) & 1;

    if (bit == 0)
    {
        u8 type = 0x1B;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x1BC);
        part->tag = bit;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = type;
        AddToPartList(gUnknown_030012EC, part);
    }
}

void SpawnCrateGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = *GetCurrentLevelFlags(gLevelState) & 2;

    if (bit == 0)
    {
        u8 tag = 1;
        u8 type = 0x1D;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = type;
        AddToPartList(gUnknown_030012EC, part);

        {
            struct gfx_part *p = SpawnEffectPart(gEntitySpawner, 0x2B, 2, a1, a2, bit);
            p->unk_28_0 = 1;
            p->hidden = 0;
        }
    }
}

void SpawnGemPathGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if ((*GetCurrentLevelFlags(gLevelState) & 4) == 0)
    {
        u8 tag = 1;
        u8 type = 0x1E;
        struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

        part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
        part->tag = tag;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
        part->kind = type;
        AddToPartList(gUnknown_030012EC, part);
    }
}

void SpawnRedGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != 1)
    {
        if ((gLevelState->flags & 1) == 0)
        {
            u8 tag = 3;
            u8 type = 0x1F;
            struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = tag;
            ResetSpriteFrameTimer(part);
            ResetSpriteFrameIndex(part);
            SetSpriteAnimDone(part, 0);
            part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
            part->kind = type;
            AddToPartList(gUnknown_030012EC, part);
        }
    }
    else
    {
        SpawnCortexBossGem(a0, a1, a2, a3, 0);
    }
}

void SpawnGreenGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != 1)
    {
        if ((gLevelState->flags & 4) == 0)
        {
            u8 tag = 2;
            u8 type = 0x20;
            struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = tag;
            ResetSpriteFrameTimer(part);
            ResetSpriteFrameIndex(part);
            SetSpriteAnimDone(part, 0);
            part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
            part->kind = type;
            AddToPartList(gUnknown_030012EC, part);
        }
    }
    else
    {
        SpawnCortexBossGem(a0, a1, a2, a3, 1);
    }
}

void SpawnYellowGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != 1)
    {
        u8 bit = gLevelState->flags & 2;

        if (bit == 0)
        {
            u8 type = 0x22;
            struct gfx_part *part = CreateSpriteObj(a0, a1, a2, a3);

            part->bank = (struct anim_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = bit;
            ResetSpriteFrameTimer(part);
            ResetSpriteFrameIndex(part);
            SetSpriteAnimDone(part, 0);
            part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
            part->kind = type;
            AddToPartList(gUnknown_030012EC, part);
        }
    }
    else
    {
        SpawnCortexBossGem(a0, a1, a2, a3, 2);
    }
}
