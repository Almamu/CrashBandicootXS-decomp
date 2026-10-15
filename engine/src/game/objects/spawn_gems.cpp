#include "game/level/spawners.hpp"
#include "game/level/level_state.hpp"

extern "C" {
#include "game/bosses/bosses.h"
#include "game/level/level.h"
#include "globals.h"
}

using Game::Entities::MovingSprite;
using Game::Entities::Sprite;
using Game::Entities::TouchableList;
using Game::Level::EntitySpawner;
using Game::Objects::Pickups;

/* 0x0801EA5C-0x0801EF0C (GitHub issue #30), formerly
 * asm/code_3_2_17_1e990.s: six spawners, five of them entity spawners of
 * gEntitySpawnFuncs (types 0x07, 0x08, 0x0A-0x0C) and SpawnCrateGem, which
 * level_state.cpp calls directly. Each one
 * spawns a sprite (Sprite::Create, CreateSpriteObj) with a fixed bank, tag and type byte
 * (+0x0A) and registers it with the gTouchableList manager, unless
 * the level's "already collected" bit for it is set:
 *
 * - SpawnCrystal/SpawnCrateGem/SpawnGemPathGem test bits 0/1/2 of the byte
 *   gLevelState->GetCurrentLevelFlags() points at; SpawnCrateGem also spawns a
 *   second (0x2B) effect through SpawnEffectPart.
 * - SpawnRedGem/SpawnGreenGem/SpawnYellowGem first ask GetBossIndex whether
 *   the level is in mode 1, and if so hand over to SpawnCortexBossGem
 *   (cortex.cpp) with kind 0/1/2 instead; otherwise they test
 *   bits 0/2/1 of gLevelState+2.
 *
 * C++ since #664 part 9 (engine/include/game/level/spawners.hpp), built with old_agbcp
 * (Makefile OLD_AGBCC_OBJS): the ROM materializes each bit mask before
 * loading the byte it is ANDed with, old_agbcc's tell. Under it all six
 * were plain C with no pins, and are plain C++ now (the current agbcc
 * misses all six - likely also what parked the spawn_gem_platforms.cpp
 * siblings, issue #31). See docs/matching/archive/issue-30-graphics-loading.md,
 * "Tenth pass". Since #915 all six are static members of Pickups
 * (Pickups::SpawnCrystal, ..., engine/include/game/objects/pickups.hpp). */

/* The `tag`/`type` locals are not just naming: the ROM loads both
 * constants into callee-saved registers before the Sprite::Create call and
 * stores them from there afterwards, which is how this compiler treats a
 * variable set before the call (a literal would be loaded at the store). */
void Pickups::SpawnCrystal(u32 id, u16 x, u16 y, u16 param)
{
    s32 bit = gLevelState->GetCurrentLevelFlags()->flags & LEVEL_FLAG_CRYSTAL;

    if (bit == 0) {
        u8 type = EVENT_CRYSTAL;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(37);
        part->tag = bit;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);
    }
}

void Pickups::SpawnCrateGem(u32 id, u16 x, u16 y, u16 param)
{
    u8 bit = gLevelState->GetCurrentLevelFlags()->flags & LEVEL_FLAG_CRATE_GEM;

    if (bit == 0) {
        /* `tag`, `type` are needed: inlined, the code is 4 instructions shorter */
        u8 tag = 1;
        u8 type = EVENT_CRATE_GEM;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);

        MovingSprite *p = EntitySpawner::Get()->SpawnEffectPart(0x2B, 2, x, y, bit);
        p->mirrorBits.gfxMode = OAM_OBJ_MODE_SEMI_TRANSPARENT;
        p->f.b.visible = 0;
    }
}

void Pickups::SpawnGemPathGem(u32 id, u16 x, u16 y, u16 param)
{
    if ((gLevelState->GetCurrentLevelFlags()->flags & LEVEL_FLAG_GEM_PATH_GEM) == 0) {
        /* `tag`, `type` are needed: inlined, the code is 4 instructions shorter */
        u8 tag = 1;
        u8 type = EVENT_GEM_PATH_GEM;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = type;
        TouchableList()->Add(part);
    }
}

void Pickups::SpawnRedGem(u32 id, u16 x, u16 y, u16 param)
{
    if (gLevelState->GetBossIndex() != BOSS_NEO_CORTEX) {
        if ((gLevelState->progress.flags & PROGRESS_FLAG_RED_GEM) == 0) {
            /* `tag`, `type` are needed: inlined, the code is 5 instructions shorter */
            u8 tag = 3;
            u8 type = EVENT_RED_GEM;
            Sprite *part = Sprite::Create(id, x, y, param);

            part->bank = gSpriteBankSet->Bank(32);
            part->tag = tag;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(id, x, y, param, CORTEX_BOSS_GEM_KIND_RED);
    }
}

void Pickups::SpawnGreenGem(u32 id, u16 x, u16 y, u16 param)
{
    if (gLevelState->GetBossIndex() != BOSS_NEO_CORTEX) {
        if ((gLevelState->progress.flags & PROGRESS_FLAG_GREEN_GEM) == 0) {
            /* `tag`, `type` are needed: inlined, the code is 5 instructions shorter */
            u8 tag = 2;
            u8 type = EVENT_GREEN_GEM;
            Sprite *part = Sprite::Create(id, x, y, param);

            part->bank = gSpriteBankSet->Bank(32);
            part->tag = tag;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(id, x, y, param, CORTEX_BOSS_GEM_KIND_GREEN);
    }
}

void Pickups::SpawnYellowGem(u32 id, u16 x, u16 y, u16 param)
{
    if (gLevelState->GetBossIndex() != BOSS_NEO_CORTEX) {
        u8 bit = gLevelState->progress.flags & PROGRESS_FLAG_YELLOW_GEM;

        if (bit == 0) {
            /* `type` is needed: inlined, the code is 5 instructions shorter */
            u8 type = EVENT_YELLOW_GEM;
            Sprite *part = Sprite::Create(id, x, y, param);

            part->bank = gSpriteBankSet->Bank(32);
            part->tag = bit;
            part->ResetFrameTimer();
            part->ResetFrameIndex();
            part->SetAnimDone(0);
            part->palette = part->GetAnimPaletteSlot();
            part->kind = type;
            TouchableList()->Add(part);
        }
    } else {
        SpawnCortexBossGem(id, x, y, param, CORTEX_BOSS_GEM_KIND_YELLOW);
    }
}
