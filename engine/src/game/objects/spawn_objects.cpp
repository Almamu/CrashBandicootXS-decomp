#include "game/level/spawners.hpp"
#include "game/objects/platform.hpp"
#include "game/enemies/enemy_ctrl.hpp"
#include "game/ui/level_select.hpp"
#include "game/level/level_state.hpp"
#include "game/ui/menus.hpp"

extern "C" {
#include "game/level/level.h"
#include "globals.h"
}

using Game::Enemies::EnemyCtrl;
using Game::Enemies::PeriodicSpawner;
using Game::Entities::AddUpdateOnly;
using Game::Entities::DecorationList;
using Game::Entities::Sprite;
using Game::Objects::LaunchPad;
using Game::Objects::Pickups;
using Game::Objects::Platform;

/* The decoration, platform and seal-spawner spawners, between the
 * bosses' and the crates' (ROM 0x08021748-0x08021A4C; #664,
 * include/game/level/spawners.hpp). Built with old_agbcp. Since #915 the platforms',
 * the launch pad's and the seal spawner's are static members of the
 * classes they create (Platform::SpawnRock, LaunchPad::SpawnEntity,
 * PeriodicSpawner::Spawn, ...), and the decorations' (plain Sprites)
 * are Pickups' (Pickups::SpawnSeaweed, ..., include/game/objects/pickups.hpp); the
 * empty slot stays free. */

/* Builds a sprite on sprite bank 45, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void Pickups::SpawnSeaweed(u32 id, u16 x, u16 y, u16 param)
{
    Sprite *part = Sprite::Create(id, x, y, param);

    part->bank = gSpriteBankSet->Bank(45);
    part->StartAnim(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). SpawnSeaweed without the animation reset. */
void Pickups::SpawnSeaweedNoAnimReset(u32 id, u16 x, u16 y, u16 param)
{
    Sprite *part = Sprite::Create(id, x, y, param);

    part->bank = gSpriteBankSet->Bank(45);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* Builds a sprite on sprite bank 44, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void Pickups::SpawnFlame(u32 id, u16 x, u16 y, u16 param)
{
    Sprite *part = Sprite::Create(id, x, y, param);

    part->bank = gSpriteBankSet->Bank(44);
    part->StartAnim(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* Plain `CreatePlatform` trampoline (docs/rom_map.md; same callee as
 * spawn_gem_platforms.cpp's twin family), kind PLATFORM_KIND_ROCK. */
void Platform::SpawnRock(u32 id, u16 x, u16 y, u16 param)
{
    Platform::Create(id, x, y, param, PLATFORM_KIND_ROCK);
}

/* Entity type 0x57: plain `CreatePlatform` trampoline, kind `6` - bank 39
 * anim 6, a green pad that turns red and tips over; its mover
 * (UpdatePlatformMover kind 6) brings it back 120 frames after the
 * animation ends. In the Neo Cortex fight the mover is
 * CreateCortexBossPlatformMover. */
void Platform::SpawnFlip(u32 id, u16 x, u16 y, u16 param)
{
    Platform::Create(id, x, y, param, PLATFORM_KIND_FLIP);
}

/* Spawns a PLATFORM_KIND_BONUS_DONE platform if gLevelState->IsBonusRoundDone()
 * is set or gLevelState+0x8C is nonzero, else PLATFORM_KIND_BONUS, and hands the
 * result to SetBonusPlatform. */
void Platform::SpawnBonus(u32 id, u16 x, u16 y, u16 param)
{
    Platform *result;

    if (gLevelState->IsBonusRoundDone() || gLevelState->timeTrial)
        result = Platform::Create(id, x, y, param, PLATFORM_KIND_BONUS_DONE);
    else
        result = Platform::Create(id, x, y, param, PLATFORM_KIND_BONUS);
    gLevelState->SetBonusPlatform(result);
}

/* Plain `CreatePlatform` trampoline, kind PLATFORM_KIND_MEDIUM. */
void Platform::SpawnMedium(u32 id, u16 x, u16 y, u16 param)
{
    Platform::Create(id, x, y, param, PLATFORM_KIND_MEDIUM);
}

/* Plain `CreatePlatform` trampoline, kind PLATFORM_KIND_SMALL. */
void Platform::SpawnSmall(u32 id, u16 x, u16 y, u16 param)
{
    Platform::Create(id, x, y, param, PLATFORM_KIND_SMALL);
}

/* Plain `CreatePlatform` trampoline, kind PLATFORM_KIND_LARGE. */
void Platform::SpawnLarge(u32 id, u16 x, u16 y, u16 param)
{
    Platform::Create(id, x, y, param, PLATFORM_KIND_LARGE);
}

/* Plain tail-call trampoline to LaunchPad::Spawn (SpawnLaunchPad,
 * level_select.cpp). */
void LaunchPad::SpawnEntity(u32 id, u16 x, u16 y, u16 param)
{
    LaunchPad::Spawn(id, x, y, param);
}

/* The spawner of entity types 0x0D-0x0F, 0x11, 0x36, 0x3C, 0x3E and 0x46
 * (gEntitySpawnFuncs): does nothing. No level places these types. */
void SpawnNoEntity(void)
{
}

/* A PeriodicSpawner (0x28 bytes) that spawns a seal (SpawnSeal) every 0x78
 * frames at (x, y), always active, in the update-only list. */
void PeriodicSpawner::Spawn(u32 id, u16 x, u16 y, u16 param)
{
    PeriodicSpawner *obj;
    /* `zero` is needed: inlined, the instructions are reordered */
    s32 zero;

    obj = new PeriodicSpawner;
    zero = 0;
    obj->callback = EnemyCtrl::SpawnSeal;
    obj->period = 0x78;
    obj->phase = zero;
    obj->x = INT_TO_Q8(x);
    obj->y = INT_TO_Q8(y);
    obj->f.flags |= ENTITY_FLAG_ALWAYS_ACTIVE;
    AddUpdateOnly(obj);
}
