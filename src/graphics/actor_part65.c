#include "core.h"

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.c/actor_part28.c/actor_part32.c. A second
 * object kind (constructed by the parked `CreateHovercraftSideGun`, vtable
 * `gHovercraftSideGunVtable`) reuses a death/"dead" byte flag at
 * `self+0x6c`. See docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constant getter - returns `self`'s death flag (`self+0x6c`), the same
 * shape as `IsHovercraftCannonUnshootable` (actor_part34.c). */
u8 IsHovercraftLauncherUnshootable(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x6c];
}

asm(".align 2, 0");
