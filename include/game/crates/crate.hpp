#ifndef GUARD_CRATE_HPP
#define GUARD_CRATE_HPP

/* The crate as C++ (#664, docs/cplusplus.md, parts 7e and 7g): the class
 * behind gCrateVtable (src/game/crates/), with its types (the outline group,
 * the placement record, the state bits). It has no C view. The object
 * the issue #12 "physics/collision" cluster (ROM 0x0800D040-0x0800FC70,
 * src/game/crates/crate_hit.cpp/crate_break.cpp) turned out to be is this
 * class; see docs/matching/archive/issue-12-physics-collision.md.
 *
 * No `#pragma interface`: g++ emits the vtable in crate_update.cpp (see
 * ctrl.hpp); cxx_symbols.txt maps the mangled names onto the C names.
 *
 * Crate is in the namespace Game::Crates (#1033, docs/cplusplus.md
 * "Namespaces"), with the crate list (crate_list.hpp) and the player's
 * collision queue (part_list.hpp): `_vt.Q34Game6Crates5Crate`. The
 * records (`struct crate_group`, `struct crate_placement`) and the
 * C-linkage ApplyCrateCollision stay global. */

#include "game/entities/sprite_obj.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "constants/crates.h"
#include "game/crates/crates.h"
}

namespace Game
{
namespace Crates
{
class Crate;
} // namespace Crates
} // namespace Game

/* A group of objects that trigger together (ActivateIronSwitchCrate
 * builds it): an iron switch crate's outline crates. */
struct crate_group {
    s32 count;
    Game::Crates::Crate *items[0];
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

/* Crate's `slotState` (slot crates), as UpdateSlotCrate and the crate_fields.cpp
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

/* A slot crate's paramA: the faces it may stop on (Crate::UpdateSlot
 * skips a face whose bit is clear; Crate::OpenSlot opens the face of the
 * phase it stopped on). */
#define CRATE_SLOT_FACE_MYSTERY 1 // phase 2, animation 0xB: a "?" crate (OpenMystery)
#define CRATE_SLOT_FACE_LIFE    2 // phase 1, animation 9: an extra life
#define CRATE_SLOT_FACE_TNT     4 // phase 3, animation 0xD: it explodes

/* An explosive crate's `blastState` (#843, docs/flags/crate_blast.md).
 * DropAbove arms the explosive crates it drops far only while the crate
 * it runs for is NONE. */
#define CRATE_BLAST_STATE_NONE    0    // not armed
#define CRATE_BLAST_STATE_ARMED   1    // dropped far enough to explode when it lands (UpdateFall)
#define CRATE_BLAST_STATE_BLASTED 0xff // BlastNearby has run

/* A crate's placement record: entry `slot` of the room's parameter
 * records (`gEntityFlags->list`, level_data.h), as CreateCrate reads it.
 * Bytes 6-8 are per-kind (see Crate's paramA/paramB/fallSpeed and the
 * word at 0x48). */
struct crate_placement {
    u8 flags;   // 0x00 - bit 5: has a time-trial kind; bit 6: Aku Aku when assisted
                //        (checkpoint: the respawn's X mirror); bit 7: checkpoint
                //        when assisted (CRATE_PLACEMENT_FLAG_*; bits 1-2, the basic
                //        crate's mirror: ENTITY_PARAMS_FLAG_*, constants/entity_params.h)
    u8 options; // 0x01 - bit 0: life crate when assisted; bits 1-3 (slot): its faces
                //        (CRATE_PLACEMENT_OPTION_*); bits 4-7 unused
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

/* On a checkpoint crate, bit 6 is instead the respawn's X mirror:
 * Crate::Create copies it into paramA (`TEST_BIT_SIGN`, core.h), and
 * breaking the crate hands it to SetCheckpointAtPlayer, which keeps it as
 * LevelProgress::checkpointFlags (CHECKPOINT_FLAG_MIRROR_X). Set on 5
 * checkpoint crates in the data. */
#define CRATE_PLACEMENT_BIT_CHECKPOINT_MIRROR_X  6
#define CRATE_PLACEMENT_FLAG_CHECKPOINT_MIRROR_X (1 << CRATE_PLACEMENT_BIT_CHECKPOINT_MIRROR_X)

/* crate_placement.options bit 0: an assisted "?" or slot crate becomes a
 * life crate (after the two flags above). */
#define CRATE_PLACEMENT_OPTION_ASSIST_LIFE 1

/* crate_placement.options bits 1-3: the faces a slot crate may stop on
 * (Crate::Create turns them into paramA's CRATE_SLOT_FACE_*). Every slot
 * crate in the data has the "?" and life faces. Bits 4-7 are never set
 * or read. */
#define CRATE_PLACEMENT_OPTION_SLOT_MYSTERY 2
#define CRATE_PLACEMENT_OPTION_SLOT_LIFE    4
#define CRATE_PLACEMENT_OPTION_SLOT_TNT     8

/* Crate::trialKind with no time-trial kind (ResetCrate):
 * Crate::ConvertForTimeTrial leaves such a crate alone. */
#define CRATE_TRIAL_KIND_NONE -1

namespace Game
{
namespace Crates
{

/* A crate (CreateCrate allocates 0x64 bytes): a sprite with the fall
 * state, the per-kind parameters and the stack links. `kind` is the crate
 * type (CRATE_KIND_*, constants/crates.h), `state` the state and busy
 * bits (CRATE_STATE_*). The head is Sprite's: `bank` the sprite bank,
 * `tag` the animation, `frame` its step (FinishBrokenCrate blasts on the
 * first tick of `stepTimer`), and `animDone` set once it ends (UpdateCrate
 * then resets `frame` and clears the busy bit). */
class Crate : public Game::Entities::Sprite
{
public:
    s32 fallTargetY;   // 0x40 - Q8 y the crate lands at (DropCratesAbove sets it;
                       //        UpdateCrateFall snaps `y` to it when the fall ends)
    s32 fallDistance;  // 0x44 - Q8 distance still to fall, 0: resting
    union {            // 0x48 - one word, read per kind (placement halfword +8 for outlines):
        s32 solidKind; //   5 (outline): the entity type (ENTITY_*) it turns into
                       //   (SolidifyOutlineCrate); also loaded from `trialKind` by
                       //   ConvertForTimeTrial
        // 3 (iron switch): its outline crates; NULL or PHYS_NO_GROUP: none
        struct crate_group *group;
        s32 bounceTimer; // 12 (bouncy wumpa): -0x2A until the first bounce, then 360
                         //   frames counted down by UpdateCrate (BounceWumpaCrate)
        s32 slotState;   // 15 (slot): bits 0-2 phase (face 0-3, bit 2 started), 3-5 spins
                         //   left at this stage, 6-7 stage (gSlotCrateTimers index; 0
                         //   idle); see UpdateSlotCrate and CRATE_SLOT_*
        s32 pressed;     // 6 (nitro switch): set once ActivateNitroSwitchCrate has fired
        s32 blastState;  // explosive kinds: CRATE_BLAST_STATE_*, ARMED once it has fallen
                         //   far enough to explode on landing (DropCratesAbove/
                         //   UpdateCrateFall), BLASTED once it has blasted (BlastNearbyCrates)
    };
    s8 fallSpeed; // 0x4C - UpdateCrateFall's per-tick speed (ramps up to 5); the iron
                  //        switch instead keeps its step delay here (placement byte 8,
                  //        reloaded into `timer` after each step)
    u8 state;     // 0x4D - low 7 bits: state (1: committed), bit 7: busy (CRATE_STATE_*)
    u8 kind;      // 0x4E - index into the gCrateKind* tables and gCrateHitResponse
    u8 timer;     // 0x4F
    u8 paramA;    // 0x50 - per-kind parameter (placement byte 6 for kinds 3/5):
                  //        1 (checkpoint): placement flag bit 6, the respawn's X
    //        mirror, handed to SetCheckpointAtPlayer; 3 (iron switch): group id, then the
    //        step counter once activated; 5 (outline): group id (matches
    //        its switch's); 12 (bouncy wumpa): set while a bounce
    //        animation runs; 15 (slot): mask of the faces it may stop on
    //        (CRATE_SLOT_FACE_*, from placement byte 1 bits 1-3)
    u8 paramB;         // 0x51 - per-kind parameter (placement byte 6/7): 3 (iron switch):
                       //        number of steps; 5 (outline): the step it solidifies on;
                       //        11 ("?"): contents (9: random, OpenMysteryCrate); 12 (bouncy
                       //        wumpa): bounces so far (breaks after 5); 15 (slot): placement
                       //        byte 6
    s32 trialKind;     // 0x54 - the entity type (ENTITY_*) it becomes in a time trial
                       //        (placement halfword +4, ENTITY_NITRO_SWITCH_CRATE read as
                       //        ENTITY_BASIC_CRATE); CRATE_TRIAL_KIND_NONE: none. See
                       //        ConvertForTimeTrial
    u8 touched;        // 0x58
    u8 groupAllocated; // 0x59 - `group` was allocated (ActivateIronSwitchCrate)
    Crate *above;      // 0x5C - the crate stacked on this one
    Crate *below;      // 0x60 - the crate this one stands on

    Crate();               // InitCrate: inline in crate_create.cpp (`new Crate`)
    virtual void Update(); // 3 UpdateCrate
    virtual void Draw();   // 4 DrawCrate
    virtual s32 IsInsideRect(struct aabb *box);                    // 8 IsCrateInsideRect
    virtual s32 GetClassId();                                      // 9 GetCrateClassId: 3
    virtual ~Crate();                                              // 10 DestroyCrate
    static Crate *Create(u16 id, u16 x, u16 y, u16 slot, u8 type); // CreateCrate

    /* gEntitySpawnFuncs' crate slots (spawn_crates.cpp, #915): each
     * creates its kind (CRATE_KIND_*). */
    static void SpawnBasic(u32 id, u16 x, u16 y, u16 param);       // SpawnBasicCrate
    static void SpawnCheckpoint(u32 id, u16 x, u16 y, u16 param);  // SpawnCheckpointCrate
    static void SpawnAkuAku(u32 id, u16 x, u16 y, u16 param);      // SpawnAkuAkuCrate
    static void SpawnIronSwitch(u32 id, u16 x, u16 y, u16 param);  // SpawnIronSwitchCrate
    static void SpawnArrow(u32 id, u16 x, u16 y, u16 param);       // SpawnArrowCrate
    static void SpawnOutline(u32 id, u16 x, u16 y, u16 param);     // SpawnOutlineCrate
    static void SpawnNitroSwitch(u32 id, u16 x, u16 y, u16 param); // SpawnNitroSwitchCrate
    static void SpawnIron(u32 id, u16 x, u16 y, u16 param);        // SpawnIronCrate
    static void SpawnIronArrow(u32 id, u16 x, u16 y, u16 param);   // SpawnIronArrowCrate
    static void SpawnLife(u32 id, u16 x, u16 y, u16 param);        // SpawnLifeCrate
    static void SpawnNitro(u32 id, u16 x, u16 y, u16 param);       // SpawnNitroCrate
    static void SpawnMystery(u32 id, u16 x, u16 y, u16 param);     // SpawnMysteryCrate
    static void SpawnBouncyWumpa(u32 id, u16 x, u16 y, u16 param); // SpawnBouncyWumpaCrate
    static void SpawnReinforced(u32 id, u16 x, u16 y, u16 param);  // SpawnReinforcedCrate
    static void SpawnTnt(u32 id, u16 x, u16 y, u16 param);         // SpawnTntCrate
    static void SpawnSlot(u32 id, u16 x, u16 y, u16 param);        // SpawnSlotCrate
    static void SpawnTime1(u32 id, u16 x, u16 y, u16 param);       // SpawnTimeCrate1
    static void SpawnTime2(u32 id, u16 x, u16 y, u16 param);       // SpawnTimeCrate2
    static void SpawnTime3(u32 id, u16 x, u16 y, u16 param);       // SpawnTimeCrate3


    void Reset(); // ResetCrate
    Crate *GetBelow();
    Crate *GetAbove();
    void SetBelow(Crate *crate);
    void SetAbove(Crate *crate);
    Crate *GetTop();
    Crate *GetBottom();
    s32 CollideWithPlayer(u32 idx, s32 testX, s32 testY); // CollideCrateWithPlayer
    u8 PlayerHitboxOverlapsAt(const struct hitbox_quad *quad, struct aabb *box, s32 xOffset,
                              s32 yOffset);
    Crate *ResolveStackHit(struct aabb *box, u8 *foundFlag); // ResolveStackCrateHit
    void BreakIfTouchedByPlayer();                           // BreakCrateTouchedByPlayer
    u8 PlayerAnimWouldTouch(s32 action);                     // PlayerAnimWouldTouchCrate
    void OpenLife(bool flag6);                               // OpenLifeCrate
    void OpenAkuAku();                                       // OpenAkuAkuCrate
    u8 IsKindBreakable(u32 kind);                            // IsCrateKindBreakable

    /* The slot crate's word (crate_fields.cpp). */
    u32 GetSlotStage();
    void DecrementSlotStage();
    void SetSlotStage(u8 stage);
    void ClearSlotStage();
    u32 GetSlotSpins();
    void DecrementSlotSpins();
    void SetSlotSpins(u8 spins);
    void SetSlotPhase(u8 phase);
    u32 GetSlotPhase();
    /* The other accessors (crate_fields.cpp). */
    void SetKind(u8 value);
    u8 GetKind();
    void SetFallDistance(s32 value);
    s32 GetFallDistance();
    void SetState(u8 value);
    u32 GetState();
    void SetFallSpeed(u8 value);
    s32 GetFallSpeed();
    u32 IsBusy();
    void SetBusy();
    void ClearBusy();
    void SetTouched(u8 value);
    u8 GetParamB();
    u8 GetParamA();
    void SetSolidKind(u8 value);
    void SetTrialKind(s32 value);
    s32 GetTrialKind();

    /* The hits, breaks and explosions (crate_break.cpp, part 7g; the
     * switches and outlines in crate_switches.cpp, FinishBroken on in
     * crate_states.cpp). */
    void QueuePlayerCollision(s32 idx); // QueueCratePlayerCollision
    void ApplyCollision(s32 attack, s32 code, s32 edge, s32 depth, struct vec2 pos, s32 hit,
                        bool limited, bool above, bool forced); // ApplyCrateCollision
    void ClearStackTouched();                                   // ClearCrateStackTouched
    void MarkStackTouched(struct aabb *box);                    // MarkCrateStackTouched
    void BounceWumpa();                                         // BounceWumpaCrate
    void LightTnt();                                            // LightTntCrate
    void OpenCheckpoint();                                      // OpenCheckpointCrate
    void BreakInStack(u8 flag, bool once, u32 dir);             // BreakCrateInStack
    void Break(u32 byBlast);                                    // BreakCrate
    void OpenMystery(bool flag);                                // OpenMysteryCrate
    void OpenSlot(bool flag);                                   // OpenSlotCrate
    void DropAbove();                                           // DropCratesAbove
    void Explode(u8 near);                                      // ExplodeCrate
    void BlastNearby(s32 dist);                                 // BlastNearbyCrates
    void ActivateNitroSwitch();                                 // ActivateNitroSwitchCrate
    void ActivateIronSwitch();                                  // ActivateIronSwitchCrate
    void SolidifyOutlines();                                    // SolidifyOutlineCrates
    void SolidifyOutline();                                     // SolidifyOutlineCrate
    void FinishBroken();                                        // FinishBrokenCrate
    void UpdateTntCountdown();
    void UpdateSlot(); // UpdateSlotCrate
    void UpdateFall(); // UpdateCrateFall

    /* The passes over every crate of the crate list (#915): static, as
     * they take no crate. They walk gCrateList's slots and act on the
     * entries whose class id is 3 (the crates), and they sit among
     * Crate's methods in the ROM (crate_switches.cpp, crate_time_trial.cpp).
     * cxx_symbols.txt maps them to their C names. */
    static void UpdateAll();      // UpdateCrates: play_room.cpp, once a frame
    static void DetonateNitros(); // DetonateNitroCrates
    static void BreakInArea(s32 x, s32 y, s32 dist, s32 height); // BreakCratesInArea
    static void ConvertForTimeTrial(); // ConvertCratesForTimeTrial: StartTimeTrial

    /* Sets `frame` to `idx`, clamped to the animation's last step. `idx`
     * being a parameter keeps a constant argument in its own register,
     * which the callers' later stores reuse (BreakCrate, UpdateCrate). */
    void ClampFrame(s32 idx)
    {
        u8 n = bank->anims[tag].frameCount;

        CLAMP_INDEX(idx, n);
        frame = idx;
    }
};

} // namespace Crates
} // namespace Game

/* The line steppers (line_steppers.hpp), inline: FindLineCrossing
 * (crate_reset.cpp) inlines them. crate.cpp, which has their out-of-line
 * copies, defines CRATE_LINE_STEP itself and includes them at its end. */
#ifndef CRATE_LINE_STEP
#define CRATE_LINE_STEP inline
#include "engine/util/line_steppers.hpp"
#endif

/* codegen: Crate::ApplyCollision under its C name (cxx_symbols.txt), as
 * CollisionQueue::Resolve (collision_queue.cpp) calls it: the three bool
 * flags passed as one-byte structs, which go to their stack slots with
 * `strb`, as in the ROM (a bool argument is stored as a word). */
extern "C" void ApplyCrateCollision(Game::Crates::Crate *self, s32 kind, s32 code, s32 edge,
                                    s32 depth, struct vec2 pos, s32 hit, struct byte_arg p20,
                                    struct byte_arg p21, struct byte_arg pforced);

COMPILE_TIME_ASSERT(crate_hpp, sizeof(Game::Crates::Crate) == 0x64);

#endif /* !GUARD_CRATE_HPP */
