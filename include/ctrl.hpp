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
}

class SpriteObj;

/* struct ctrl (objects.h): the controllers' base class. g++ 2.x puts the
 * vtable pointer after the fields of the first class that has virtual
 * methods, which is why it sits at +0x0C. Each virtual method's slot in
 * gCtrlVtable is its declaration order, from slot 1 (slot 0 is the empty
 * RTTI slot: the game was built with -fno-rtti). */
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
};

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(Ctrl) == 0x10);

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

COMPILE_TIME_ASSERT(ctrl_hpp, sizeof(EffectCtrl) == 0x10);

#endif /* !GUARD_CTRL_HPP */
