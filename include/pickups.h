#ifndef GUARD_PICKUPS_H
#define GUARD_PICKUPS_H

/* The pickups subsystem (src/pickups/): the wumpa fruit, the extra life
 * and the time-trial stopwatch, include/pickups.hpp's C++ classes
 * (#664). For the C files, wumpas and extra lives are `struct
 * orbit_part`s (include/orbit_part.h); the stopwatch is a plain 0x40-byte
 * sprite object. The prototypes below keep the methods' C names
 * (cxx_symbols.txt), for the C callers.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * `ResetActionCtrl` (src/pickups/wumpa.cpp) only shares the file's ROM
 * range; it is in player.h. */

#include "core.h"
#include "actor.h"
#include "orbit_part.h"
#include "vtable.h"

/* A copy of one of the hop width tables, as the struct UpdateExtraLifeHop
 * and UpdateWumpaHop copy onto the stack in one go. */
struct three_words {
    s32 a[3];
};

/* The x offset scale of each hop mode (1-3) of UpdateExtraLifeHop and
 * UpdateWumpaHop (src/data/object_tables_16bb6c.c). */
extern const s32 gExtraLifeHopWidths[3];
extern const s32 gWumpaHopWidths[3];

/* src/pickups/extra_life.cpp */
extern void PickUpExtraLife(struct orbit_part *self, u8 randomize);
extern void SendExtraLifeToHud(struct orbit_part *self);
extern void UpdateExtraLifeHop(struct orbit_part *self);
extern void ResetExtraLifePickup(struct orbit_part *self);
extern void SetExtraLifePos(struct orbit_part *self, s32 x, s32 y);
extern void SetExtraLifeHop(struct orbit_part *self, u8 mode);
extern void SetExtraLifeCounter(struct orbit_part *self, u8 val);

/* src/pickups/wumpa_update.cpp */
extern void PickUpWumpa(struct orbit_part *self, u8 randomize);
extern void SendWumpaToHud(struct orbit_part *self);
extern void StartWumpaPayout(struct orbit_part *self);
extern void UpdateWumpaHop(struct orbit_part *self);

/* src/pickups/wumpa.cpp */
extern void ResetWumpaPickup(struct orbit_part *self);
extern void SetWumpaPos(struct orbit_part *self, s32 x, s32 y);
extern void SetWumpaHop(struct orbit_part *self, s32 mode);
extern void SetWumpaCounter(struct orbit_part *self, u8 value);
extern void ResetStopwatch(struct actor *self);

#endif /* GUARD_PICKUPS_H */
