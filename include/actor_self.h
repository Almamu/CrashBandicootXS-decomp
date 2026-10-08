#ifndef GUARD_ACTOR_SELF_H
#define GUARD_ACTOR_SELF_H

#include "core.h"
#include "vtable.h"

/*
 * The C view of the 3D actors' base class, ActorSelf (actor_self.hpp;
 * C++ since #664 part 11a, src/actor/actor.cpp), for the files still in
 * C (the yeti, the IWRAM sorter): an object built by `InitActorPart`
 * (ActorSelf's constructor), with its vtable pointer at +0x50. The fields have the class's names and types
 * (actor_self.hpp checks their offsets). Every 3D
 * actor class is C++ since #664 part 11g, and the C macros that called
 * through them (ACTOR_PMF_CALL, ACTOR_VCALL, ...) are gone, as are the
 * pointer-to-member records of the C state tables (struct actor_pmf,
 * ACTOR_PMF: the tables are C++, #656).
 *
 * Only the common prefix (0x00-0x53) is described here: every derived
 * class lays out its own fields from +0x54 on, in each translation
 * unit's own struct that embeds this one as its first member. See
 * docs/matching/archive/issue-58-*.md and issue-57-0x0802fbf0-actor.md.
 */

/* One entry of `actor_self.anims` (AnimPart's, actor_self.hpp), stride
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
 * anim_table_record's box_14, `actor_self.box` and several small src/data
 * tables are this. A fixed box is copied into an actor by struct
 * assignment (`box = gJetpackRocketBox`): the ROM's `ldm`/`stm`. */
struct anim_box {
    s16 x, y, z;
    s16 w, h, d;
}; // 0xC
COMPILE_TIME_ASSERT(actor_self_h, sizeof(struct anim_box) == 0xC);

/* actor_self.sortKey bits. The low 15 bits are the draw-order key
 * ((depth >> 1) & 0x7f80 | ((|x| + |y|) >> 11) & 0x7f); InitActorPart,
 * UpdateActor and UpdateActorDepth set bit 15 when the actor's depth is past
 * GetActorBgLayerDepth(), and the draw functions then give its sprite OAM
 * priority 2 (attr 2 | 0x800) so it goes behind the BG layer. */
#define SORT_KEY_FLAG_BEHIND_BG 0x8000

struct actor_self {
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;               // 0x04
    s32 animTime;                    // 0x08 - Q8 frame accumulator
    s32 animIndex;                   // 0x0C - current index into anims
    u16 animTimer;                   // 0x10
    u8 animDone;                     // 0x12 - set once the current sequence has played through
    s32 sortKey; // 0x14 - draw order: RunActorCategoryFrame heapsorts the draw list
                 //        by it (HeapSortActorsByKey); bit 15 also sets OAM priority
                 //        (SORT_KEY_FLAG_BEHIND_BG)
    s32 palette; // 0x18 - OBJ palette bank (OAM attr 2 << 12), from anim_table_record.palette
    s32 x;       // 0x1C
    s32 y;       // 0x20
    s32 z;       // 0x24
    s32 state;   // 0x28
    u8 visible;  // 0x2C - nonzero: drawn (RunActorCategoryFrame only puts these in
                 //        gActorDrawList); InitActorPart sets it to 1
    struct anim_table_record *record; // 0x30 - the record InitActorPart was given (actor_anim.h);
                                      //        the draw functions scale by its baseDepth
    s32 depth;                        // 0x34
    struct anim_box box;     // 0x38 - collision box, copied from record->box_14 by InitActorPart
    s32 stateTime;           // 0x44 - frames spent in `state`
    struct actor_self *prev; // 0x48 - circular actor list (rooted at the player, gActorList):
    struct actor_self *next; // 0x4C   InitActorPart appends before the head; the draw and
                             //        teardown loops walk `next` from the head
    // 0x50 - the class's vtable (vtable.h's slots): slot 1 the destructor (called
    // with 3 to delete), 2 Update, 3 Draw; HpActor's 4 "damage" (a strength), 6
    // "get HP" (GetActorHp, the jetpack player's HP percentage), 7 "release"
    const struct vtable_slot *vtable;
};

#endif /* !GUARD_ACTOR_SELF_H */
