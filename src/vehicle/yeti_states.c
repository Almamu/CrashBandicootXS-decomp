#include "core.h"
#include "math_util.h"
#include "match.h"
#include "memory.h"
#include "util.h"
#include "audio.h"
#include "actor.h"
#include "vehicle.h"
#include "globals.h"

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

/* codegen: PlayAmbientSfx takes a fifth argument, a one-byte struct on
 * the stack (audio.h). YetiStateChase stores the byte at sp itself
 * (`mov r4, sp; mov r1, #1; strb r1, [r4]`); passing a `struct byte_arg`
 * schedules the `mov r1, #1` before the `mov r4, sp`.
 * docs/headers_plan.md */
extern void PlayAmbientSfx_4(void *self, s32 id, s32 frameOffset,
                             s32 volumeMul) asm("PlayAmbientSfx");

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
void YetiStateChase(void)
{
    u8 dummyStack;

    if (GetCellAnimSpeed() == 0x24) {
        gYetiPosition = INT_TO_Q8(GetCellAnimDistance()) - gYetiDistance;
    } else {
        gYetiPosition += 0x99;
        gYetiDistance = INT_TO_Q8(GetCellAnimDistance()) - gYetiPosition;
    }

    if (gYetiDistance > 0xa000) {
        gYetiDistance = 0xa000;
        gYetiPosition = INT_TO_Q8(GetCellAnimDistance()) - gYetiDistance;
    }

    {
        s32 tier = Q8_TO_INT(gYeti->animTime);

        if (gYetiDistance <= 0x4FFF) {
            if (tier == 0xc) {
                void *a0 = gAudioContext;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                u8 *stackPtr = &dummyStack;

                *stackPtr = 1;
                PlayAmbientSfx_4(a0, 0x3f, a2, a3);
                ShakeActorBg(0x200);
            } else if (tier == 0x1c) {
                void *a0 = gAudioContext;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                u8 *stackPtr = &dummyStack;

                *stackPtr = 1;
                PlayAmbientSfx_4(a0, 0x40, a2, a3);
                ShakeActorBg(0x200);
            } else if (tier == 0xd || tier == 0x1d) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        if (gYeti->animDone != 0) {
            if (gYetiDistance > 0x5A00) {
                goto do_transition;
            }

            if (GetCellAnimSpeed() > 0x24) {
                s32 v = (u16)RandRange(0x100);
                u8 *tableBase = (u8 *)gYetiChargeParams;
                s32 offset = gYetiParamsIndex * 0xc;
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
                u8 *tableBase = (u8 *)gYetiChargeParams;
                s32 offset = gYetiParamsIndex * 0xc;
                u8 *tablePlus8 = tableBase + 8;
                s32 threshold = *(s32 *)(tablePlus8 + offset);

                if (v >= threshold) {
                    goto end_transition;
                }
            }

        do_transition:
            gYetiState = 1;
            {
                struct actor_self *bc = gYeti;

                bc->animIndex = 1;
                bc->animTimer = bc->anims[1].duration;
                bc->animDone = 0;
                bc->animTime = 0;
            }

            if (gYetiDistance <= 0x7800) {
                PlaySfx(gAudioContext, SFX_YETI_CHASE, 0x100);
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
void YetiStateCharge(void)
{
    s32 *c8 = &gYetiPosition;
    u8 *table = (u8 *)gYetiChargeParams;
    s32 idx = gYetiParamsIndex;

    *c8 += *(s32 *)(table + idx * 0xc);
    gYetiDistance = INT_TO_Q8(GetCellAnimDistance()) - gYetiPosition;

    if (gYetiDistance > 0xa000) {
        gYetiDistance = 0xa000;
        gYetiPosition = INT_TO_Q8(GetCellAnimDistance()) - gYetiDistance;
    }

    {
        s32 tier = Q8_TO_INT(gYeti->animTime);

        if (gYetiDistance <= 0x4FFF) {
            if (tier == 0xb) {
                PlaySfx(gAudioContext, SFX_YETI_STOMP_1, 0x100);
                /* The ROM cross-jumps only the ShakeActorBg(0x200) tail
                 * of the two cues; without the barrier the PlaySfx call
                 * is shared too (as YetiStateChase's is). */
                MATCH_BARRIER();
                ShakeActorBg(0x200);
            } else if (tier == 0x1b) {
                PlaySfx(gAudioContext, SFX_YETI_STOMP_2, 0x100);
                ShakeActorBg(0x200);
            } else if (tier == 0xc || tier == 0x1c) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        struct actor_self *bc = gYeti;

        if (bc->animDone != 0) {
            gYetiState = 0;
            bc->animIndex = 0;
            bc->animTimer = bc->anims[0].duration;
            bc->animDone = 0;
            bc->animTime = 0;
        }
    }
}
