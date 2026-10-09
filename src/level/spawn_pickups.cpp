#include "spawners.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "part_list.hpp"
#include "font.hpp"
#include "level_state.hpp"
#include "audio.hpp"

extern "C" {
#include "util.h"
#include "text.h"
#include "gfx.h"
#include "memory.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* The pickup spawners: the powers, the stopwatch, the blue gem, the
 * crate gem's marker and the wumpa (#664, include/spawners.hpp), ROM
 * 0x08021D80-0x08022188. The player starts, the entity spawner's calls
 * and InitLevelState follow in spawn_markers.cpp. Built with
 * old_agbcp. */

/* A sprite of bank `bank` (an offset into the sprite bank table),
 * animation `tag`, kind `kind`, in the touchable list. The callers pass
 * `tag` and `kind` in variables: the ROM loads both into callee-saved
 * registers before the Sprite::Create call and stores them from there
 * (a literal 0 is loaded again at the store). */
static inline Sprite *SpawnTouchable(u32 a0, u16 a1, u16 a2, u16 a3, s32 bank, u8 tag, u8 kind)
{
    Sprite *part = Sprite::Create(a0, a1, a2, a3);

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + bank);
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->kind = kind;
    TouchableList()->Add(part);
    return part;
}

/* The four powers (sprite bank record 38, `+0x1C8`, the one
 * InitPowerDialog's dialog box uses): body slam (tag 1, kind 0x25),
 * tornado spin (0, 0x24), double jump (2, 0x23) and turbo run (3,
 * 0x26). */
void SpawnBodySlamPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 1;
    u8 kind = 0x25;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnTornadoSpinPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 0;
    u8 kind = 0x24;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnDoubleJumpPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 2;
    u8 kind = 0x23;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

void SpawnTurboRunPower(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u8 tag = 3;
    u8 kind = 0x26;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x1c8, tag, kind);
}

/* The time trial's stopwatch (bank `+0x1B0`, kind 0x1C, always active),
 * once the level's crystal is saved. */
void SpawnStopwatch(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if ((u8)gLevelState->IsCrystalSaved()) {
        Stopwatch *part = Stopwatch::Create(arg0, arg1, arg2, arg3);

        part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x1b0);
        part->StartAnim(0);
        part->palette = part->GetAnimPaletteSlot();
        part->kind = 0x1c;
        part->f.flags |= 0x10;
        TouchableList()->Add(part);
    }
}

/* The blue gem (bank record 32, `+0x180`, tag 4, kind 0x21), unless
 * collected (bit 3 of the level state's gem flags). */
void SpawnBlueGem(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->progress.flags & 8)
        return;
    u8 tag = 4;
    u8 kind = 0x21;

    SpawnTouchable(arg0, arg1, arg2, arg3, 0x180, tag, kind);
}

/* Entity type 0x0C (the crate gem's marker): where the crate gem appears
 * (SetCrateGemPos) once every crate is broken.
 *
 * The ROM truncates both coordinates in one batch (`lsl r1; lsl r2; lsr
 * r3, r1; lsr r4, r2`) into fresh registers, one of them callee-saved:
 * the point is one `struct vec2` value in a register pair (r3:r4, hence
 * the pushed r4) stored to the stack. g++ gives it a DImode pseudo when
 * it is assigned from a compound literal: a named `struct vec2` returned
 * by an inline, or one built field by field, is bound to the implicit
 * copy constructor's reference argument, which makes it addressable, so
 * it lives on the stack (#662 round 3; the C had the four instructions
 * in asm). */
void SpawnCrateGemMarker(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    struct vec2 point;

    point = (struct vec2){ arg1, arg2 };
    gLevelState->SetCrateGemPos(&point.x);
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the spawn
 * table). Builds a CreateSpriteObj part and adds it to gTouchableList,
 * like the pickup spawners above.
 *
 * Same overall spawn shape as `SpawnBodySlamPower`'s family above, but with
 * the master-table record index (`index`), `tag` and `kind`
 * field all taken as *runtime* parameters instead of fixed constants
 * (matches `SpawnEffectPart`'s already-documented `param1*12` runtime-
 * indexed access to `gSpriteBankTable`'s record array, docs/
 * rom_map.md). */
void *CreateTouchableSprite(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch)
{
    Sprite *part = Sprite::Create(cx, cy, cw, ch);

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + index * 12);
    part->tag = tag;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
    part->palette = part->GetAnimPaletteSlot();
    part->kind = field0A;
    TouchableList()->Add(part);
    return part;
}

/* Spawns a wumpa (`CreateWumpa`, wumpa_update.cpp) with this slot's four
 * arguments, unless `gLevelState+0x8c` (time trial) is set. */
void SpawnWumpa(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (gLevelState->timeTrial == 0)
        Wumpa::Create(arg0, arg1, arg2, arg3);
}
