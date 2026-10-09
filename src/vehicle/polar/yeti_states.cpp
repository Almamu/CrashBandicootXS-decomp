#include "actor_self.hpp"
#include "audio.hpp"
#include "yeti.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "util.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "globals.h"
}

/* The `gYeti`-rooted position-tracking object with tier-
 * threshold sound cues, already documented in docs/rom_map.md ("A
 * fourth vtable table, a third RAM-struct family" onward): an
 * accumulate-then-clamp-at-0xA000 pair on `gYetiPosition`/`gYetiDistance`
 * driven from `gYetiChargeParams` (a table of `{s32,s32,s32}`,
 * stride 0xc, indexed by `gYetiParamsIndex`), branching to different
 * `PlaySfx`/`PlayAmbientSfx` tier cues depending on the current "tier"
 * value read from the object's own `+8` field, and a shared
 * kind/anim-reset "transition" tail gated on the object's `+0x12` done
 * flag. `YetiStateChase` and `YetiStateCharge` are two of `gYetiStateFuncs`'s
 * four vtable slots operating on this object (see
 * docs/matching/archive/issue-54-actor-d3a8.md). */

/* Re-derives `gYetiPosition`/`gYetiDistance` (a small per-frame ease
 * toward a `GetCellAnimDistance()`-driven target, with a `+0x99` nudge on the
 * "already settled" branch), clamps `gYetiDistance` to `0xA000`, then - only
 * while `gYetiDistance <= 0x4FFF` - fires a tier-keyed cue off the object's
 * own `+8`-field-derived "tier": tiers `0xc`/`0x1c` call the
 * `PlaySfx`-sibling `PlayAmbientSfx` (id `0x3E8`, volume `0x100`, plus a
 * byte flag passed via the stack) followed by `ShakeActorBg(0x200)`;
 * tiers `0xd`/`0x1d` call `ShakeActorBg(0x100)` alone. Finally, while the
 * object's `+0x12` done flag is set, runs a two-stage
 * `GetCellAnimSpeed()`/`RandRange()`-gated check against
 * `gYetiChargeParams[gYetiParamsIndex]`'s `+4`/`+8` thresholds to
 * decide whether to fire the kind-1/anim-reset transition (plus a sound
 * cue while `gYetiDistance <= 0x7800`). */
void Yeti::StateChase()
{
    if (GetCellAnimSpeed() == 0x24) {
        position = INT_TO_Q8(GetCellAnimDistance()) - distance;
    } else {
        position += 0x99;
        distance = INT_TO_Q8(GetCellAnimDistance()) - position;
    }

    if (distance > 0xa000) {
        distance = 0xa000;
        position = INT_TO_Q8(GetCellAnimDistance()) - distance;
    }

    {
        s32 tier = Q8_TO_INT(anim->animTime);

        if (distance <= 0x4FFF) {
            if (tier == 0xc) {
                gAudioContext->PlayAmbientSfx(0x3f, 0x3E8, 0x100, true);
                ShakeActorBg(0x200);
            } else if (tier == 0x1c) {
                gAudioContext->PlayAmbientSfx(0x40, 0x3E8, 0x100, true);
                ShakeActorBg(0x200);
            } else if (tier == 0xd || tier == 0x1d) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        if (anim->animDone != 0) {
            if (distance > 0x5A00) {
                goto do_transition;
            }

            if (GetCellAnimSpeed() > 0x24) {
                s32 v = (u16)RandRange(0x100);
                u8 *tableBase = (u8 *)chargeParams;
                s32 offset = paramsIndex * 0xc;
                u8 *tablePlus4 = tableBase + 4;
                s32 threshold = *(s32 *)(tablePlus4 + offset);

                if (v < threshold) {
                    goto do_transition;
                }
            }

            if (GetCellAnimSpeed() > 0x24) {
                goto end_transition;
            }
            {
                s32 v = (u16)RandRange(0x100);
                u8 *tableBase = (u8 *)chargeParams;
                s32 offset = paramsIndex * 0xc;
                u8 *tablePlus8 = tableBase + 8;
                s32 threshold = *(s32 *)(tablePlus8 + offset);

                if (v >= threshold) {
                    goto end_transition;
                }
            }

        do_transition:
            state = 1;
            {
                anim->RestartAnim(1);
            }

            if (distance <= 0x7800) {
                gAudioContext->PlaySfx(SFX_YETI_CHASE, 0x100);
            }
        end_transition:;
        }
    }
}

/* Sibling to `YetiStateChase` above, on the same object: instead of the
 * ease/settle pair, directly nudges `gYetiPosition` by
 * `gYetiChargeParams[gYetiParamsIndex]`'s own `+0` field before
 * re-deriving `gYetiDistance`/clamping. The tier cues use plain `PlaySfx`
 * (ids `0x3f`/`0x40`) instead of `PlayAmbientSfx`, keyed off tiers
 * `0xb`/`0x1b` (with `0xc`/`0x1c` sharing the `ShakeActorBg(0x100)`-only
 * branch this time). The done-flag tail is a plain unconditional
 * kind-0/anim-reset (no threshold gate, no sound cue) - the counterpart
 * "settle" step to `YetiStateChase`'s tier-1 "arm" step. */
void Yeti::StateCharge()
{
    s32 *c8 = &position;
    u8 *table = (u8 *)chargeParams;
    s32 idx = paramsIndex;

    *c8 += *(s32 *)(table + idx * 0xc);
    distance = INT_TO_Q8(GetCellAnimDistance()) - position;

    if (distance > 0xa000) {
        distance = 0xa000;
        position = INT_TO_Q8(GetCellAnimDistance()) - distance;
    }

    {
        s32 tier = Q8_TO_INT(anim->animTime);

        if (distance <= 0x4FFF) {
            /* The two stomp cues share their ShakeActorBg(0x200) (the
             * ROM's one cross-jumped tail). */
            if (tier == 0xb || tier == 0x1b) {
                if (tier == 0xb)
                    gAudioContext->PlaySfx(SFX_YETI_STOMP_1, 0x100);
                else
                    gAudioContext->PlaySfx(SFX_YETI_STOMP_2, 0x100);
                ShakeActorBg(0x200);
            } else if (tier == 0xc || tier == 0x1c) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        AnimPart *bc = anim;

        if (bc->animDone != 0) {
            state = 0;
            bc->RestartAnim(0);
        }
    }
}
