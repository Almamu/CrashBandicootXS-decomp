#include "spawners.hpp"
#include "platform.hpp"
#include "boss_ctrl.hpp"
#include "player.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include "level.h"
#include "globals.h"
}

/* The spawners of the bosses and the room exit (#664,
 * include/spawners.hpp), ROM 0x08021280-0x08021668. Built with
 * old_agbcp. */

/* Switches to animation `t` from its start. `t` is an s32: its 1 is
 * loaded before the tag's address, as in the ROM. */
static inline void SetTag(MovingSprite *p, s32 t)
{
    p->tag = t;
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
}

/* The part's X and Y mirror bits from its parameter record's flags (bit
 * 1 clear: X mirrored; bit 2: Y mirrored). */
static inline void SetMirrorFromRecord(MovingSprite *part, u16 index)
{
    const struct entity_params *rec = EntityParams(index);

    part->mirrorFlags.mirrorX = (rec->flags >> 1 ^ 1) & 1;
    part->mirrorFlags.mirrorY = rec->flags >> 2 & 1;
}

/* Entity type 0x55, the room's exit. Every room but room 16 and the boss
 * rooms places exactly one. The tag-0x12 zone is collision class 0x12,
 * the player event that PlayerHandleEvent answers with RequestRoomExit;
 * the kind-4 platform is bank 39's glowing exit pad.
 *
 * Three-way spawner. Outside the gem path and the bonus round, in the
 * first room of a level of theme 0, a 0x64x0x64 entity of kind 0x12 in
 * the update-only list. Otherwise, unless the player is in another
 * control mode (a vehicle), the exit pad (a kind-4 platform) and the
 * crate gem's position just above-left of it; in a vehicle a 0x28x0x28
 * entity.
 *
 * Kept from the C: four pins and a MATCH_KEEP. The ROM computes the
 * point's x and y into fresh registers (`subs r2, r1, #2`; `adds r3,
 * r0, #0; subs r3, #30`); g++ reuses their inputs, with every spelling
 * tried (locals, an inline setter, a point class with a constructor, a
 * loop around the stores), the same gap as SpawnCrateGemMarker
 * (spawn_pickups.cpp). The ROM's shape is the point built as one
 * `struct vec2` value in a register pair (r2:r3) and then stored: the C
 * compiler gives exactly that for an inline returning a `struct vec2`,
 * but g++ keeps a struct value in memory, so here it takes the pins. */
void SpawnRoomExit(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (!gLevelState->IsInGemPath() && !gLevelState->IsInBonusRound() &&
        !gLevelState->GetRoomIndex() && gLevelTable[gLevelState->GetCurrentLevel()].theme == 0) {
        Entity *part = Entity::Create(arg0, arg1, arg2, arg3);

        part->SetSize(0x64, 0x64);
        part->kind = 0x12;
        AddUpdateOnly(part);
    } else if (gPlayer->ctrlMode == 0) {
        Platform *pad = Platform::Create(arg0, arg1, arg2, arg3, 4);

        MATCH_HOLD_REG(s32, px, r1) = Q8_TO_INT(pad->x);
        MATCH_HOLD_REG(s32, x, r2) = px - 2;
        MATCH_HOLD_REG(s32, py, r0);
        MATCH_HOLD_REG(s32, y, r3);
        s32 point[2];

        MATCH_KEEP(x);
        py = Q8_TO_INT(pad->y);
        y = py - 0x1E;
        point[0] = x;
        point[1] = y;
        gLevelState->SetCrateGemPos(point);
    } else {
        Entity *part = Entity::Create(arg0, arg1, arg2, arg3);

        part->SetSize(0x28, 0x28);
        part->kind = 0x12;
        AddUpdateOnly(part);
    }
}

/* Dingodile: his part (bank 0x288), always active, kind 1, mirrored by
 * his record, in the collidable list, and his controller
 * (DingodileCtrl, from his position), the level's boss. */
void SpawnDingodile(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    DingodileCtrl *hdr;

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x288);
    part->palette = part->GetAnimPaletteSlot();
    part->f.flags |= 0x10;
    part->SetKind(1);
    SetMirrorFromRecord(part, arg3);
    CollidableList()->Add(part);
    hdr = new DingodileCtrl(arg1, arg2);
    part->mover = hdr;
    hdr->Attach(part);
    gLevelState->SetLevelBoss(hdr);
}

/* Tiny: his part (bank 0x294) and controller (TinyCtrl), mirrored by
 * his record, always active, in the collidable list; the level's boss. */
void SpawnTiny(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    TinyCtrl *hdr;

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x294);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new TinyCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    SetMirrorFromRecord(part, arg3);
    part->f.flags |= 0x10;
    CollidableList()->Add(part);
    gLevelState->SetLevelBoss(hdr);
}

/* Neo Cortex: his part (bank 0x27C, animation 1) and controller
 * (CortexBossCtrl), mirrored by his record, always active, in the
 * foreground list and not animating; the level's boss. */
void SpawnCortexBoss(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    CortexBossCtrl *hdr;

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27c);
    SetTag(part, 1);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new CortexBossCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    SetMirrorFromRecord(part, arg3);
    part->f.flags |= 0x10;
    ForegroundList()->Add(part);
    part->animating = 0;
    gLevelState->SetLevelBoss(hdr);
}
