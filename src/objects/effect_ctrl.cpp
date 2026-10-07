#include "ctrl.hpp"
#include "sprite_obj.hpp"

/* The effect controller, ROM 0x0800CBF4-0x0800CD00: the first object
 * built as C++ (#664, docs/cplusplus.md). It's old_agbcp's: the file is
 * in the Makefile's OLD_AGBCC_OBJS (the constant-before-`ldrb` flag ORs
 * in Update are old_agbcc's tell), and cxx_symbols.txt gives the
 * methods their C names. The sprite object it drives is sprite_obj.hpp's
 * MovingSprite. */

/* Marks the effect part gone when it has left the screen, when it has
 * been touched (flags bit 3), or when its animation has played through.
 * The three tests aren't else-chained: each marks it again. MarkGone sets
 * the gone bit through the bitfield view of +0x0C and bit 3 is tested
 * through the byte view: that is what makes the second copy reuse the
 * tested byte and its `1` for the OR, in the ROM's registers. */
void EffectCtrl::Update(MovingSprite *part)
{
    if (!part->IsOnScreen())
        part->MarkGone();
    if ((part->f.flags >> 3) & 1)
        part->MarkGone();
    if (part->animDone)
        part->MarkGone();
}

void EffectCtrl::HandleEvent(MovingSprite *, s32, s32)
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
