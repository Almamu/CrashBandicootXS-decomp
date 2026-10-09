#define ACTOR_SELF_DESTRUCTOR_OUT_OF_LINE
#include "actor_self.hpp"
#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
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
 * (an affine sprite unless 1:1, double size below 0x100) and projected
 * through gActorFocalLength around the BG's center; culled off screen,
 * OAM priority 2 when SORT_KEY_FLAG_BEHIND_BG. */
void ActorSelf::Draw()
{
    s32 dist = depth;
    s32 scale = __divsi3(dist << 8, record->baseDepth);
    /* r5: unpinned, the projection and the screen y swap r4 and r5 (no
     * spelling changes global allocation's ranking of the two). The
     * projection is local to the first basic block, so local allocation
     * gives it r4 before the screen y (global) is ranked; #662 round 2:
     * scoping it (or `dist`, `scale`) in blocks, and testing `scale`
     * instead of `flag`, don't change that. #662 round 3: nor do the
     * screen y computed through a temporary (as the x is) or as one
     * expression, or the projection declared just before its use; the
     * projection stays local to block 0, which ends at the scale test,
     * and no -f flag or pair of flags makes it global. */
    MATCH_HOLD_REG(s32, proj, r5) = __divsi3(gActorFocalLength << 0xc, dist);
    s32 screenY;
    s32 screenX;
    u8 *data;
    u8 *frame;
    u32 flag;
    s32 halfW;
    s32 halfH;

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

    /* Two pointers: the height is read through the call's result (r0),
     * the width and the OAM call through its copy (r7). */
    data = GetAnimFrameData();
    frame = data;

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
