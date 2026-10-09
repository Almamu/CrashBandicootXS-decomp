#define ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE
#include "actor_self.hpp"
#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"
}

/* The 3D actor base class, ActorSelf (#664 part 11a, include/actor_self.hpp):
 * the constructor, Update and Draw, the depth and draw-order key, the
 * accessors and the destructor (the key method: g++ emits gActorVtable
 * here). The C-linkage helpers around it in the ROM are in
 * actor_category_hooks.cpp (before), actor_spawn_collected.cpp and
 * actor_palette_cycle.cpp (after); they were in this file until #770.
 * See docs/matching/archive/issue-50-actor-2a69c.md. */

/* The depth (the distance past the cell animation's, absolute) and the
 * draw-order key: (depth >> 1) & 0x7f80, then ((|x| + |y|) >> 11) & 0x7f,
 * and SORT_KEY_FLAG_BEHIND_BG past the BG layer. The constructor and
 * Update expand it; UpdateDepth is its out-of-line copy. */
static inline void RefreshDepth(ActorSelf *self)
{
    s32 d = self->z - INT_TO_Q8(GetCellAnimDistance());
    s32 value;

    self->depth = ABS_BRANCHLESS(d);
    value = (self->depth >> 1) & 0x7f80;
    value |= ((ABS_BRANCHLESS(self->y) + ABS_BRANCHLESS(self->x)) >> 0xb) & 0x7f;
    self->sortKey = value;

    if (self->depth > GetActorBgLayerDepth())
        self->sortKey |= SORT_KEY_FLAG_BEHIND_BG;
}

/* InitActorPart: the animation (AnimPart's inline constructor, from the
 * record's keyframes, frames and palette), the position, the record and
 * its box, state 0, visible, the depth, and the link into the actor list
 * before its root (or a list of its own when it is the first). */
ActorSelf::ActorSelf(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : AnimPart(rec->table_A, rec->table_B, rec->palette)
{
    this->x = x;
    this->y = y;
    this->z = z;
    record = (struct anim_table_record *)rec;
    box = rec->box_14;

    state = 0;
    stateTime = 0;
    visible = 1;

    RefreshDepth(this);

    {
        ActorSelf *head = gActorList;

        if (head != NULL) {
            next = head;
            prev = head->prev;
            head->prev = this;
            prev->next = this;
        } else {
            next = this;
            prev = this;
        }
    }
}

/* Slot 2: the depth; deletes the actor once it is more than 0x200 outside
 * the clip range, else counts the state's frames and plays the
 * animation, looping the sequence back once its frame reaches the
 * keyframe's loopThreshold. */
void ActorSelf::Update()
{
    RefreshDepth(this);

    if (depth > gActorFarClipDepth + 0x200 || depth < gActorNearClipDepth - 0x200) {
        delete this;
        return;
    }

    stateTime += 1;
    animTime += (s16)animTimer;
    animDone = 0;

    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        animTime -= INT_TO_Q8(anims[animIndex].loopThreshold - anims[animIndex].loopBase);
        animDone = 1;
    }
}

/* Slot 3: the sprite, scaled by depth against the record's baseDepth
 * and projected through gActorFocalLength around the BG's center, drawn
 * by DrawFrameAt (actor_self.hpp; an affine sprite unless 1:1, double
 * size below 0x100, culled off screen, OAM priority 2 when
 * SORT_KEY_FLAG_BEHIND_BG). */
void ActorSelf::Draw()
{
    s32 dist = depth;
    s32 scale = (dist << 8) / record->baseDepth;
    s32 proj = (gActorFocalLength << 0xc) / dist;
    s32 screenY;
    s32 screenX;

    {
        s32 off = GetActorBgCenterY();
        s32 t = y;

        screenY = t * proj;
        screenY >>= 0xc;
        screenY += off;
        screenY = Q8_TO_INT(screenY);
    }
    {
        s32 off = GetActorBgCenterX();
        s32 t = x;

        t = t * proj;
        t >>= 0xc;
        t += off;
        screenX = Q8_TO_INT(t);
    }
    DrawFrameAt(screenX, screenY, scale);
}

void ActorSelf::UpdateDepth()
{
    RefreshDepth(this);
}

/* The record's `index` (its slot in the animation table). */
u8 ActorSelf::GetRecordIndex()
{
    return record->index;
}

void ActorSelf::EnterState(s32 st, s32 idx)
{
    SetState(st, idx);
}

s32 ActorSelf::GetZ()
{
    return z;
}

s32 ActorSelf::GetY()
{
    return y;
}

s32 ActorSelf::GetX()
{
    return x;
}

/* The box in world space: its corner offset by the position (whole
 * units), the size unchanged. UNUSED - no caller anywhere in the ROM. */
struct anim_box ActorSelf::GetWorldBox()
{
    struct anim_box b = box;
    s32 dx = x >> 8;
    s32 dy = y >> 8;
    s32 dz = z >> 8;
    struct anim_box *p = &b;

    p->x = p->x + dx;
    p->y = p->y + dy;
    p->z = p->z + dz;
    return b;
}

u8 ActorSelf::IsVisible()
{
    return visible;
}

/* Slot 1, DestroyActor: the out-of-line copy of the inline destructor
 * (actor_self.hpp). */
ActorSelf::~ActorSelf()
{
    Unlink();
}
