#ifndef GUARD_FRAME_LIMITER_HPP
#define GUARD_FRAME_LIMITER_HPP

/* The VBlank wait and its frame limit (src/engine/sys/irq.cpp, before
 * VBlankHandler, #915): every frame loop waits for the next VBlank here,
 * and with the limit on the wait stretches to `interval` VBlanks a frame.
 * Nothing in the ROM turns the limit on.
 *
 * An all-static class, like the interrupt handlers (docs/cplusplus.md,
 * "The frame limiter as an all-static class"): its variables are separate
 * IWRAM globals that no other object reads, and static data members
 * compile like those globals, static member functions like plain
 * functions. cxx_symbols.txt (block "#915 FrameLimiter") maps each member
 * to its C name (`_Q36Engine3Sys12FrameLimiter.target` is gFrameLimitTarget, ...):
 * sym_iwram.txt places `target` and `interval`, iwram_data.cpp defines
 * `enabled` with the other initialised IWRAM globals. The functions keep
 * their C names the same way (`WaitForVBlank__Q36Engine3Sys12FrameLimiter` is
 * WaitForVBlank).
 *
 * In the namespace Engine::Sys (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

namespace Engine
{
namespace Sys
{

class FrameLimiter
{
public:
    static void WaitForVBlank();     // WaitForVBlank
    static void Disable();           // DisableFrameLimit: UNUSED
    static void Set(u32 frameCount); // SetFrameLimit: UNUSED

private:
    static u8 enabled;   // gFrameLimitEnabled (iwram_data.cpp)
    static u32 target;   // gFrameLimitTarget - the gVBlankCounter the wait runs to
    static u32 interval; // gFrameLimitInterval - the VBlanks a frame
};

} // namespace Sys
} // namespace Engine

#endif // GUARD_FRAME_LIMITER_HPP
