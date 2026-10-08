#ifndef GUARD_CRATE_HPP
#define GUARD_CRATE_HPP

/* The crate as C++ (#664, docs/cplusplus.md, parts 7e and 7g): the class
 * behind gCrateVtable (src/crates/). It has no C view; crate.h has its
 * C types (the outline group, the placement record, the state bits).
 *
 * No `#pragma interface`: g++ emits the vtable in crate_update.cpp (see
 * ctrl.hpp); cxx_symbols.txt maps the mangled names onto the C names. */

#include "sprite_obj.hpp"

extern "C" {
#include "crate.h"
#include "crates.h"
}

/* A crate (CreateCrate allocates 0x64 bytes): a sprite with the fall
 * state, the per-kind parameters and the stack links. `kind` is the crate
 * type (CRATE_KIND_*, constants/crates.h), `state` the state and busy
 * bits (CRATE_STATE_*). The head is Sprite's: `anim` the sprite bank,
 * `tag` the animation, `frame` its step (FinishBrokenCrate blasts on the
 * first tick of `stepTimer`), and `animDone` set once it ends (UpdateCrate
 * then resets `frame` and clears the busy bit). */
class Crate : public Sprite
{
public:
    s32 fallTargetY;   // 0x40 - Q8 y the crate lands at (DropCratesAbove sets it;
                       //        UpdateCrateFall snaps `y` to it when the fall ends)
    s32 fallDistance;  // 0x44 - Q8 distance still to fall, 0: resting
    union {            // 0x48 - one word, read per kind (placement halfword +8 for outlines):
        s32 solidKind; //   5 (outline): the entity type (ENTITY_*) it turns into
                       //   (SolidifyOutlineCrate); also loaded from `trialKind` by
                       //   ConvertCratesForTimeTrial
        // 3 (iron switch): its outline crates; NULL or PHYS_NO_GROUP: none
        struct crate_group *group;
        s32 bounceTimer; // 12 (bouncy wumpa): -0x2A until the first bounce, then 360
                         //   frames counted down by UpdateCrate (BounceWumpaCrate)
        s32 slotState;   // 15 (slot): bits 0-2 phase (face 0-3, bit 2 started), 3-5 spins
                         //   left at this stage, 6-7 stage (gSlotCrateTimers index; 0
                         //   idle); see UpdateSlotCrate and CRATE_SLOT_*
        s32 pressed;     // 6 (nitro switch): set once ActivateNitroSwitchCrate has fired
        s32 blastState;  // explosive kinds: 1 once it has fallen far enough to explode
                         //   on landing (DropCratesAbove/UpdateCrateFall), 0xFF once it
                         //   has blasted (BlastNearbyCrates)
    };
    s8 fallSpeed; // 0x4C - UpdateCrateFall's per-tick speed (ramps up to 5); the iron
                  //        switch instead keeps its step delay here (placement byte 8,
                  //        reloaded into `timer` after each step)
    u8 state;     // 0x4D - low 7 bits: state (1: committed), bit 7: busy (CRATE_STATE_*)
    u8 kind;      // 0x4E - index into the gCrateKind* tables and gCrateHitResponse
    u8 timer;     // 0x4F
    u8 paramA;    // 0x50 - per-kind parameter (placement byte 6 for kinds 3/5):
                  //        1 (checkpoint): placement flag bit 6, handed to
                  //        SetCheckpointAtPlayer; 3 (iron switch): group id, then the
                  //        step counter once activated; 5 (outline): group id (matches
                  //        its switch's); 12 (bouncy wumpa): set while a bounce
                  //        animation runs; 15 (slot): mask of the faces it may stop on
                  //        (placement byte 1 bits 1-3)
    u8 paramB;    // 0x51 - per-kind parameter (placement byte 6/7): 3 (iron switch):
                  //        number of steps; 5 (outline): the step it solidifies on;
                  //        11 ("?"): contents (9: random, OpenMysteryCrate); 12 (bouncy
                  //        wumpa): bounces so far (breaks after 5); 15 (slot): placement
                  //        byte 6
    u8 unk_52[2];
    s32 trialKind;     // 0x54 - the entity type (ENTITY_*) it becomes in a time trial
                       //        (placement halfword +4, ENTITY_NITRO_SWITCH_CRATE read as
                       //        ENTITY_BASIC_CRATE); -1: none (ResetCrate). See
                       //        ConvertCratesForTimeTrial
    u8 touched;        // 0x58
    u8 groupAllocated; // 0x59 - `group` was allocated (ActivateIronSwitchCrate)
    u8 unk_5A[2];
    Crate *above; // 0x5C - the crate stacked on this one
    Crate *below; // 0x60 - the crate this one stands on

    Crate();               // InitCrate: inline in crate_create.cpp (`new Crate`)
    virtual void Update(); // 3 UpdateCrate
    virtual void Draw();   // 4 DrawCrate
    virtual s32 IsInsideRect(struct aabb *box);                    // 8 IsCrateInsideRect
    virtual s32 GetClassId();                                      // 9 GetCrateClassId: 3
    virtual ~Crate();                                              // 10 DestroyCrate
    static Crate *Create(u16 id, u16 x, u16 y, u16 slot, u8 type); // CreateCrate

    void Reset(); // ResetCrate
    Crate *GetBelow();
    Crate *GetAbove();
    void SetBelow(Crate *crate);
    void SetAbove(Crate *crate);
    Crate *GetTop();
    Crate *GetBottom();
    s32 CollideWithPlayer(u32 idx, s32 testX, s32 testY); // CollideCrateWithPlayer
    u8 PlayerHitboxOverlapsAt(struct hitbox_quad *quad, struct aabb *box, s32 xOffset, s32 yOffset);
    Crate *ResolveStackHit(struct aabb *box, u8 *foundFlag); // ResolveStackCrateHit
    void BreakIfTouchedByPlayer();                           // BreakCrateTouchedByPlayer
    u8 PlayerAnimWouldTouch(s32 action);                     // PlayerAnimWouldTouchCrate
    void OpenLife(bool flag6);                               // OpenLifeCrate
    void OpenAkuAku();                                       // OpenAkuAkuCrate
    u8 IsKindBreakable(u32 kind);                            // IsCrateKindBreakable

    /* The slot crate's word (slot_crate.cpp). */
    u32 GetSlotStage();
    void DecrementSlotStage();
    void SetSlotStage(u32 stage);
    void ClearSlotStage();
    u32 GetSlotSpins();
    void DecrementSlotSpins();
    void SetSlotSpins(u32 spins);
    void SetSlotPhase(u32 phase);
    u32 GetSlotPhase();
    /* The other accessors (slot_crate.cpp). */
    void SetKind(u8 value);
    u8 GetKind();
    void SetFallDistance(s32 value);
    s32 GetFallDistance();
    void SetState(u32 value);
    u32 GetState();
    void SetFallSpeed(u8 value);
    s32 GetFallSpeed();
    u32 IsBusy();
    void SetBusy();
    void ClearBusy();
    void SetTouched(u8 value);
    u8 GetParamB();
    u8 GetParamA();
    void SetSolidKind(u32 value);
    void SetTrialKind(s32 value);
    s32 GetTrialKind();

    /* The hits, breaks and explosions (crate_break.cpp, part 7g). */
    void QueuePlayerCollision(s32 idx); // QueueCratePlayerCollision
    void ApplyCollision(s32 attack, s32 code, s32 edge, s32 depth, struct e08c_pos pos, s32 hit,
                        bool limited, bool above, bool forced); // ApplyCrateCollision
    void ClearStackTouched();                                   // ClearCrateStackTouched
    void MarkStackTouched(struct aabb *box);                    // MarkCrateStackTouched
    void BounceWumpa();                                         // BounceWumpaCrate
    void LightTnt();                                            // LightTntCrate
    void OpenCheckpoint();                                      // OpenCheckpointCrate
    void BreakInStack(u8 flag, bool once, u32 dir);             // BreakCrateInStack
    void Break(u32 arg1);                                       // BreakCrate
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

    /* Switches to animation `t` from its start: the three-call idiom
     * every state change uses. */
    void SetTag(u8 t)
    {
        tag = t;
        ResetFrameTimer();
        ResetFrameIndex();
        SetAnimDone(0);
    }

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

/* The line steppers (crate_line_step.hpp), inline: FindLineCrossing
 * (crate_reset.cpp) inlines them. crate.cpp, which has their out-of-line
 * copies, defines CRATE_LINE_STEP itself and includes them at its end. */
#ifndef CRATE_LINE_STEP
#define CRATE_LINE_STEP inline
#include "crate_line_step.hpp"
#endif

/* codegen: Crate::ApplyCollision under its C name (cxx_symbols.txt), as
 * CollisionQueue::Resolve (collision_queue.cpp) calls it: the three bool
 * flags passed as one-byte structs, which go to their stack slots with
 * `strb`, as in the ROM (a bool argument is stored as a word). */
extern "C" void ApplyCrateCollision(Crate *self, s32 kind, s32 code, s32 edge, s32 depth,
                                    struct e08c_pos pos, s32 hit, struct byte_arg p20,
                                    struct byte_arg p21, struct byte_arg pforced);

COMPILE_TIME_ASSERT(crate_hpp, sizeof(Crate) == 0x64);

#endif /* !GUARD_CRATE_HPP */
