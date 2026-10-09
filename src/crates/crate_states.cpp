#include "crate.hpp"
#include "spawners.hpp"
#include "crate_list.hpp"
#include "part_list.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include "pickups.h"
#include "util.h"
#include "player.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "sprite_bank.h"
}

/* The crate's per-state ticks (#664, include/crate.hpp, part 7g): a
 * breaking crate's (FinishBroken), the TNT countdown, the slot crate's and
 * the fall's, ROM 0x0800F798-0x0800FDC8: the end of crate_break.cpp's
 * "physics/collision" cluster, split out of it (#768). Built with
 * old_agbcp, as the C was with old_agbcc. */

/* Entry `i` of the player's ring of touched crates (GetPlayerListEntry
 * inlined): none while the ring is locked (`ctrlMode`). */
static inline Crate *RingAt(Player *p, s32 i)
{
    if (p->ctrlMode == 0 && (i <= 4 || i < p->listCount))
        return (Crate *)p->list[i];
    return 0;
}

/* A committed (breaking) crate's tick: an explosive one blasts at steps
 * 3 and 6 of its animation. When the animation ends, the crate leaves its
 * stack and, unless it is a checkpoint crate, is marked gone (and removed
 * from the player's ring of touched crates, which empties). */
void Crate::FinishBroken()
{
    if (gCrateKindExplosive[kind] && stepTimer == 0) {
        if (frame == 3)
            BlastNearby(0x14);
        else if (frame == 6)
            BlastNearby(0x28);
    }

    if (animDone) {
        Crate *prev = GetBelow();
        Crate *next = GetAbove();
        s32 i;

        if (prev != 0 && next != 0) {
            next->SetBelow(prev);
            prev->SetAbove(next);
        } else if (next != 0) {
            next->SetBelow(0);
        } else if (prev != 0) {
            prev->SetAbove(0);
        }

        if (kind == CRATE_KIND_CHECKPOINT)
            return;
        gCrateListChanged = 1;
        MarkGone();
        i = 0;
        if (i < gPlayer->listCount) {
            do {
                if (RingAt(gPlayer, i) == this)
                    gPlayer->listCount = 0;
                i++;
            } while (i < gPlayer->listCount);
        }
    } else if (kind != CRATE_KIND_CHECKPOINT) {
        gCrateListChanged = 1;
    }
}

/* A lit TNT crate's countdown, each time its timer runs out: 3, 2 (each
 * with its animation and tick), then it explodes (if idle). */
void Crate::UpdateTntCountdown()
{
    u8 k;

    if (timer != 0)
        return;

    k = kind;
    switch (k) {
    case CRATE_KIND_TNT_LIT_3:
        SetTag(0x13);
        gAudioContext->PlaySfx(SFX_TNT_TICK, 0x100);
        kind = CRATE_KIND_TNT_LIT_2;
        timer = 0x3c;
        break;
    case CRATE_KIND_TNT_LIT_2:
        SetTag(0x12);
        gAudioContext->PlaySfx(SFX_TNT_TICK, 0x100);
        kind = CRATE_KIND_TNT_LIT_1;
        timer = 0x3c;
        break;
    case CRATE_KIND_TNT_LIT_1:
        if ((state & CRATE_STATE_MASK) == 0)
            Explode(0);
        break;
    }
}

/* The slot crate's tick (slotState: see crate.h's CRATE_SLOT_*). An idle
 * one (stage 0) starts at stage 1 when the player comes within 0x4F by
 * 0x3F pixels. Each time its timer runs out, it turns to its next face:
 * once spinning, the faces it may stop on (paramA's mask), counting down
 * the stage's spins and moving on a stage after them (after stage 3 it
 * turns to iron); else its idle animations. The timer is reloaded from
 * gSlotCrateTimers by stage.
 *
 * The phase test (`w1`), the loop (`lw`) and the `0x38` switch (`w2`)
 * each have their own local; `lw`'s phase is cleared and set in place
 * (`lw &= ...; lw |= nx`), which keeps it in r1. The count update is
 * written as separate in-place
 * steps on a fresh local (`t = (r - 1) << 24; cw &= 0xc7; t >>= 21;
 * cw |= t`), which ties the `& 0xc7` to the reloaded word's register and
 * the shift to `t`'s, as the ROM does; a single `(w & 0xc7) | (t << 3)`
 * expression left 7 halfwords off. */
void Crate::UpdateSlot()
{
    s32 w;
    s32 ph0;
    s32 w1;

    w = slotState;
    if (!(w & CRATE_SLOT_STAGE_MASK)) {
        Player *pl = gPlayer;
        s32 d;

        d = Q8_TO_INT(pl->x);
        d -= Q8_TO_INT(x);
        MAKE_ABS(d);
        if (d <= 0x4f) {
            d = Q8_TO_INT(pl->y);
            d -= Q8_TO_INT(y);
            MAKE_ABS(d);
            if (d <= 0x3f) {
                w &= CRATE_SLOT_CLEAR_STAGE;
                w |= 0x40;
                w &= CRATE_SLOT_CLEAR_SPINS;
                w |= 0x10;
                slotState = w;
            }
        }
    }
    if (timer != 0)
        return;
    ph0 = slotState & CRATE_SLOT_PHASE_MASK;
    ph0 &= CRATE_SLOT_PHASE_STARTED;
    w1 = slotState;
    if (ph0 && tag == 8) {
        s32 done = 0;

        do {
            s32 ph;
            s32 nx;
            s32 lw;

            lw = slotState;
            nx = ((lw & CRATE_SLOT_PHASE_MASK) + 1) & 3;
            ph = nx;
            lw &= CRATE_SLOT_CLEAR_PHASE;
            lw |= nx;
            slotState = lw;
            switch (ph) {
            case 0:
                SetTag(7);
                if (slotState & CRATE_SLOT_STAGE_MASK) {
                    u8 r = GetSlotSpins();

                    if (r != 0) {
                        u32 t = (r - 1) << 24;
                        s32 cw = slotState;

                        cw &= CRATE_SLOT_CLEAR_SPINS;
                        t >>= 21;
                        cw |= t;
                        slotState = cw;
                    }
                    w = slotState;
                    if (!(w & CRATE_SLOT_SPINS_MASK)) {
                        s32 w2 = (w & CRATE_SLOT_CLEAR_SPINS) | 0x10;

                        slotState = w2;
                        switch (
                            (s32)((u32)(w2 & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT)) {
                        case 1:
                            slotState = (w2 & CRATE_SLOT_CLEAR_STAGE) | 0x80;
                            break;
                        case 2:
                            slotState = (w2 & CRATE_SLOT_CLEAR_STAGE) | 0xc0;
                            break;
                        case 3:
                            SetTag(0x20);
                            kind = CRATE_KIND_IRON;
                            break;
                        }
                    }
                }
                goto out;
            case 1:
                if (paramA & 2) {
                    SetTag(9);
                    goto out;
                }
                break;
            case 2:
                if (paramA & 1) {
                    SetTag(0xb);
                    goto out;
                }
                break;
            case 3:
                if (paramA & 4) {
                    SetTag(0xd);
                    done = 1;
                }
                break;
            }
        } while (!done);
    out:
        {
            const struct sprite_anim *anims = bank->anims;
            const struct sprite_anim *a = &anims[tag];
            u32 slot = gPaletteCache->GetSlot(a->paletteId);

            palette = slot;
        }
        {
            s32 d = (s32)((u32)(slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT);

            timer = gSlotCrateTimers[d];
        }
    } else {
        {
            s32 p = (w1 & CRATE_SLOT_PHASE_MASK) | CRATE_SLOT_PHASE_STARTED;

            w1 = p | (w1 & CRATE_SLOT_CLEAR_PHASE);
        }
        slotState = w1;
        timer = 1;
        if (tag == 0xc)
            SetTag(0xa);
        else if (tag == 0xa)
            SetTag(8);
        else {
            switch ((s32)((u32)(slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT)) {
            case 0:
            case 1:
                SetTag(0xc);
                break;
            case 2:
                SetTag(0xa);
                break;
            case 3:
                SetTag(8);
                break;
            }
        }
        {
            const struct sprite_anim *anims = bank->anims;
            const struct sprite_anim *a = &anims[tag];
            u32 slot = gPaletteCache->GetSlot(a->paletteId);

            palette = slot;
        }
        if (slotState & CRATE_SLOT_STAGE_MASK)
            gAudioContext->PlaySfx(SFX_SLOT_CRATE_SPIN, 0x100);
    }
}

/* The crate's fall (fallDistance, Q8), one step per unit of speed: down
 * 0x100 a step (0x40 in the player's slow mode), or up while the speed is
 * negative. When it lands, it snaps to its target, and an explosive crate
 * explodes (if it was set to, or is a nitro crate) or, a TNT crate, lights
 * unless it is the top of a stack; the TNT crates below the one under it
 * light too. The speed then ramps up to 5, or clears once the fall is
 * done.
 *
 * Matching notes (docs/matching/archive/issue-12-13-25-naked-retry.md): the
 * speed byte is re-read through `fallSpeed` each time (GCSE keeps
 * its address in sb), `speed--` is written in both step arms, the
 * neighbour walk skips the first neighbour, and one temporary `t` both
 * carries `fallTargetY` into `y` and re-reads `x` at the bottom of the loop
 * (the ROM's r1). */
void Crate::UpdateFall()
{
    s32 remaining = fallDistance;
    s32 speed;
    s32 acc;
    s32 t;

    if (remaining == 0)
        return;
    gCrateListChanged = 1;
    speed = fallSpeed;
    if (speed == 0)
        speed = 1;
    acc = 0;
    t = x;
    if (speed < 0) {
        do {
            acc -= 0x100;
            remaining += 0x100;
            speed++;
        } while (speed != 0);
    } else {
        do {
            if (remaining > 0) {
                if (gPlayer->ctrlMode == 1) {
                    acc += 0x40;
                    remaining -= 0x40;
                    speed--;
                } else {
                    acc += 0x100;
                    remaining -= 0x100;
                    speed--;
                }
            } else {
                Crate *n;
                u8 k;

                speed = 1;
                t = fallTargetY;
                y = t;
                acc = 0;
                fallDistance = remaining;
                if (gCrateKindExplosive[k = kind]) {
                    if (blastState != 0 || k == CRATE_KIND_NITRO) {
                        if ((state & CRATE_STATE_MASK) == 0)
                            Explode(0);
                    } else if (k == CRATE_KIND_TNT) {
                        Crate *next = GetAbove();
                        Crate *prev = GetBelow();

                        if (next != 0 || prev == 0)
                            LightTnt();
                    }
                }
                n = GetBelow();
                speed--;
                if (n != 0) {
                    n = n->GetBelow();
                    while (n != 0) {
                        if (n->kind == CRATE_KIND_TNT)
                            n->LightTnt();
                        n = n->GetBelow();
                    }
                }
            }
            t = x;
        } while (speed != 0);
    }
    {
        s32 ny = y + acc;

        x = t;
        y = ny;
    }
    fallDistance = remaining;
    if (remaining == 0)
        fallSpeed = 0;
    else {
        if (++fallSpeed == 0)
            ++fallSpeed;
        LIMIT_MAX(fallSpeed, 5);
    }
}
