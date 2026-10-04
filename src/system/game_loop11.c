#include "core.h"

extern void *sub_8026EDC(s32 size);
extern void *InitLevelState(void *arg0);

/* Lazily allocates `gLevelStateSingleton` (0x1cc bytes) through
 * `InitLevelState` the first time it's needed, then returns it. Its own
 * file: ROM-adjacent to `PlayRoom` (now matched, `game_loop39.c`)
 * and the still-raw `RunRoom` on both sides
 * (asm/code_3_2_17_236ec.s before it, `PlayRoom`/
 * asm/code_3_2_17_23a1c.s after), so it can't share an object file
 * with either matched neighbor without splitting the ROM-contiguous
 * layout. */
extern void *gLevelStateSingleton;
void *GetLevelState(void)
{
    if (gLevelStateSingleton == NULL) {
        gLevelStateSingleton = InitLevelState(sub_8026EDC(0x1cc));
    }
    return gLevelStateSingleton;
}
