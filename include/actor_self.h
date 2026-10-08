#ifndef GUARD_ACTOR_SELF_H
#define GUARD_ACTOR_SELF_H

#include "core.h"
#include "vtable.h"

/*
 * The plain records of the 3D actors (class ActorSelf and its AnimPart
 * base, actor_self.hpp; src/actor/): the animation frames and the boxes.
 * ActorSelf has no C view: `struct actor_self` went with its last C
 * reader (#754). See docs/matching/archive/issue-58-*.md and
 * issue-57-0x0802fbf0-actor.md.
 */

/* One entry of AnimPart's `anims` (actor_self.hpp), stride
 * 12 - only the five fields actually read by matched functions are
 * named; the rest (0xa-0xb) isn't exercised by any function matched so
 * far. */
struct anim_frame_record {
    u16 duration;   // 0x00 - copied into the owning self's `animTimer` on a sequence reset
    s16 frameIndex; // 0x02 - added to GetAnimFrameBaseOffset()'s result, indexes frameOffsets
    // 0x04 - UpdateJetpackCheckpointText wraps self->animTime back once the frame base
    // offset reaches this value
    s16 loopThreshold;
    s16 loopBase; // 0x06 - subtracted from loopThreshold (then <<8) as the wrap amount
    u16 attr;     // 0x08 - packed into the high halfword of GetAnimFrameAttr's return value
    u8 unknown_0a[2];
};

struct anim_table_record;

/* A box in the actors' 16-bit world units: position then size. The
 * anim_table_record's box_14, ActorSelf's `box` and several small src/data
 * tables are this. A fixed box is copied into an actor by struct
 * assignment (`box = gJetpackRocketBox`): the ROM's `ldm`/`stm`. */
struct anim_box {
    s16 x, y, z;
    s16 w, h, d;
}; // 0xC
COMPILE_TIME_ASSERT(actor_self_h, sizeof(struct anim_box) == 0xC);

/* ActorSelf::sortKey's bits. The low 15 bits are the draw-order key
 * ((depth >> 1) & 0x7f80 | ((|x| + |y|) >> 11) & 0x7f); InitActorPart,
 * UpdateActor and UpdateActorDepth set bit 15 when the actor's depth is past
 * GetActorBgLayerDepth(), and the draw functions then give its sprite OAM
 * priority 2 (attr 2 | 0x800) so it goes behind the BG layer. */
#define SORT_KEY_FLAG_BEHIND_BG 0x8000

#endif /* !GUARD_ACTOR_SELF_H */
