#include "core.h"
#include "memory.h"

extern void *gLevelState;
extern s32 gUnknown_03000868;
extern s32 *gUnknown_03000850[];

extern void *GetLevelState(void);
extern void sub_802369C(void);
extern void sub_8023674(void *state);
extern void sub_8037620(void);
extern s32 sub_80371B4(void);
extern void sub_80375EC(void);
extern void sub_8023658(void *state);
extern void UpdateGameFrame(void *state);

/* The game's top-level loop (called once from `AgbMain`, see
 * src/system/main.c): sets up the central per-level state object
 * (`gLevelState`, see docs/rom_map.md's "hud"/"game_loop"
 * investigations for what its fields mean), the on-screen counter
 * widget (`sub_8037620`/`sub_80371B4`/`sub_80375EC`, src/audio/
 * counter_selector*.c), then runs `UpdateGameFrame` forever, freeing
 * scratch memory before and after each frame. Never actually returns -
 * the `s32` return type only exists to match `AgbMain`'s
 * `if (MainLoop() != 0)` guard, which this loop never reaches. */
s32 MainLoop(void)
{
    gLevelState = GetLevelState();
    sub_802369C();
    sub_8023674(gLevelState);
    sub_8037620();
    gUnknown_03000868 = sub_80371B4();
    sub_80375EC();
    sub_8023658(gLevelState);

    for (;;) {
        mem_free_bytes(MEM_HEAP_BOTH);
        UpdateGameFrame(gLevelState);
        mem_free_bytes(MEM_HEAP_BOTH);
    }
}

/* Two-level table lookup: `gUnknown_03000850` is an array of per-
 * "counter widget mode" (`gUnknown_03000868`, set above from
 * `sub_80371B4`'s return) tables, each indexed by `index`. Shape/
 * meaning of the tables themselves not established yet. */
s32 sub_8026F38(s32 index)
{
    return gUnknown_03000850[gUnknown_03000868][index];
}
