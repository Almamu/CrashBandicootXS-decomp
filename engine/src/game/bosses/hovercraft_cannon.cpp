#include "game/bosses/boss_actors.hpp"
#include "game/bosses/hovercraft.hpp"
#include "engine/audio/audio.hpp"
#include "game/vehicles/vehicle.hpp"
#include "engine/actor/actor_bg.hpp"

extern "C" {
#include "engine/actor/actor.h"
#include "globals.h"
#include "engine/util/math_util.h"
}

using Engine::Actor::ActorBg;
using Game::Bosses::Hovercraft;
using Game::Bosses::HovercraftCannon;
using Game::Bosses::HovercraftCannonFlash;
using Game::Entities::ActorSelf;
using Game::Entities::HpActor;
using Game::Vehicles::Jetpack::JetpackCannonball;

/* The hovercraft's cannon (#664 part 11h, engine/include/game/bosses/boss_actors.hpp): an
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

    x = Hovercraft::GetX() + INT_TO_Q8(32);
    y = Hovercraft::GetY() + INT_TO_Q8(48);
    z = Hovercraft::GetZ() - INT_TO_Q8(1);

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
            /* `absDx` is needed (inlined, the instructions are reordered); `absDy`
             * stays beside it */
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= 0xFFF) {
                const struct hovercraft_attack *table;
                s32 n;

                JetpackCannonball::Spawn(x, y, z, dx, dy);
                HovercraftCannonFlash::Spawn(x, y, z);

                n = count + 1;
                count = n;
                table = Hovercraft::GetAttack();
                if (n == table->timing[1].burst) {
                    count = slot;
                    table = Hovercraft::GetAttack();
                    next = table->timing[1].burstDelay;
                } else {
                    table = Hovercraft::GetAttack();
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

    if (depth > INT_TO_Q8(75))
        SetState(HOVERCRAFT_CANNON_STATE_WAIT, 0);
}

/* Takes `amount` off the hit points (with the hovercraft's hit flash); at
 * zero, the cannon is dead: one part less for the hovercraft, state 2
 * with animation 2, and the explosion sound; otherwise the hit sound. */
void HovercraftCannon::Damage(s32 amount)
{
    Hovercraft::StartHitFlash();
    hp -= amount;

    if (hp <= 0) {
        dead = 1;
        Hovercraft::LosePart();
        SetState(HOVERCRAFT_CANNON_STATE_DESTROYED, 2);
        gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    } else {
        gAudioContext->PlaySfx(SFX_HOVERCRAFT_PART_HIT, 0x100);
    }
}

/* The state method, then the common update unless the destroyed state's
 * animation has played through. */
void HovercraftCannon::Update()
{
    /* `st` is needed: inlined, the instructions are reordered */
    s32 st;
    s32 step;

    (this->*stateFuncs[state])();

    st = state;
    step = 1;
    if (st == HOVERCRAFT_CANNON_STATE_DESTROYED && animDone != 0)
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
    state = HOVERCRAFT_CANNON_STATE_WAIT;
    dead = 0;
}

/* Shakes the BG, and deletes the cannon once its animation is done. */
void HovercraftCannon::StateDestroyed()
{
    ActorBg::Shake(INT_TO_Q8(4));

    if (animDone != 0)
        delete this;
}

/* Follows the hovercraft, and once it is close enough (depth 0x4AFF),
 * starts firing: the first cooldown from the attack's cannon timing,
 * state 1 with animation 1. */
void HovercraftCannon::StateWait()
{
    x = Hovercraft::GetX() + INT_TO_Q8(32);
    y = Hovercraft::GetY() + INT_TO_Q8(48);
    z = Hovercraft::GetZ() - INT_TO_Q8(1);

    if (depth <= INT_TO_Q8(75) - 1) {
        cooldown = Hovercraft::GetAttack()->timing[1].delay;
        count = 0;
        SetState(HOVERCRAFT_CANNON_STATE_FIRE, 1);
    }
}

/* Update without the common update: returns 0 once the destroyed state's
 * animation has played through, 1 otherwise. */
s32 HovercraftCannon::RunState()
{
    (this->*stateFuncs[state])();

    if (state == HOVERCRAFT_CANNON_STATE_DESTROYED && animDone != 0)
        return 0;
    return 1;
}

u8 HovercraftCannon::IsUnshootable()
{
    return dead;
}
