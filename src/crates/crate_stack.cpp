#include "crate.hpp"
#include "spawners.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
}

/* The Aku Aku and life crates, and the crate's stack walks and player
 * collision (#664, include/crate.hpp). */

/* The Aku Aku crate: unless the player is passing through (the
 * `collides` flag clear), it gains a mask (EVENT_MASK_GAIN) with its
 * sound. */
void Crate::OpenAkuAku()
{
    Player *p = gPlayer;

    if (p->f.flags >> 7) {
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
    if (eid != 0xFFFF) {
        if ((u8)IsEntityIdActivated(gEntityFlags, eid) == 0)
            SetEntityIdActivated(gEntityFlags, id);
    }
    s32 px = Q8_TO_INT(x);
    s32 py = Q8_TO_INT(y) + 3;
    gEntitySpawner->DropExtraLife(px, py, 0, 3, flag6);
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
        if (dx <= 0x3FFF) {
            s32 dy = y - testY;

            MAKE_ABS(dy);
            if (dy <= 0x3FFF) {
                if (kind != CRATE_KIND_OUTLINE)
                    QueuePlayerCollision(idx);
            }
        }
    }
    ClearTouched();
    return 0;
}
