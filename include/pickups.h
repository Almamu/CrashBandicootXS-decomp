#ifndef GUARD_PICKUPS_H
#define GUARD_PICKUPS_H

/* The pickups subsystem (src/pickups/): the wumpa fruit, the extra life
 * and the time-trial stopwatch, include/pickups.hpp's C++ classes
 * (#664), and the tables they read.
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
#include "vtable.h"

/* A position pair: the pickups' home position (`anchor`), which their
 * constructors copy from the head of the object in one go (the ROM's
 * paired `ldr; ldr; str; str`). */
struct orbit_vec {
    s32 x;
    s32 y;
};

/* A copy of one of the hop width tables, as the struct UpdateExtraLifeHop
 * and UpdateWumpaHop copy onto the stack in one go. */
struct three_words {
    s32 a[3];
};

/* The x offset scale of each hop mode (1-3) of UpdateExtraLifeHop and
 * UpdateWumpaHop (src/data/object_tables_16bb6c.c). */
extern const s32 gExtraLifeHopWidths[3];
extern const s32 gWumpaHopWidths[3];

#endif /* GUARD_PICKUPS_H */
