#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "actor.h"
#include "level.h"
#include "globals.h"
}

/* 0x08032480-0x080326E4 (#664 part 11g, include/vehicle.hpp), between
 * jetpack_crates.cpp and jetpack_collected_wumpa.cpp:
 *
 * - JetpackRocket (gJetpackRocketVtable): swings down to a height, then
 *   explodes; it hurts the player once on contact.
 * - JetpackRing::Update (the ring's other methods are in
 *   jetpack_collected_wumpa.cpp).
 *
 * See docs/matching/archive/issue-59-0x08031784-actor.md and
 * issue-60-61-gap-31a6c-part2.md. */

/* The jetpack player, the actor list's root. */
static inline JetpackPlayer *Player()
{
    return (JetpackPlayer *)gActorList;
}

/* Slot 2: while it flies (animation 0), it swings around `originX` and
 * comes down by `stepY` until `limitY`, where it explodes (Launch); on the
 * player it explodes at once, hurting the player by 0xE. Once exploded,
 * it is gone when the animation has played through, and hurts the player
 * on contact once. Then ActorSelf's update. */
void JetpackRocket::Update()
{
    if (animIndex != 0) {
        goto exploded;
    }

    if ((u8)IsTouchingPlayer(this)) {
        Player()->Damage(0xE);
        hit = 1;
        Launch();
    }

    /* Launch has just left animation 0: the exploded path. */
    if (animIndex != 0) {
        goto exploded;
    }

    x = originX + gSineTable[((((stateTime << 6) >> 4) & 0xFF) + 0x40) & 0xFF] * 16;
    if (y > limitY) {
        y += stepY;
    } else {
        box = gJetpackRocketBox;
        Launch();
    }
    goto tail;

exploded:
    if (animDone != 0) {
        delete this;
        return;
    }

    if (hit == 0 && (u8)IsTouchingPlayer(this)) {
        Player()->Damage(0xE);
        hit = 1;
    }

tail:
    ActorSelf::Update();
}

/* Explodes: palette 7, animation 1. */
void JetpackRocket::Launch()
{
    triggered = 1;
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    palette = 7;
    RestartAnim(1);
}

/* Slot 4: once the hit points run out, it is shot down: palette 4,
 * animation 2. */
void JetpackRocket::Damage(s32 amount)
{
    if ((hp -= amount) > 0) {
        return;
    }

    triggered = 1;
    hit = 1;
    gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
    palette = 4;
    RestartAnim(2);
}

/* CreateJetpackRocket: built at depth 0xFA00 with its target height
 * (`y`, within +-0x3F00) and swing centre (`x`, within +-0x8000), it
 * comes down to the height in 0xC6 frames. */
JetpackRocket::JetpackRocket(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : HpActor(rec, x, 0xFA00, z, 1)
{
    LIMIT_MAX(y, 0x3F00);
    LIMIT_MIN(y, -0x3F00);
    limitY = y;
    LIMIT_MAX(this->x, 0x8000);
    LIMIT_MIN(this->x, -0x8000);
    originX = this->x;
    stepY = (limitY - 0xFA00) / 0xC6;
    hit = 0;
    triggered = 0;
    gAudioContext->PlaySfx(SFX_JETPACK_ROCKET, 0x100);
}

/* Slot 5: exploded or shot. */
s32 JetpackRocket::IsUnshootable()
{
    return triggered;
}

/* Slot 2: a ring of record 0x1F the player touches counts as passed
 * (JetpackPlayer::PassRing, with the ring's centre: `x` less the record's
 * offset), and plays its cue once; then ActorSelf's update. */
void JetpackRing::Update()
{
    if ((u8)record->index == 0x1F && (u8)IsTouchingPlayer(this)) {
        JetpackPlayer *player = Player();

        player->PassRing(x - record->spawnX, y);
        if (cued == 0) {
            cued = 1;
            gAudioContext->PlaySfx(SFX_JETPACK_RING, 0x100);
        }
    }

    ActorSelf::Update();
}
