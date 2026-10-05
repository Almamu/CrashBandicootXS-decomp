#include "core.h"
#include "irq.h"

extern void IrqClearHandler(s32 interruptIndex);
extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);
extern void LinkSerialIntr(void);
extern void LinkTimer3Intr(void);
extern void LinkStop(void);
extern void ResetLinkSessionState(u8 *self);
extern void IwramFree(u8 *address);

/* "Start" step of the link session - counterpart to `LinkStop`
 * above. Disables the Serial/Timer3 IRQ lines (same IME-guarded
 * pattern), installs `LinkSerialIntr` as the Serial IRQ handler and
 * enables it, and - only if `arm3` is set - also installs `LinkTimer3Intr`
 * as the Timer3 IRQ handler, enables it, and arms Timer3 with a fixed
 * reload/control word. Always ends with IME re-enabled. `arg0` (the
 * session pointer every sibling function in this file takes) is never
 * read past the prologue - the ROM genuinely ignores it here, same as
 * `WaitForVBlank` in src/system/irq.c. */
s32 LinkStart(void *arg0, u32 flags)
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
s32 ResetLinkSession(u8 *self)
{
    LinkStop();
    ResetLinkSessionState(self);
    return 0;
}

/* Resets the session (`ResetLinkSession`), then walks a dead loop computing
 * `self+0xd0` from `self+0x3f0` in steps of 0xc8 (4 iterations, result
 * unused - reads as a leftover/inlined bounds-check artifact rather
 * than anything with an observable effect), then - only if `flags` bit
 * 0 is set - tears the session down (`IwramFree`, matched in
 * src/graphics/aabb_util.c, also used by src/audio/audio_context.c's
 * `DestroyAudioContext` on an unrelated object - a generic free/release call).
 */
void DestroyLinkSession(u8 *self, u32 flags)
{
    u8 *p;

    ResetLinkSession(self);

    p = self + 0xd0;
    if (p != NULL) {
        u8 *q = self + 0x3f0;
        if (p != q) {
            do {
                q -= 0xc8;
            } while (p != q);
        }
    }

    if (flags & 1) {
        IwramFree(self);
    }
}

/* Session object constructor: zeroes the transient TX ring bookkeeping
 * (self+0xc4/0xc8, self+0xcc=0x7f), zeroes the same trio for all 4
 * per-player sub-records (self+0x18c+playerIndex*0xc8, matching the
 * layout `ResetLinkSessionState` above also touches), resets the session
 * (`ResetLinkSession`), clears self+5, and returns `self`. */
void *InitLinkSession(u8 *arg0)
{
    register u8 *self asm("r4");
    u8 *p;
    register s32 offset asm("r6");
    register s32 i asm("r1");
    register s32 zero asm("r2");
    register s32 fill asm("r5");
    register s32 sentinel asm("r3");

    self = arg0;
    *(s32 *)(self + 0xc4) = 0;
    *(s32 *)(self + 0xc8) = 0;
    *(s32 *)(self + 0xcc) = 0x7f;

    i = 3;
    zero = 0;
    fill = 0x7f;
    sentinel = -1;
    offset = 0xc6 << 1;
    p = self + offset;
    do {
        *(s32 *)(p + 0) = zero;
        *(s32 *)(p + 4) = zero;
        *(s32 *)(p + 8) = fill;
        p += 0xc8;
        i--;
    } while (i != sentinel);

    ResetLinkSession(self);
    self[5] = 0;

    return self;
}

extern s32 HandleLinkSerial(void *session, u32 reg);
extern void *gLinkSession;

/* The Serial-IRQ handler installed by `LinkStart` above: forwards
 * into the still-raw per-frame SIO data pump (`HandleLinkSerial`) with the
 * session object and SIODATA32's low half register address. */
void LinkSerialIntr(void)
{
    HandleLinkSerial(gLinkSession, REG_ADDR_SIODATA32);
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
