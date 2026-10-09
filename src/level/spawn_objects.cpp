#include "spawners.hpp"
#include "platform.hpp"
#include "enemy_ctrl.hpp"
#include "level_select.hpp"
#include "level_state.hpp"

extern "C" {
#include "menus.h"
#include "level.h"
#include "globals.h"
}

/* The decoration, platform and seal-spawner spawners, between the
 * bosses' and the crates' (ROM 0x08021748-0x08021A4C; #664,
 * include/spawners.hpp). Built with old_agbcp. */

/* Builds a sprite on sprite bank +0x21c, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnSeaweed(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x21c);
    part->StartAnim(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). SpawnSeaweed without the animation reset. */
void SpawnSeaweedNoAnimReset(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x21c);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* Builds a sprite on sprite bank +0x210, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnFlame(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x210);
    part->StartAnim(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = EVENT_NONE;
    DecorationList()->Add(part);
}

/* Plain `CreatePlatform` trampoline (docs/rom_map.md; same callee as
 * spawn_gem_platforms.cpp's twin family), id `8`. */
void SpawnRockPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform::Create(arg0, arg1, arg2, arg3, 8);
}

/* Entity type 0x57: plain `CreatePlatform` trampoline, kind `6` - bank 39
 * anim 6, a green pad that turns red and tips over; its mover
 * (UpdatePlatformMover kind 6) brings it back 120 frames after the
 * animation ends. In the Neo Cortex fight the mover is
 * CreateCortexBossPlatformMover. */
void SpawnFlipPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform::Create(arg0, arg1, arg2, arg3, 6);
}

/* Spawns a platform of kind 7 if gLevelState->IsBonusRoundDone()
 * is set or gLevelState+0x8C is nonzero, else id 5, and hands the
 * result to SetBonusPlatform. */
void SpawnBonusPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform *result;

    if (gLevelState->IsBonusRoundDone() || gLevelState->timeTrial)
        result = Platform::Create(arg0, arg1, arg2, arg3, 7);
    else
        result = Platform::Create(arg0, arg1, arg2, arg3, 5);
    gLevelState->SetBonusPlatform(result);
}

/* Plain `CreatePlatform` trampoline, id `2`. */
void SpawnMediumPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform::Create(arg0, arg1, arg2, arg3, 2);
}

/* Plain `CreatePlatform` trampoline, id `1`. */
void SpawnSmallPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform::Create(arg0, arg1, arg2, arg3, 1);
}

/* Plain `CreatePlatform` trampoline, id `0`. */
void SpawnLargePlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform::Create(arg0, arg1, arg2, arg3, 0);
}

/* Plain tail-call trampoline to LaunchPad::Spawn (SpawnLaunchPad,
 * level_select.cpp). */
void SpawnLaunchPadEntity(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    LaunchPad::Spawn(arg0, arg1, arg2, arg3);
}

/* The spawner of entity types 0x0D-0x0F, 0x11, 0x36, 0x3C, 0x3E and 0x46
 * (gEntitySpawnFuncs): does nothing. No level places these types. */
void SpawnNoEntity(void)
{
}

/* A PeriodicSpawner (0x28 bytes) that spawns a seal (SpawnSeal) every 0x78
 * frames at (arg1, arg2), always active, in the update-only list. */
void SpawnSealSpawner(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    PeriodicSpawner *obj;
    s32 zero;

    obj = new PeriodicSpawner;
    zero = 0;
    obj->callback = SpawnSeal;
    obj->period = 0x78;
    obj->phase = zero;
    obj->x = INT_TO_Q8(arg1);
    obj->y = INT_TO_Q8(arg2);
    obj->f.flags |= 0x10;
    AddUpdateOnly(obj);
}
