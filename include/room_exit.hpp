#ifndef GUARD_ROOM_EXIT_HPP
#define GUARD_ROOM_EXIT_HPP

/* The room exit flag (src/level/room.cpp, #915): set when something ends
 * the room (the exit's zone, a power, a boss falling out, the player's
 * exit moves); LevelProgress::PlayRoom clears it when the room starts,
 * and RunRoom polls it every frame, then fades out.
 *
 * An all-static class, like the collected spawns (docs/cplusplus.md, "The
 * room exit flag as an all-static class"): its one variable is an IWRAM
 * global that no other object reads, and a static data member compiles
 * like that global, static member functions like plain functions.
 * cxx_symbols.txt (block "#915 RoomExit") maps each member to its C name
 * (`_8RoomExit.requested` is gRoomExitRequested, which iwram_data.cpp
 * defines with the other initialised IWRAM globals;
 * `Request__8RoomExit` is RequestRoomExit, ...).
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

class RoomExit
{
public:
    static void Clear();     // ClearRoomExit
    static void Request();   // RequestRoomExit
    static u8 IsRequested(); // IsRoomExitRequested

private:
    static u8 requested; // gRoomExitRequested (iwram_data.cpp)
};

#endif // GUARD_ROOM_EXIT_HPP
