#ifndef GUARD_ACTOR_SELF_HPP
#define GUARD_ACTOR_SELF_HPP

/* The 3D actors' base classes as C++ (#664, docs/cplusplus.md):
 *
 *   AnimPart   0x1C  none          src/actor/actor_anim.cpp
 *   ActorSelf  0x54  gActorVtable  src/actor/actor.cpp
 *   HpActor    0x58  none (no ROM table: every subclass has its own)
 *
 * `AnimPart` is the animation state, with no virtual methods: the
 * airship (a 0x1C-byte `new AnimPart`, airship.c) is nothing else.
 * `ActorSelf` adds the position, state and the circular actor list
 * rooted at gActorList; its vtable pointer follows its fields (+0x50),
 * as in every root class. `HpActor` adds the hit points of the jetpack
 * levels' and the 3D bosses' actors and vtable slots 4-6. The classes
 * built on these are in vehicle.hpp and boss_actors.hpp (and LogoActor
 * in frontend.hpp). `struct actor_self` (actor_self.h) is ActorSelf's C
 * view, for the files still in C.
 *
 * The actors live in IWRAM's heap (mem_alloc's MEM_HEAP_IWRAM flag),
 * which is AnimPart's own operator new and delete: the ROM's destructors
 * free with a direct mem_free, not OperatorDelete.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

extern "C" {
#include "core.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
}

class AnimPart
{
public:
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;               // 0x04
    s32 animTime;                    // 0x08 - Q8 frame accumulator
    s32 animIndex;                   // 0x0C - current index into anims
    u16 animTimer;                   // 0x10
    u8 animDone;                     // 0x12
    u8 unk_13;                       // 0x13
    s32 sortKey;                     // 0x14 - draw order (actor_self.h)
    s32 palette;                     // 0x18 - OBJ palette bank

    /* Inline: InitActorPart and CreateAirship expand it. */
    AnimPart(struct anim_frame_record *a, u32 *offsets, s32 pal)
    {
        anims = a;
        frameOffsets = offsets;
        palette = pal;
        SetAnim(0);
    }

    s32 GetAnimFrameBaseOffset(); // GetAnimFrameBaseOffset
    s32 GetAnimFrameAttr();       // GetAnimFrameAttr
    u8 *GetAnimFrameData();       // GetAnimFrameData
    void SetAnim(s32 idx);        // SetActorAnim

    /* SetAnim's body, inline (CreateActor's obstacles and goal, the polar
     * player's constructor). */
    void RestartAnim(s32 idx)
    {
        animIndex = idx;
        animTimer = anims[idx].duration;
        animDone = 0;
        animTime = 0;
    }

    static void *operator new(size_t size)
    {
        return mem_alloc(size, MEM_HEAP_IWRAM);
    }
    static void operator delete(void *p)
    {
        mem_free(p);
    }
};

COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(AnimPart) == 0x1C);

class ActorSelf : public AnimPart
{
public:
    s32 x;                            // 0x1C
    s32 y;                            // 0x20
    s32 z;                            // 0x24
    s32 state;                        // 0x28
    u8 visible;                       // 0x2C
    u8 unk_2D[3];                     // 0x2D
    struct anim_table_record *record; // 0x30
    s32 depth;                        // 0x34
    struct anim_box box;              // 0x38
    s32 stateTime;                    // 0x44 - frames spent in `state`
    ActorSelf *prev;                  // 0x48 - the circular actor list
    ActorSelf *next;                  // 0x4C
    // 0x50: the vtable pointer

    ActorSelf(const struct anim_table_record *rec, s32 x, s32 y, s32 z); // InitActorPart

    /* 1 DestroyActor: unlinks the actor from the actor list. Inline (the
     * definition below), as every subclass's destructor expands it;
     * actor.cpp has the out-of-line copy. */
    virtual ~ActorSelf();
    virtual void Update(); // 2 UpdateActor
    virtual void Draw();   // 3 DrawActor

    void UpdateDepth();               // UpdateActorDepth
    u8 GetRecordIndex();              // GetActorRecordIndex
    void EnterState(s32 st, s32 idx); // SetActorState: SetState out of line
    s32 GetZ();                       // GetActorZ
    s32 GetY();                       // GetActorY
    s32 GetX();                       // GetActorX
    struct anim_box GetWorldBox();    // GetActorWorldBox
    u8 IsVisible();                   // IsActorVisible

    /* State `st`, restarting animation sequence `idx` (actor_self.h's
     * ACTOR_SET_STATE). */
    void SetState(s32 st, s32 idx)
    {
        state = st;
        stateTime = 0;
        animIndex = idx;
        animTimer = anims[idx].duration;
        animDone = 0;
        animTime = 0;
    }

    /* The unlink from the actor list (the destructor's body). */
    void Unlink()
    {
        next->prev = prev;
        prev->next = next;
    }
};

COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(ActorSelf) == 0x54);
COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(ActorSelf) == sizeof(struct actor_self));

/* actor.cpp defines ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE for its own copy. */
#ifndef ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE
inline ActorSelf::~ActorSelf()
{
    Unlink();
}
#endif

/* The actors with hit points (vehicle.h's `struct actor_hp`): the jetpack
 * levels' and the 3D bosses'. Its constructor is inline (the C's
 * InitHpActor): every subclass's expands it. Slot 5 has no default in the
 * ROM: the class's vtable was never emitted, and each subclass returns its
 * own constant; the jetpack player's is this class's
 * (IsJetpackPlayerUnshootable). */
class HpActor : public ActorSelf
{
public:
    s32 hp; // 0x54

    HpActor(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 hp)
        : ActorSelf(rec, x, y, z)
    {
        this->hp = hp;
    }

    virtual void Damage(s32 amount); // 4 DamageActor (no damage)
    virtual s32 IsUnshootable();     // 5 IsJetpackPlayerUnshootable (0)
    virtual s32 GetHp();             // 6 GetActorHp
};

COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(HpActor) == 0x58);

#endif /* GUARD_ACTOR_SELF_HPP */
