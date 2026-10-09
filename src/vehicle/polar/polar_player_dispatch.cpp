#include "vehicle.hpp"

/* The polar player's state dispatch (#664 part 11a, include/vehicle.hpp)
 * and IsPolarPlayerInactive, ROM 0x0802C208-0x0802C270, between
 * polar_player_actions.cpp and polar_pickups.cpp. */

/* The state method for `state`, through the pointer-to-member table
 * stateFuncs (gPolarPlayerStateFuncs). */
void PolarPlayer::RunState()
{
    (this->*stateFuncs[state])();
}

/* Whether the player is inactive (no caller). */
u8 IsPolarPlayerInactive(void)
{
    return gPolarPlayerInactive;
}
