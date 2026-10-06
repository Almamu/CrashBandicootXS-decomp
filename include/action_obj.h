#ifndef GUARD_ACTION_OBJ_H
#define GUARD_ACTION_OBJ_H

/* The player/action object behind the gActionCtrlStateTable 42-slot action
 * table (docs/rom_map.md), as far as src/player/action_ctrl_states.c and
 * action_ctrl_hang.c (GitHub issue #17, both built with old_agbcc) use it.
 * action_ctrl_states.c and friends reach the same fields
 * through raw offsets.
 *
 * `vt` is a gcc 2.x method table ({s16 this-adjust; fn} entries, called
 * through libgcc's _call_via_r2/_call_via_r3 trampolines), `part` the on-screen object it animates, and the
 * +0x27..+0x32 bytes two "next action" trios (+0x31/+0x2F/+0x27 and
 * +0x32/+0x30/+0x28) the table's dispatcher consumes. */

#include "player.h"

struct act_method {
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct act_vtable {
    u8 unk_00[0x10];
    struct act_method m10; // 0x10
    u8 unk_18[8];
    struct act_method m20; // 0x20 - "set animation"
    struct act_method m28; // 0x28
    struct act_method m30; // 0x30
    struct act_method m38; // 0x38
    struct act_method m40; // 0x40
    struct act_method m48; // 0x48
    struct act_method m50; // 0x50 - "set part animation"
};

/* One entry of the per-object table `act.anims` points at: indices into
 * gCtrlMotionRecords's 12-byte records for the +0x27 and +0x28 actions. */
struct act_anim_pair {
    s32 first;
    s32 second;
};

struct act {
    u8 unk_00[4];
    struct act_anim_pair **anims; // 0x04
    s32 state;                    // 0x08
    struct act_vtable *vt;        // 0x0C
    struct player *part;          // 0x10
    u8 unk_14[4];
    s32 frame;  // 0x18
    s32 frames; // 0x1C
    u8 charge;  // 0x20
    u8 unk_21;
    u8 unk_22; // 0x22
    u8 unk_23; // 0x23
    u8 unk_24[2];
    u8 spinCooldown; // 0x26 - frames until the next spin is allowed (set to 12, counts down)
    u8 motionX;      // 0x27 - queued X motion entry (anims->first)
    u8 motionY;      // 0x28 - queued Y motion entry (anims->second)
    u8 turboRun;     // 0x29 - set on entering the turbo run (L, state 4); landing resumes it
                     //        instead of the plain run; the idle state clears it
    u8 unk_2A;
    u8 bumpTimer;     // 0x2B - 3 after a crate's side stopped the X motion (event 12); counts down
                      //        while at most one crate is touched, then re-queues bumpedMotionX
    u8 bumpedMotionX; // 0x2C - the motionX that bump cancelled
    u8 prevState;     // 0x2D - `state` before the last SetActionCtrlMode (a flip jump, 9,
                      //        turns the mid-air body slam into the flip body slam)
    // 0x2E - part->slippery last frame; UpdateActionCtrl calls sub_8012238 on a change
    u8 prevSlippery;
    u8 motionXPending;   // 0x2F - ApplyActionCtrlMotion applies motionX
    u8 motionYPending;   // 0x30 - ApplyActionCtrlMotion applies motionY
    u8 motionXKeepSpeed; // 0x31 - apply with SetCtrlTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x32 - the same for Y
    u8 idleFidget;       // 0x33 - an idle fidget anim (0xE/5/0x1A, after 8/20/30 s) is playing;
                         //        UpdateActionCtrl doesn't force the idle anim back meanwhile
    u8 slamBlocked;      // 0x34 - R was held through a bounce (events 13/14): the mid-air body
                         //        slam needs R released first (UpdateActionCtrl clears it then)
};

typedef void (*act_fn1)(void *self, s32 a);
typedef void (*act_fn2)(void *self, void *a, s32 b);

#define ACT_VCALL1(obj, m, a)                                                  \
    do                                                                         \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } while (0)
#define ACT_VCALL2(obj, m, a, b)                                               \
    do                                                                         \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)

/* The same calls wrapped in `if (1) { ... } else (void)0` instead of
 * `do { ... } while (0)` (include/actor_self.h explains the difference).
 * agbcc treats the `do`/`while` as a loop, which keeps CSE from carrying
 * a constant from before the call to a store after it. Most handlers match
 * either way; ActionCtrlStateAirSpin needs the loop form and ActionCtrlStateHangMove (and the
 * other handlers that keep a 1 in a callee-saved register across the
 * calls) needs this one. */
#define ACT_CALL1(obj, m, a)                                                   \
    if (1)                                                                     \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } else (void)0
#define ACT_CALL2(obj, m, a, b)                                                \
    if (1)                                                                     \
    {                                                                          \
        struct act_method *_m = &(obj)->vt->m;                                 \
        ((act_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } else (void)0

/* gKeys is the input word: low half held, high half newly
 * pressed. Handlers copy it to a stack slot and read the halves back from
 * there; the halves go through the local's address (a union or struct
 * member read is folded into a halfword load of the global itself). */
#define INPUT_HELD(in) (*(u16 *)&(in))
#define INPUT_PRESSED(in) (*(u16 *)((u8 *)&(in) + 2))

/* Byte read-modify-writes of part+0x0D, through a plain byte pointer: as
 * a struct member store, gcc's expansion leaves a dead `& 0` whose 0 CSE
 * then reuses for later zero stores, moving them (see
 * tiny_update.c). The mask arrives as an `s32` parameter so
 * old_agbcc materializes it before the load. */
#define ACT_PART_FLAGS0D(p) (*((u8 *)(p) + 0xD))

static inline void ActAndFlags0D(struct player *part, s32 mask)
{
    ACT_PART_FLAGS0D(part) &= mask;
}

static inline void ActOrFlags0D(struct player *part, s32 bits)
{
    ACT_PART_FLAGS0D(part) |= bits;
}

/* Queues action `next` on the +0x32/+0x30/+0x28 trio. As an inline
 * parameter, old_agbcc materializes `next` before the three stores, as
 * the ROM does. */
static inline void ActSetNext(struct act *self, s32 next)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    self->motionY = next;
}

#endif // GUARD_ACTION_OBJ_H
