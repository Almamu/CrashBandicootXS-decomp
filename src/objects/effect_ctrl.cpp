#include "ctrl.hpp"

extern "C" {
#include "globals.h"
#include "entity_bits.h"
}

/* The effect controller, ROM 0x0800CBF4-0x0800CD00: the first object
 * built as C++ (#664, docs/cplusplus.md). It's old_agbcp's: the file is
 * in the Makefile's OLD_AGBCC_OBJS (the constant-before-`ldrb` flag ORs
 * in Update are old_agbcc's tell), and cxx_symbols.txt gives the
 * methods their C names.
 *
 * The sprite object it drives, as far as this file reads it: struct
 * actor's header (id @8, flags @0xC, the vtable pointer @0x18) and struct
 * gobj's animDone @0x38. Entity is the 0x1C-byte base class of
 * gEntityVtable (its own fields, then the vtable pointer); SpriteObj adds
 * the sprite fields after it. Only the methods this file calls are named:
 * slot 5 is IsEntityOnScreen/IsSpriteObjOnScreen. */
class Entity
{
public:
    s32 x;  // 0x00
    s32 y;  // 0x04
    u16 id; // 0x08 - the "gone" bit index; ENTITY_ID_NONE: none
    u8 kind;
    u8 unused_0B;
    union {
        u8 flags; // 0x0C
        struct {
            u8 gone:1;
            u8 unk_1:7;
        } b; // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    } f;
    s16 halfW; // 0x10
    s16 halfH; // 0x12
    u8 rawW;   // 0x14
    u8 rawH;   // 0x15
    u8 unused_16[2];
    // 0x18: the vtable pointer

    virtual void CheckPlayerContact(); // 1
    virtual void GetBounds();          // 2
    virtual void Update();             // 3
    virtual void Draw();               // 4
    virtual u8 IsOnScreen();           // 5
};

COMPILE_TIME_ASSERT(effect_ctrl_cpp, sizeof(Entity) == 0x1C);

class SpriteObj : public Entity
{
public:
    u8 unk_1C[0x1C];
    u8 animDone; // 0x38 - struct gobj.animDone
};

/* Marks `t` gone: the flag, and its bit in the bitmap. The gone bit is
 * set through a bitfield view of +0x0C and bit 3 is tested through the
 * byte view: that is what makes the second copy reuse the tested byte
 * and its `1` for the OR, in the ROM's registers. */
static inline void MarkGone(SpriteObj *t)
{
    ENTITY_MARK_GONE(t->f.b.gone, t->id);
}

/* Marks the effect part gone when it has left the screen, when it has
 * been touched (flags bit 3), or when its animation has played through.
 * The three tests aren't else-chained: each marks it again. */
void EffectCtrl::Update(SpriteObj *part)
{
    if (!part->IsOnScreen())
        MarkGone(part);
    if ((part->f.flags >> 3) & 1)
        MarkGone(part);
    if (part->animDone)
        MarkGone(part);
}

void EffectCtrl::HandleEvent(SpriteObj *, s32, s32)
{
}

/* Empty: the constructor calls it where CreateEnemyCtrl calls
 * ResetEnemyCtrl. */
void EffectCtrl::Reset()
{
}

/* g++ sets the vtable pointer back to gEffectCtrlVtable, then calls
 * ~Ctrl (DestroyCtrl) with the same `__in_chrg` flags, which frees the
 * object when bit 0 is set (a `delete`). */
EffectCtrl::~EffectCtrl()
{
}

/* Ctrl() (InitCtrl), then the vtable pointer, then the body. A g++ 2.x
 * constructor returns `this`. */
EffectCtrl::EffectCtrl()
{
    Reset();
}
