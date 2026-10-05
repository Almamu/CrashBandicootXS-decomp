#include "core.h"
#include "irq.h"
#include "link.h"

/* The GBA multiplayer link-cable/SIO transport - see docs/rom_map.md's
 * SIO/link-cable section. `ResetLinkSessionState` resets a per-session object at
 * `gLinkSession` (still uncharacterized beyond the offsets touched
 * here and in src/save/save_transfer.c/save_menu_draw.c); 4
 * per-player 0xc8-byte sub-records live at session+playerIndex*0xc8. */

extern void IrqClearHandler(s32 interruptIndex);
extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);

/* Fills `self`'s first 8 bytes with a fixed 0xEC pattern (byte 0 masked
 * to its low nibble, byte 1 zeroed), then hashes bytes 1-5 with a
 * CRC-16-style table walk seeded at 0x1234 (`gCrc16Table`,
 * also used by `HandleLinkSerial`), stores the 16-bit result
 * into bytes 6-7, and folds its low nibble into byte 0's low nibble
 * (preserving byte 0's high nibble; that high nibble is always 0 at
 * this point from the earlier mask, which is why the ROM's own
 * `(self[0]>>4) + hash` tail - reconstructing `hash` from the just-
 * stored bytes 6/7 rather than reusing the register - reduces to plain
 * `hash & 0xF`). Called by `ResetLinkSessionState` on each per-player 8-byte
 * sub-record - reads as generating a deterministic per-slot
 * handshake/session id.
 *
 * Once a NAKED transcription; it matches as plain C under old_agbcc
 * (link_handshake.o is on the Makefile's OLD_AGBCC_OBJS) with no pins
 * (docs/matching/early-rom-naked-retry-2.md). The fill loop is written
 * over an integer address so the compare is the ROM's signed
 * `cmp; bge` (the ROM got it from strength-reducing `self[i]`, which
 * gcc here declines: "giv not worth while"); `c` ahead of it puts the
 * 0xec load first. `hash` is a u32 truncated with a `(u16)` cast each
 * step, and read back as `(u16)hash >> 8` into a u32 `hi`: with a u16
 * `hash`, CSE folds `hash >> 8` into `(x << 16) >> 24` of the loop's
 * zero-extend temporary. The new low nibble is masked in SImode
 * (`w`) before the bitfield store, so the 15 is an SImode constant and
 * reload's move2add derives the -16 from it (`sub r0, #0x1f`); a QImode
 * 15 gives `mov #16; neg`. */
void MakeLinkHandshakeId(u8 *self)
{
    u8 *p;
    u32 hash;
    s32 i;

    {
        u8 c = 0xec;
        s32 a = (s32)self + 7;

        do {
            *(u8 *)a = c;
        } while (--a >= (s32)self);
    }
    self[0] &= 0xf;
    self[1] = 0;
    hash = 0x1234;
    p = self + 1;
    for (i = 4; i != -1; i--) {
        hash = (u16)(gCrc16Table[((hash >> 8) ^ *p) & 0xff] ^ (hash << 8));
        p++;
    }
    {
        u32 hi;
        u32 v, n;
        s32 w;

        self[6] = hash;
        self[7] = hi = (u16)hash >> 8;
        n = ((struct nibble_pair *)self)->hi;
        v = (hi << 8) | self[6];
        w = (n + v) & 0xf;
        ((struct nibble_pair *)self)->lo = w;
    }
}

/* "Stop" step of the link session: disables the Serial and Timer3 IRQ
 * lines (each individually IME-guarded), clears their installed
 * handlers, restores IME, resets RCNT to general-purpose mode, sets
 * SIOCNT to a fixed idle value, reloads Timer3 (stopped) with 0xBBBC,
 * and acknowledges both IRQ flags in IF. Always returns 0. Its callers
 * in this file pass the session in r0 (`self` is unused), which the
 * NON_MATCHING drafts below reproduce by passing it. */
s32 LinkStop(struct link_session *self)
{
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

    IrqClearHandler(INTR_INDEX_SERIAL);
    IrqClearHandler(INTR_INDEX_TIMER3);

    REG_IME = 1;

    REG_RCNT = 0;
    REG_SIOCNT = 0x3000;
    REG_TM3CNT = 0xBBBC;

    REG_IF |= 0x80;
    REG_IF |= 0x40;

    return 0;
}
