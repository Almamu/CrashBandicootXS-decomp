#include "crate.hpp"
#include "spawners.hpp"
#include "player.hpp"
#include "engine/audio/audio.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

using Game::Crash::Player;
using Game::Crates::Crate;
using Game::Level::EntitySpawner;

/* The Aku Aku and life crates, and the crate's stack walks and player
 * collision (#664, include/crate.hpp). */

/* The Aku Aku crate: unless the player is passing through (the
 * `collides` flag clear), it gains a mask (EVENT_MASK_GAIN) with its
 * sound. */
void Crate::OpenAkuAku()
{
    Player *p = gPlayer;

    if (p->f.flags >> ENTITY_BIT_COLLIDES) {
        p->HandleEvent(0, EVENT_MASK_GAIN, 0);
        gAudioContext->PlaySfx(SFX_AKU_AKU_GAIN, 0x100);
    }
}

/* The life crate: the break sound, the crate's entity id marked
 * activated (not again once it is), and an extra life dropped three
 * pixels below the crate. */
void Crate::OpenLife(bool flag6)
{
    gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
    u16 eid = id;
    if (eid != 0xFFFF && gEntityFlags->IsActivated(eid) == 0)
        gEntityFlags->SetActivated(id);
    /* `px`, `py` are needed: inlined, the instructions are reordered */
    s32 px = Q8_TO_INT(x);
    s32 py = Q8_TO_INT(y) + 3;
    EntitySpawner::Get()->DropExtraLife(px, py, 0, PICKUP_MODE_HOP_UP, flag6);
}

/* gCrateKindBreakable[kind]. */
u8 Crate::IsKindBreakable(u32 kind)
{
    return gCrateKindBreakable[kind];
}

/* The top of the stack the crate is in: the crates above it, up to the
 * first committed one (state 1). The ROM has the two returns before the
 * loop, which the gotos give. */
Crate *Crate::GetTop()
{
    Crate *cur = GetAbove();
    Crate *next;

    if (cur == 0)
        goto top;
    if ((cur->state & CRATE_STATE_MASK) != 1)
        goto loop;
top:
    return this;
end:
    return cur;
loop:
    next = cur->GetAbove();
    if (next == 0)
        goto end;
    if ((next->state & CRATE_STATE_MASK) == 1)
        goto end;
    cur = next;
    goto loop;
}

/* The bottom of the stack, the same way down. */
Crate *Crate::GetBottom()
{
    Crate *cur = GetBelow();
    Crate *next;

    if (cur == 0)
        goto top;
    if ((cur->state & CRATE_STATE_MASK) != 1)
        goto loop;
top:
    return this;
end:
    return cur;
loop:
    next = cur->GetBelow();
    if (next == 0)
        goto end;
    if ((next->state & CRATE_STATE_MASK) == 1)
        goto end;
    cur = next;
    goto loop;
}

/* Unless the crate is committed (state 1), or is an outline crate, a
 * player at (testX, testY) (Q8) within 0x3FFF of it queues a collision
 * with it (QueueCratePlayerCollision; `idx` is the player's action). The
 * touched flag is cleared either way. */
s32 Crate::CollideWithPlayer(u32 idx, s32 testX, s32 testY)
{
    if ((state & CRATE_STATE_MASK) != 1) {
        s32 dx = x - testX;

        MAKE_ABS(dx);
        if (dx <= INT_TO_Q8(64) - 1) {
            s32 dy = y - testY;

            MAKE_ABS(dy);
            if (dy <= INT_TO_Q8(64) - 1 && kind != CRATE_KIND_OUTLINE)
                QueuePlayerCollision(idx);
        }
    }
    ClearTouched();
    return 0;
}

/* The crate's accessors (#664, include/crate.hpp): the slot crate's
 * word, then the plain fields. */

/* The slot crate's stage (`slotState` bits 6-7). */
u32 Crate::GetSlotStage()
{
    return ((u32)slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT;
}

/* Steps the stage down by one, unless it is 0. */
void Crate::DecrementSlotStage()
{
    u8 stage = GetSlotStage();

    if (stage != 0) {
        /* `next` is needed: inlined, the code is 1 instruction shorter */
        u8 next = stage - 1;
        slotState = (slotState & CRATE_SLOT_CLEAR_STAGE) | (next << CRATE_SLOT_STAGE_SHIFT);
    }
}

void Crate::SetSlotStage(u8 stage)
{
    slotState = (slotState & CRATE_SLOT_CLEAR_STAGE) | (stage << CRATE_SLOT_STAGE_SHIFT);
}

void Crate::ClearSlotStage()
{
    u32 v = slotState;
    v &= CRATE_SLOT_CLEAR_STAGE;
    slotState = v;
}

/* The spins left at this stage (`slotState` bits 3-5). */
u32 Crate::GetSlotSpins()
{
    return ((u32)slotState & CRATE_SLOT_SPINS_MASK) >> CRATE_SLOT_SPINS_SHIFT;
}

/* Steps the spins down by one, unless they are 0. */
void Crate::DecrementSlotSpins()
{
    u8 spins = GetSlotSpins();

    if (spins != 0) {
        /* `next` is needed: inlined, the code is 1 instruction shorter */
        u8 next = spins - 1;
        slotState = (slotState & CRATE_SLOT_CLEAR_SPINS) | (next << CRATE_SLOT_SPINS_SHIFT);
    }
}

void Crate::SetSlotSpins(u8 spins)
{
    slotState = (slotState & CRATE_SLOT_CLEAR_SPINS) | (spins << CRATE_SLOT_SPINS_SHIFT);
}

/* The face (`slotState` bits 0-2). */
void Crate::SetSlotPhase(u8 phase)
{
    slotState = (slotState & CRATE_SLOT_CLEAR_PHASE) | phase;
}

u32 Crate::GetSlotPhase()
{
    return slotState & CRATE_SLOT_PHASE_MASK;
}

void Crate::SetKind(u8 value)
{
    kind = value;
}

u8 Crate::GetKind()
{
    return kind;
}

void Crate::SetFallDistance(s32 value)
{
    fallDistance = value;
}

s32 Crate::GetFallDistance()
{
    return fallDistance;
}

/* The low 7 bits of `state`; the busy bit stays. */
void Crate::SetState(u8 value)
{
    state = value | (state & CRATE_STATE_BUSY);
}

u32 Crate::GetState()
{
    return state & CRATE_STATE_MASK;
}

void Crate::SetFallSpeed(u8 value)
{
    fallSpeed = value;
}

s32 Crate::GetFallSpeed()
{
    return fallSpeed;
}

u32 Crate::IsBusy()
{
    if (state & CRATE_STATE_BUSY)
        return 1;
    return 0;
}

/* Sets the busy bit and the player's `busy` latch. The ROM loads the 1
 * after the player, before its `busy` address: a local `one` assigned
 * between the two. */
void Crate::SetBusy()
{
    /* `p` is needed: inlined, the instructions are reordered */
    Player *p;
    u32 one;

    state |= CRATE_STATE_BUSY;
    p = gPlayer;
    one = 1;
    p->busy = one;
}

/* Clears the busy bit and the player's `busy` latch. */
void Crate::ClearBusy()
{
    state &= CRATE_STATE_MASK;
    gPlayer->busy = 0;
}

void Crate::SetTouched(u8 value)
{
    touched = value;
}

u8 Crate::GetParamB()
{
    return paramB;
}

u8 Crate::GetParamA()
{
    return paramA;
}

/* The whole word at 0x48, from a byte (unlike SetSlotPhase). */
void Crate::SetSolidKind(u8 value)
{
    solidKind = value;
}

void Crate::SetTrialKind(s32 value)
{
    trialKind = value;
}

s32 Crate::GetTrialKind()
{
    return trialKind;
}
