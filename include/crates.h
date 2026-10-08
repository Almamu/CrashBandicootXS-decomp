#ifndef GUARD_CRATES_H
#define GUARD_CRATES_H

/* The crates subsystem (src/crates/): the crate object (class Crate,
 * crate.hpp; crate.h has its C types), its collision and breaking, the
 * slot crate, and the crate list (the bucketed grid of crates and other
 * collidable parts).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The crate list (`CrateList`, include/crate_list.hpp) is set up by
 * InitCrateList, which src/objects/part_list.cpp holds for ROM order. */

#include "core.h"
#include "math_util.h"
#include "aabb.h"
#include "gfx.h"
#include "objects.h"
#include "byte_arg.h"
#include "vtable.h"
#include "constants/attack_kinds.h"
#include "constants/crates.h"
#include "constants/entities.h"


/* One entry of the crate list's free list (crate_list.hpp's
 * CrateGridLink), as `struct pool_init_node` below points at it. */
struct pool_link {
    struct pool_node *node; // 0x00
    struct pool_link *next; // 0x04
};

/* codegen: a grid node (crate_list.hpp's CrateGridNode) with untyped
 * fields, the view CrateList::ResetGrid (include/crate_list.hpp) zeroes
 * the nodes through.
 * Through the real node pointer fields, gcc takes the zeroing stores as
 * possible writes to the list's `nodes` and reloads it
 * (docs/headers_plan.md, "Codegen findings"). */
struct pool_init_node {
    void *data;
    void *next;
    struct pool_link *wrap;
    void *link;
    u8 mark;
};

/* The crate tables, indexed by crate kind (src/data/object_tables_16bb6c.c). */
extern const u8 gSlotCrateTimers[4];
extern const u8 gCrateKindCounted[CRATE_KIND_COUNT];
extern const u8 gCrateKindBreakable[CRATE_KIND_COUNT];
extern const u8 gCrateKindExplosive[CRATE_KIND_COUNT];
extern const u8 gCrateKindUnbreakable[CRATE_KIND_COUNT];
extern const s32 gCrateHitResponse[CRATE_KIND_COUNT][7];
extern const u8 gAttackKindBreakLimited[8];

/* Set when the crate list changes (sym_iwram.txt). */
extern u8 gCrateListChanged;

/* src/crates/crate_break.cpp */
extern void UpdateCrates(void);
extern void DetonateNitroCrates(void);
extern void BreakCratesInArea(s32 x, s32 y, s32 dist, s32 height);

/* src/crates/crate.cpp */
extern void ResolvePlayerCollisions(void);
/* FindLineCrossingYMajor and FindLineCrossingXMajor are C++ functions
 * (include/crate.hpp). */

/* src/crates/crate_reset.cpp */
extern s32 FindLineCrossing(s32 pos, s32 count, s32 a, s32 b, s32 limit);

/* src/crates/crate_time_trial.cpp */
extern void ConvertCratesForTimeTrial(void);

#endif /* GUARD_CRATES_H */
