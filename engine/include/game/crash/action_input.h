#ifndef GUARD_ACTION_INPUT_H
#define GUARD_ACTION_INPUT_H

/* Helpers of the action controller's C++ files (engine/include/game/crash/action_ctrl.hpp,
 * engine/src/game/crash/action_ctrl*.cpp): the input word's halves. The byte view of
 * the player's flags2 is in action_ctrl.hpp. The controller has no C view:
 * `struct act` went with its last C user (ResetActionCtrl, part 7h of
 * #664), and its C prototypes with the state table's conversion to C++
 * (engine/src/game/crash/data/action_ctrl_state_table.cpp, #656). */


/* gKeys is the input word: low half held, high half newly
 * pressed. Handlers copy it to a stack slot and read the halves back from
 * there; the halves go through the local's address (a union or struct
 * member read is folded into a halfword load of the global itself). */
#define INPUT_HELD(in) (*(u16 *)&(in))
#define INPUT_PRESSED(in) (*(u16 *)((u8 *)&(in) + 2))

#endif // GUARD_ACTION_INPUT_H
