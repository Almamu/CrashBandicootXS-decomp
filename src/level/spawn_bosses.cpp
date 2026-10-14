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
 * old_agbcp. Since #915 the bosses' and Mega Mix's are static members of
 * their controllers' classes (DingodileCtrl::Spawn, TinyCtrl::Spawn,
 * CortexBossCtrl::Spawn, MegaMixCtrl::Spawn; include/boss_ctrl.hpp); the
 * room exit, a bare Entity or a Platform by the level, stays free. */

/* The part's X and Y mirror bits from its parameter record's flags (bit
 * 1 clear: X mirrored; bit 2: Y mirrored). */
static inline void SetMirrorFromRecord(MovingSprite *part, u16 index)
{
    const struct entity_params *rec = EntityParams(index);

    part->mirrorFlags.mirrorX = (rec->flags >> ENTITY_PARAMS_BIT_FLIP_X ^ 1) & 1;
    part->mirrorFlags.mirrorY = TEST_BIT_SHR(rec->flags, ENTITY_PARAMS_BIT_FLIP_Y);
}

/* Entity type 0x55, the room's exit. Every room but room 16 and the boss
 * rooms places exactly one. The tag-0x12 zone is collision class 0x12,
 * the player event that PlayerHandleEvent answers with LevelProgress::RequestRoomExit;
 * the kind-4 platform is bank 39's glowing exit pad.
 *
 * Three-way spawner. Outside the gem path and the bonus round, in the
 * first room of a level of THEME_SNOW, a 0x64x0x64 entity of kind 0x12 in
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
void SpawnRoomExit(u32 id, u16 x, u16 y, u16 param)
{
    if (!gLevelState->IsInGemPath() && !gLevelState->IsInBonusRound() &&
        !gLevelState->GetRoomIndex() &&
        gLevelTable[gLevelState->GetCurrentLevel()].theme == THEME_SNOW) {
        Entity *part = Entity::Create(id, x, y, param);

        part->SetSize(0x64, 0x64);
        part->kind = EVENT_ROOM_EXIT;
        AddUpdateOnly(part);
    } else if (gPlayer->ctrlMode == PLAYER_CTRL_MODE_ACTION) {
        Platform *pad = Platform::Create(id, x, y, param, PLATFORM_KIND_EXIT_PAD);
        struct vec2 point;

        point = (struct vec2){ Q8_TO_INT(pad->x) - 2, Q8_TO_INT(pad->y) - 0x1E };
        gLevelState->SetCrateGemPos(&point.x);
    } else {
        Entity *part = Entity::Create(id, x, y, param);

        part->SetSize(0x28, 0x28);
        part->kind = EVENT_ROOM_EXIT;
        AddUpdateOnly(part);
    }
}

/* Dingodile: his part (bank 0x288), always active, kind 1, mirrored by
 * his record, in the collidable list, and his controller
 * (DingodileCtrl, from his position), the level's boss. */
void DingodileCtrl::Spawn(u32 id, u16 x, u16 y, u16 param)
{
    MovingSprite *part = MovingSprite::Create(id, x, y, param);
    DingodileCtrl *hdr;

    part->bank = gSpriteBankSet->Bank(54);
    part->palette = part->GetAnimPaletteSlot();
    part->f.flags |= ENTITY_FLAG_ALWAYS_ACTIVE;
    part->SetKind(1);
    SetMirrorFromRecord(part, param);
    CollidableList()->Add(part);
    hdr = new DingodileCtrl(x, y);
    part->mover = hdr;
    hdr->Attach(part);
    gLevelState->SetLevelBoss(hdr);
}

/* Tiny: his part (bank 0x294) and controller (TinyCtrl), mirrored by
 * his record, always active, in the collidable list; the level's boss. */
void TinyCtrl::Spawn(u32 id, u16 x, u16 y, u16 param)
{
    MovingSprite *part = MovingSprite::Create(id, x, y, param);
    TinyCtrl *hdr;

    part->bank = gSpriteBankSet->Bank(55);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new TinyCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    SetMirrorFromRecord(part, param);
    part->f.flags |= ENTITY_FLAG_ALWAYS_ACTIVE;
    CollidableList()->Add(part);
    gLevelState->SetLevelBoss(hdr);
}

/* Neo Cortex: his part (bank 0x27C, animation 1) and controller
 * (CortexBossCtrl), mirrored by his record, always active, in the
 * foreground list and not animating; the level's boss. */
void CortexBossCtrl::Spawn(u32 id, u16 x, u16 y, u16 param)
{
    MovingSprite *part = MovingSprite::Create(id, x, y, param);
    CortexBossCtrl *hdr;

    part->bank = gSpriteBankSet->Bank(53);
    part->StartAnim(1);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new CortexBossCtrl;
    part->mover = hdr;
    hdr->Attach(part);
    SetMirrorFromRecord(part, param);
    part->f.flags |= ENTITY_FLAG_ALWAYS_ACTIVE;
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
 * on sprite bank 30 at (x, y), its palette that of its first
 * animation, not mirrored, driven by a MegaMixCtrl; kind 1, not
 * colliding, out of contact, invulnerable and always active, in the
 * collidable list. */
void MegaMixCtrl::Spawn(u32 id, u16 x, u16 y, u16 param)
{
    MovingSprite *part = MovingSprite::Create(id, x, y, param);
    MegaMixCtrl *hdr;

    part->bank = gSpriteBankSet->Bank(30);
    part->x = INT_TO_Q8(x);
    part->y = INT_TO_Q8(y);
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
