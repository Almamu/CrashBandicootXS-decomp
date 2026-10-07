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
 * The two pins are still needed in C++: unpinned, g++ gives `this` r2
 * and `part` r4 where the ROM has r3 and r2 (the C needed the same two,
 * plus gotos for the block order, which the switch gives). */
void StompedHopPadCtrl::Update(SpriteObj *partArg)
{
    MATCH_HOLD_REG(StompedHopPadCtrl *, self, r3) = this;
    MATCH_HOLD_REG(SpriteObj *, part, r2) = partArg;
    s32 y;

    switch (self->state) {
    case 0:
        self->state = 1;
        self->SetTargetAnim(part, 8);
        break;
    case 1:
        y = part->y + 0x400;
        part->y = y;
        if (y >= INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x2000)
            self->state = 2;
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
void OneShotAnimCtrl::Update(SpriteObj *part)
{
    if (part->animDone)
        part->MarkGone();
}
