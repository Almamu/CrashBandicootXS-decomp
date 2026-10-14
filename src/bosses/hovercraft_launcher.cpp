#include "boss_actors.hpp"
#include "hovercraft.hpp"
#include "engine/audio/audio.hpp"
#include "vehicle.hpp"
#include "engine/actor/actor_bg.hpp"

extern "C" {
#include "engine/util/util.h"
#include "engine/actor/actor.h"
#include "globals.h"
#include "engine/util/math_util.h"
#include "constants/actor_kinds.h"
}

using Engine::Actor::ActorBg;
using Game::Bosses::Hovercraft;
using Game::Bosses::HovercraftLauncher;
using Game::Entities::ActorSelf;
using Game::Entities::HpActor;
using Game::Vehicles::Jetpack::JetpackPlayer;

/* The hovercraft's launcher (#664 part 11h, include/boss_actors.hpp): the
 * cannon's sibling, an HpActor on the hovercraft that launches planes,
 * bombers and balloons once the hovercraft has lost two parts. See
 * docs/matching/archive/issue-62-0x08033804-actor.md and
 * docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* HovercraftCannon::StateFire's sibling: follows the hovercraft and, once
 * the cooldown is over and the player is in range, launches one of three
 * bombers at random (JetpackPlayer::CreateActor's JETPACK_KIND_BOMBER_HOME,
 * _BOB_VERTICAL and _CIRCLE, with no spawn record) and starts the
 * next cooldown from the attack's launcher timing. Once the hovercraft
 * is past it (depth 0x4B00) and falling back (its state 3), back to
 * state 0 with animation 2. */
void HovercraftLauncher::StateLaunch()
{
    s32 slot;
    s32 next;

    x = Hovercraft::GetX() + INT_TO_Q8(30);
    y = Hovercraft::GetY() - INT_TO_Q8(48);
    z = Hovercraft::GetZ() - INT_TO_Q8(1);

    slot = cooldown;
    if (slot == 0) {
        ActorSelf *player = gActorList;
        s32 angle = (player->z - z) / -0x1AA;

        if (angle > 0) {
            s32 scale = 0x1000 / angle;
            s32 rawDx = (player->x - x) * scale;
            /* `dx` is needed: inlined, the instructions differ */
            s32 dx = Q12_TO_INT(rawDx);
            s32 rawDy = (player->y - y) * scale;
            /* `dy` is needed: inlined, the instructions are reordered */
            s32 dy = Q12_TO_INT(rawDy);
            s32 signDx = rawDx >> 31;
            /* `absDx` is needed (inlined, the instructions are reordered); `absDy`
             * stays beside it */
            s32 absDx = (dx ^ signDx) - signDx;
            s32 signDy = rawDy >> 31;
            s32 absDy = (dy ^ signDy) - signDy;

            if (absDx + absDy <= INT_TO_Q8(16) - 1) {
                s32 kind = RandRange(3); // a roll, 0-2: which bomber
                const struct hovercraft_attack *table;
                s32 n;

                if (kind == 0)
                    JetpackPlayer::CreateActor(JETPACK_KIND_BOMBER_HOME, x, y, z, 0);
                else if (kind == 1)
                    JetpackPlayer::CreateActor(JETPACK_KIND_BOMBER_BOB_VERTICAL, x, y, z, 0);
                else
                    JetpackPlayer::CreateActor(JETPACK_KIND_BOMBER_CIRCLE, x, y, z, 0);

                n = count + 1;
                count = n;
                table = Hovercraft::GetAttack();
                if (n == table->timing[2].burst) {
                    count = 0;
                    table = Hovercraft::GetAttack();
                    next = table->timing[2].burstDelay;
                } else {
                    table = Hovercraft::GetAttack();
                    next = table->timing[2].delay;
                }
                goto store;
            }
        }
    } else {
        next = slot - 1;
    store:
        cooldown = next;
    }

    if (depth > 0x4B00 && Hovercraft::GetState() == HOVERCRAFT_STATE_FALL_BACK)
        SetState(HOVERCRAFT_LAUNCHER_STATE_WAIT, 2);
}

/* HovercraftCannon::Damage's twin, but only while launching (state 1),
 * and the destroyed state's animation is 3. */
void HovercraftLauncher::Damage(s32 amount)
{
    s32 st = state;

    if (st == HOVERCRAFT_LAUNCHER_STATE_LAUNCH) {
        Hovercraft::StartHitFlash();
        hp -= amount;

        if (hp <= 0) {
            dead = st;
            Hovercraft::LosePart();
            SetState(HOVERCRAFT_LAUNCHER_STATE_DESTROYED, 3);
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        } else {
            gAudioContext->PlaySfx(SFX_HOVERCRAFT_PART_HIT, 0x100);
        }
    }
}

/* The state method, then the common update unless the destroyed state's
 * animation has played through. */
void HovercraftLauncher::Update()
{
    /* `st` is needed: inlined, the instructions are reordered */
    s32 st;
    s32 step;

    (this->*stateFuncs[state])();

    st = state;
    step = 1;
    if (st == HOVERCRAFT_LAUNCHER_STATE_DESTROYED && animDone != 0)
        step = 0;
    if (step)
        ActorSelf::Update();
}

/* 25 hit points, state 0. */
HovercraftLauncher::HovercraftLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 0x19)
{
    spawnX = x;
    spawnY = y;
    SetState(HOVERCRAFT_LAUNCHER_STATE_WAIT, 0);
    dead = 0;
}

/* Shakes the BG, and deletes the launcher once its animation is done. */
void HovercraftLauncher::StateDestroyed()
{
    ActorBg::Shake(INT_TO_Q8(4));

    if (animDone != 0)
        delete this;
}

/* Follows the hovercraft, and once it has lost two parts and is closing
 * in (its state 2), or falling back (state 3) close enough (depth
 * 0x4AFF), starts launching: state 1 with animation 1. */
void HovercraftLauncher::StateWait()
{
    x = Hovercraft::GetX() + INT_TO_Q8(30);
    y = Hovercraft::GetY() - INT_TO_Q8(48);
    z = Hovercraft::GetZ() - INT_TO_Q8(1);

    if (Hovercraft::GetPartsLeft() <= 2 &&
        (Hovercraft::GetState() == HOVERCRAFT_STATE_CLOSE_IN ||
         (Hovercraft::GetState() == HOVERCRAFT_STATE_FALL_BACK && depth <= INT_TO_Q8(75) - 1))) {
        cooldown = 0;
        count = 0;
        SetState(HOVERCRAFT_LAUNCHER_STATE_LAUNCH, 1);
    }
}

/* Update without the common update: returns 0 once the destroyed state's
 * animation has played through, 1 otherwise. */
s32 HovercraftLauncher::RunState()
{
    (this->*stateFuncs[state])();

    if (state == HOVERCRAFT_LAUNCHER_STATE_DESTROYED && animDone != 0)
        return 0;
    return 1;
}

u8 HovercraftLauncher::IsUnshootable()
{
    return dead;
}
