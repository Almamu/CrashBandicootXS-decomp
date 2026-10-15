/* --level N and --room R (main.c): the level select's choice is level N,
 * and the level starts in its room R, whatever the keys picked. The same
 * as tools/boottest.py's level scripts do on mGBA (setting
 * LevelSelectLoop's return and SelectRoom's roomIndex), so that a key
 * file can play any stage on both: a polar or jetpack stage, a boss.
 *
 * The link wraps the two (platform.mk, --wrap): LevelSelect::Run, which
 * stores LevelSelect::Loop's choice in its argument (game_frame.cpp's
 * `room.level`), and LevelProgress::SelectRoom (room_select.cpp), which
 * loads the room roomIndex names. Each override is used once: the level
 * at the first level select, the room at the first SelectRoom after it. */

#include "gba/gba.h"
#include "host_platform.h"

/* LevelProgress's first fields (engine/include/game/level/level_state.hpp):
 * two s32s, at the same offsets on any host. */
struct LevelProgressHead {
    s32 level;
    s32 roomIndex;
};

s32 __real__ZN4Game2Ui11LevelSelect3RunEPi(s32 *level);
_Bool __real__ZN4Game5Level13LevelProgress10SelectRoomEv(struct LevelProgressHead *self);

static int sLevelPicked;
static int sRoomPicked;

s32 __wrap__ZN4Game2Ui11LevelSelect3RunEPi(s32 *level)
{
    s32 result = __real__ZN4Game2Ui11LevelSelect3RunEPi(level);

    if (gHostOptions.level >= 0 && !sLevelPicked) {
        sLevelPicked = 1;
        *level = gHostOptions.level;
    }
    return result;
}

_Bool __wrap__ZN4Game5Level13LevelProgress10SelectRoomEv(struct LevelProgressHead *self)
{
    if (gHostOptions.room >= 0 && sLevelPicked && !sRoomPicked) {
        sRoomPicked = 1;
        self->roomIndex = gHostOptions.room;
    }
    return __real__ZN4Game5Level13LevelProgress10SelectRoomEv(self);
}
