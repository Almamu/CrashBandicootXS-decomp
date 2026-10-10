#ifndef GUARD_PICKUPS_H
#define GUARD_PICKUPS_H

/* The pickups subsystem (src/pickups/): the wumpa fruit, the extra life
 * and the time-trial stopwatch are C++ classes (include/pickups.hpp,
 * #664). This header keeps what C needs: the hop width tables, which
 * src/data/object_tables_16bb6c.c defines. */

#include "core.h"

/* The x offset scale of each hop mode (1-3) of UpdateExtraLifeHop and
 * UpdateWumpaHop (src/data/object_tables_16bb6c.c). */
extern const s32 gExtraLifeHopWidths[3];
extern const s32 gWumpaHopWidths[3];

#endif /* GUARD_PICKUPS_H */
