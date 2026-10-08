#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "link.h"
#include "util.h"
}

/* "Start" step of the link session - counterpart to `Stop`
 * (link_handshake.cpp). UNUSED: Update writes the same steps out. Disables the Serial/Timer3 IRQ lines (same IME-guarded
 * pattern), installs `LinkSerialIntr` as the Serial IRQ handler and
 * enables it, and - only if `arm3` is set - also installs `LinkTimer3Intr`
 * as the Timer3 IRQ handler, enables it, and arms Timer3 with a fixed
 * reload/control word. Always ends with IME re-enabled. `this` is never
 * read - the ROM genuinely ignores it here, same as `WaitForVBlank` in
 * src/system/irq.cpp. */
s32 LinkSession::Start(u32 flags)
{
    u8 arm3 = (u8)flags;
    u16 savedIme;

    REG_IME = 0;
    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x80;
    REG_IME = savedIme;

    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE &= ~0x40;
    REG_IME = savedIme;

    IrqClearHandler(INTR_INDEX_TIMER3);
    IrqSetHandler(INTR_INDEX_SERIAL, LinkSerialIntr);
    REG_IE |= 0x80;

    if (arm3 != 0) {
        IrqSetHandler(INTR_INDEX_TIMER3, LinkTimer3Intr);
        REG_IE |= 0x40;
        REG_TM3CNT = 0x00C0BBBC;
    }

    REG_IME = 1;
    return 0;
}

/* Small standalone helper: resets RCNT to general-purpose mode and sets
 * SIOCNT to a fixed idle-multiplayer-mode value. Always returns 0.
 * UNUSED. */
s32 LinkSetupSio(void)
{
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT |= 0x4003;
    return 0;
}

/* Convenience "full reset": stop (`Stop`) then re-init the session
 * (`ResetState`). Always returns 0. */
s32 LinkSession::Reset()
{
    Stop();
    ResetState();
    return 0;
}

/* `delete gLinkSession` (DestroyLinkSession): resets the session
 * (`Reset`); g++ then destroys the members, the four players in a loop
 * from `&players[4]` back to `players` that does nothing (LinkRing's
 * destructor is empty), and frees the session when `__in_chrg` bit 0 is
 * set (the class's operator delete, IwramFree). */
LinkSession::~LinkSession()
{
    Reset();
}

/* `new LinkSession` (InitLinkSession): g++ resets the session's outgoing
 * ring and the four players' incoming rings (LinkRing's constructor:
 * `count`/`readPos` 0, `writePos` 0x7f), then the body resets the
 * session (`Reset`) and clears `enabled`. */
LinkSession::LinkSession()
{
    Reset();
    enabled = 0;
}

/* The Serial-IRQ handler installed by `Start` above and by Update:
 * forwards into the SIO data pump (`HandleSerial`) with SIODATA32's low
 * half register address. */
void LinkSerialIntr(void)
{
    gLinkSession->HandleSerial((u16 *)REG_ADDR_SIODATA32);
}

/* The Timer3-IRQ handler installed by `Start` above (the
 * handshake-timeout retry beat): re-arms Timer3 (stop, set SIOCNT's
 * start-transfer bit, restart). */
void LinkTimer3Intr(void)
{
    REG_TM3CNT_H = 0;
    REG_SIOCNT |= 0x80;
    REG_TM3CNT_H = 0xC0;
}
