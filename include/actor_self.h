#ifndef GUARD_ACTOR_SELF_H
#define GUARD_ACTOR_SELF_H

/*
 * The shared per-instance "self" object of the 0x0802xxxx-0x0803xxxx
 * actor zone (issues #54-#62 - see docs/matching/issue-58-*.md and
 * issue-57-0x0802fbf0-actor.md): a gcc 2.x C++ object built by
 * `InitActorPart`, whose method table pointer sits at +0x50. Methods are
 * called through the `sub_803AD7C`..`sub_803AD88` libgcc `_call_via_rN`
 * thunks with the `this` pointer pre-adjusted by the method record's
 * own `thisOffset`.
 *
 * Only the common prefix (0x00-0x57) is described here - every derived
 * class lays out its own fields from +0x54 on, so those live in each
 * translation unit's own struct that embeds this one as its first
 * member. Most of the older actor_part*.c files still use raw offsets
 * into the same object.
 */

/* One entry of `actor_self.anims` (`anim_part_instance.frameTable` in
 * actor_anim.c), stride 12 - only the five fields actually read by
 * matched functions are named; the rest (0xa-0xb) isn't exercised by
 * any function matched so far. */
struct anim_frame_record {
    u16 duration;       // 0x00 - copied into the owning self's `animTimer` on a sequence reset
    s16 frameIndex;     // 0x02 - added to GetAnimFrameBaseOffset()'s result, indexes frameOffsets
    s16 loopThreshold;  // 0x04 - sub_803B4EC wraps self->animTime back once the frame base
                        // offset reaches this value
    s16 loopBase;       // 0x06 - subtracted from loopThreshold (then <<8) as the wrap amount
    u16 attr;           // 0x08 - packed into the high halfword of sub_803B060's return value
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
    struct actor_method m08;   // 0x08 - "hit/destroy" (called with arg 3)
    u8 unk_10[0x10];
    struct actor_method m20;   // 0x20 - "damage" (called on the player with a strength)
    u8 unk_28[0x10];
    struct actor_method m38;   // 0x38 - "release" (no argument; sub_8031858)
};

/* A gcc 2.x pointer-to-member-function record, as stored in the
 * per-state dispatch tables (gStaticData_0817C260/0817C280/...):
 * `index > 0` selects virtual slot `index - 1` of the method table
 * found at `this + vtableOffset`, otherwise `fn` is called directly. */
struct actor_pmf {
    s16 thisOffset;             // 0x00
    s16 index;                  // 0x02
    union {
        s16 vtableOffset;       // 0x04 - index > 0
        void *fn;               // 0x04 - index <= 0
    } u;
};

/* Initializer for a non-virtual `&Class::method` constant, the only kind
 * the ROM's tables hold: gcc 2.x stores it as thisOffset 0, index -1 and
 * the (Thumb) code address. Used by the src/data tables (docs/data.md). */
#define ACTOR_PMF(func) { 0, -1, { .fn = (void *)(func) } }

struct actor_self {
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;          // 0x04
    s32 animTime;               // 0x08 - Q8 frame accumulator
    s32 animIndex;              // 0x0C - current index into anims
    u16 animTimer;              // 0x10
    u8 animDone;                // 0x12 - set once the current sequence has played through
    u8 unk_13;
    s32 visible;                // 0x14 - set to 1 by sub_8032718 (meaning unconfirmed)
    s32 unk_18;                 // 0x18
    s32 x;                      // 0x1C
    s32 y;                      // 0x20
    s32 z;                      // 0x24
    s32 state;                  // 0x28
    u8 unk_2C[8];
    s32 depth;                  // 0x34
    u8 unk_38[0xC];
    s32 stateTime;              // 0x44 - frames spent in `state`
    u8 unk_48[8];
    struct actor_vtable *vtable; // 0x50
};

/* Words inside the byte arrays above, which other files still index
 * directly: the `struct anim_table_record` InitActorPart was given
 * (+0x30, actor_anim.h) and the circular list links it sets up (+0x48
 * next, +0x4C prev; the list is rooted at the player, gUnknown_03000884). */
struct anim_table_record;
#define ACTOR_RECORD(self) (*(struct anim_table_record **)&(self)->unk_2C[4])
#define ACTOR_LINK_NEXT(self) (((struct actor_self **)(self)->unk_48)[0])
#define ACTOR_LINK_PREV(self) (((struct actor_self **)(self)->unk_48)[1])

/* The statement macros below are wrapped in `if (1) { ... } else (void)0`
 * rather than the usual `do { ... } while (0)`: agbcc treats the latter
 * as a real loop when weighing register priorities, which was enough to
 * change ACTOR_PMF_CALL's register allocation away from the ROM's. */

/* Resets `self` into state `st`, restarting animation sequence `idx`.
 * Both values go through locals so constant pairs are materialized
 * before the stores, as the ROM does. */
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

typedef void (*actor_method_fn)(void *self, s32 arg);

/* Virtual call through `obj`'s method table (a gcc 2.x C++ virtual
 * call). */
#define ACTOR_VCALL(obj, m, arg)                                               \
    if (1)                                                                     \
    {                                                                          \
        struct actor_vtable *_vt = (obj)->vtable;                              \
        ((actor_method_fn)_vt->m.fn)((u8 *)(obj) + _vt->m.thisOffset, (arg));  \
    } else (void)0

/* `(self->*table[self->state])()` - a gcc 2.x pointer-to-member-function
 * call through one of the per-state dispatch tables. The table entry is
 * re-read after the virtual/non-virtual split exactly the way the
 * compiler expanded the member-pointer call. */
#define ACTOR_PMF_CALL(self, table)                                            \
    if (1)                                                                     \
    {                                                                          \
        struct actor_method _m;                                                \
        void (*_fn)(void *);                                                   \
        s32 _index = (table)[(self)->state].index;                             \
        s32 _off;                                                              \
                                                                               \
        if (_index > 0) {                                                      \
            _m = (*(struct actor_method **)((u8 *)(self)                       \
                    + (table)[(self)->state].u.vtableOffset))[_index - 1];     \
            _fn = _m.fn;                                                       \
        } else {                                                               \
            _fn = (table)[(self)->state].u.fn;                                 \
        }                                                                      \
        _off = (table)[(self)->state].thisOffset;                              \
        {                                                                      \
            s32 _d;                                                            \
            if (_index > 0) {                                                  \
                _d = _m.thisOffset + _off;                                     \
            } else {                                                           \
                _d = _off;                                                     \
            }                                                                  \
            _fn((u8 *)(self) + _d);                                            \
        }                                                                      \
    } else (void)0

/* Thumb code makes indirect calls with `bl _call_via_rN`; this ROM's
 * copies of those libgcc helpers are the sub_803AD78..sub_803AD94
 * trampolines (src/system/reg_trampolines.c). Expand once per
 * translation unit that uses ACTOR_VCALL/ACTOR_PMF_CALL. */
#define ACTOR_CALL_VIA_ALIASES                                                 \
    asm(".set _call_via_r1, sub_803AD7C\n"                                     \
        ".set _call_via_r2, sub_803AD80\n"                                     \
        ".set _call_via_r3, sub_803AD84\n"                                     \
        ".set _call_via_r4, sub_803AD88\n");

#endif /* !GUARD_ACTOR_SELF_H */
