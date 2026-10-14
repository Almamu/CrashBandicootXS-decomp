#include "game/objects/platform.hpp"
#include "game/level/level_state.hpp"

extern "C" {
#include "game/level/level.h"
#include "globals.h"
}

using Game::Entities::Sprite;
using Game::Entities::TouchableList;
using Game::Objects::Platform;

/* 0x08020E84-0x08021280 (GitHub issue #31): four of the entity spawners
 * of gEntitySpawnFuncs (entity types 0x51-0x54; docs/rom_map.md called
 * them "trigger effect type N" functions) - the same family as
 * SpawnCrystal-SpawnYellowGem (src/game/objects/spawn_gems.cpp).
 *
 * Each one tests one "collected" bit of the level progress record
 * (gLevelState + 2). If it is set, it spawns the gem platform
 * (Platform::Create) with a per-gem kind, or PLATFORM_KIND_GEM_PATH_DONE
 * when IsGemPathDone says so or the record's +0x8C byte is set, handed to
 * SetGemPlatform. Otherwise it spawns the full visual effect: a
 * CreateSpriteObj part on sprite bank 32, with a per-slot tag,
 * built through the ResetSpriteFrameTimer/ResetSpriteFrameIndex/SetSpriteAnimDone OAM trio, and
 * registered with the gTouchableList manager.
 *
 *   function (C name)                                bit  kind (PLATFORM_KIND_*)  tag
 *   Platform::SpawnGemRed (SpawnRedGemPlatform)       1    0xB GEM_RED            7
 *   Platform::SpawnGemYellow (SpawnYellowGemPlatform) 2    0x3 GEM_YELLOW         5
 *   Platform::SpawnGemGreen (SpawnGreenGemPlatform)   4    0xA GEM_GREEN          6
 *   Platform::SpawnGemBlue (SpawnBlueGemPlatform)     8    0x9 GEM_BLUE           8
 *
 * They are Platform's static members since #915 (include/game/objects/platform.hpp).
 *
 * C++ since #664 part 9 (include/game/level/spawners.hpp), built with old_agbcp
 * (Makefile OLD_AGBCC_OBJS): the ROM materializes the bit mask before
 * loading the byte it is ANDed with, old_agbcc's tell. Under it all four
 * were plain C with no pins, and are plain C++ now; they had been
 * parked as NAKED after drafts under the current agbcc stalled on
 * register allocation (see docs/matching/archive/issue-31-trigger-effect-type-n.md,
 * "Old-compiler pass"). The two Platform::Create calls are written
 * separately, one per kind: the ROM repeats the `id` truncation in
 * both arms and shares the rest of the call, which is gcc's
 * cross-jumping of two call sites (a single call with the kind in a
 * variable truncates `id` once, after the join). */

/* The `tag` locals are set before the Sprite::Create call on purpose: the
 * ROM loads the constant into a callee-saved register up front and
 * stores it from there afterwards. */
void Platform::SpawnGemRed(u32 id, u16 x, u16 y, u16 param)
{
    u8 bit = gLevelState->progress.flags & PROGRESS_FLAG_RED_GEM;

    if (bit) {
        Platform *snd;

        if (gLevelState->IsGemPathDone() || gLevelState->timeTrial)
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_PATH_DONE);
        else
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_RED);
        gLevelState->SetGemPlatform(snd);
    } else {
        u8 tag = 7;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = bit;
        TouchableList()->Add(part);
        part->f.b.visible = 0;
    }
}

void Platform::SpawnGemYellow(u32 id, u16 x, u16 y, u16 param)
{
    u8 bit = gLevelState->progress.flags & PROGRESS_FLAG_YELLOW_GEM;

    if (bit) {
        Platform *snd;

        if (gLevelState->IsGemPathDone() || gLevelState->timeTrial)
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_PATH_DONE);
        else
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_YELLOW);
        gLevelState->SetGemPlatform(snd);
    } else {
        /* `tag` is needed: inlined, the code is 2 instructions shorter */
        u8 tag = 5;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = bit;
        TouchableList()->Add(part);
        part->f.b.visible = 0;
    }
}

void Platform::SpawnGemGreen(u32 id, u16 x, u16 y, u16 param)
{
    u8 bit = gLevelState->progress.flags & PROGRESS_FLAG_GREEN_GEM;

    if (bit) {
        Platform *snd;

        if (gLevelState->IsGemPathDone() || gLevelState->timeTrial)
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_PATH_DONE);
        else
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_GREEN);
        gLevelState->SetGemPlatform(snd);
    } else {
        /* `tag` is needed: inlined, the code is 2 instructions shorter */
        u8 tag = 6;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = bit;
        TouchableList()->Add(part);
        part->f.b.visible = 0;
    }
}

/* Mask and tag are the same constant here, so the ROM keeps one copy of
 * it in a callee-saved register for both uses. */
void Platform::SpawnGemBlue(u32 id, u16 x, u16 y, u16 param)
{
    u8 bit = gLevelState->progress.flags & PROGRESS_FLAG_BLUE_GEM;

    if (bit) {
        Platform *snd;

        if (gLevelState->IsGemPathDone() || gLevelState->timeTrial)
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_PATH_DONE);
        else
            snd = Platform::Create(id, x, y, param, PLATFORM_KIND_GEM_BLUE);
        gLevelState->SetGemPlatform(snd);
    } else {
        u8 tag = 8;
        Sprite *part = Sprite::Create(id, x, y, param);

        part->bank = gSpriteBankSet->Bank(32);
        part->tag = tag;
        part->ResetFrameTimer();
        part->ResetFrameIndex();
        part->SetAnimDone(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = bit;
        TouchableList()->Add(part);
        part->f.b.visible = 0;
    }
}
