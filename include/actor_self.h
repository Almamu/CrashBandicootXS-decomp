#ifndef GUARD_ACTOR_SELF_H
#define GUARD_ACTOR_SELF_H

/*
 * The C view of the 3D actors' base class, ActorSelf (actor_self.hpp;
 * C++ since #664 part 11a, src/actor/actor.cpp), for the files still in
 * C: an object built by `InitActorPart` (ActorSelf's constructor), its
 * vtable pointer at +0x50, and the layouts of its vtable slots and
 * pointer-to-member records (struct actor_pmf, for the src/data tables).
 * Every 3D actor class is C++ since #664 part 11g, and the C macros that
 * called through them (ACTOR_PMF_CALL, ACTOR_VCALL, ...) are gone.
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

/* A gcc 2.x method-table record: `this` adjustment plus code pointer. */
struct actor_method {
    s16 thisOffset;
    u8 unk_02[2];
    void *fn;
};

struct actor_vtable {
    u8 unk_00[8];
    struct actor_method destroy; // 0x08 - slot 1, the (virtual) destructor; called with 3 to delete
    u8 unk_10[0x10];
    struct actor_method m20; // 0x20 - "damage" (called on the player with a strength)
    u8 unk_28[8];
    // 0x30 - slot 6, "get HP": GetActorHp returns HpActor's `hp` (actor_self.hpp), the jetpack player's
    // GetJetpackPlayerHpPercent its HP as a percentage (UpdateHudPercentCounters shows it)
    struct actor_method getHp;
    struct actor_method m38; // 0x38 - "release" (no argument; DamageJetpackBalloon)
};

/* A gcc 2.x pointer-to-member-function record, as stored in the
 * per-state dispatch tables (gJetpackPlaneStateFuncs/0817C280/...):
 * `index > 0` selects virtual slot `index - 1` of the method table
 * found at `this + vtableOffset`, otherwise `fn` is called directly. */
struct actor_pmf {
    s16 thisOffset; // 0x00
    s16 index;      // 0x02
    union {
        s16 vtableOffset; // 0x04 - index > 0
        void *fn;         // 0x04 - index <= 0
    } u;
};

/* Initializer for a non-virtual `&Class::method` constant, the only kind
 * the ROM's tables hold: gcc 2.x stores it as thisOffset 0, index -1 and
 * the (Thumb) code address. Used by the src/data tables (docs/data.md). */
#define ACTOR_PMF(func) { 0, -1, { .fn = (void *)(func) } }

struct anim_table_record;

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
    u8 unk_13;
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
    u8 unk_2D[3];
    struct anim_table_record *record; // 0x30 - the record InitActorPart was given (actor_anim.h);
                                      //        the draw functions scale by its baseDepth
    s32 depth;                        // 0x34
    u8 box[0xC];             // 0x38 - collision box, copied from record->box_14 by InitActorPart
                             //        (ActorsOverlap and friends read it as a struct box16)
    s32 stateTime;           // 0x44 - frames spent in `state`
    struct actor_self *prev; // 0x48 - circular actor list (rooted at the player, gActorList):
    struct actor_self *next; // 0x4C   InitActorPart appends before the head; the draw and
                             //        teardown loops walk `next` from the head
    struct actor_vtable *vtable; // 0x50
};

/* Resets `self` into state `st`, restarting animation sequence `idx`:
 * the C spelling of ActorSelf::SetState (actor_self.hpp). No C file uses
 * it any more (every 3D actor class is C++, #664 part 11). Both values go
 * through locals so constant pairs are materialized before the stores,
 * as the ROM does; the `if (1) { ... } else (void)0` wrapper, rather than
 * `do { ... } while (0)`, is because agbcc treats the latter as a real
 * loop when weighing register priorities. */
#define ACTOR_SET_STATE(self, st, idx)                                         \
    if (1)                                                                     \
    {                                                                          \
        s32 _st = (st);                                                        \
        s32 _idx = (idx);                                                      \
        (self)->state = _st;                                                   \
        (self)->stateTime = 0;                                                 \
        (self)->animIndex = _idx;                                              \
        (self)->animTimer = (self)->anims[_idx].duration;                      \
        (self)->animDone = 0;                                                  \
        (self)->animTime = 0;                                                  \
    } else (void)0

#endif /* !GUARD_ACTOR_SELF_H */
