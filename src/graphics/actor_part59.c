#include "core.h"
#include "memory.h"

/* The `gUnknown_030014BC`-rooted position-tracking object with tier-
 * threshold sound cues, already documented in docs/rom_map.md ("A
 * fourth vtable table, a third RAM-struct family" onward): an
 * accumulate-then-clamp-at-0xA000 pair on `gUnknown_030014C8`/`030014CC`
 * driven from `gStaticData_0817A7F8` (a table of `{s32,s32,s32}`,
 * stride 0xc, indexed by `gUnknown_030014D4`), branching to different
 * `PlaySfx`/`sub_80019F8` tier cues depending on the current "tier"
 * value read from the object's own `+8` field, and a shared
 * kind/anim-reset "transition" tail gated on the object's `+0x12` done
 * flag. `sub_802DB2C` and `sub_802DCC0` are two of `gStaticData_0817A840`'s
 * four vtable slots operating on this object (see
 * docs/matching/issue-54-actor-d3a8.md). */

extern s32 sub_8029B98(void);
extern s32 sub_8029B2C(void);
extern s32 gUnknown_030014CC;
extern s32 gUnknown_030014C8;
extern void *gUnknown_030014BC;
extern void *gUnknown_030012BC;
extern s32 sub_80019F8(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_8029E28(s32 arg0);
extern s32 gUnknown_030014D4;
extern u8 gStaticData_0817A7F8[];
extern s32 sub_8000E1C(s32 arg0);
extern s32 gUnknown_030014D0;
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* Re-derives `gUnknown_030014C8`/`030014CC` (a small per-frame ease
 * toward a `sub_8029B2C()`-driven target, with a `+0x99` nudge on the
 * "already settled" branch), clamps `030014CC` to `0xA000`, then - only
 * while `030014CC <= 0x4FFF` - fires a tier-keyed cue off the object's
 * own `+8`-field-derived "tier": tiers `0xc`/`0x1c` call the
 * `PlaySfx`-sibling `sub_80019F8` (id `0x3E8`, volume `0x100`, plus a
 * byte flag passed via the stack) followed by `sub_8029E28(0x200)`;
 * tiers `0xd`/`0x1d` call `sub_8029E28(0x100)` alone. Finally, while the
 * object's `+0x12` done flag is set, runs a two-stage
 * `sub_8029B98()`/`sub_8000E1C()`-gated check against
 * `gStaticData_0817A7F8[gUnknown_030014D4]`'s `+4`/`+8` thresholds to
 * decide whether to fire the kind-1/anim-reset transition (plus a sound
 * cue while `030014CC <= 0x7800`). */
void sub_802DB2C(void)
{
    u8 dummyStack;

    if (sub_8029B98() == 0x24) {
        gUnknown_030014C8 = (sub_8029B2C() << 8) - gUnknown_030014CC;
    } else {
        gUnknown_030014C8 += 0x99;
        gUnknown_030014CC = (sub_8029B2C() << 8) - gUnknown_030014C8;
    }

    if (gUnknown_030014CC > 0xa000) {
        gUnknown_030014CC = 0xa000;
        gUnknown_030014C8 = (sub_8029B2C() << 8) - gUnknown_030014CC;
    }

    {
        s32 tier = *(s32 *)((u8 *)gUnknown_030014BC + 8) >> 8;

        if (gUnknown_030014CC <= 0x4FFF) {
            if (tier == 0xc) {
                void *a0 = gUnknown_030012BC;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                register u8 *stackPtr asm("r4") = &dummyStack;
                register u8 one asm("r1") = 1;

                *stackPtr = one;
                sub_80019F8(a0, 0x3f, a2, a3);
                sub_8029E28(0x200);
            } else if (tier == 0x1c) {
                void *a0 = gUnknown_030012BC;
                s32 a2 = 0x3E8;
                s32 a3 = 0x100;
                register u8 *stackPtr asm("r4") = &dummyStack;
                register u8 one asm("r1") = 1;

                *stackPtr = one;
                sub_80019F8(a0, 0x40, a2, a3);
                sub_8029E28(0x200);
            } else if (tier == 0xd || tier == 0x1d) {
                sub_8029E28(0x100);
            }
        }
    }

    {
        if (((u8 *)gUnknown_030014BC)[0x12] != 0) {
            if (gUnknown_030014CC > 0x5A00) {
                goto do_transition;
            }

            if (sub_8029B98() > 0x24) {
                s32 v = (u16)sub_8000E1C(0x100);
                u8 *tableBase = gStaticData_0817A7F8;
                s32 offset = gUnknown_030014D4 * 0xc;
                u8 *tablePlus4 = tableBase + 4;
                s32 threshold = *(s32 *)(tablePlus4 + offset);

                if (v < threshold) {
                    goto do_transition;
                }
            }

            if (sub_8029B98() > 0x24) {
                goto end_transition;
            }
            {
                s32 v = (u16)sub_8000E1C(0x100);
                u8 *tableBase = gStaticData_0817A7F8;
                s32 offset = gUnknown_030014D4 * 0xc;
                u8 *tablePlus8 = tableBase + 8;
                s32 threshold = *(s32 *)(tablePlus8 + offset);

                if (v >= threshold) {
                    goto end_transition;
                }
            }

        do_transition:
            gUnknown_030014D0 = 1;
            {
                u8 *bc = gUnknown_030014BC;

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

            if (gUnknown_030014CC <= 0x7800) {
                PlaySfx(gUnknown_030012BC, 0x20, 0x100);
            }
        end_transition:
            ;
        }
    }
}

/* Sibling to `sub_802DB2C` above, on the same object: instead of the
 * ease/settle pair, directly nudges `gUnknown_030014C8` by
 * `gStaticData_0817A7F8[gUnknown_030014D4]`'s own `+0` field before
 * re-deriving `030014CC`/clamping. The tier cues use plain `PlaySfx`
 * (ids `0x3f`/`0x40`) instead of `sub_80019F8`, keyed off tiers
 * `0xb`/`0x1b` (with `0xc`/`0x1c` sharing the `sub_8029E28(0x100)`-only
 * branch this time). The done-flag tail is a plain unconditional
 * kind-0/anim-reset (no threshold gate, no sound cue) - the counterpart
 * "settle" step to `sub_802DB2C`'s tier-1 "arm" step. */
void sub_802DCC0(void)
{
    s32 *c8 = &gUnknown_030014C8;
    u8 *table = gStaticData_0817A7F8;
    s32 idx = gUnknown_030014D4;

    *c8 += *(s32 *)(table + idx * 0xc);
    gUnknown_030014CC = (sub_8029B2C() << 8) - gUnknown_030014C8;

    if (gUnknown_030014CC > 0xa000) {
        gUnknown_030014CC = 0xa000;
        gUnknown_030014C8 = (sub_8029B2C() << 8) - gUnknown_030014CC;
    }

    {
        s32 tier = *(s32 *)((u8 *)gUnknown_030014BC + 8) >> 8;

        if (gUnknown_030014CC <= 0x4FFF) {
            if (tier == 0xb) {
                PlaySfx(gUnknown_030012BC, 0x3f, 0x100);
                asm volatile("" ::: "memory");
                sub_8029E28(0x200);
            } else if (tier == 0x1b) {
                PlaySfx(gUnknown_030012BC, 0x40, 0x100);
                sub_8029E28(0x200);
            } else if (tier == 0xc || tier == 0x1c) {
                sub_8029E28(0x100);
            }
        }
    }

    {
        u8 *bc = gUnknown_030014BC;

        if (bc[0x12] != 0) {
            s32 *d0 = &gUnknown_030014D0;
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
