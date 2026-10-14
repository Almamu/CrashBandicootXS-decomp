#ifndef GUARD_DISPLAY_HPP
#define GUARD_DISPLAY_HPP

/* The display registers' shadows (src/gfx/display.cpp, #915): REG_DISPCNT
 * as `dispcnt`, which the Show/Hide helpers edit a bit at a time and
 * CommitDispcnt writes, and the blend registers as `blendRegs`, which
 * SetupRoomBlend builds and CommitBlendRegs writes. The title screen, the
 * logos, the credits, the starfield and the rooms edit `dispcnt` directly
 * too.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The display registers as an all-static class"): the shadows are two
 * IWRAM globals (0x03001280-0x0300128B), and static data members compile
 * like those globals, static member functions like plain functions. The
 * data members are only declared: cxx_symbols.txt (block "#915 Display")
 * maps each one to its C name (`_Q36Engine3Sys7Display.dispcnt` is gDispcnt, ...),
 * whose address is in sym_iwram.txt, and the functions keep their C names
 * the same way (`ShowBg2__Q36Engine3Sys7Display` is ShowBg2).
 *
 * In the namespace Engine::Sys (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "gfx.h"
}

namespace Engine
{
namespace Sys
{

class Display
{
public:
    static void SetMode(s32 val);  // SetDispcntMode: DISPCNT_MODE_*
    static void HideBg3();         // HideBg3
    static void HideBg2();         // HideBg2
    static void HideBg1();         // HideBg1
    static void HideBg0();         // HideBg0
    static void HideObj();         // HideObj
    static void ShowBg3();         // ShowBg3
    static void ShowBg2();         // ShowBg2
    static void ShowBg1();         // ShowBg1
    static void ShowBg0();         // ShowBg0
    static void ShowObj();         // ShowObj
    static void SetObjMapping2D(); // SetObjMapping2D: UNUSED
    static void SetObjMapping1D(); // SetObjMapping1D
    static void CommitDispcnt();   // CommitDispcnt: dispcnt to REG_DISPCNT
    static void CommitBlendRegs(); // CommitBlendRegs: blendRegs to BLDCNT/BLDALPHA/BLDY

    /* IWRAM (sym_iwram.txt) */
    static struct blend_regs blendRegs; // gBlendRegs
    static union dispcnt dispcnt;       // gDispcnt
};

} // namespace Sys
} // namespace Engine

#endif // GUARD_DISPLAY_HPP
