#include "actor_self.hpp"
#include "audio.hpp"
#include "yeti.hpp"
#include "vehicle.hpp"
#include "cell_anim.hpp"
#include "actor_bg.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "util.h"
#include "actor.h"
#include "globals.h"
}

using Engine::Actor::ActorBg;
using Engine::Gfx::CellAnim;
using Game::Entities::AnimPart;
using Game::Vehicles::Polar::Yeti;

/* The `gYeti`-rooted position-tracking object with tier-
 * threshold sound cues, already documented in docs/rom_map.md ("A
 * fourth vtable table, a third RAM-struct family" onward): an
 * accumulate-then-clamp-at-0xA000 pair on `gYetiPosition`/`gYetiDistance`
 * driven from `gYetiChargeParams` (struct YetiCharge records, indexed
 * by `gYetiParamsIndex`), branching to different
 * `PlaySfx`/`PlayAmbientSfx` tier cues depending on the current "tier"
 * value read from the object's own `+8` field, and a shared
 * kind/anim-reset "transition" tail gated on the object's `+0x12` done
 * flag. `YetiStateChase` and `YetiStateCharge` are two of `gYetiStateFuncs`'s
 * four vtable slots operating on this object (see
 * docs/matching/archive/issue-54-actor-d3a8.md). */

/* Re-derives `gYetiPosition`/`gYetiDistance` (a small per-frame ease
 * toward a `CellAnim::GetDistance()`-driven target, with a `+0x99` nudge on the
 * "already settled" branch), clamps `gYetiDistance` to `0xA000`, then - only
 * while `gYetiDistance <= 0x4FFF` - fires a tier-keyed cue off the object's
 * own `+8`-field-derived "tier": tiers `0xc`/`0x1c` call the
 * `PlaySfx`-sibling `PlayAmbientSfx` (id `0x3E8`, volume `0x100`, plus a
 * byte flag passed via the stack) followed by `ActorBg::Shake(0x200)`;
 * tiers `0xd`/`0x1d` call `ActorBg::Shake(0x100)` alone. Finally, while the
 * object's `+0x12` done flag is set, runs a two-stage
 * `CellAnim::GetSpeed()`/`RandRange()`-gated check against
 * `chargeParams[paramsIndex]`'s `fastChance`/`slowChance` rolls to
 * decide whether to fire the kind-1/anim-reset transition (plus a sound
 * cue while `gYetiDistance <= 0x7800`). */
void Yeti::StateChase()
{
    if (CellAnim::GetSpeed() == 0x24) {
        position = INT_TO_Q8(CellAnim::GetDistance()) - distance;
    } else {
        position += 0x99;
        distance = INT_TO_Q8(CellAnim::GetDistance()) - position;
    }

    if (distance > INT_TO_Q8(160)) {
        distance = INT_TO_Q8(160);
        position = INT_TO_Q8(CellAnim::GetDistance()) - distance;
    }

    s32 tier = Q8_TO_INT(anim->animTime);

    if (distance <= INT_TO_Q8(80) - 1) {
        if (tier == 0xc) {
            gAudioContext->PlayAmbientSfx(SFX_YETI_STOMP_1, 0x3E8, 0x100, true);
            ActorBg::Shake(INT_TO_Q8(2));
        } else if (tier == 0x1c) {
            gAudioContext->PlayAmbientSfx(SFX_YETI_STOMP_2, 0x3E8, 0x100, true);
            ActorBg::Shake(INT_TO_Q8(2));
        } else if (tier == 0xd || tier == 0x1d) {
            ActorBg::Shake(INT_TO_Q8(1));
        }
    }

    if (anim->animDone != 0) {
        if (distance > INT_TO_Q8(90)) {
            goto do_transition;
        }

        if (CellAnim::GetSpeed() > 0x24 &&
            RandRange(0x100) < chargeParams[paramsIndex].fastChance) {
            goto do_transition;
        }

        if (CellAnim::GetSpeed() > 0x24) {
            goto end_transition;
        }
        if (RandRange(0x100) >= chargeParams[paramsIndex].slowChance) {
            goto end_transition;
        }

    do_transition:
        state = YETI_STATE_CHARGE;
        anim->RestartAnim(1);

        if (distance <= INT_TO_Q8(120)) {
            gAudioContext->PlaySfx(SFX_YETI_CHASE, 0x100);
        }
    end_transition:;
    }
}

/* Sibling to `YetiStateChase` above, on the same object: instead of the
 * ease/settle pair, directly nudges `gYetiPosition` by
 * `chargeParams[paramsIndex].step` before
 * re-deriving `gYetiDistance`/clamping. The tier cues use plain `PlaySfx`
 * (ids `0x3f`/`0x40`) instead of `PlayAmbientSfx`, keyed off tiers
 * `0xb`/`0x1b` (with `0xc`/`0x1c` sharing the `ActorBg::Shake(0x100)`-only
 * branch this time). The done-flag tail is a plain unconditional
 * kind-0/anim-reset (no threshold gate, no sound cue) - the counterpart
 * "settle" step to `YetiStateChase`'s tier-1 "arm" step. */
void Yeti::StateCharge()
{
    position += chargeParams[paramsIndex].step;
    distance = INT_TO_Q8(CellAnim::GetDistance()) - position;

    if (distance > INT_TO_Q8(160)) {
        distance = INT_TO_Q8(160);
        position = INT_TO_Q8(CellAnim::GetDistance()) - distance;
    }

    s32 tier = Q8_TO_INT(anim->animTime);

    if (distance <= INT_TO_Q8(80) - 1) {
        /* The two stomp cues share their ActorBg::Shake(0x200) (the
         * ROM's one cross-jumped tail). */
        if (tier == 0xb || tier == 0x1b) {
            if (tier == 0xb)
                gAudioContext->PlaySfx(SFX_YETI_STOMP_1, 0x100);
            else
                gAudioContext->PlaySfx(SFX_YETI_STOMP_2, 0x100);
            ActorBg::Shake(INT_TO_Q8(2));
        } else if (tier == 0xc || tier == 0x1c) {
            ActorBg::Shake(INT_TO_Q8(1));
        }
    }

    AnimPart *bc = anim;

    if (bc->animDone != 0) {
        state = YETI_STATE_CHASE;
        bc->RestartAnim(0);
    }
}
