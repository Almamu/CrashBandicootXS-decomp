#include "vehicle.hpp"

/* The polar player's state dispatch (#664 part 11a, include/vehicle.hpp):
 * the state method for `state`, through the pointer-to-member table
 * stateFuncs (gPolarPlayerStateFuncs). */
void PolarPlayer::RunState()
{
    (this->*stateFuncs[state])();
}
