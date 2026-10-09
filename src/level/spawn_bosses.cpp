#include "spawners.hpp"
#include "platform.hpp"
#include "boss_ctrl.hpp"
#include "player.hpp"
#include "level_state.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* The spawners of the bosses, the room exit and Mega-Mix (#664,
 * include/spawners.hpp), ROM 0x08021280-0x08021748. Built with
 * old_agbcp. */

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
 * The ROM computes the point's x and y into fresh registers (`subs r2,
 * r1, #2`; `adds r3, r0, #0; subs r3, #30`): the point is one `struct
 * vec2` value in a register pair (r2:r3), then stored. g++ holds it in
 * DImode when it is assigned from a compound literal. A named `struct
 * vec2` copied whole (an inline's return value, an initializer) goes
 * through the implicit copy constructor, whose reference argument makes
 * the source addressable, so it gets a stack slot of its own; one copied
 * field by field is two SImode values offset in place (`subs r1, #2`).
 * #662 round 3 (the C++ had four pins and a MATCH_KEEP). */
void SpawnRoomExit(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    if (!gLevelState->IsInGemPath() && !gLevelState->IsInBonusRound() &&
        !gLevelState->GetRoomIndex() && gLevelTable[gLevelState->GetCurrentLevel()].theme == 0) {
        Entity *part = Entity::Create(arg0, arg1, arg2, arg3);

        part->SetSize(0x64, 0x64);
        part->kind = EVENT_ROOM_EXIT;
        AddUpdateOnly(part);
    } else if (gPlayer->ctrlMode == 0) {
        Platform *pad = Platform::Create(arg0, arg1, arg2, arg3, 4);
        struct vec2 point;

        point = (struct vec2){ Q8_TO_INT(pad->x) - 2, Q8_TO_INT(pad->y) - 0x1E };
        gLevelState->SetCrateGemPos(&point.x);
    } else {
        Entity *part = Entity::Create(arg0, arg1, arg2, arg3);

        part->SetSize(0x28, 0x28);
        part->kind = EVENT_ROOM_EXIT;
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
    part->StartAnim(1);
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

/* Inline so old_agbcp re-truncates GetPaletteSlot's u8 result before the
 * nibble insert, as the ROM does. */
static inline void SetPalette(MovingSprite *part, s32 slot)
{
    part->palette = slot;
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

    part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x168);
    part->x = INT_TO_Q8(arg1);
    part->y = INT_TO_Q8(arg2);
    part->StartAnim(0);
    SetPalette(part, gPaletteCache->GetSlot(part->bank->anims->paletteId));
    part->mirrorFlags.mirrorX = 0;
    part->mirrorFlags.mirrorY = 0;
    hdr = new MegaMixCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    part->kind = EVENT_HIT;
    part->f.b.collides = 0;
    part->f.b.visible = 0;
    part->f.b.vulnerable = 0;
    part->f.b.active = 1;
    CollidableList()->Add(part);
}
