#ifndef GUARD_ACTOR_SELF_HPP
#define GUARD_ACTOR_SELF_HPP

/* The 3D actors' base classes as C++ (#664, docs/cplusplus.md):
 *
 *   AnimPart   0x1C  none          src/actor/anim_part.cpp
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
 * in frontend.hpp). They have no C view (actor_self.h's `struct
 * actor_self` went with its last C reader, #754); actor_self.h keeps the
 * plain records they use.
 *
 * The actors live in IWRAM's heap (mem_alloc's MEM_HEAP_IWRAM flag),
 * which is AnimPart's own operator new and delete: the ROM's destructors
 * free with a direct mem_free, not OperatorDelete.
 *
 * No `#pragma interface`: g++ emits ActorSelf's vtable (gActorVtable) in
 * actor.cpp and HpActor's in inline_copies_actors.cpp; AnimPart has none (see
 * ctrl.hpp). */

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "gfx.h"
}

/* ActorSelf::sortKey's bits. The low 15 bits are the draw-order key
 * ((depth >> 1) & 0x7f80 | ((|x| + |y|) >> 11) & 0x7f); InitActorPart,
 * UpdateActor and UpdateActorDepth set bit 15 when the actor's depth is past
 * GetActorBgLayerDepth(), and the draw functions then give its sprite OAM
 * priority 2 (attr 2 | 0x800) so it goes behind the BG layer. */
#define SORT_KEY_FLAG_BEHIND_BG 0x8000

class AnimPart
{
public:
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;               // 0x04
    s32 animTime;                    // 0x08 - Q8 frame accumulator
    s32 animIndex;                   // 0x0C - current index into anims
    u16 animTimer;                   // 0x10
    u8 animDone;                     // 0x12 - set once the current sequence has played through
    // 0x14 - draw order: RunActorCategoryFrame heapsorts the draw list by it
    //        (HeapSortActorsByKey); bit 15 also sets OAM priority
    //        (SORT_KEY_FLAG_BEHIND_BG, actor_self.h)
    s32 sortKey;
    s32 palette; // 0x18 - OBJ palette bank (OAM attr 2 << 12), from anim_table_record.palette

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

    /* The current frame's entry in frameOffsets (the keyframe's
     * frameIndex plus the whole frames of animTime), as a pointer: what
     * the 3D draws inline (polar_player.cpp, yeti_graphics.cpp). Unlike
     * GetAnimFrameData, no gCategorySpriteSheet base is added. */
    u8 *CurFrame()
    {
        s32 t = Q8_TO_INT(animTime);

        return (u8 *)frameOffsets[anims[animIndex].frameIndex + t];
    }

    /* GetAnimFrameAttr's body, inline: the keyframe's attr in the high
     * half (OAM attribute 1). */
    s32 CurAttr()
    {
        s32 idx = animIndex;
        struct anim_frame_record *table = anims;

        return (s32)table[idx].attr << 16;
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
    s32 x;     // 0x1C
    s32 y;     // 0x20
    s32 z;     // 0x24
    s32 state; // 0x28
    // 0x2C - nonzero: drawn (RunActorCategoryFrame only puts these in
    //        gActorDrawList); InitActorPart sets it to 1
    u8 visible;
    // 0x30 - the record InitActorPart was given (actor_anim.h); the draw
    //        functions scale by its baseDepth
    struct anim_table_record *record;
    s32 depth;           // 0x34
    struct anim_box box; // 0x38 - collision box, copied from record->box_14 by InitActorPart
    s32 stateTime;       // 0x44 - frames spent in `state`
    // 0x48 - the circular actor list (rooted at the player, gActorList):
    //        InitActorPart appends before the head; the draw and teardown
    //        loops walk `next` from the head
    ActorSelf *prev;
    ActorSelf *next; // 0x4C
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

    /* State `st`, restarting animation sequence `idx`. */
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

    void DrawFrameAt(s32 screenX, s32 screenY, s32 scale);
};

COMPILE_TIME_ASSERT(actor_self_hpp, sizeof(ActorSelf) == 0x54);

/* actor.cpp defines ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE for its own copy. */
#ifndef ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE
inline ActorSelf::~ActorSelf()
{
    Unlink();
}
#endif

/* The current frame at screen position (screenX, screenY), `scale` the
 * depth scale (0x100 is 1:1): double size below 0x100, affine unless
 * 1:1, centred on the frame's size and culled off screen, OAM priority 2
 * when SORT_KEY_FLAG_BEHIND_BG. ActorSelf::Draw (actor.cpp) and
 * PolarCollectedWumpa::Draw (polar_pickups.cpp) expand it. The frame
 * pointer has a copy: ActorSelf::Draw reads the height through the
 * call's result (r0) and the width and the OAM call through the copy
 * (r7). Written out in Draw instead of inlined, the projection there is
 * a block-local value that local-alloc puts in r4 before the screen Y is
 * ranked (an r5 pin until #662 round 5); as this inline's parameters,
 * the screen X and Y are copied in after the projection, which then
 * shares r5 with the screen X as in the ROM. */
inline void ActorSelf::DrawFrameAt(s32 screenX, s32 screenY, s32 scale)
{
    u8 *frame = GetAnimFrameData();
    u8 *data = frame;
    u32 flag;
    s32 halfW;
    s32 halfH;

    flag = 0;
    if (scale <= 0xff)
        flag = 0x200;

    if (flag != 0)
        halfW = frame[0] << 3;
    else
        halfW = frame[0] << 2;

    if (flag != 0)
        halfH = data[1] << 3;
    else
        halfH = data[1] << 2;

    screenX -= halfW;
    screenY -= halfH;

    if (screenY > 0x9f)
        return;
    if (screenY + halfH * 2 < 0)
        return;
    if (screenX > 0xef)
        return;
    if (screenX + halfW * 2 < 0)
        return;

    if (scale != 0x100)
        flag |= 0x100;

    {
        s32 attr = GetAnimFrameAttr();
        u32 packed = (screenY & 0xff) | (((u32)screenX & 0x1ff) << 16) | attr | flag;
        u32 pal = palette;
        u32 pre = pal << 0xc;
        u32 attr2;

        if (sortKey & SORT_KEY_FLAG_BEHIND_BG)
            attr2 = ((pre | 0x800) << 0x10) >> 0x10;
        else
            attr2 = (pal << 0x1c) >> 0x10;

        SetupSpriteFrameOam(frame, packed, attr2, scale);
    }
}

/* The 3D actors' box test (actor_category_frame.cpp's player hooks and
 * FindShotTarget, polar_nitro.cpp's DetonateNearby; UpdateYeti,
 * IsTouchingYeti and IsTouchingAirship spell out the same test): each
 * actor's box (`box`, +0x38) moved to its position in whole units, the
 * two compared on Z, Y and X. WorldBox is the body of
 * ActorSelf::GetWorldBox (actor.cpp, GetActorWorldBox, which has no
 * caller). BoxOverlap takes the two boxes by reference, so g++ binds each
 * returned box to a temporary: the ROM's two `MemCopy32(box, box, 12)`
 * self-copies are that binding's, which the C spelled out as calls
 * (docs/cplusplus.md, part 11b). */
static inline u8 BoxOverlap(const struct anim_box &b, const struct anim_box &a)
{
    if (b.z < a.z + a.d && b.z + b.d > a.z && b.y < a.y + a.h && b.y + b.h > a.y &&
        b.x < a.x + a.w && b.x + b.w > a.x)
        goto hit;
    return 0;
hit:
    return 1;
}

/* The same test through pointers (IsTouchingYeti, UpdateYeti), whose
 * boxes are in one stack frame struct rather than temporaries. */
static inline u8 BoxOverlap(const struct anim_box *b, const struct anim_box *a)
{
    if (b->z < a->z + a->d && b->z + b->d > a->z && b->y < a->y + a->h && b->y + b->h > a->y &&
        b->x < a->x + a->w && b->x + b->w > a->x)
        goto hit;
    return 0;
hit:
    return 1;
}

/* Moves box `b` by (x, y, z) whole units (IsTouchingYeti, UpdateYeti,
 * IsTouchingAirship). */
static inline void BoxMove(struct anim_box *b, s32 x, s32 y, s32 z)
{
    b->x += x;
    b->y += y;
    b->z += z;
}

static inline struct anim_box WorldBox(ActorSelf *s)
{
    struct anim_box b = s->box;
    s32 dx = s->x >> 8;
    s32 dy = s->y >> 8;
    s32 dz = s->z >> 8;

    b.x += dx;
    b.y += dy;
    b.z += dz;
    return b;
}

/* The actors with hit points (0x58 bytes): the jetpack
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

/* src/actor/actor_category_frame.cpp: the first actor other than `self`
 * a shot hits (JetpackShot::Update). C linkage, C++ callers only. */
extern "C" HpActor *FindShotTarget(ActorSelf *self);

#endif /* GUARD_ACTOR_SELF_HPP */
