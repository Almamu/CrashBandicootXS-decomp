#include "spawners.hpp"

extern "C" {
#include "bosses.h"
#include "level.h"
#include "globals.h"
}

/* 0x0801EA5C-0x0801EF0C (GitHub issue #30), formerly
 * asm/code_3_2_17_1e990.s: six spawners, five of them entity spawners of
 * gEntitySpawnFuncs (types 0x07, 0x08, 0x0A-0x0C) and SpawnCrateGem, which
 * level_state.cpp calls directly. Each one
 * spawns a sprite (Sprite::Create, CreateSpriteObj) with a fixed bank offset, tag and type byte
 * (+0x0A) and registers it with the gTouchableList manager, unless
 * the level's "already collected" bit for it is set:
 *
 * - SpawnCrystal/SpawnCrateGem/SpawnGemPathGem test bits 0/1/2 of the byte
 *   GetCurrentLevelFlags(gLevelState) points at; SpawnCrateGem also spawns a
 *   second (0x2B) effect through SpawnEffectPart.
 * - SpawnRedGem/SpawnGreenGem/SpawnYellowGem first ask GetBossIndex whether
 *   the level is in mode 1, and if so hand over to SpawnCortexBossGem
 *   (cortex.cpp) with kind 0/1/2 instead; otherwise they test
 *   bits 0/2/1 of gLevelState+2.
 *
 * C++ since #664 part 9 (include/spawners.hpp), built with old_agbcp
 * (Makefile OLD_AGBCC_OBJS): the ROM materializes each bit mask before
 * loading the byte it is ANDed with, old_agbcc's tell. Under it all six
 * were plain C with no pins, and are plain C++ now (the current agbcc
 * misses all six - likely also what parked the spawn_gem_platforms.cpp
 * siblings, issue #31). See docs/matching/archive/issue-30-graphics-loading.md,
 * "Tenth pass". */

/* The `tag`/`type` locals are not just naming: the ROM loads both
 * constants into callee-saved registers before the Sprite::Create call and
 * stores them from there afterwards, which is how this compiler treats a
 * variable set before the call (a literal would be loaded at the store). */
void SpawnCrystal(u32 a0, u16 a1, u16 a2, u16 a3)
{
    s32 bit = *GetCurrentLevelFlags(gLevelState) & LEVEL_FLAG_CRYSTAL;

    if (bit == 0) {
        u8 type = 0x1B;
        Sprite *part = Sprite::Create(a0, a1, a2, a3);

        part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x1BC);
        part->tag = bit;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);
    }
}

void SpawnCrateGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    u8 bit = *GetCurrentLevelFlags(gLevelState) & LEVEL_FLAG_CRATE_GEM;

    if (bit == 0) {
        u8 tag = 1;
        u8 type = 0x1D;
        Sprite *part = Sprite::Create(a0, a1, a2, a3);

        part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);

        {
            MovingSprite *p = gEntitySpawner->SpawnEffectPart(0x2B, 2, a1, a2, bit);
            p->mirrorBits.gfxMode = 1;
            p->f.b.visible = 0;
        }
    }
}

void SpawnGemPathGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if ((*GetCurrentLevelFlags(gLevelState) & LEVEL_FLAG_GEM_PATH_GEM) == 0) {
        u8 tag = 1;
        u8 type = 0x1E;
        Sprite *part = Sprite::Create(a0, a1, a2, a3);

        part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);
    }
}

void SpawnRedGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != BOSS_NEO_CORTEX) {
        if ((gLevelState->progress.flags & 1) == 0) {
            u8 tag = 3;
            u8 type = 0x1F;
            Sprite *part = Sprite::Create(a0, a1, a2, a3);

            part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = tag;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(a0, a1, a2, a3, 0);
    }
}

void SpawnGreenGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != BOSS_NEO_CORTEX) {
        if ((gLevelState->progress.flags & 4) == 0) {
            u8 tag = 2;
            u8 type = 0x20;
            Sprite *part = Sprite::Create(a0, a1, a2, a3);

            part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = tag;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(a0, a1, a2, a3, 1);
    }
}

void SpawnYellowGem(u32 a0, u16 a1, u16 a2, u16 a3)
{
    if (GetBossIndex(gLevelState) != BOSS_NEO_CORTEX) {
        u8 bit = gLevelState->progress.flags & 2;

        if (bit == 0) {
            u8 type = 0x22;
            Sprite *part = Sprite::Create(a0, a1, a2, a3);

            part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
            part->tag = bit;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(a0, a1, a2, a3, 2);
    }
}
