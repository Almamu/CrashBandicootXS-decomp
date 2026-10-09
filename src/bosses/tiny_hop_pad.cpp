#include "bg_layer.hpp"
#include "ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "level.h"
#include "globals.h"
#include "math_util.h"
#include "match.h"
}

/* GitHub issue #22, ROM 0x080187FC-0x08018884 - non-adjacent to
 * airship_fireball.c since the raw `UpdateTiny`/`SetTinyState`/
 * `PickTinyHopTarget`/`SpawnTinyFallingLeaves` block sits between them (see
 * asm/code_3_2_17_18008.s). Two controllers (include/ctrl.hpp): the
 * stomped hop pad's, and OneShotAnimCtrl's Update. */

/* State 0 plays animation 8 and moves to state 1. State 1 sinks the pad
 * 4 pixels a frame until it is 32 pixels below layer 0's bottom edge,
 * then moves to state 2, where it stays.
 *
 * The pin on `part` (r2) is still needed in C++: with neither pinned,
 * g++ gives `this` r2 and `part` r4 where the ROM has r3 and r2; once
 * `part` is pinned, `this` lands in r3 by itself. (The C needed a pin on
 * both, plus gotos for the block order, which the switch gives.)
 * #662 round 2: state 1's test through a `bool` (an inline or a local)
 * gives the ROM's registers but keeps the flag as a `movs` 0/1 and a
 * second compare; the permuter on the C++ matched only with
 * `do { } while (0)` wrappers, `x++; x--;` no-ops or a redundant copy of
 * `part`. */
void StompedHopPadCtrl::Update(MovingSprite *partArg)
{
    MATCH_HOLD_REG(MovingSprite *, part, r2) = partArg;
    s32 y;

    switch (state) {
    case 0:
        state = 1;
        SetTargetAnim(part, 8);
        break;
    case 1:
        y = part->y + 0x400;
        part->y = y;
        if (y >= INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x2000)
            state = 2;
        break;
    case 2:
        break;
    }
}

StompedHopPadCtrl::~StompedHopPadCtrl()
{
}

StompedHopPadCtrl::StompedHopPadCtrl()
{
}

/* Marks the sprite object gone once its animation has played through. */
void OneShotAnimCtrl::Update(MovingSprite *part)
{
    if (part->animDone)
        part->MarkGone();
}
