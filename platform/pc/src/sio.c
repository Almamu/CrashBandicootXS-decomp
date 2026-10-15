/* The serial port with nothing attached: the link is stubbed by the
 * owner's decision (#1093), as a GBA with no cable (docs/platforms.md,
 * "The link").
 *
 * The game's one use of the port is the save menu's "load link game"
 * (SaveMenu::LinkExchange, src/game/ui/save_menu_draw.cpp), which puts it
 * in multi-player mode and waits in LinkSession::Update
 * (src/engine/link/link_session.cpp) for SIOCNT's SD terminal (bit 3,
 * every console ready). With no cable both of the GBA's terminals are
 * pulled high, so it reads SI high (bit 2: a child) and SD high, and its
 * multi-player id 0 with no error; a child's start bit is read only and
 * stays clear, its transfers are the parent's. mGBA's port with no link
 * reads the same, `value & 0xFF83 | 0xC` (it keeps a written start bit,
 * which the game never reads). So the session starts, as a child, installs
 * the serial IRQ, gets no transfer (the platform raises no serial IRQ,
 * irq.c), and after 15 frames resets and starts over, until B leaves the
 * menu.
 *
 * REG_SIOCNT (gba/io_reg.h) is this call: it sets the read-only bits for
 * the mode the game wrote, before the game reads or writes the register
 * through the pointer it returns. The other serial registers are plain
 * memory: SIOMULTI0-3 are only read by the serial IRQ, which never comes. */

#include "gba/gba.h"

volatile uint16_t *HostSioCnt(void)
{
    vu16 *siocnt = (vu16 *)(REG_BASE + REG_OFFSET_SIOCNT);
    u16 value = *siocnt;

    /* Multi-player mode: SIOCNT bits 12-13 = 2, with RCNT bit 15 clear. */
    if (!(*(vu16 *)(REG_BASE + REG_OFFSET_RCNT) & 0x8000) && (value & 0x3000) == SIO_MULTI_MODE) {
        value &= ~(SIO_ID | SIO_ERROR | SIO_MULTI_BUSY);
        value |= SIO_MULTI_SI | SIO_MULTI_SD;
        *siocnt = value;
    }
    return siocnt;
}
