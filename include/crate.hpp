#ifndef GUARD_CRATE_HPP
#define GUARD_CRATE_HPP

/* The crate as C++ (#664, docs/cplusplus.md, part 7e): the class behind
 * gCrateVtable (src/crates/). crate.h's `struct crate` is its C view, for
 * the files that are still C (crate_break.c, the crate list and grid,
 * collision_queue.c, room_entities.c).
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp); cxx_symbols.txt
 * maps the mangled names onto the C names. */
#pragma interface

#include "sprite_obj.hpp"

extern "C" {
#include "crate.h"
#include "crates.h"
}

/* A crate (CreateCrate allocates 0x64 bytes): a sprite with the fall
 * state, the per-kind parameters and the stack links. `kind` is the crate
 * type (CRATE_KIND_*, constants/crates.h), `state` the state and busy
 * bits (CRATE_STATE_*). The per-kind word at 0x48 is documented in
 * crate.h. */
class Crate : public Sprite
{
public:
    s32 fallTargetY;  // 0x40 - Q8 y the crate lands at
    s32 fallDistance; // 0x44 - Q8 distance still to fall, 0: resting
    union {           // 0x48 - one word, read per kind (crate.h)
        s32 solidKind;
        struct crate_group *group;
        s32 bounceTimer;
        s32 slotState;
        s32 pressed;
        s32 blastState;
    };
    s8 fallSpeed; // 0x4C - UpdateCrateFall's speed; the iron switch's step delay
    u8 state;     // 0x4D - low 7 bits: state (1: committed), bit 7: busy
    u8 kind;      // 0x4E
    u8 timer;     // 0x4F
    u8 paramA;    // 0x50 - per-kind parameter (crate.h)
    u8 paramB;    // 0x51
    u8 unk_52[2];
    s32 trialKind;     // 0x54 - the entity type it becomes in a time trial; -1: none
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

    /* Still C, in crate_break.c (part 7g). */
    void QueuePlayerCollision(s32 idx); // QueueCratePlayerCollision
    void BreakInStack(u32 arg1, u32 arg2, u32 dir);
    void Explode(u8 near);
    void UpdateTntCountdown();
    void UpdateSlot();       // UpdateSlotCrate
    void UpdateFall();       // UpdateCrateFall
    void FinishBroken();     // FinishBrokenCrate
    void SolidifyOutlines(); // SolidifyOutlineCrates
    void SolidifyOutline();  // SolidifyOutlineCrate

    /* Switches to animation `t` from its start: the three-call idiom
     * every state change uses (crate.h's PhysSetTag). */
    void SetTag(u8 t)
    {
        tag = t;
        ResetFrameTimer();
        ResetFrameIndex();
        SetAnimDone(0);
    }

    /* Sets `frame` to `idx`, clamped to the animation's last step. `idx`
     * being a parameter keeps a constant argument in its own register
     * (crate.h's PhysSetFrame). */
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

COMPILE_TIME_ASSERT(crate_hpp, sizeof(Crate) == 0x64);
COMPILE_TIME_ASSERT(crate_hpp, sizeof(Crate) == sizeof(struct crate));

#endif /* !GUARD_CRATE_HPP */
