/* The key input: GetDpadDirection, UpdateKeys and KeyInput's constructor
 * (ClearKeys). Split from irq.cpp (#767), same flags (old_agbcc). */

#include "spawners.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "globals.h"
}

/* The held d-pad bits as a direction 0-8 (0 = none), through
 * gDpadDirectionTable. `input` (gInput at every call site) is unused. */
u8 GetDpadDirection(void *input)
{
    u8 idx = 0;
    if (gKeys.half.held & DPAD_RIGHT)
        idx |= 8;
    if (gKeys.half.held & DPAD_LEFT)
        idx |= 4;
    if (gKeys.half.held & DPAD_DOWN)
        idx |= 2;
    if (gKeys.half.held & DPAD_UP)
        idx |= 1;
    return gDpadDirectionTable[idx];
}

/* Reads the raw (active-low) hardware key register, inverts it to
 * active-high, records the newly pressed bits in gKeys' `pressed`,
 * updates `held` to the new state, then returns 1 if the low 4 bits
 * (A/B/Select/Start) are all held - a "soft reset" combo check. The
 * combo is a variable compared against the masked keys, as the ROM
 * compares two registers (a literal 0xF would be `cmp r0, #15`).
 * `input` (gInput at every call site) is unused. */
s32 UpdateKeys(void *input)
{
    u16 keys = ~REG_KEYINPUT;
    s32 combo;

    gKeys.half.pressed = keys & ~gKeys.half.held;
    gKeys.half.held = keys;
    combo = 0xF;
    keys &= combo;
    if (combo == keys)
        return 1;
    return 0;
}

/* ClearKeys: clears gKeys (held and newly pressed). It returns `this`,
 * as a constructor does, which is why the ROM keeps r0 free and builds
 * the stores in r1/r2. */
KeyInput::KeyInput()
{
    gKeys.half.held = 0;
    gKeys.half.pressed = 0;
}
