#ifndef GUARD_BOSS_CTRL_HPP
#define GUARD_BOSS_CTRL_HPP

/* The boss controllers as C++ (#664, docs/cplusplus.md): BossCtrl and
 * the bosses' controllers built on it (src/bosses/).
 *
 * `#pragma interface`: no vtable is emitted for these (see ctrl.hpp);
 * cxx_symbols.txt maps their mangled names onto the C names. */
#pragma interface

#include "ctrl.hpp"
#include "sprite_obj.hpp"

/* The boss controller (gBossCtrlVtable, src/player/input_ctrl_queue.cpp,
 * struct boss_ctrl in player.h): the base class of the bosses'
 * controllers (Mega Mix, Tiny, Neo Cortex's fight, Dingodile and his
 * shield and rocket/stalactite). Its event handler keeps the event's msg
 * and arg; nothing reads them back. The word at 0x10 is the controlled
 * part (GetTarget) or, in the bosses that don't use that, a counter:
 * Tiny's round. */
class BossCtrl : public Ctrl
{
public:
    union {
        void *target; // 0x10 - the controlled part
        s32 counter;  //        or a subclass's counter
    };
    s32 msg; // 0x14
    s32 arg; // 0x18

    BossCtrl(); // CreateBossCtrl
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);
    virtual ~BossCtrl(); // DestroyBossCtrl
    void *GetTarget();   // GetCtrlTarget
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(BossCtrl) == sizeof(struct boss_ctrl));

/* The Mega Mix boss's controller (gMegaMixCtrlVtable, struct
 * mega_mix_ctrl in bosses.h; src/bosses/mega_mix.cpp, and its Update,
 * UpdateMegaMix, in src/bosses/mega_mix_update.cpp). Its motion records
 * are gMegaMixMotionRecords, not gCtrlMotionRecords. */
class MegaMixCtrl : public BossCtrl
{
public:
    s32 stamp; // 0x1C - reset to -1 by Reset
    u8 latch;  // 0x20

    MegaMixCtrl(); // CreateMegaMixCtrl
    virtual void Update(SpriteObj *part);
    virtual ~MegaMixCtrl(); // DestroyMegaMixCtrl
    virtual void StartTargetMotionXFromSet(SpriteObj *part, s32 index);
    virtual void StartTargetMotionYFromSet(SpriteObj *part, s32 index);
    void SetMotionYFromSet(SpriteObj *part, s32 index);
    void SetMotionXFromSet(SpriteObj *part, s32 index);
    void Reset();
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(MegaMixCtrl) == sizeof(struct mega_mix_ctrl));

/* The Tiny boss's controller (gTinyVtable; src/bosses/tiny_update.cpp).
 * Tiny hops his part along parabolic arcs between gTouchableList's
 * anchors (the hop pads), stomping them: Update steps the hop and the
 * state machine, SetState enters a state. `counter` is the round (1-3,
 * the hits taken). Its constructor and destructor are still C, in
 * src/bosses/cortex.c (CreateTiny, DestroyTiny), as is StartHop
 * (StartTinyHop). spawn_bosses.c creates it in a 0x4C-byte block. */
class TinyCtrl : public BossCtrl
{
public:
    s32 nextState; // 0x1C - the state after the anim (5) or the wait (4)
    s32 timer;     // 0x20
    s32 stomped;   // 0x24 - the anchor to stomp, or -1
    s32 anchor;    // 0x28 - the anchor hopped to (a gTouchableList index)
    s32 count;     // 0x2C
    s32 x;         // 0x30 - the hop's start
    s32 y;         // 0x34
    s32 steps;     // 0x38 - the hop's steps left
    s32 total;     // 0x3C - and its length
    s32 dy;        // 0x40
    s32 dx;        // 0x44
    s16 *squares;  // 0x48 - i * i >> 8 for i = 0..0x100 (CreateTiny)

    TinyCtrl(); // CreateTiny
    virtual void Update(SpriteObj *part);
    virtual ~TinyCtrl(); // DestroyTiny
    void SetState(SpriteObj *part, s32 next);
    s32 PickHopTarget();
    void SpawnFallingLeaves(SpriteObj *part, s32 n);
    void StartHop(SpriteObj *part); // StartTinyHop
};

COMPILE_TIME_ASSERT(boss_ctrl_hpp, sizeof(TinyCtrl) == 0x4C);

#endif /* !GUARD_BOSS_CTRL_HPP */
