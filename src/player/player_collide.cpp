#include "bg_layer.hpp"
#include "player.hpp"
#include "level_state.hpp"

extern "C" {
#include "level.h"
#include "sprite_bank.h"
#include "globals.h"
#include "math_util.h"
}

/* Player::CheckPlayerContact (include/player.hpp; CollidePlayer): the
 * player's collision with the objects and the terrain. Built by old_agbcp
 * (the `movs #0x40`/`movs #8` before their `ldrb`). */

/* While the player collides (`collides`): the object pass (TouchPlayer)
 * and the ground sprite's terrain probe, with the level layers recording
 * the terrain kind under the player. Leaving a carrier counts as standing.
 * The kind then sets the ground's effect: 1 hurts (the mask is lost, the
 * hit event), 5 is slippery, 7 and 10 push left and right. Last, hang
 * terrain (code 6) at the frame's anchor grabs (EVENT_HANG_GRAB, with `y`
 * snapped to the tile's bottom row) and its absence releases
 * (EVENT_HANG_RELEASE). Returns the probe's axes.
 *
 * The first `hitAxes` store and the kinds' stores of 1 go through
 * Player's inline Store methods: as inline parameters, the values are
 * loaded before the fields' addresses, as in the ROM (which also keeps
 * the kinds' tails from being cross-jumped). */
s32 Player::CheckPlayerContact()
{
    if (f.flags >> 7) {
        u8 k;
        const struct sprite_point *off;
        s32 px, py;

        StoreHitAxes(0);
        cleared = 0;
        TouchPlayer();
        cleared = 1;
        gLevelLayers->probeFlag = 1;
        GroundSprite::CheckPlayerContact();
        gLevelLayers->probeFlag = 0;
        if (carried != 0) {
            hitAxes |= 8;
            carried = 0;
            slippery = 0;
            pushLeft = 0;
            pushRight = 0;
        }
        k = gLevelLayers->kind;
        if (k != 0) {
            switch (k) {
            case 1:
                f.b.vulnerable = 1;
                deadline = 0;
                gLevelState->SetMaskLevel(MASK_LEVEL_NONE);
                HandleEvent(0, EVENT_HIT, 0);
                break;
            case 2:
            case 3:
            case 4:
                break;
            case 5:
                pushLeft = 0;
                pushRight = 0;
                StoreSlippery(1);
                break;
            case 7:
                pushRight = 0;
                slippery = 0;
                StorePushLeft(1);
                break;
            case 6:
            case 8:
            case 9:
                break;
            case 10:
                pushLeft = 0;
                slippery = 0;
                StorePushRight(1);
                break;
            }
            gLevelLayers->kind = 0;
        } else if (hitAxes == 8) {
            pushLeft = 0;
            pushRight = 0;
            slippery = 0;
        }

        off = FrameAnchor();
        px = Q8_TO_INT(x);
        py = Q8_TO_INT(y);
        if (mirrorBits.flipX < 0)
            px -= off->x;
        else
            px += off->x;
        py += off->y;
        if (GetTerrainFlagsAt(gLevelLayers, px, py) == 6) {
            if (hanging == 0) {
                s32 snap = (py & 0x00FFFFF8) + 7;

                snap -= py;
                y += INT_TO_Q8(snap);
                HandleEvent(0, EVENT_HANG_GRAB, 0);
            }
        } else if (hanging != 0) {
            HandleEvent(0, EVENT_HANG_RELEASE, 0);
        }
    }
    return hitAxes;
}
