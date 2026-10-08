#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "actor.h"
#include "vehicle.h"
#include "globals.h"
#include "math_util.h"
}

/* The hovercraft's cannon (#664 part 11h, include/boss_actors.hpp): an
 * HpActor that rides on the hovercraft and fires cannonballs at the
 * player. See docs/matching/archive/issue-62-0x08033804-actor.md. */

/* Follows the hovercraft and, once the cooldown is over and the player
 * is in its line of fire, fires a cannonball at the player (with a muzzle
 * flash) and starts the next cooldown from the attack's cannon timing.
 * Once the hovercraft is past it (depth 0x4B00), back to state 0.
 *
 * Both divisions are plain `/` through the ROM's own `__divsi3` - as a
 * libcall they don't clobber memory, so `z` stays CSE'd in `r6` across
 * them - and the new cooldown is stored at one shared `store:` label from
 * all three paths. */
void HovercraftCannon::StateFire()
{
    s32 slot;
    s32 next;

    x = GetHovercraftX() + 0x2000;
    y = GetHovercraftY() + 0x3000;
    z = GetHovercraftZ() - 0x100;

    slot = cooldown;
    if (slot == 0) {
        ActorSelf *player = gActorList;
        s32 angle = (player->z - z) / -0x1AA;

        if (angle > 0) {
            s32 scale = 0x1000 / angle;
            s32 rawDx = (player->x - x) * scale;
            s32 dx = Q12_TO_INT(rawDx);
            s32 rawDy = (player->y - y) * scale;
            s32 dy = Q12_TO_INT(rawDy);
            s32 signDx = rawDx >> 31;
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                const struct hovercraft_attack *table;
                s32 n;

                SpawnJetpackCannonball(x, y, z, dx, dy);
                SpawnHovercraftCannonFlash(x, y, z);

                n = count + 1;
                count = n;
                table = GetHovercraftAttack();
                if (n == table->timing[1].burst) {
                    count = slot;
                    table = GetHovercraftAttack();
                    next = table->timing[1].burstDelay;
                } else {
                    table = GetHovercraftAttack();
                    next = table->timing[1].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        cooldown = next;
    }

    if (depth > 0x4B00)
        SetState(0, 0);
}

/* Takes `amount` off the hit points (with the hovercraft's hit flash); at
 * zero, the cannon is dead: one part less for the hovercraft, state 2
 * with animation 2, and the explosion sound; otherwise the hit sound. */
void HovercraftCannon::Damage(s32 amount)
{
    StartHovercraftHitFlash();
    hp -= amount;

    if (hp <= 0) {
        dead = 1;
        LoseHovercraftPart();
        SetState(2, 2);
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    } else {
        gAudioContext->PlaySfx(SFX_HOVERCRAFT_PART_HIT, 0x100);
    }
}

/* The state method, then the common update unless the destroyed state's
 * animation has played through. */
void HovercraftCannon::Update()
{
    s32 st;
    s32 step;

    (this->*stateFuncs[state])();

    st = state;
    step = 1;
    if (st == 2 && animDone != 0)
        step = 0;
    if (step)
        ActorSelf::Update();
}

/* 15 hit points, state 0. */
HovercraftCannon::HovercraftCannon(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 15)
{
    spawnX = x;
    spawnY = y;
    state = 0;
    dead = 0;
}

/* Shakes the BG, and deletes the cannon once its animation is done. */
void HovercraftCannon::StateDestroyed()
{
    ShakeActorBg(0x400);

    if (animDone != 0)
        delete this;
}

/* Follows the hovercraft, and once it is close enough (depth 0x4AFF),
 * starts firing: the first cooldown from the attack's cannon timing,
 * state 1 with animation 1. */
void HovercraftCannon::StateWait()
{
    x = GetHovercraftX() + 0x2000;
    y = GetHovercraftY() + 0x3000;
    z = GetHovercraftZ() - 0x100;

    if (depth <= 0x4AFF) {
        cooldown = GetHovercraftAttack()->timing[1].delay;
        count = 0;
        SetState(1, 1);
    }
}

/* Update without the common update: returns 0 once the destroyed state's
 * animation has played through, 1 otherwise. */
s32 HovercraftCannon::RunState()
{
    (this->*stateFuncs[state])();

    if (state == 2 && animDone != 0)
        return 0;
    return 1;
}

s32 HovercraftCannon::IsUnshootable()
{
    return dead;
}
