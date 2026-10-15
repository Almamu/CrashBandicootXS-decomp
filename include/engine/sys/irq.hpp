#ifndef GUARD_IRQ_HPP
#define GUARD_IRQ_HPP

/* The interrupt handlers (src/engine/sys/irq.cpp, #915): the per-IRQ handler
 * table IntrMain (platform/gba/asm/intr_main.s) dispatches through, the handlers each
 * IrqSetHandler call replaced, and the VBlank handler with its eight
 * callback slots. The audio (the VCount IRQ), the link (serial, timer 3),
 * the fades (a VBlank callback) and AgbMain use it.
 *
 * An all-static class, like the yeti and the airship (docs/cplusplus.md,
 * "The interrupt handlers as an all-static class"): the tables are
 * separate IWRAM globals (0x030009E8-0x03000A7F) that no other C++ object
 * reads, and static data members compile like those globals, static
 * member functions like plain functions. The data members are only
 * declared: cxx_symbols.txt (block "#915 Irq") maps each one to its C
 * name (`_Q36Engine3Sys3Irq.table` is gIntrTable, ...), whose address is in
 * games/xs/sym_iwram.txt and which crt0.s and intr_main.s use, and the functions
 * keep their C names the same way (`SetHandler__Q36Engine3Sys3IrqiPFv_v` is
 * IrqSetHandler).
 *
 * In the namespace Engine::Sys (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
#include "engine/sys/irq.h"
}

namespace Engine
{
namespace Sys
{

class Irq
{
public:
    static void ClearHandler(s32 interruptIndex);                 // IrqClearHandler
    static void RestoreHandler(s32 interruptIndex);               // IrqRestoreHandler
    static void SetHandler(s32 interruptIndex, irq_handler_t fn); // IrqSetHandler
    static void Disable();                                        // IrqDisable
    static u32 Setup();                                           // IrqSetup
    static void EnableVBlankHandler();                            // EnableVBlankHandler
    static void DisableVBlankHandler();                           // DisableVBlankHandler
    static void RemoveVBlankCallback(s32 index);                  // RemoveVBlankCallback
    static s32 AddVBlankCallback(void (*fn)());                   // AddVBlankCallback

private:
    static void EmptyHandler();  // IrqEmptyHandler: an IRQ with no handler
    static void VBlankHandler(); // VBlankHandler: the VBlank IRQ's handler

    /* IWRAM (games/xs/sym_iwram.txt) */
    static irq_handler_t table[14];                 // gIntrTable - one per REG_IF bit
    static irq_handler_t prevTable[14];             // gPrevIntrTable - what SetHandler replaced
    static struct vblank_callbacks vblankCallbacks; // gVBlankCallbacks
};

} // namespace Sys
} // namespace Engine

#endif // GUARD_IRQ_HPP
