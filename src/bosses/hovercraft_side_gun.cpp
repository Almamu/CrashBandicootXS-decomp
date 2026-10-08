#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "actor.h"
#include "globals.h"
}

/* The hovercraft's side guns (#664 part 11h, include/boss_actors.hpp):
 * two HpActors on the hovercraft's sides (`left`, SpawnHovercraft) that
 * fire fireballs in bursts. See
 * docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* 24 hit points before the hovercraft's first level (GetHovercraftLevel,
 * the level index), 16 after; on the left side animation 0 and an X
 * offset of -0x4100, on the right animation 1 and 0x8400; the first
 * fireball after the attack's side gun delay.
 *
 * `left` is a `bool`: the caller stores it as a byte on the stack, and
 * this reads it back with `ldrb` (a `u8` would be a promoted word). The
 * hit points are HpActor's argument, so they are computed before the
 * base constructor runs, as in the ROM. */
HovercraftSideGun::HovercraftSideGun(const struct anim_table_record *rec, s32 x, s32 y, s32 z,
                                     bool left)
    : HpActor(rec, x, y, z, GetHovercraftLevel() == 0 ? 0x18 : 0x10)
{
    s32 idx;
    u8 l;

    this->left = left;
    state = 0;
    l = this->left;
    idx = 1;
    if (l != 0)
        idx = 0;
    SetState(0, idx);
    dead = 0;
    if (this->left != 0)
        offX = -0x4100;
    else
        offX = 0x8400;
    offY = 0xa00;
    offZ = -1;
    visible = 0;
    orbitTimer = GetHovercraftAttack()->timing[0].delay;
    lap = 0;
}

/* Takes `amount` off the hit points (with the hovercraft's hit flash); at
 * zero, the gun is dead: one part less for the hovercraft, visible, state
 * 1 with its side's animation, and the explosion sound; otherwise the
 * hit sound. */
void HovercraftSideGun::Damage(s32 amount)
{
    StartHovercraftHitFlash();
    hp -= amount;

    if (hp <= 0) {
        s32 idx;
        u8 l;

        LoseHovercraftPart();
        dead = 1;
        visible = 1;
        l = left;
        idx = 1;
        if (l != 0)
            idx = 0;
        SetState(1, idx);
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    } else {
        gAudioContext->PlaySfx(SFX_HOVERCRAFT_PART_HIT, 0x100);
    }
}

/* Follows the hovercraft and, while the gun is alive (state 0) and the
 * hovercraft close enough (depth 0x2800), fires a fireball whenever the
 * timer runs out, restarting it from the attack's side gun timing.
 * Update's body after the common update, and RunState's. */
static inline void StepSideGun(HovercraftSideGun *self)
{
    self->x = GetHovercraftX() + self->offX;
    self->y = GetHovercraftY() + self->offY;
    self->z = GetHovercraftZ() + self->offZ;

    if (self->state == 0 && self->depth > 0x2800) {
        s32 timer = self->orbitTimer;
        s32 next;

        if (timer == 0) {
            s32 n;

            SpawnHovercraftFireball(self->x, self->y, self->z);
            n = self->lap + 1;
            self->lap = n;

            if (n == GetHovercraftAttack()->timing[0].burst) {
                self->lap = timer;
                next = GetHovercraftAttack()->timing[0].burstDelay;
            } else {
                next = GetHovercraftAttack()->timing[0].delay;
            }
        } else {
            next = timer - 1;
        }

        self->orbitTimer = next;
    }
}

/* The common update, then StepSideGun. */
void HovercraftSideGun::Update()
{
    ActorSelf::Update();
    StepSideGun(this);
}

/* Update without the common update.
 * UNUSED - no caller anywhere in the ROM (checked every src/ file and
 * every word-aligned Thumb pointer in baserom.gba). */
void HovercraftSideGun::RunState()
{
    StepSideGun(this);
}

s32 HovercraftSideGun::IsUnshootable()
{
    return dead;
}

/* gHovercraftCannonFlashVtable slot 4: none (the cannon's muzzle flash
 * can't be hurt). */
void HovercraftCannonFlash::Damage(s32)
{
}
