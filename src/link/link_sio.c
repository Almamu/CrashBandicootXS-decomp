#include "core.h"
#include "irq.h"
#include "link.h"
#include "util.h"

extern void IrqClearHandler(s32 interruptIndex);
extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);

/* "Start" step of the link session - counterpart to `LinkStop`
 * above. Disables the Serial/Timer3 IRQ lines (same IME-guarded
 * pattern), installs `LinkSerialIntr` as the Serial IRQ handler and
 * enables it, and - only if `arm3` is set - also installs `LinkTimer3Intr`
 * as the Timer3 IRQ handler, enables it, and arms Timer3 with a fixed
 * reload/control word. Always ends with IME re-enabled. `self` (the
 * session pointer every sibling function in this file takes) is never
 * read past the prologue - the ROM genuinely ignores it here, same as
 * `WaitForVBlank` in src/system/irq.c. */
s32 LinkStart(struct link_session *self, u32 flags)
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
    IrqSetHandler(INTR_INDEX_SERIAL, (irq_handler_t *)LinkSerialIntr);
    REG_IE |= 0x80;

    if (arm3 != 0) {
        IrqSetHandler(INTR_INDEX_TIMER3, (irq_handler_t *)LinkTimer3Intr);
        REG_IE |= 0x40;
        REG_TM3CNT = 0x00C0BBBC;
    }

    REG_IME = 1;
    return 0;
}

/* Small standalone helper: resets RCNT to general-purpose mode and sets
 * SIOCNT to a fixed idle-multiplayer-mode value. Always returns 0. */
s32 LinkSetupSio(void)
{
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT |= 0x4003;
    return 0;
}

/* Convenience "full reset": stop (`LinkStop`) then re-init the
 * session (`ResetLinkSessionState`). Always returns 0. */
s32 ResetLinkSession(struct link_session *self)
{
    LinkStop(self);
    ResetLinkSessionState(self);
    return 0;
}

/* Resets the session (`ResetLinkSession`), then walks a dead loop from
 * `&players[4]` back to `players` (4 iterations, result unused - the
 * empty destructor loop of the `players` array), then - only if `flags` bit
 * 0 is set - tears the session down (`IwramFree`, matched in
 * src/util/aabb.c, also used by src/audio/audio.c's
 * `DestroyAudioContext` on an unrelated object - a generic free/release call).
 */
void DestroyLinkSession(struct link_session *self, u32 flags)
{
    struct link_player *p;

    ResetLinkSession(self);

    p = self->players;
    if (p != NULL) {
        struct link_player *q = &self->players[4];
        if (p != q) {
            do {
                q--;
            } while (p != q);
        }
    }

    if (flags & 1) {
        IwramFree((u8 *)self);
    }
}

/* Session object constructor: zeroes the transient TX ring bookkeeping
 * (`ring.count`/`readPos`, `writePos` = 0x7f), zeroes the same trio
 * for all 4 per-player rings (`players[i].ring`, at self+0x18c +
 * playerIndex*0xc8; the loop keeps the raw offset, which the ROM builds
 * as `0xc6 << 1` in r6), resets the session (`ResetLinkSession`), clears
 * `field_5`, and returns `self`. */
struct link_session *InitLinkSession(struct link_session *arg0)
{
    register struct link_session *self asm("r4");
    u8 *p;
    register s32 offset asm("r6");
    register s32 i asm("r1");
    register s32 zero asm("r2");
    register s32 fill asm("r5");
    register s32 sentinel asm("r3");

    self = arg0;
    self->ring.count = 0;
    self->ring.readPos = 0;
    self->ring.writePos = 0x7f;

    i = 3;
    zero = 0;
    fill = 0x7f;
    sentinel = -1;
    offset = 0xc6 << 1;
    p = (u8 *)self + offset;
    do {
        *(s32 *)(p + 0) = zero;
        *(s32 *)(p + 4) = zero;
        *(s32 *)(p + 8) = fill;
        p += 0xc8;
        i--;
    } while (i != sentinel);

    ResetLinkSession(self);
    self->field_5 = 0;

    return self;
}

/* The Serial-IRQ handler installed by `LinkStart` above: forwards
 * into the still-raw per-frame SIO data pump (`HandleLinkSerial`) with the
 * session object and SIODATA32's low half register address. */
void LinkSerialIntr(void)
{
    HandleLinkSerial(gLinkSession, (u16 *)REG_ADDR_SIODATA32);
}

/* The Timer3-IRQ handler installed by `LinkStart` above (the
 * handshake-timeout retry beat): re-arms Timer3 (stop, set SIOCNT's
 * start-transfer bit, restart). */
void LinkTimer3Intr(void)
{
    REG_TM3CNT_H = 0;
    REG_SIOCNT |= 0x80;
    REG_TM3CNT_H = 0xC0;
}
