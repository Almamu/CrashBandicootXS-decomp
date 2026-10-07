#include "spawners.hpp"
#include "platform.hpp"
#include "crate.hpp"
#include "boss_ctrl.hpp"
#include "enemy_ctrl.hpp"

extern "C" {
#include "menus.h"
#include "level.h"
#include "globals.h"
}

/* Spawner table entries next to the bosses' (ROM 0x08021668-0x08021BFC;
 * #664, include/spawners.hpp). Built with old_agbcp. */

/* Inline so old_agbcp re-truncates GetPaletteSlot's u8 result before the
 * nibble insert, as the ROM does. */
static inline void SetPalette(MovingSprite *part, s32 slot)
{
    part->palette = slot;
}

/* `t` is an s32: its constant is loaded before the tag's address. */
static inline void SetTag(Sprite *part, s32 t)
{
    part->tag = t;
}

/* Entity type 0x49, Mega-Mix (see mega_mix_update.cpp): a moving sprite
 * on sprite bank +0x168 at (arg1, arg2), its palette that of its first
 * animation, not mirrored, driven by a MegaMixCtrl; kind 1, not
 * colliding, out of contact, invulnerable and always active, in the
 * collidable list. */
void SpawnMegaMix(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    MegaMixCtrl *hdr;

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x168);
    part->x = INT_TO_Q8(arg1);
    part->y = INT_TO_Q8(arg2);
    SetTag(part, 0);
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    SetPalette(part, GetPaletteSlot(gPaletteCache, part->bank->anims->paletteId));
    part->mirrorFlags.mirrorX = 0;
    part->mirrorFlags.mirrorY = 0;
    hdr = new MegaMixCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    part->kind = 1;
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->f.b.vulnerable = 0;
    part->f.b.active = 1;
    CollidableList()->Add(part);
}

/* Builds a sprite on sprite bank +0x21c, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnSeaweed(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x21c);
    SetTag(part, 0);
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = 0;
    DecorationList()->Add(part);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). SpawnSeaweed without the animation reset. */
void SpawnSeaweedNoAnimReset(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x21c);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = 0;
    DecorationList()->Add(part);
}

/* Builds a sprite on sprite bank +0x210, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnFlame(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Sprite *part = Sprite::Create(arg0, arg1, arg2, arg3);

    part->anim = (struct anim_table *)(SPRITE_BANK_BASE + 0x210);
    SetTag(part, 0);
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->kind = 0;
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

/* Spawns a platform of kind 7 if IsBonusRoundDone(gLevelState)
 * is set or gLevelState+0x8C is nonzero, else id 5, and hands the
 * result to SetBonusPlatform. */
void SpawnBonusPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Platform *result;

    if (IsBonusRoundDone(gLevelState) || gLevelState->timeTrial)
        result = Platform::Create(arg0, arg1, arg2, arg3, 7);
    else
        result = Platform::Create(arg0, arg1, arg2, arg3, 5);
    SetBonusPlatform(gLevelState, result);
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

/* Plain tail-call trampoline to `SpawnLaunchPad` (still raw). */
void SpawnLaunchPadEntity(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnLaunchPad(arg0, arg1, arg2, arg3);
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

/* Plain `CreateCrate` entity-constructor trampoline (docs/rom_map.md;
 * same dispatch family as src/level/spawn_crates.cpp's
 * types 1-7), type `0x12`. */
void SpawnTimeCrate3(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_3);
}

/* Plain `CreateCrate` trampoline, type `0x11`. */
void SpawnTimeCrate2(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_2);
}

/* Plain `CreateCrate` trampoline, type `0x10`. */
void SpawnTimeCrate1(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TIME_1);
}

/* Plain `CreateCrate` trampoline, type `0xf`. */
void SpawnSlotCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_SLOT);
}

/* Plain `CreateCrate` trampoline, type `0xe`. */
void SpawnTntCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_TNT);
}

/* Plain `CreateCrate` trampoline, type `0xd`: the reinforced crate (bank
 * 31 animation 6, a wooden crate with metal-banded edges). Only a body
 * slam (attack kind 5, gActionCtrlStateAttackKinds) or the invincibility
 * mask breaks it: gCrateHitResponse row 13 bounces every other attack,
 * and QueueCratePlayerCollision turns a body slam moving up into a bounce
 * too. */
void SpawnReinforcedCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_REINFORCED);
}

/* Plain `CreateCrate` trampoline, type `0xc`. */
void SpawnBouncyWumpaCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_BOUNCY_WUMPA);
}

/* Plain `CreateCrate` trampoline, type `0xb`. */
void SpawnMysteryCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_MYSTERY);
}

/* Plain `CreateCrate` trampoline, type `0xa`. */
void SpawnNitroCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_NITRO);
}

/* Plain `CreateCrate` trampoline, type `9`. */
void SpawnLifeCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_LIFE);
}

/* Plain `CreateCrate` trampoline, type `8`. */
void SpawnIronArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON_ARROW);
}

/* Plain `CreateCrate` trampoline, type `7`. Last function in this ROM
 * region - `asm/code_3_2_17_21280.s` (still-raw text past this point
 * used to continue here) now ends right before this file's span, at
 * `SpawnCortexBoss`'s literal pool. */
void SpawnIronCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    Crate::Create(arg0, arg1, arg2, arg3, CRATE_KIND_IRON);
}
