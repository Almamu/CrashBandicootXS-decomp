#ifndef GUARD_CRATE_H
#define GUARD_CRATE_H

#include "gobj_1a794.h"
#include "math_util.h"
#include "constants/crates.h"

/* The crates' C types (CreateCrate, gCrateVtable): the object the issue
 * #12 "physics/collision" cluster (ROM 0x0800D040-0x0800FC70,
 * src/crates/crate_hit.cpp/crate_break.cpp) turned out to be is the C++
 * class Crate (include/crate.hpp), which documents the fields. Its C
 * view, `struct crate`, went with its last user (#656). `kind` is the
 * crate type CreateCrate picks (CRATE_KIND_*, constants/crates.h,
 * generated from data/levels/crate_kinds.json). See
 * docs/matching/archive/issue-12-physics-collision.md. */

/* A group of objects that trigger together (ActivateIronSwitchCrate
 * builds it): an iron switch crate's outline crates. */
struct crate_group {
    s32 count;
#ifdef __cplusplus
    class Crate *items[0];
#else
    struct crate *items[0];
#endif
};

#define PHYS_NO_GROUP ((struct crate_group *)-1)
#define PHYS_HAS_GROUP(g) ((u32)(g) + 1 > 1)

/* Crate's `state`: the low 7 bits
 * are the state (0: idle; 1: committed - the physics AABB tests skip it
 * and UpdateCrate runs FinishBrokenCrate), bit 7 is busy (set when a hit
 * starts an animation; UpdateCrate clears it, with gPlayer->busy, once
 * `animDone` is set). */
#define CRATE_STATE_MASK 0x7f
#define CRATE_STATE_BUSY 0x80

/* Crate's `slotState` (slot crates), as UpdateSlotCrate and the slot_crate.cpp
 * accessors read the raw word: `phase` (bits 0-2; bit 2: the spin has
 * started), `spins` (3-5, full turns left at this stage) and `stage` (6-7,
 * 0 idle, 1-3 faster each time, gSlotCrateTimers; past 3 it turns to
 * iron). The CLEAR_ masks keep the other two fields. */
#define CRATE_SLOT_PHASE_MASK    7
#define CRATE_SLOT_PHASE_STARTED 4 // phase bit 2: the spin has started
#define CRATE_SLOT_SPINS_MASK    0x38
#define CRATE_SLOT_SPINS_SHIFT   3
#define CRATE_SLOT_STAGE_MASK    0xc0
#define CRATE_SLOT_STAGE_SHIFT   6
#define CRATE_SLOT_CLEAR_PHASE   0xf8
#define CRATE_SLOT_CLEAR_SPINS   0xc7
#define CRATE_SLOT_CLEAR_STAGE   0x3f

/* A crate's placement record: entry `slot` of the room's parameter
 * records (`gEntityFlags->list`, level_data.h), as CreateCrate reads it.
 * Bytes 6-8 are per-kind (see Crate's paramA/paramB/fallSpeed and the
 * word at 0x48). */
struct crate_placement {
    u8 flags;   // 0x00 - bit 5: has a time-trial kind; bit 6: Aku Aku when assisted
                //        (checkpoint: SetCheckpointAtPlayer's flag); bit 7: checkpoint
                //        when assisted (CRATE_PLACEMENT_FLAG_*)
    u8 options; // 0x01 - bit 0: life crate when assisted; bits 1-3 (slot): its faces
    u8 unk_02[2];
    s16 trialKind;     // 0x04 - kind + 0x15 in a time trial, 0x1B read as 0x15
    u8 param6;         // 0x06 - 3/5: paramA (group id); 11/15: paramB
    u8 param7;         // 0x07 - 3/5: paramB (step count / step)
    union {            // 0x08
        u8 stepDelay;  // 3 (iron switch): fallSpeed
        s16 solidKind; // 5 (outline): solidKind
    } u08;
};

/* crate_placement.flags. When the player has died often enough to get
 * crate assistance (CreateCrate: GetDeaths >= GetCrateAssistDeaths), a
 * "?" or slot crate turns into an Aku Aku or a checkpoint crate. */
#define CRATE_PLACEMENT_FLAG_TRIAL_KIND        0x20 // `trialKind` is used (the special kinds always)
#define CRATE_PLACEMENT_FLAG_ASSIST_AKU_AKU    0x40
#define CRATE_PLACEMENT_FLAG_ASSIST_CHECKPOINT 0x80 // also: an activated "?"/slot crate is a checkpoint

/* crate_placement.options bit 0: an assisted "?" or slot crate becomes a
 * life crate (after the two flags above). */
#define CRATE_PLACEMENT_OPTION_ASSIST_LIFE 1

#endif // GUARD_CRATE_H
