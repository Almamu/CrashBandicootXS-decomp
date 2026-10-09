#include "part_list.hpp"

extern "C" {
#include "memory.h"
#include "globals.h"
}

/* The palette cycles (gPaletteCycles; #664, part 7c; include/part_list.hpp).
 * The HUD part that followed is hud/hud_part.cpp since #767. An old_agbcp object (OLD_AGBCC_OBJS): Tick's
 * index loads take old_agbcp's registers. */

/* Steps each cycle whose period has elapsed this frame: the colours of
 * `targets[i]` at the indices of `lists[i]` rotate by one place,
 * forwards (`direction` set) or backwards. */
void PaletteCycles::Tick()
{
    s32 i;
    s32 n;

    if (!active)
        return;
    i = 0;
    n = count;
    for (; i < n; i++) {
        u16 *target;
        u16 *list;
        u16 carry;
        s32 j;

        if (gRoomFrameCount % periods[i] != 0)
            continue;
        target = targets[i];
        list = lists[i];
        if (direction) {
            s32 len = counts[i];

            carry = target[list[len - 1]];
            for (j = 0; j < len; j++) {
                u16 old = target[list[j]];

                target[list[j]] = carry;
                carry = old;
            }
        } else {
            carry = target[list[0]];
            for (j = counts[i] - 1; j >= 0; j--) {
                u16 old = target[list[j]];

                target[list[j]] = carry;
                carry = old;
            }
        }
    }
}

/* Adds a cycle of the `listCount` colours of `targets` that `lists`
 * indexes, stepped every 60 / `rate` frames. */
void PaletteCycles::Add(u16 *targets, u16 *lists, s32 rate, s32 listCount,
                        struct byte_arg direction)
{
    u8 dir = direction.v;

    if (targets == 0 || lists == 0 || counts == 0) {
        active = 0;
        return;
    }
    this->targets[count] = targets;
    this->lists[count] = lists;
    periods[count] = 60 / rate;
    active = 1;
    fields_e[count] = 0;
    counts[count] = listCount;
    count += 1;
    this->direction = dir;
}

/* Removes every cycle. */
void PaletteCycles::Clear()
{
    s32 i;

    active = 0;
    for (i = 0; i < 3; i++) {
        targets[i] = 0;
        lists[i] = 0;
    }
    count = 0;
}

PaletteCycles::~PaletteCycles()
{
}

PaletteCycles::PaletteCycles()
{
    s32 i;

    active = 0;
    for (i = 0; i < 3; i++) {
        targets[i] = 0;
        lists[i] = 0;
    }
}
