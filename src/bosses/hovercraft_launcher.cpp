#include "boss_actors.hpp"
#include "audio.hpp"

extern "C" {
#include "util.h"
#include "actor.h"
#include "vehicle.h"
#include "globals.h"
#include "math_util.h"
}

/* The hovercraft's launcher (#664 part 11h, include/boss_actors.hpp): the
 * cannon's sibling, an HpActor on the hovercraft that launches planes,
 * bombers and balloons once the hovercraft has lost two parts. See
 * docs/matching/archive/issue-62-0x08033804-actor.md and
 * docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* HovercraftCannon::StateFire's sibling: follows the hovercraft and, once
 * the cooldown is over and the player is in range, launches one of three
 * kinds at random (CreateJetpackActor kinds 5, 6 and 8) and starts the
 * next cooldown from the attack's launcher timing. Once the hovercraft
 * is past it (depth 0x4B00) and falling back (its state 3), back to
 * state 0 with animation 2. */
void HovercraftLauncher::StateLaunch()
{
    s32 slot;
    s32 next;

    x = GetHovercraftX() + 0x1E00;
    y = GetHovercraftY() - 0x3000;
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
                s32 kind = (u16)RandRange(3);
                const struct hovercraft_attack *table;
                s32 n;

                if (kind == 0)
                    CreateJetpackActor(5, x, y, z, (struct actor_spawn *)slot);
                else if (kind == 1)
                    CreateJetpackActor(6, x, y, z, (struct actor_spawn *)slot);
                else
                    CreateJetpackActor(8, x, y, z, (struct actor_spawn *)slot);

                n = count + 1;
                count = n;
                table = GetHovercraftAttack();
                if (n == table->timing[2].burst) {
                    count = 0;
                    table = GetHovercraftAttack();
                    next = table->timing[2].burstDelay;
                } else {
                    table = GetHovercraftAttack();
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

    if (depth > 0x4B00 && GetHovercraftState() == 3)
        SetState(0, 2);
}

/* HovercraftCannon::Damage's twin, but only while launching (state 1),
 * and the destroyed state's animation is 3. */
void HovercraftLauncher::Damage(s32 amount)
{
    s32 st = state;

    if (st == 1) {
        StartHovercraftHitFlash();
        hp -= amount;

        if (hp <= 0) {
            dead = st;
            LoseHovercraftPart();
            SetState(2, 3);
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

/* 25 hit points, state 0. */
HovercraftLauncher::HovercraftLauncher(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, y, z, 0x19)
{
    spawnX = x;
    spawnY = y;
    SetState(0, 0);
    dead = 0;
}

/* Shakes the BG, and deletes the launcher once its animation is done. */
void HovercraftLauncher::StateDestroyed()
{
    ShakeActorBg(0x400);

    if (animDone != 0)
        delete this;
}

/* Follows the hovercraft, and once it has lost two parts and is closing
 * in (its state 2), or falling back (state 3) close enough (depth
 * 0x4AFF), starts launching: state 1 with animation 1. */
void HovercraftLauncher::StateWait()
{
    x = GetHovercraftX() + 0x1E00;
    y = GetHovercraftY() - 0x3000;
    z = GetHovercraftZ() - 0x100;

    if (GetHovercraftPartsLeft() <= 2 &&
        (GetHovercraftState() == 2 || (GetHovercraftState() == 3 && depth <= 0x4AFF))) {
        cooldown = 0;
        count = 0;
        SetState(1, 1);
    }
}

/* Update without the common update: returns 0 once the destroyed state's
 * animation has played through, 1 otherwise. */
s32 HovercraftLauncher::RunState()
{
    (this->*stateFuncs[state])();

    if (state == 2 && animDone != 0)
        return 0;
    return 1;
}

s32 HovercraftLauncher::IsUnshootable()
{
    return dead;
}
