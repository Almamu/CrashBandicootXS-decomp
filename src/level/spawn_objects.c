#include "core.h"
#include "sprite_bank.h"
#include "text_popup.h"
#include "enemies.h"
#include "menus.h"
#include "crates.h"
#include "bosses.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* Spawner table entries next to the text popups (ROM 0x08021668-0x08021BFC).
 * Built with old_agbcc; see include/text_popup.h. */

/* Bit view of actor.flags (+0x0C). */
struct part_flags_21668 {
    u8 unk_0:2;
    u8 bit2:1;
    u8 unk_3:1;
    u8 bit4:1;
    u8 unk_5:1;
    u8 bit6:1;
    u8 bit7:1;
};

#define PART_FLAGS(part) ((struct part_flags_21668 *)&(part)->base.flags)

/* Inline so old_agbcc re-truncates GetPaletteSlot's u8 result before the
 * nibble insert, as the ROM does. */
static inline void SetFrameNibble(struct popup_part *part, s32 frame)
{
    part->frameNibble = frame;
}

/* Entity type 0x49, Mega-Mix (see mega_mix_update.c). Popup-family variant:
 * builds a CreateMovingSprite part on animation table +0x168 at (arg1, arg2) in Q8, takes its frame from the tile cache
 * record of the first animation, clears the collected bits, attaches a
 * newly allocated CreateMegaMixCtrl header, then shows it (flags: clear bits 7/2/6, set
 * bit 4) and registers it with gCollidableList's manager. */
void SpawnMegaMix(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateMovingSprite(arg0, arg1, arg2, arg3);
    struct part_ctrl *hdr;

    part->anim = POPUP_ANIM(0x168);
    part->base.x = arg1 << 8;
    part->base.y = arg2 << 8;
    SetPartTag(part, 0);
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    SetFrameNibble(
        part,
        GetPaletteSlot(gPaletteCache, ((const struct sprite_bank *)part->anim)->anims->paletteId));
    part->flipX = 0;
    part->flipY = 0;
    hdr = CreateMegaMixCtrl(OperatorNew(0x24));
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    part->base.field_0A = 1;
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    PART_FLAGS(part)->bit6 = 0;
    PART_FLAGS(part)->bit4 = 1;
    AddToPartList(gCollidableList, part);
}

/* Builds a CreateSpriteObj sprite part on animation table +0x21c, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnSeaweed(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x21c);
    SetPartTag(part, 0);
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    AddToPartList((struct part_list *)gDecorationList, part);
}

/* SpawnSeaweed without the animation reset. */
void sub_80217D0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x21c);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    AddToPartList((struct part_list *)gDecorationList, part);
}

/* Builds a CreateSpriteObj sprite part on animation table +0x210, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gDecorationList's manager. */
void SpawnFlame(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = CreateSpriteObj(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x210);
    SetPartTag(part, 0);
    ResetSpriteFrameTimer(part);
    ResetSpriteFrameIndex(part);
    SetSpriteAnimDone(part, 0);
    part->frameNibble = GetSpriteAnimPaletteSlot((struct actor *)part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    AddToPartList((struct part_list *)gDecorationList, part);
}

/* Plain `CreatePlatform` trampoline (docs/rom_map.md; same callee as
 * spawn_gem_platforms.c's twin family), id `8`. */
void SpawnRockPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 8);
}

/* Entity type 0x57: plain `CreatePlatform` trampoline, kind `6` - bank 39
 * anim 6, a green pad that turns red and tips over; its mover
 * (UpdatePlatformMover kind 6) brings it back 120 frames after the
 * animation ends. In the Neo Cortex fight the mover is
 * CreateCortexBossPlatformMover. */
void SpawnFlipPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 6);
}

/* Spawns a CreatePlatform part with id 7 if IsBonusRoundDone(gLevelState)
 * is set or gLevelState+0x8C is nonzero, else id 5, and hands the
 * result to SetBonusPlatform. */
void SpawnBonusPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    s32 result;

    if (IsBonusRoundDone(gLevelState) || gLevelState->timeTrial)
        result = (s32)CreatePlatform(arg0, arg1, arg2, arg3, 7);
    else
        result = (s32)CreatePlatform(arg0, arg1, arg2, arg3, 5);
    SetBonusPlatform(gLevelState, result);
}

/* Plain `CreatePlatform` trampoline, id `2`. */
void SpawnMediumPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 2);
}

/* Plain `CreatePlatform` trampoline, id `1`. */
void SpawnSmallPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 1);
}

/* Plain `CreatePlatform` trampoline, id `0`. */
void SpawnLargePlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 0);
}

/* Plain tail-call trampoline to `SpawnLaunchPad` (still raw). */
void SpawnLaunchPadEntity(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    SpawnLaunchPad(arg0, arg1, arg2, arg3);
}

/* Empty stub. */
void nullsub_21(void)
{
}

/* `new`s a 0x28-byte CreatePeriodicSpawner object, installs SpawnSeal as its
 * callback with a 0x78-frame period, places it at (arg1, arg2) in Q8, sets
 * flags bit 4 and registers it with gUpdateOnlyPartList's manager. */
void SpawnSealSpawner(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct periodic_spawner *obj;
    s32 zero;

    obj = CreatePeriodicSpawner(OperatorNew(0x28));
    zero = 0;
    obj->callback = (void (*)(void))SpawnSeal;
    obj->period = 0x78;
    obj->phase = zero;
    obj->base.x = arg1 << 8;
    obj->base.y = arg2 << 8;
    obj->base.flags |= 0x10;
    AddToPartList((struct part_list *)gUpdateOnlyPartList, obj);
}

/* Plain `CreateCrate` entity-constructor trampoline (docs/rom_map.md;
 * same dispatch family as src/level/spawn_crates.c's
 * types 1-7), type `0x12`. */
void SpawnTimeCrate3(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0x12);
}

/* Plain `CreateCrate` trampoline, type `0x11`. */
void SpawnTimeCrate2(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0x11);
}

/* Plain `CreateCrate` trampoline, type `0x10`. */
void SpawnTimeCrate1(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0x10);
}

/* Plain `CreateCrate` trampoline, type `0xf`. */
void SpawnSlotCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xf);
}

/* Plain `CreateCrate` trampoline, type `0xe`. */
void SpawnTntCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xe);
}

/* Plain `CreateCrate` trampoline, type `0xd`. */
void sub_8021B00(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xd);
}

/* Plain `CreateCrate` trampoline, type `0xc`. */
void SpawnBouncyWumpaCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xc);
}

/* Plain `CreateCrate` trampoline, type `0xb`. */
void SpawnMysteryCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xb);
}

/* Plain `CreateCrate` trampoline, type `0xa`. */
void SpawnNitroCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 0xa);
}

/* Plain `CreateCrate` trampoline, type `9`. */
void SpawnLifeCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 9);
}

/* Plain `CreateCrate` trampoline, type `8`. */
void SpawnIronArrowCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 8);
}

/* Plain `CreateCrate` trampoline, type `7`. Last function in this ROM
 * region - `asm/code_3_2_17_21280.s` (still-raw text past this point
 * used to continue here) now ends right before this file's span, at
 * `SpawnCortexBoss`'s literal pool. */
void SpawnIronCrate(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreateCrate(arg0, arg1, arg2, arg3, 7);
}
