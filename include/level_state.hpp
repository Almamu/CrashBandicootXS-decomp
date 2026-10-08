#ifndef GUARD_LEVEL_STATE_HPP
#define GUARD_LEVEL_STATE_HPP

/* The game context as C++ (#664 cleanup, docs/cplusplus.md): the level
 * state (gLevelStateSingleton, gLevelState; level_state.h's struct
 * level_state), whose constructor builds the hot subsystem singletons
 * (InitLevelState, src/level/spawn_pickups.cpp) and whose destructor
 * frees them (DestroyLevelState, src/level/level_cutscene.cpp; UNUSED:
 * the game never leaves MainLoop). GetLevelState (level_state.cpp) makes
 * the one instance. Its other functions (level_state.cpp, bonus_round.c,
 * ...) keep C linkage and take the struct. It has no vtable.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to
 * emit; the pragma keeps g++ from emitting out-of-line copies of inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

extern "C" {
#include "core.h"
#include "level_state.h"
}

class LevelState : public level_state
{
public:
    LevelState();  // InitLevelState
    ~LevelState(); // DestroyLevelState
};

COMPILE_TIME_ASSERT(level_state_hpp, sizeof(LevelState) == 0x1CC);

#endif /* !GUARD_LEVEL_STATE_HPP */
