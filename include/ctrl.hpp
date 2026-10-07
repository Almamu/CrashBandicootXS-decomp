#ifndef GUARD_CTRL_HPP
#define GUARD_CTRL_HPP

/* The controller classes as C++ (#664, docs/cplusplus.md), for the
 * objects built by agbcp/old_agbcp (the Makefile's CXX_OBJS).
 *
 * `#pragma interface` keeps g++ from emitting the classes' vtables: the
 * ROM's are the C tables in src/data/entity_vtables_7e3bec.c, and
 * cxx_symbols.txt maps the mangled vtable, method, constructor and
 * destructor names onto those tables' and functions' C names. */
#pragma interface

extern "C" {
#include "core.h"
#include "objects.h"
#include "player.h"
#include "bosses.h"
}

class SpriteObj;

/* The controllers' base class (src/objects/ctrl.cpp); struct ctrl
 * (objects.h) is its C view, for the C files, and must keep the same
 * layout (checked below, like every class here against its C struct).
 * g++ 2.x puts the vtable pointer after the fields of the first class
 * that has virtual methods, which is why it sits at +0x0C. Each virtual
 * method's slot in gCtrlVtable is its declaration order, from slot 1
 * (slot 0 is the empty RTTI slot: the game was built with -fno-rtti).
 * Slots 1, 4, 6 and 8 are still C: UpdateCtrl (an empty function in
 * system/boot.c), SetCtrlMode, StartCtrlTargetMotionY and
 * SetCtrlTargetMotionY (player/player_flags.c). */
class Ctrl
{
public:
    void *owner;                     // 0x00 - the attached sprite object (Attach)
    const struct entry_set *animSet; // 0x04
    s32 state;                       // 0x08 - GetCtrlMode/SetCtrlMode
    // 0x0C: the vtable pointer, gCtrlVtable or a subclass's

    Ctrl();                                                                   // InitCtrl
    virtual void Update(SpriteObj *part);                                     // 1 UpdateCtrl
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);          // 2 CtrlHandleEvent
    virtual void Attach(SpriteObj *owner);                                    // 3 AttachCtrl
    virtual void SetMode(s32 mode);                                           // 4 SetCtrlMode
    virtual void StartTargetMotionX(SpriteObj *part, const s32 *vec);         // 5
    virtual void StartTargetMotionY(SpriteObj *part, const speed_ramp *ramp); // 6
    virtual void SetTargetMotionX(SpriteObj *part, const s32 *vec);           // 7
    virtual void SetTargetMotionY(SpriteObj *part, const speed_ramp *ramp);   // 8
    virtual ~Ctrl();                                                          // 9 DestroyCtrl
    virtual u8 SetTargetAnim(SpriteObj *part, s32 anim);                      // 10
    virtual void StartTargetMotionXFromSet(SpriteObj *part, s32 index);       // 11
    virtual void StartTargetMotionYFromSet(SpriteObj *part, s32 index);       // 12
    s32 GetMode();                                                            // GetCtrlMode
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(Ctrl) == sizeof(struct ctrl));

/* The effect controller (src/objects/effect_ctrl.cpp, gEffectCtrlVtable):
 * a spawned effect part's controller. SpawnEffectPart (entity_spawner.c)
 * creates one per part, `new EffectCtrl` (InitEffectCtrl(OperatorNew(0x10))
 * in its C). */
class EffectCtrl : public Ctrl
{
public:
    EffectCtrl(); // InitEffectCtrl
    virtual void Update(SpriteObj *part);
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);
    virtual ~EffectCtrl(); // DestroyEffectCtrl
    void Reset();          // ResetEffectCtrl
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(EffectCtrl) == sizeof(struct ctrl));

/* The Tiny boss's stomped hop pad's controller
 * (src/bosses/tiny_hop_pad.cpp, gStompedHopPadVtable): `state` 0 plays
 * animation 8, 1 sinks the pad, 2 is done. */
class StompedHopPadCtrl : public Ctrl
{
public:
    StompedHopPadCtrl(); // CreateStompedHopPadCtrl
    virtual void Update(SpriteObj *part);
    virtual ~StompedHopPadCtrl(); // DestroyStompedHopPadCtrl
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(StompedHopPadCtrl) == sizeof(struct ctrl));

/* A controller that marks its sprite object gone once the animation has
 * played through (gOneShotAnimCtrlVtable). Its Update is in
 * src/bosses/tiny_hop_pad.cpp; its constructor and destructor are still
 * C, in src/bosses/cortex.c (CreateOneShotAnimCtrl, DestroyOneShotAnimCtrl). */
class OneShotAnimCtrl : public Ctrl
{
public:
    OneShotAnimCtrl();
    virtual void Update(SpriteObj *part);
    virtual ~OneShotAnimCtrl();
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(OneShotAnimCtrl) == sizeof(struct ctrl));

/* The input controller (gInputCtrlVtable, struct input_ctrl in player.h):
 * the player's controller in the rooms where the input alone moves the
 * player. Only its motion queue setters are C++ so far
 * (src/player/input_ctrl_queue.cpp); the rest of it is still C, in
 * src/player/input_ctrl.c, so the virtual methods are only declared here. */
class InputCtrl : public Ctrl
{
public:
    struct player *target; // 0x10
    u8 motionX;            // 0x14 - queued X motion entry (animSet->entries[][0])
    u8 motionY;            // 0x15 - queued Y motion entry (animSet->entries[][1])
    u8 dirState;           // 0x16
    u8 motionXPending;     // 0x17 - ApplyInputCtrlMotion applies motionX
    u8 motionYPending;     // 0x18 - ApplyInputCtrlMotion applies motionY
    u8 motionXKeepSpeed;   // 0x19 - apply with SetTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed;   // 0x1A - the same for Y
    u8 unk_1B;
    struct follow_child *cameraLead; // 0x1C - CreateCameraLead's object (camera_lead.h)
    u8 flag20;                       // 0x20
    u8 unk_21[3];
    s32 timer; // 0x24

    InputCtrl(); // CreateInputCtrl
    virtual void Update(SpriteObj *part);
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);
    virtual void Attach(SpriteObj *owner);
    virtual ~InputCtrl();
    u8 IsMotionXPending();
    void QueueMotionYKeepSpeed(u8 entry);
    void QueueMotionXKeepSpeed(u8 entry);
    void QueueMotionY(u8 entry);
    void QueueMotionX(u8 entry);
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(InputCtrl) == sizeof(struct input_ctrl));

/* The boss controller (gBossCtrlVtable, src/player/input_ctrl_queue.cpp,
 * struct boss_ctrl in player.h): the base class of the bosses'
 * controllers (Mega Mix, Tiny, Neo Cortex's fight, Dingodile and his
 * shield and rocket/stalactite). Its event handler keeps the event's msg
 * and arg; nothing reads them back. */
class BossCtrl : public Ctrl
{
public:
    void *target; // 0x10 - the controlled part
    s32 msg;      // 0x14
    s32 arg;      // 0x18

    BossCtrl(); // CreateBossCtrl
    virtual void HandleEvent(SpriteObj *sender, s32 event, s32 arg);
    virtual ~BossCtrl(); // DestroyBossCtrl
    void *GetTarget();   // GetCtrlTarget
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(BossCtrl) == sizeof(struct boss_ctrl));

/* The Mega Mix boss's controller (gMegaMixCtrlVtable, struct
 * mega_mix_ctrl in bosses.h; src/bosses/mega_mix.cpp). Its Update
 * (UpdateMegaMix) is still C, in src/bosses/mega_mix_update.c. Its
 * motion records are gMegaMixMotionRecords, not gCtrlMotionRecords. */
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

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(MegaMixCtrl) == sizeof(struct mega_mix_ctrl));

#endif /* !GUARD_CTRL_HPP */
