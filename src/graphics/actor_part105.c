#include "core.h"

/* A frame-tick counter, incremented every category-load-loop tick
 * (see InitActorCategory, still raw) and snapshotted into
 * gActorMissedNitros at the top of SelectActorCategory (also still raw)
 * for the "how long has this sub-effect run" bookkeeping the
 * gActorSpawnTable spawnTable accessors use. */
extern s32 gActorMissedNitros;
extern s32 gActorCheckpoint;

void AddActorMissedNitro(void)
{
    gActorMissedNitros++;
}

s32 GetActorMissedNitros(void)
{
    return gActorMissedNitros;
}

s32 GetActorCheckpoint(void)
{
    return gActorCheckpoint;
}
