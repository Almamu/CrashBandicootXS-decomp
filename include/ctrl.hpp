#ifndef GUARD_CTRL_HPP
#define GUARD_CTRL_HPP

/* The controller classes as C++ (#664, docs/cplusplus.md), for the
 * objects built by agbcp/old_agbcp (the Makefile's CXX_OBJS).
 *
 * No `#pragma interface`: g++ emits each class's vtable in the object
 * that defines its first non-inline virtual method (its key method),
 * and ldscript.txt places it at its ROM address (docs/cplusplus.md,
 * "Emitting the vtables"). Ctrl's own, gCtrlVtable, is emitted in
 * src/system/bios_util.cpp, which has its key method, Update (UpdateCtrl).
 * cxx_symbols.txt maps the mangled vtable, method, constructor and
 * destructor names onto their C names. */

extern "C" {
#include "core.h"
#include "objects.h"
#include "player.h"
#include "bosses.h"
}

class MovingSprite;

/* The controllers' base class (src/objects/ctrl.cpp), 0x10 bytes
 * (checked below, like every class here against the ROM's size).
 * g++ 2.x puts the vtable pointer after the fields of the first class
 * that has virtual methods, which is why it sits at +0x0C. Each virtual
 * method's slot in gCtrlVtable is its declaration order, from slot 1
 * (slot 0 is the empty RTTI slot: the game was built with -fno-rtti).
 * Slot 1, Update (UpdateCtrl), is an empty function in system/bios_util.cpp.
 * SetMode, StartTargetMotionY, SetTargetMotionY and SetAnimSet start
 * src/objects/ctrl.cpp, as the rest. */
class Ctrl
{
public:
    MovingSprite *owner;             // 0x00 - the attached sprite (Attach)
    const struct entry_set *animSet; // 0x04
    s32 state;                       // 0x08 - GetCtrlMode/SetCtrlMode
    // 0x0C: the vtable pointer, gCtrlVtable or a subclass's

    Ctrl();                                                              // InitCtrl
    virtual void Update(MovingSprite *part);                             // 1 UpdateCtrl
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg);  // 2 CtrlHandleEvent
    virtual void Attach(MovingSprite *owner);                            // 3 AttachCtrl
    virtual void SetMode(s32 mode);                                      // 4 SetCtrlMode
    virtual void StartTargetMotionX(MovingSprite *part, const s32 *vec); // 5
    virtual void StartTargetMotionY(MovingSprite *part, const speed_ramp *ramp); // 6
    virtual void SetTargetMotionX(MovingSprite *part, const s32 *vec);           // 7
    virtual void SetTargetMotionY(MovingSprite *part, const speed_ramp *ramp);   // 8
    virtual ~Ctrl();                                                             // 9 DestroyCtrl
    virtual s32 SetTargetAnim(MovingSprite *part, s32 anim);                     // 10
    virtual void StartTargetMotionXFromSet(MovingSprite *part, s32 index);       // 11
    virtual void StartTargetMotionYFromSet(MovingSprite *part, s32 index);       // 12
    s32 GetMode();                                                               // GetCtrlMode
    void SetAnimSet(const struct entry_set *set);                                // SetCtrlAnimSet
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(Ctrl) == 0x10);

/* The effect controller (src/objects/effect_ctrl.cpp, gEffectCtrlVtable):
 * a spawned effect part's controller. SpawnEffectPart (entity_spawner.cpp)
 * creates one per part, `new EffectCtrl` (InitEffectCtrl(OperatorNew(0x10))
 * in its C). */
class EffectCtrl : public Ctrl
{
public:
    EffectCtrl(); // InitEffectCtrl
    virtual void Update(MovingSprite *part);
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg);
    virtual ~EffectCtrl(); // DestroyEffectCtrl
    void Reset();          // ResetEffectCtrl
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(EffectCtrl) == 0x10);

/* The Tiny boss's stomped hop pad's controller
 * (src/bosses/tiny.cpp, gStompedHopPadVtable): `state` 0 plays
 * animation 8, 1 sinks the pad, 2 is done. */
class StompedHopPadCtrl : public Ctrl
{
public:
    StompedHopPadCtrl(); // CreateStompedHopPadCtrl
    virtual void Update(MovingSprite *part);
    virtual ~StompedHopPadCtrl(); // DestroyStompedHopPadCtrl
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(StompedHopPadCtrl) == 0x10);

/* A controller that marks its sprite object gone once the animation has
 * played through (gOneShotAnimCtrlVtable). Its Update is in
 * src/bosses/tiny.cpp, with its constructor and destructor. */
class OneShotAnimCtrl : public Ctrl
{
public:
    OneShotAnimCtrl();
    virtual void Update(MovingSprite *part);
    virtual ~OneShotAnimCtrl();
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(OneShotAnimCtrl) == 0x10);

#endif /* !GUARD_CTRL_HPP */
