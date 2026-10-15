#include "game/vehicles/polar/yeti.hpp"
#include "game/vehicles/vehicle.hpp"

extern "C" {
#include "core.h"
#include "engine/actor/actor_anim.h"
}

using Game::Vehicles::Polar::Yeti;

/*
 * ROM 0x0817A850-0x0817A880. Linked in ROM order between data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* Yeti::keyframes (include/game/vehicles/polar/yeti.hpp): the keyframes of the yeti (yeti.cpp,
 * of compressed frame set B), with yeti_frames.cpp's
 * gYetiFrames as their frame table. */
const struct anim_frame_record Yeti::keyframes[4] = {
    { 128, 0, 30, 0, 0x0, { 0, 0 } },
    { 128, 30, 31, 0, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
};
