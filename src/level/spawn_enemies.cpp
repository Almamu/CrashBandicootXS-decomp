#include "spawners.hpp"
#include "enemy_ctrl.hpp"

extern "C" {
#include "match.h"
#include "audio.h"
#include "level.h"
#include "globals.h"
}

/* The enemy spawners (#664, include/spawners.hpp), ROM
 * 0x0801EF0C-0x08020E84; read as "two-line text popup" spawners at
 * first. Each builds a moving sprite, attaches a new EnemyCtrl to it
 * (twice: once before and once after the controller's kind and the
 * part's `mover`), mirrors it by its parameter record and adds it to the
 * collidable list; the tails set the controller's animations, ranges and
 * state. Built with old_agbcp. */

/* The animation table at `offset` in the sprite banks. */
static inline struct anim_table *BankAnim(u32 offset)
{
    return (struct anim_table *)(SPRITE_BANK_BASE + offset);
}

/* The setters below are inline because old_agbcp schedules a store's
 * value before its address only when the value arrives as an inline
 * function's parameter; written in place, the order flips. */
static inline void SetKind(Sprite *part, s32 value)
{
    part->kind = value;
}

static inline void SetAnims(EnemyCtrl *hdr, const s32 *anims)
{
    hdr->anims = anims;
}

/* The mask arrives as an `s32` so it is derived from a still-live
 * constant (`subs r0, #0x43`), as in the ROM. */
static inline void AndFlags(Sprite *part, s32 mask)
{
    part->f.flags &= mask;
}

/* Animation `anim` from its start. */
static inline void SetAnim(Sprite *part, s32 anim)
{
    part->tag = anim;
    part->ResetFrameTimer();
    part->ResetFrameIndex();
    part->SetAnimDone(0);
}

/* The multi-field setters load every value before storing any, as the ROM
 * does; written as separate statements, each load and store interleave. */
static inline void SetHitBox(EnemyCtrl *hdr, s32 l, s32 t, s32 r, s32 b)
{
    hdr->boxL = l;
    hdr->boxR = r;
    hdr->boxT = t;
    hdr->boxB = b;
}

static inline void SetAttackCycle(EnemyCtrl *hdr, s32 idleTime, s32 attackTime, s32 cycleOffset)
{
    hdr->idleTime = idleTime;
    hdr->attackTime = attackTime;
    hdr->cycleOffset = cycleOffset;
}

static inline void SetWave(EnemyCtrl *hdr, s32 period, s32 phase, s32 amplitude)
{
    hdr->period = period;
    hdr->phase = phase;
    hdr->amplitude = amplitude;
}

static inline void SetShotTiming(EnemyCtrl *hdr, s32 period, s32 phase)
{
    hdr->shotPeriod = period;
    hdr->shotPhase = phase;
}

/* The X and Y mirror bits from a parameter record's flags (bit 1 clear:
 * X mirrored; bit 2: Y mirrored). */
static inline void SetMirror(Sprite *part, const struct entity_params *rec)
{
    part->mirrorFlags.mirrorX = (rec->flags >> 1 ^ 1) & 1;
    part->mirrorFlags.mirrorY = rec->flags >> 2 & 1;
}

/* The u8 parameter keeps the toggled bit's truncation where the ROM
 * has it. */
static inline void SetFlipX(Sprite *part, u8 value)
{
    part->mirrorFlags.mirrorX = value;
}

static inline void SetMirrorX(Sprite::MirrorFlags *bits, u8 value)
{
    bits->mirrorX = value;
}

/* Bank +0x9C, animation 0 from its start; patrols (state 2) within the
 * record's X range. */
void SpawnLizard(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x9c);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_LIZARD;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnim(part, 0);
    hdr->SetState(2);
    hdr->SetRangeX(rec2->p.patrol.rangeX);
}

/* Bank +0x84; state 0x11, then gVultureAnimMap and a 100x50 trigger box at
 * its origin. */
void SpawnVulture(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0x84);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_VULTURE;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    hdr->SetState(0x11);
    hdr->anims = gVultureAnimMap;
    SetHitBox(hdr, 0, 0, 100, 50);
}

/* Bank +0x78, animation 1 from its start, kind 6; state 3, then
 * gVenusFlytrapAnimMap and a 45x20 trigger box raised 20 pixels. */
void SpawnVenusFlytrap(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0x78);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_VENUS_FLYTRAP;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    SetAnim(part, 1);
    part->kind = 6;
    hdr->SetState(3);
    hdr->anims = gVenusFlytrapAnimMap;
    SetHitBox(hdr, 0, -20, 45, 20);
}

/* Entity type 0x2B: an enemy on sprite bank 14 that patrols (state 2,
 * UpdateEnemyPatrol); placed only in the jungle rooms 15, 27 and 32. The
 * species isn't identified, so the name is generic.
 *
 * Bank +0xA8; patrols (state 2) within the record's X range. */
void SpawnPatrollingJungleEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xa8);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PATROLLING_JUNGLE_ENEMY;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    hdr->SetState(2);
    hdr->SetRangeX(rec2->p.patrol.rangeX);
}

/* Bank +0x90, gBlowgunTribesmanAnimMap; the record's attack cycle, and a
 * shot every (idle + attack) / 2 frames, offset by the cycle offset plus a
 * quarter of that; state 0x10. */
void SpawnBlowgunTribesman(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;
    s32 mid;

    part->anim = BankAnim(0x90);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_BLOWGUN_TRIBESMAN;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnims(hdr, gBlowgunTribesmanAnimMap);
    SetAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                   rec2->p.cycle.cycleOffset);
    mid = (rec2->p.cycle.idleTime + rec2->p.cycle.attackTime) / 2;
    SetShotTiming(hdr, mid, rec2->p.cycle.cycleOffset + mid / 4);
    hdr->SetState(0x10);
}

/* Bank +0xB4, animation 0 from its start, gPenguinAnimMap; the record's
 * attack cycle and X range; state 0xD. */
void SpawnPenguin(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xb4);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PENGUIN;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnim(part, 0);
    SetAnims(hdr, gPenguinAnimMap);
    SetAttackCycle(hdr, rec2->p.rangeAttackFirst.idleTime, rec2->p.rangeAttackFirst.attackTime,
                   rec2->p.rangeAttackFirst.cycleOffset);
    hdr->SetRangeX(rec2->p.rangeAttackFirst.rangeX);
    hdr->SetState(0xd);
}

/* A ground sprite on bank +0xCC, animation 0 (set before the second attach),
 * always active; state 5, and SFX_SEAL_SPAWN. The seal spawner
 * (SpawnSealSpawner, a PeriodicSpawner) calls it. */
void SpawnSeal(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    GroundSprite *part = GroundSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0xcc);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_SEAL;
    SetAnim(part, 0);
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    part->f.flags |= 0x10;
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    hdr->SetState(5);
    PlaySfx(gAudioContext, SFX_SEAL_SPAWN, 0x100);
}

/* Bank +0xC0, kind 6; patrols (state 2) within the record's X range. */
void SpawnPolarBear(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xc0);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_POLAR_BEAR;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    part->kind = 6;
    hdr->SetState(2);
    hdr->SetRangeX(rec2->p.patrol.rangeX);
}

/* Bank +0x3C, animation 2 from its start, kind 5, gPufferfishAnimMap; the
 * record's attack cycle and wave; state 14. */
void SpawnPufferfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x3c);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PUFFERFISH;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnim(part, 2);
    part->kind = 5;
    SetAnims(hdr, gPufferfishAnimMap);
    SetAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                   rec2->p.cycle.cycleOffset);
    SetWave(hdr, rec2->p.cycle.wavePeriod, rec2->p.cycle.wavePhase, rec2->p.cycle.waveAmplitude);
    hdr->SetState(14);
}

/* Bank +0x30, kind 6, gSharkAnimMap; state 15, then the record's X range and
 * Y range and speed. */
void SpawnShark(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x30);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_SHARK;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    part->kind = 6;
    SetAnims(hdr, gSharkAnimMap);
    hdr->SetState(15);
    hdr->SetRangeX(rec2->p.rangeHomingY.rangeX);
    hdr->SetRangeYSpeed(rec2->p.rangeHomingY.rangeY, rec2->p.rangeHomingY.speedY,
                        rec2->p.rangeHomingY.accelY);
}

/* Bank +0x24, animation 0 from its start, X mirror flipped, kind 6; state 1. */
void SpawnMorayEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0x24);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_MORAY_EEL;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    SetAnim(part, 0);
    {
        s32 f = part->mirrorFlags.mirrorX;
        SetFlipX(part, !f);
    }
    part->kind = 6;
    hdr->SetState(1);
}

/* Bank +0x60, kind 3, gElectricEelAnimMap; the record's attack cycle and X
 * range; state 13. */
void SpawnElectricEel(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x60);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_ELECTRIC_EEL;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    part->kind = 3;
    SetAnims(hdr, gElectricEelAnimMap);
    SetAttackCycle(hdr, rec2->p.rangeCycle.idleTime, rec2->p.rangeCycle.attackTime,
                   rec2->p.rangeCycle.cycleOffset);
    hdr->SetRangeX(rec2->p.rangeCycle.rangeX);
    hdr->SetState(13);
}

/* Bank +0x54, gSquidAnimMap (without the default map first); state 7. */
void SpawnSquid(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0x54);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_SQUID;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    hdr->anims = gSquidAnimMap;
    hdr->SetState(7);
}

/* Bank +0x6C; state 6, then the record's wave. */
void SpawnJellyfish(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x6c);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_JELLYFISH;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    hdr->SetState(6);
    SetWave(hdr, rec2->p.wave.period, rec2->p.wave.phase, rec2->p.wave.amplitude);
}

/* Bank +0x12C, X mirror flipped, kind 2, flag bit 6 cleared; state 1. */
void SpawnLaserBarrier(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0x12c);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_LASER_BARRIER;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    {
        s32 f = part->mirrorFlags.mirrorX;
        part->mirrorFlags.mirrorX = f == 0;
    }
    SetKind(part, 2);
    AndFlags(part, ~0x40);
    hdr->SetState(1);
}

/* Entity type 0x38: an enemy on sprite bank 27 (a shell that shoots out
 * flaming spikes) that attacks in place (state 4, UpdateEnemyAttackCycle);
 * placed only in space rooms. Species not identified.
 *
 * Bank +0x144, gStationarySpaceEnemyAnimMap; the record's attack cycle;
 * state 4. */
void SpawnStationarySpaceEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x144);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_STATIONARY_SPACE_ENEMY;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnims(hdr, gStationarySpaceEnemyAnimMap);
    SetAttackCycle(hdr, rec2->p.attackFirst.idleTime, rec2->p.attackFirst.attackTime,
                   rec2->p.attackFirst.cycleOffset);
    hdr->SetState(4);
}

/* Entity type 0x39: an enemy on sprite bank 24 that patrols and attacks
 * (state 13); placed only in space rooms. Species not identified.
 *
 * Bank +0x120, gPatrollingSpaceEnemyAnimMap; the record's attack cycle and X
 * range; state 0xD. */
void SpawnPatrollingSpaceEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x120);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PATROLLING_SPACE_ENEMY;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnims(hdr, gPatrollingSpaceEnemyAnimMap);
    SetAttackCycle(hdr, rec2->p.rangeCycle.idleTime, rec2->p.rangeCycle.attackTime,
                   rec2->p.rangeCycle.cycleOffset);
    hdr->SetRangeX(rec2->p.rangeCycle.rangeX);
    hdr->SetState(0xd);
}

/* 0x28 pixels above its spot, bank +0x15C, gSaucerLabAssistantAnimMap; an
 * attack cycle of 0x78 and 0x5A frames from the record's offset, an X range
 * of 0x28; state 0x12. */
void SpawnSaucerLabAssistant(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    u16 y = arg2 - 0x28;
    MovingSprite *part = MovingSprite::Create(arg0, arg1, y, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x15c);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_SAUCER_LAB_ASSISTANT;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnims(hdr, gSaucerLabAssistantAnimMap);
    SetAttackCycle(hdr, 0x78, 0x5a, rec2->p.cycle.cycleOffset);
    hdr->SetRangeX(0x28);
    hdr->SetState(0x12);
}

/* Bank +0x138, kind 0xA, flag bit 6 cleared, gCrusherAnimMap; the record's
 * attack cycle; state 4. */
void SpawnPistonCrusher(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x138);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PISTON_CRUSHER;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetKind(part, 0xa);
    AndFlags(part, ~0x40);
    SetAnims(hdr, gCrusherAnimMap);
    SetAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                   rec2->p.cycle.cycleOffset);
    hdr->SetState(4);
}

/* Bank +0x114, X mirror flipped, gFlamethrowerLabAssistantAnimMap; the
 * record's attack cycle; state 4.
 *
 * Kept from the C (the same workarounds less one MATCH_USE): the ROM
 * keeps the mirror byte's address (part+0x28) in r3 across
 * AddToPartList through a stack slot (`str r3, [sp]` just before the
 * call, `ldr r3, [sp]` before the second flip), &gEntityFlags in r9 and
 * -0x11 in sl. g++ ranks the address (6 references) above both and
 * gives it a callee-saved register, with the same code under every
 * spelling tried (inline setters taking the sprite or the bits, a
 * pointer taken earlier, the sign test, the record lookup moved); the
 * register allocation is the same as the C front end's. So:
 * - `q` is the address in a stack slot (`MATCH_USE_MEM`), stored inside
 *   the call's argument after a copy of `part` (`MATCH_KEEP`), so the
 *   store follows the r1 move;
 * - the r3 hold (`h3`) keeps r3 free over the animation map store and
 *   the second lookup, so &gEntityFlags is reloaded through r1;
 * - three extra references lift rec2 above the reloaded `q` (rec2 keeps
 *   r2), and one puts `p` ahead of the flag byte (r3, then r4). */
void SpawnFlamethrowerLabAssistant(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;
    Sprite::MirrorFlags *q;
    MATCH_HOLD_REG(s32, h3, r3);

    part->anim = BankAnim(0x114);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_FLAMETHROWER_LAB_ASSISTANT;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    // clang-format off
    CollidableList()->Add(({
        MovingSprite *t = part;

        MATCH_KEEP(t);
        q = &part->mirrorFlags;
        t;
    }));
    // clang-format on
    MATCH_USE_MEM(q);
    MATCH_HOLD(h3);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    MATCH_USE(h3);
    MATCH_USE(rec2);
    MATCH_USE(rec2);
    MATCH_USE(rec2);
    {
        Sprite::MirrorFlags *p = q;
        s32 f;

        MATCH_USE(p);
        f = p->mirrorX;
        SetMirrorX(p, !f);
    }
    SetKind(part, 1);
    SetAnims(hdr, gFlamethrowerLabAssistantAnimMap);
    SetAttackCycle(hdr, rec2->p.attackFirst.idleTime, rec2->p.attackFirst.attackTime,
                   rec2->p.attackFirst.cycleOffset);
    hdr->SetState(4);
}

/* Entity type 0x41: an enemy on sprite bank 22 that follows the player's
 * X within its range (state 9, UpdateEnemyHomingX/UpdateEnemyOscillateX);
 * placed only in sewer rooms. Species not identified.
 *
 * Bank +0x108; state 9, then the record's X range and speed and a wave of
 * {0x80, 0, 0x14}. */
void SpawnHomingSewerEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x108);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_HOMING_SEWER_ENEMY;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    hdr->SetState(9);
    hdr->SetRangeXSpeed(rec2->p.homingX.rangeX, rec2->p.homingX.speedX, rec2->p.homingX.accelX);
    SetWave(hdr, 0x80, 0, 0x14);
}

/* Entity type 0x42: an enemy on sprite bank 20 that patrols (state 2);
 * placed only in sewer rooms. Species not identified.
 *
 * Bank +0xF0, gPatrollingSewerEnemyAnimMap; patrols (state 2) within the
 * record's X range. */
void SpawnPatrollingSewerEnemy(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xf0);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_PATROLLING_SEWER_ENEMY;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    SetAnims(hdr, gPatrollingSewerEnemyAnimMap);
    hdr->SetState(2);
    hdr->SetRangeX(rec2->p.patrol.rangeX);
}

/* Bank +0xFC; patrols (state 2) within the record's X range. */
void SpawnRat(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xfc);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_RAT;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    hdr->SetState(2);
    hdr->SetRangeX(rec2->p.patrol.rangeX);
}

/* Bank +0xE4, kind 7, gPatrollingSewerEnemyAnimMap; state 8. */
void SpawnFrog(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;

    part->anim = BankAnim(0xe4);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_FROG;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    part->kind = 7;
    hdr->anims = gPatrollingSewerEnemyAnimMap;
    hdr->SetState(8);
}

/* Bank +0x48, kind 4; state 0xB, then the record's X and Y ranges and
 * speeds. */
void SpawnSeaMine(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0x48);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_SEA_MINE;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    part->kind = 4;
    hdr->SetState(0xb);
    hdr->SetRangeXSpeed(rec2->p.homingXY.rangeX, rec2->p.homingXY.speedX, rec2->p.homingXY.accelX);
    hdr->SetRangeYSpeed(rec2->p.homingXY.rangeY, rec2->p.homingXY.speedY, rec2->p.homingXY.accelY);
}

/* Bank +0xD8, kind 0xA, flag bit 6 cleared, gCrusherAnimMap; the record's
 * attack cycle; state 4. */
void SpawnWoodenCrusher(u32 arg0, u16 arg1, u16 arg2, u16 arg3)
{
    MovingSprite *part = MovingSprite::Create(arg0, arg1, arg2, arg3);
    EnemyCtrl *hdr;
    const struct entity_params *rec;
    const struct entity_params *rec2;

    part->anim = BankAnim(0xd8);
    part->palette = part->GetAnimPaletteSlot();
    hdr = new EnemyCtrl;
    hdr->Attach(part);
    hdr->kind = ENEMY_KIND_WOODEN_CRUSHER;
    part->mover = hdr;
    hdr->Attach(part);
    SetKind(part, 1);
    part->f.flags &= 0x7f;
    rec = EntityParams(arg3);
    SetMirror(part, rec);
    CollidableList()->Add(part);
    SetAnims(hdr, gEnemyDefaultAnimMap);
    rec2 = EntityParams(arg3);
    part->kind = 0xa;
    AndFlags(part, ~0x40);
    hdr->anims = gCrusherAnimMap;
    SetAttackCycle(hdr, rec2->p.cycle.idleTime, rec2->p.cycle.attackTime,
                   rec2->p.cycle.cycleOffset);
    hdr->SetState(4);
}
