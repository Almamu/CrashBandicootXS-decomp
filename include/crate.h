#ifndef GUARD_CRATE_H
#define GUARD_CRATE_H

#include "gobj_1a794.h"
#include "math_util.h"
#include "constants/crates.h"

/* The crate (CreateCrate, gCrateVtable): the object the issue #12
 * "physics/collision" cluster (ROM 0x0800D040-0x0800FC70,
 * src/crates/crate_hit.cpp/crate_break.cpp) turned out to be. `kind`
 * is the crate type CreateCrate picks (CRATE_KIND_*, constants/crates.h,
 * generated from data/levels/crate_kinds.json). Only the fields those functions touch are named;
 * the head (position, flags, anim table/tag, mirror bits) has the same
 * layout as Sprite's (sprite_obj.hpp). See docs/matching/archive/issue-12-physics-collision.md.
 *
 * `struct crate` is the C view of the `Crate` class (include/crate.hpp,
 * #664), for the files that are still C; crate.hpp checks that the sizes
 * agree. */
struct crate;

/* A group of objects that trigger together (ActivateIronSwitchCrate builds it). */
struct crate_group {
    s32 count;
    struct crate *items[0];
};

#define PHYS_NO_GROUP ((struct crate_group *)-1)
#define PHYS_HAS_GROUP(g) ((u32)(g) + 1 > 1)

struct crate {
    s32 x;  // 0x00
    s32 y;  // 0x04
    u16 id; // 0x08 - 0xffff: none
    u8 unk_0A[2];
    u8 flags; // 0x0C - bit 0: removed (Entity's `gone`)
    u8 unk_0D[0xB];
    const struct vtable_slot *vtable; // 0x18 - gCrateVtable
    u8 unk_1C[4];
    struct anim_table *anim; // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4; // 0x28
    s32 flipX:1;    //      bit 4: X mirrored
    s32 flipY:1;    //      bit 5: Y mirrored
    u32 unk_28_6:2;
    u32 slot:4; // 0x29 - palette/tile slot
    u32 unk_29_4:4;
    u32 unk_2A:16;
    u8 animating; // 0x2C - nonzero while the keyframe timer runs (box_part.h)
    u8 tag;       // 0x2D
    u8 unk_2E[2];
    s32 frame; // 0x30
    // 0x34 - ticks spent on the current step (FinishBrokenCrate blasts on its first tick)
    s32 stepTimer;
    u8 animDone; // 0x38 - set once the animation ends; UpdateCrate then resets `frame`
                 //        and clears the busy bit
    u8 unk_39[7];
    s32 fallTargetY;  // 0x40 - Q8 y the crate lands at (DropCratesAbove sets it; UpdateCrateFall
                      //        snaps `y` to it when the fall ends)
    s32 fallDistance; // 0x44 - Q8 distance still to fall, 0: resting
    union {           // 0x48 - one word, read per kind (placement halfword +8 for outlines):
        s32 solidKind; //   5 (outline): the entity type (ENTITY_*) it turns into (SolidifyOutlineCrate); also
                       //   loaded from `trialKind` by ConvertCratesForTimeTrial
        // 3 (iron switch): its outline crates; NULL or PHYS_NO_GROUP: none
        struct crate_group *group;
        s32 bounceTimer; // 12 (bouncy wumpa): -0x2A until the first bounce, then 360 frames
                         //   counted down by UpdateCrate (BounceWumpaCrate)
        s32 slotState;   // 15 (slot): bits 0-2 phase (face 0-3, bit 2 started), 3-5 spins
                         //   left at this stage, 6-7 stage (gSlotCrateTimers index; 0 idle);
                         //   see UpdateSlotCrate and CRATE_SLOT_*
        s32 pressed;     // 6 (nitro switch): set once ActivateNitroSwitchCrate has fired
        s32 blastState;  // explosive kinds: 1 once it has fallen far enough to explode on
                         //   landing (DropCratesAbove/UpdateCrateFall), 0xFF once it has
                         //   blasted (BlastNearbyCrates)
    } u48;
    s8 fallSpeed; // 0x4C - UpdateCrateFall's per-tick speed (ramps up to 5); the iron switch
                  //        instead keeps its step delay here (placement byte 8, reloaded
                  //        into `timer` after each step)
    u8 state;     // 0x4D - low 7 bits: state (1: committed), bit 7: busy (CRATE_STATE_*)
    u8 kind;      // 0x4E - index into the gCrateKind* tables and gCrateHitResponse
    u8 timer;     // 0x4F
    u8 paramA;    // 0x50 - per-kind parameter (placement byte 6 for kinds 3/5):
                  //        1 (checkpoint): placement flag bit 6, handed to SetCheckpointAtPlayer;
                  //        3 (iron switch): group id, then the step counter once activated;
                  //        5 (outline): group id (matches its switch's);
                  //        12 (bouncy wumpa): set while a bounce animation runs;
                  //        15 (slot): mask of the faces it may stop on (placement byte 1 bits 1-3)
    // 0x51 - per-kind parameter (placement byte 6/7):
    //        3 (iron switch): number of steps; 5 (outline): the step it solidifies on;
    //        11 ("?"): contents (9: random, OpenMysteryCrate);
    //        12 (bouncy wumpa): bounces so far (breaks after 5); 15 (slot): placement byte 6
    u8 paramB;
    u8 unk_52[2];
    // 0x54 - the entity type (ENTITY_*) the crate becomes in a time trial (placement
    //        halfword +4, ENTITY_NITRO_SWITCH_CRATE read as ENTITY_BASIC_CRATE); -1: none (ResetCrate). See ConvertCratesForTimeTrial
    s32 trialKind;
    u8 touched;        // 0x58
    u8 groupAllocated; // 0x59 - ActivateIronSwitchCrate allocated `u48.group` (freed when it fires)
    u8 unk_5A[2];
    struct crate *above; // 0x5C - the crate stacked on this one (GetCrateAbove/SetCrateAbove)
    struct crate *below; // 0x60 - the crate this one stands on (GetCrateBelow/SetCrateBelow)
};

/* crate.state (box_part.h's `physMode`, the same byte): the low 7 bits
 * are the state (0: idle; 1: committed - the physics AABB tests skip it
 * and UpdateCrate runs FinishBrokenCrate), bit 7 is busy (set when a hit
 * starts an animation; UpdateCrate clears it, with gPlayer->busy, once
 * `animDone` is set). */
#define CRATE_STATE_MASK 0x7f
#define CRATE_STATE_BUSY 0x80

/* crate.u48.slotState (slot crates), as UpdateSlotCrate and the slot_crate.cpp
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
 * Bytes 6-8 are per-kind (see `struct crate` paramA/paramB/fallSpeed/u48). */
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
        s16 solidKind; // 5 (outline): u48.solidKind
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
