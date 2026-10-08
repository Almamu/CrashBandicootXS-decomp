#ifndef GUARD_ACTOR_SELF_HPP
#define GUARD_ACTOR_SELF_HPP

/* The 3D actor base class as C++ (#664, docs/cplusplus.md):
 *
 *   ActorSelf  0x54  gActorVtable  src/actor/actor.c (still C)
 *
 * Only what the C++ objects built on it need so far (part 10b: the
 * company logo actor, LogoActor in frontend.hpp). Its code is still C
 * (InitActorPart, DestroyActor, UpdateActor, DrawActor, ...):
 * cxx_symbols.txt maps the methods to those names. `struct actor_self`
 * (actor_self.h) is its C view.
 *
 * The vtable pointer follows the class's own fields (+0x50), as in every
 * root class. The actors live in IWRAM's heap (mem_alloc's 0x80000000
 * flag), which is the class's own operator new and delete: the ROM's
 * destructors free with a direct mem_free, not OperatorDelete.
 *
 * `#pragma interface`: no vtable is emitted (see ctrl.hpp). */
#pragma interface

extern "C" {
#include "core.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
}

class ActorSelf
{
public:
    struct anim_frame_record *anims;  // 0x00
    u32 *frameOffsets;                // 0x04
    s32 animTime;                     // 0x08 - Q8 frame accumulator
    s32 animIndex;                    // 0x0C - current index into anims
    u16 animTimer;                    // 0x10
    u8 animDone;                      // 0x12
    u8 unk_13;                        // 0x13
    s32 sortKey;                      // 0x14
    s32 palette;                      // 0x18
    s32 x;                            // 0x1C
    s32 y;                            // 0x20
    s32 z;                            // 0x24
    s32 state;                        // 0x28
    u8 visible;                       // 0x2C
    u8 unk_2D[3];                     // 0x2D
    struct anim_table_record *record; // 0x30
    s32 depth;                        // 0x34
    u8 box[0xC];                      // 0x38
    s32 stateTime;                    // 0x44 - frames spent in `state`
    ActorSelf *prev;                  // 0x48 - the circular actor list
    ActorSelf *next;                  // 0x4C
    // 0x50: the vtable pointer

    ActorSelf(const struct anim_table_record *rec, s32 x, s32 y, s32 z); // InitActorPart

    /* 1 DestroyActor: unlinks the actor from the actor list. */
    virtual ~ActorSelf()
    {
        next->prev = prev;
        prev->next = next;
    }
    virtual void Update(); // 2 UpdateActor
    virtual void Draw();   // 3 DrawActor

    s32 GetAnimFrameBaseOffset(); // GetAnimFrameBaseOffset

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

    static void *operator new(size_t size)
    {
        return mem_alloc(size, 0x80000000);
    }
    static void operator delete(void *p)
    {
        mem_free(p);
    }
};

COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(ActorSelf) == 0x54);
COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(ActorSelf) == sizeof(struct actor_self));

#endif /* GUARD_ACTOR_SELF_HPP */
