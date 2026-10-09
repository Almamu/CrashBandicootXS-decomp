#include "pickups.hpp"
#include "action_ctrl.hpp"
#include "hud.hpp"
#include "audio.hpp"

extern "C" {
#include "math_util.h"
#include "util.h"
#include "gfx.h"
#include "globals.h"
#include "player.h"
}

/* The stopwatch, the time trial's pickup (#664, include/pickups.hpp).
 * g++ emits gStopwatchVtable here. The action controller's Reset, which
 * the ROM puts after it, starts action_ctrl_event.cpp. */

/* Updates the sprite while the player is within 0x180 pixels on both
 * axes; otherwise the stopwatch is gone. */
void Stopwatch::Update()
{
    s32 d = Q8_TO_INT(gPlayer->x);

    d -= Q8_TO_INT(x);
    MAKE_ABS(d);
    if (d > 0x180) {
        MarkGone();
    } else {
        d = Q8_TO_INT(gPlayer->y);
        d -= Q8_TO_INT(y);
        MAKE_ABS(d);
        if (d > 0x180)
            MarkGone();
        else
            Sprite::Update();
    }
}

/* `unused` is the spawn table slot's fourth argument. */
Stopwatch *Stopwatch::Create(u16 id, u16 x, u16 y, u16 unused)
{
    return new Stopwatch(id, x, y);
}

void Stopwatch::Reset()
{
}

Stopwatch::~Stopwatch()
{
}

Stopwatch::Stopwatch()
{
    Reset();
}
