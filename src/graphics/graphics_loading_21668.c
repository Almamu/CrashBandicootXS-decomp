#include "core.h"
#include "text_popup.h"

/* Spawner table entries next to the text popups (ROM 0x08021668-0x08021BFC).
 * Built with old_agbcc; see include/text_popup.h. */

extern void *gPaletteCache;
extern u8 *gLevelState;
extern void *gUnknown_030012E8;
extern void *gUnknown_030012F8;

extern struct popup_part *sub_8008434(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern struct enemy_ctrl *sub_8017FE8(void *mem);
extern u8 GetPaletteSlot(void *cache, s32 recordId);
extern u8 IsBonusRoundDone(void *self);
extern s32 CreatePlatform(u16 x, u16 y, u16 w, u16 h, s32 id);
extern void SetBonusPlatform(void *self, s32 value);
extern s32 sub_801B984(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeal(void);
extern void *CreateCrate(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u8 type);

/* Same shape as level_select_parts.h's anim_table/anim_record. */
struct anim_record_21668
{
    u8 unk_00[0x14];
    u8 tileRecord; // 0x14
};

struct anim_table_21668
{
    struct anim_record_21668 *records;
};

/* Bit view of actor.flags (+0x0C). */
struct part_flags_21668
{
    u8 unk_0:2;
    u8 bit2:1;
    u8 unk_3:1;
    u8 bit4:1;
    u8 unk_5:1;
    u8 bit6:1;
    u8 bit7:1;
};

#define PART_FLAGS(part) ((struct part_flags_21668 *)&(part)->base.flags)

/* SpawnSealSpawner's object: an actor with a per-frame callback. */
struct periodic_spawner
{
    struct actor base;          // 0x00
    void (*callback)(void);     // 0x1C
    s32 unk_20;
    s32 unk_24;
};

extern struct periodic_spawner *CreatePeriodicSpawner(void *mem);

/* Inline so old_agbcc re-truncates GetPaletteSlot's u8 result before the
 * nibble insert, as the ROM does. */
static inline void SetFrameNibble(struct popup_part *part, s32 frame)
{
    part->frameNibble = frame;
}

/* Popup-family variant: builds a sub_8009ED0 part on animation table
 * +0x168 at (arg1, arg2) in Q8, takes its frame from the tile cache
 * record of the first animation, clears the collected bits, attaches a
 * newly allocated sub_8017FE8 header, then shows it (flags: clear bits 7/2/6, set
 * bit 4) and registers it with gUnknown_030012F0's manager. */
void sub_8021668(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8009ED0(arg0, arg1, arg2, arg3);
    struct enemy_ctrl *hdr;

    part->anim = POPUP_ANIM(0x168);
    part->base.x = arg1 << 8;
    part->base.y = arg2 << 8;
    SetPartTag(part, 0);
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    SetFrameNibble(part, GetPaletteSlot(gPaletteCache,
        ((struct anim_table_21668 *)part->anim)->records->tileRecord));
    part->flipX = 0;
    part->unk_28_5 = 0;
    hdr = sub_8017FE8(sub_8026EDC(0x24));
    part->hdr = hdr;
    POPUP_ATTACH(hdr, part);
    part->base.field_0A = 1;
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    PART_FLAGS(part)->bit6 = 0;
    PART_FLAGS(part)->bit4 = 1;
    sub_8008E94(gUnknown_030012F0, part);
}

/* Builds a sub_8008434 sprite part on animation table +0x21c, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gUnknown_030012F8's manager. */
void SpawnSeaweed(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8008434(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x21c);
    SetPartTag(part, 0);
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    sub_8008E94(gUnknown_030012F8, part);
}

/* SpawnSeaweed without the animation reset. */
void sub_80217D0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8008434(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x21c);
    part->frameNibble = sub_800815C(part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    sub_8008E94(gUnknown_030012F8, part);
}

/* Builds a sub_8008434 sprite part on animation table +0x210, resets
 * its OAM state and frame nibble, clears flags bits 7 and 2 and
 * registers it with gUnknown_030012F8's manager. */
void SpawnFlame(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct popup_part *part = sub_8008434(arg0, arg1, arg2, arg3);

    part->anim = POPUP_ANIM(0x210);
    SetPartTag(part, 0);
    sub_80087C0(part);
    sub_80087B4(part);
    sub_800872C(part, 0);
    part->frameNibble = sub_800815C(part);
    PART_FLAGS(part)->bit7 = 0;
    PART_FLAGS(part)->bit2 = 0;
    part->base.field_0A = 0;
    sub_8008E94(gUnknown_030012F8, part);
}

/* Plain `CreatePlatform` trampoline (docs/rom_map.md; same callee as
 * trigger_effect.c's twin family), id `8`. */
void SpawnRockPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 8);
}

/* Plain `CreatePlatform` trampoline, id `6`. */
void sub_80218E8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    CreatePlatform(arg0, arg1, arg2, arg3, 6);
}

/* Spawns a CreatePlatform part with id 7 if IsBonusRoundDone(gLevelState)
 * is set or gLevelState+0x8C is nonzero, else id 5, and hands the
 * result to SetBonusPlatform. */
void SpawnBonusPlatform(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    s32 result;

    if (IsBonusRoundDone(gLevelState) || gLevelState[0x8c])
        result = CreatePlatform(arg0, arg1, arg2, arg3, 7);
    else
        result = CreatePlatform(arg0, arg1, arg2, arg3, 5);
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

/* Plain tail-call trampoline to `sub_801B984` (still raw). */
void sub_80219E0(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    sub_801B984(arg0, arg1, arg2, arg3);
}

/* Empty stub. */
void nullsub_21(void)
{
}

/* `new`s a 0x28-byte CreatePeriodicSpawner object, installs SpawnSeal as its
 * callback with +0x20 = 0x78, places it at (arg1, arg2) in Q8, sets
 * flags bit 4 and registers it with gUnknown_030012E8's manager. */
void SpawnSealSpawner(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct periodic_spawner *obj;
    s32 zero;

    obj = CreatePeriodicSpawner(sub_8026EDC(0x28));
    zero = 0;
    obj->callback = SpawnSeal;
    obj->unk_20 = 0x78;
    obj->unk_24 = zero;
    obj->base.x = arg1 << 8;
    obj->base.y = arg2 << 8;
    obj->base.flags |= 0x10;
    sub_8008E94(gUnknown_030012E8, obj);
}

/* Plain `CreateCrate` entity-constructor trampoline (docs/rom_map.md;
 * same dispatch family as src/graphics/graphics_loading_21bfc.c's
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
void sub_8021AB8(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
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
