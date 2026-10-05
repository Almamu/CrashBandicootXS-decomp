#include "core.h"
#include "memory.h"
#include "util.h"

/* The `gYeti`-rooted position-tracking object with tier-
 * threshold sound cues, already documented in docs/rom_map.md ("A
 * fourth vtable table, a third RAM-struct family" onward): an
 * accumulate-then-clamp-at-0xA000 pair on `gYetiPosition`/`030014CC`
 * driven from `gYetiChargeParams` (a table of `{s32,s32,s32}`,
 * stride 0xc, indexed by `gYetiParamsIndex`), branching to different
 * `PlaySfx`/`PlayAmbientSfx` tier cues depending on the current "tier"
 * value read from the object's own `+8` field, and a shared
 * kind/anim-reset "transition" tail gated on the object's `+0x12` done
 * flag. `YetiStateChase` and `YetiStateCharge` are two of `gYetiStateFuncs`'s
 * four vtable slots operating on this object (see
 * docs/matching/issue-54-actor-d3a8.md). */

extern s32 GetCellAnimSpeed(void);
extern s32 GetCellAnimDistance(void);
extern s32 gYetiDistance;
extern s32 gYetiPosition;
extern void *gYeti;
extern void *gAudioContext;
extern s32 PlayAmbientSfx(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void ShakeActorBg(s32 arg0);
extern s32 gYetiParamsIndex;
extern u8 gYetiChargeParams[];
extern s32 gYetiState;
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* Re-derives `gYetiPosition`/`030014CC` (a small per-frame ease
 * toward a `GetCellAnimDistance()`-driven target, with a `+0x99` nudge on the
 * "already settled" branch), clamps `030014CC` to `0xA000`, then - only
 * while `030014CC <= 0x4FFF` - fires a tier-keyed cue off the object's
 * own `+8`-field-derived "tier": tiers `0xc`/`0x1c` call the
 * `PlaySfx`-sibling `PlayAmbientSfx` (id `0x3E8`, volume `0x100`, plus a
 * byte flag passed via the stack) followed by `ShakeActorBg(0x200)`;
 * tiers `0xd`/`0x1d` call `ShakeActorBg(0x100)` alone. Finally, while the
 * object's `+0x12` done flag is set, runs a two-stage
 * `GetCellAnimSpeed()`/`RandRange()`-gated check against
 * `gYetiChargeParams[gYetiParamsIndex]`'s `+4`/`+8` thresholds to
 * decide whether to fire the kind-1/anim-reset transition (plus a sound
 * cue while `030014CC <= 0x7800`). */
void YetiStateChase(void)
{
    u8 dummyStack;

    if (GetCellAnimSpeed() == 0x24) {
        gYetiPosition = (GetCellAnimDistance() << 8) - gYetiDistance;
    } else {
        gYetiPosition += 0x99;
        gYetiDistance = (GetCellAnimDistance() << 8) - gYetiPosition;
    }

    if (gYetiDistance > 0xa000) {
        gYetiDistance = 0xa000;
        gYetiPosition = (GetCellAnimDistance() << 8) - gYetiDistance;
    }

    {
        s32 tier = *(s32 *)((u8 *)gYeti + 8) >> 8;

        if (gYetiDistance <= 0x4FFF) {
            if (tier == 0xc) {
                void *a0 = gAudioContext;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                register u8 *stackPtr asm("r4") = &dummyStack;
                register u8 one asm("r1") = 1;

                *stackPtr = one;
                PlayAmbientSfx(a0, 0x3f, a2, a3);
                ShakeActorBg(0x200);
            } else if (tier == 0x1c) {
                void *a0 = gAudioContext;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                register u8 *stackPtr asm("r4") = &dummyStack;
                register u8 one asm("r1") = 1;

                *stackPtr = one;
                PlayAmbientSfx(a0, 0x40, a2, a3);
                ShakeActorBg(0x200);
            } else if (tier == 0xd || tier == 0x1d) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        if (((u8 *)gYeti)[0x12] != 0) {
            if (gYetiDistance > 0x5A00) {
                goto do_transition;
            }

            if (GetCellAnimSpeed() > 0x24) {
                s32 v = (u16)RandRange(0x100);
                u8 *tableBase = gYetiChargeParams;
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
                u8 *tableBase = gYetiChargeParams;
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
                u8 *bc = gYeti;

                *(s32 *)(bc + 0xc) = 1;
                {
                    u16 anim = *(u16 *)(*(u8 **)bc + 0xc);
                    register u8 zero1 asm("r2") = 0;
                    register s32 zero2 asm("r3") = 0;

                    *(u16 *)(bc + 0x10) = anim;
                    bc[0x12] = zero1;
                    *(s32 *)(bc + 8) = zero2;
                }
            }

            if (gYetiDistance <= 0x7800) {
                PlaySfx(gAudioContext, 0x20, 0x100);
            }
        end_transition:
            ;
        }
    }
}

/* Sibling to `YetiStateChase` above, on the same object: instead of the
 * ease/settle pair, directly nudges `gYetiPosition` by
 * `gYetiChargeParams[gYetiParamsIndex]`'s own `+0` field before
 * re-deriving `030014CC`/clamping. The tier cues use plain `PlaySfx`
 * (ids `0x3f`/`0x40`) instead of `PlayAmbientSfx`, keyed off tiers
 * `0xb`/`0x1b` (with `0xc`/`0x1c` sharing the `ShakeActorBg(0x100)`-only
 * branch this time). The done-flag tail is a plain unconditional
 * kind-0/anim-reset (no threshold gate, no sound cue) - the counterpart
 * "settle" step to `YetiStateChase`'s tier-1 "arm" step. */
void YetiStateCharge(void)
{
    s32 *c8 = &gYetiPosition;
    u8 *table = gYetiChargeParams;
    s32 idx = gYetiParamsIndex;

    *c8 += *(s32 *)(table + idx * 0xc);
    gYetiDistance = (GetCellAnimDistance() << 8) - gYetiPosition;

    if (gYetiDistance > 0xa000) {
        gYetiDistance = 0xa000;
        gYetiPosition = (GetCellAnimDistance() << 8) - gYetiDistance;
    }

    {
        s32 tier = *(s32 *)((u8 *)gYeti + 8) >> 8;

        if (gYetiDistance <= 0x4FFF) {
            if (tier == 0xb) {
                PlaySfx(gAudioContext, 0x3f, 0x100);
                asm volatile("" ::: "memory");
                ShakeActorBg(0x200);
            } else if (tier == 0x1b) {
                PlaySfx(gAudioContext, 0x40, 0x100);
                ShakeActorBg(0x200);
            } else if (tier == 0xc || tier == 0x1c) {
                ShakeActorBg(0x100);
            }
        }
    }

    {
        u8 *bc = gYeti;

        if (bc[0x12] != 0) {
            s32 *d0 = &gYetiState;
            register s32 zero asm("r1") = 0;

            *d0 = zero;
            *(s32 *)(bc + 0xc) = zero;
            {
                u16 anim = *(u16 *)(*(u8 **)bc);
                register u8 zero2 asm("r2") = 0;

                *(u16 *)(bc + 0x10) = anim;
                bc[0x12] = zero2;
                *(s32 *)(bc + 8) = zero;
            }
        }
    }
}

asm(".align 2, 0");
