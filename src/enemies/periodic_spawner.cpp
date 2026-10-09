#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "level.h"
#include "globals.h"
#include "math_util.h"
#include "player.h"
}

/* PeriodicSpawner, the periodic trigger entity (include/enemy_ctrl.hpp),
 * ROM 0x0800CACC-0x0800CB64. g++ emits gPeriodicSpawnerVtable here. An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* While the spawner is 0xA1-0x18F pixels right of the player, calls
 * `callback` at its position once every `period` frames (offset by
 * `phase`), the gate UpdateShooter uses too. */
void PeriodicSpawner::Update()
{
    s32 selfX = Q8_TO_INT(x);
    s32 cameraX = Q8_TO_INT(gPlayer->x);

    if ((u32)(selfX - cameraX - 0xa1) <= 0xee) {
        if (__modsi3(gRoomFrameCount + period - phase, period) == 0)
            callback(0xFFFF, selfX, y >> 8, 0);
    }
}

/* g++ sets the vtable pointer back to gPeriodicSpawnerVtable and runs
 * the inline ~Entity, which sets gEntityVtable and frees the object when
 * bit 0 of the flags is set; the first store is dead and goes. */
PeriodicSpawner::~PeriodicSpawner()
{
}

/* Entity() (InitEntity), then the vtable pointer. */
PeriodicSpawner::PeriodicSpawner()
{
}

/* The gate's period and phase. */
void PeriodicSpawner::SetPeriod(s32 newPeriod, s32 newPhase)
{
    period = newPeriod;
    phase = newPhase;
}

/* The function Update calls; SpawnSealSpawner (spawn_objects.cpp) stores
 * `callback` directly.
 * UNUSED - no caller anywhere in the ROM (checked every src/ and lib/ .c
 * file and every word-aligned Thumb pointer in baserom.gba). */
void PeriodicSpawner::SetCallback(void (*newCallback)(u32 arg, u16 x, u16 y, u16 arg3))
{
    callback = newCallback;
}
