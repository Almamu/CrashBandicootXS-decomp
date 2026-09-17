#include "core.h"
#include "irq.h"

/* The GBA multiplayer link-cable/SIO transport - see docs/rom_map.md's
 * SIO/link-cable section. `sub_8001DB4` resets a per-session object at
 * `gUnknown_03000804` (still uncharacterized beyond the offsets touched
 * here and in src/graphics/settings_menu8a2.c/settings_menu.c); 4
 * per-player 0xc8-byte sub-records live at session+playerIndex*0xc8. */

extern void sub_8000544(s32 interruptIndex);
extern u16 gStaticData_0816AF10[];
extern void sub_8001CB8(u8 *self);
extern u8 gUnknown_03000800;

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: semantics fully understood. Fills `self`'s
 * first 8 bytes with a fixed 0xEC pattern (byte 0 masked to its low
 * nibble, byte 1 zeroed), then hashes bytes 1-5 with a CRC-16-style
 * table walk seeded at 0x1234 (`gStaticData_0816AF10`, also used by the
 * still-raw `sub_8002114`), stores the 16-bit result into bytes 6-7,
 * and folds its low nibble into byte 0's low nibble (preserving byte
 * 0's high nibble). Called by `sub_8001DB4` on each per-player 8-byte
 * sub-record - reads as generating a deterministic per-slot
 * handshake/session id. This project's first attempt at this specific
 * CRC-table-walk idiom (no prior art to copy register choices from):
 * the leading 0xEC-fill loop and the hash loop's setup (`hash`/`p`/`i`/
 * `table`/`mask`/`sentinel` into r1/r2/r4/ip/r6/r5) both land on the
 * ROM's exact registers, but the hash loop's body - specifically
 * computing the per-byte table index (`idx = (hash>>8) ^ *p`) - never
 * reproduces the ROM's plain `lsrs r0,r1,#8` shift; every phrasing
 * tried (plain locals, `idx`/`hash` both pinned, `idx` alone pinned,
 * operand order swapped) either emits a redundant 32-bit truncation
 * dance for the byte extraction or an unrelated r7/r8 round-trip for
 * the `*p` load, and pinning both `idx` (r0) and a separate `byte` (r7)
 * simultaneously hits an internal compiler error (fixed register r0
 * spilled for class LO_REGS) - the same unresolved gcc-2.9 scratch-
 * register class this project documents at length elsewhere (see
 * `sub_8006600`/`sub_80049CC`). Real bytes stay in
 * `asm/code_3_1_10_3_1cb8.s`, wrapped `.if NON_MATCHING == 0`. */
void sub_8001CB8(u8 *arg0)
{
    register u8 *self asm("r3");
    register u8 *cursor asm("r0");
    register u8 fillByte asm("r1");
    register u16 hash asm("r1");
    register u8 *p asm("r2");
    register s32 i asm("r4");
    register u16 *table asm("ip");
    register u8 mask asm("r6");
    register s32 sentinel asm("r5");
    register u16 *tableAddr asm("r7");

    self = arg0;
    cursor = self + 7;
    fillByte = 0xEC;
    do {
        *cursor = fillByte;
        cursor--;
    } while (cursor >= self);

    self[0] &= 0xF;
    self[1] = 0;

    hash = 0x1234;
    p = self + 1;
    i = 4;
    tableAddr = gStaticData_0816AF10;
    table = tableAddr;
    mask = 0xFF;
    sentinel = -1;
    do {
        register u8 idx asm("r0");

        idx = hash >> 8;
        idx ^= *p;
        idx &= mask;
        hash <<= 8;
        hash ^= table[idx];
        p++;
        i--;
    } while (i != sentinel);

    self[6] = (u8)hash;
    self[7] = (u8)(hash >> 8);
    self[0] = (self[0] & 0xF0) | (hash & 0xF);
}
#endif /* NON_MATCHING */

/* "Stop" step of the link session: disables the Serial and Timer3 IRQ
 * lines (each individually IME-guarded), clears their installed
 * handlers, restores IME, resets RCNT to general-purpose mode, sets
 * SIOCNT to a fixed idle value, reloads Timer3 (stopped) with 0xBBBC,
 * and acknowledges both IRQ flags in IF. Always returns 0. */
s32 sub_8001D30(void)
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

    sub_8000544(INTR_INDEX_SERIAL);
    sub_8000544(INTR_INDEX_TIMER3);

    REG_IME = 1;

    REG_RCNT = 0;
    REG_SIOCNT = 0x3000;
    REG_TM3CNT = 0xBBBC;

    REG_IF |= 0x80;
    REG_IF |= 0x40;

    return 0;
}

#if NON_MATCHING
/* NOT YET BYTE-MATCHING: semantics fully understood. Link-session
 * reset/init - see docs/rom_map.md's SIO/link-cable section. Sets the
 * link-active flag (`gUnknown_03000800`), resets a handful of
 * session-header fields, seeds a per-session 8-byte handshake id via
 * `sub_8001CB8` (self+0x30), mirrors that id into two more
 * session-header slots and every one of the 4 per-player 0xc8-byte
 * sub-records (self+playerIndex*0xc8), resets each per-player
 * sub-record's own RX-ring bookkeeping, writes the literal `0x1234`
 * link sync/ready magic into each player's data-exchange field
 * (self+playerIndex*0xc8+0xd6/0xd8), and finally programs
 * SIOMLT_SEND from the session header's own copy of the id. Every
 * load/store, branch and call is semantically confirmed against the
 * ROM (this doc comment walks the whole function field-by-field), but
 * the ROM keeps `self` in r5 and threads `r8`/`sb`/`sl`/`ip` plus four
 * cached pointers on the stack through its 4-player loop, while this
 * compiler - even with `self` pinned - allocates a materially
 * different register/stack plan (e.g. `self` lands in r6, and large
 * struct-offset immediates like `+0x18c` get built as a single
 * `movs/lsls` pair instead of the ROM's two-step `+0x108` then `+0x84`
 * decomposition). Given the size of this function (400 B, a 4-player
 * nested loop) and this project's documented gcc-2.9 scratch-register
 * nondeterminism (see `sub_8006600`/`sub_80049CC`/`sub_8001CB8`
 * above), full register-pin archaeology wasn't attempted here - the
 * gap is almost certainly the same unresolved class, not a semantic
 * error. Real bytes stay in `asm/code_3_1_10_3_1db4.s`, wrapped
 * `.if NON_MATCHING == 0`. */
void sub_8001DB4(u8 *self)
{
    s32 i;
    u8 *p;

    self[6] = 0;
    self[8] = 0;
    self[7] = 0;
    gUnknown_03000800 = 1;
    self[4] = 0;
    *(s32 *)(self + 0x1c) = -1;
    *(s32 *)(self + 0x3fc) = -1;
    *(s32 *)(self + 0xc4) = 0;
    *(s32 *)(self + 0xc8) = 0;
    *(s32 *)(self + 0xcc) = 0x7f;

    sub_8001CB8(self + 0x30);

    p = self + 0x28;
    for (i = 3; i >= 0; i--) {
        u8 lo = p[8];
        u8 hi = p[9];
        p[0] = lo;
        p[1] = hi;
        p += 2;
    }

    *(s32 *)(self + 0xc) = 0;
    *(s32 *)(self + 0x24) = 0;

    for (i = 0; i <= 3; i++) {
        u8 *player = self + i * 0xc8;
        u8 *src = self + 0x30;
        u8 *dst = player + 0xd0;
        s32 j;
        u8 v;

        *(s32 *)(player + 0x18c) = 0;
        *(s32 *)(player + 0x190) = 0;
        *(s32 *)(player + 0x194) = 0x7f;
        *(s32 *)(self + 0x100 + i * 0xc8) = 0;

        for (j = 0; j <= 3; j++) {
            u8 lo = src[0];
            u8 hi = src[1];
            dst[0] = lo;
            dst[1] = hi;
            dst += 2;
            src += 2;
        }

        v = player[0xd1];
        player[0xd1] = (v & 0xF0) | (((v & 0xF) - 1) & 0xF);

        *(s32 *)(player + 0x104) = 0;
        *(s32 *)(player + 0xfc) = 0;

        *(u16 *)(player + 0xd6) = 0x1234;
        *(u16 *)(player + 0xd8) = 0x1234;
    }

    *(s32 *)(self + 0x3f0) = 0;
    *(s32 *)(self + 0x3f4) = 0;
    *(s32 *)(self + 0x3f8) = 0;
    *(s32 *)(self + 0x38) = 0;
    *(s32 *)(self + 0x3c) = 0;

    *(u16 *)(self + 0x20) = (*(u16 *)(self + 0x20) & 0xF) | 0xF0B0;
    self[0x20] &= 0xF0;

    *(u16 *)(self + 0x400) = *(u16 *)(self + 0x20);
    REG_SIOMLT_SEND = *(u16 *)(self + 0x400);
}
#endif /* NON_MATCHING */
