#include "core.h"
#include "irq.h"
#include "link_session.h"

/* The link-cable session's per-frame handshake driver and SIO data pump
 * (split from link_handshake.c so ResetLinkSessionState can sit in its own object;
 * see link_session_reset.c). */

extern void IrqClearHandler(s32 interruptIndex);
extern u16 gCrc16Table[];
extern void IrqSetHandler(s32 interruptIndex, irq_handler_t *fn);
extern void LinkSerialIntr(void);
extern void LinkTimer3Intr(void);
extern s32 LinkStop(struct link_session *self);
extern s32 ResetLinkSessionState(struct link_session *self);

/* Link-connection/handshake driver - see docs/rom_map.md's SIO/link-
 * cable section. Called repeatedly (once per frame) until the link is
 * established or times out. `self+5` gates a one-time SIOCNT setup
 * (multiplayer mode, `0x4003`); `self+6` gates a second one-time
 * SIOCNT poke once `self+8` (the "IRQs installed" flag) is still
 * clear. Once `self+8` is clear, checks `REG_SIOCNT` bit 3 (the
 * multi-player "ready" bit): if not ready yet, resets the session
 * (`LinkStop`) and re-primes SIOCNT/SIODATA32_H, returning 0
 * ("still connecting"). If ready, flips `self+8`, derives an "is
 * player 0 / arm3" flag from `REG_SIOCNT` bits, resets `REG_SIODATA32`
 * (via `REG_TM3CNT`/`0x04000208` toggling, matching
 * `LinkStart`-family's RCNT/SIOCNT reset shape in
 * src/link/link_sio.c), installs the Serial IRQ handler
 * (`LinkSerialIntr`) always and the Timer3 IRQ handler (`LinkTimer3Intr`)
 * only when the "arm3" flag is set, programs Timer3 as a running
 * countdown timeout (`0x00C0BBBC`) in that case, and resets several
 * per-session timeout/retry fields (`self+0x1c`=-1, `self+0x14`,
 * `self+0x404`=0, `self+0x18`=0). From there (and on every call once
 * `self+8` is already set), it advances a handshake-timeout counter at
 * `self+0x404`: past 15 it forces `self+0xc`=-15 and rearms
 * `self+0x14` to a large timeout constant (0x708); once `self+7`
 * (retry-exhausted flag) is still clear, it walks a retry-backoff
 * counter at `self+0xc` (incrementing or decrementing depending on
 * whether `self+0xfc`'s stored `s32` is negative), clamping it into
 * `self+7`/giving up (return 0, via the same early-exit path as the
 * top-of-function "still initializing" case) once it exceeds -15, or
 * resetting the session (`LinkStop`+`ResetLinkSessionState`) and retrying
 * once it exceeds 14; either way it refreshes `self+0x10`/`self+0x14`
 * from each other (keeping the larger, with `self+0x18` forcing a
 * reset to 0) and, past a 0x1d threshold, resets the session again.
 * Finally increments `self+0xc` and returns 1 ("handshake in
 * progress/succeeded").
 *
 * Matched in the second near-miss sweep (37 halfwords before). The ROM
 * materializes two separate 1s after reading SIOCNT: r2 (copied to sb)
 * for `field_8`/IME and r1 for the arm3 flag. Three things reproduce
 * that:
 * - `asm("" : "+r"(one1))` keeps the flag's 1 from being merged into
 *   `one`.
 * - The ready test uses a literal 1, so `one` is a copy of that
 *   constant's register.
 * - `asm("" : "+r"(arm3))` between the eor and the and stops combine
 *   from folding `(x ^ 1) & 1` into a `bic`, which the ROM doesn't have.
 * Both asm statements emit no code. Matches under both compilers. */
s32 UpdateLinkSession(struct link_session *self)
{
    s32 arm3;
    u16 saved;
    s32 one;

    if (!self->field_5)
        return 0;
    if (!self->field_6) {
        REG_RCNT = 0;
        REG_SIOCNT = 0x2000;
        REG_SIOCNT |= 0x4003;
        self->field_6 = 1;
    }
    if (!self->field_8) {
        s32 one1;
        u32 v = REG_SIOCNT >> 3;

        one1 = 1;
        asm("" : "+r"(one1)); /* keep the flag's own 1 (r1) */
        one = 1;
        if (!(v & 1)) {
            LinkStop(self);
            REG_RCNT = 0;
            REG_SIOCNT = 0x2000;
            REG_SIOCNT |= 0x4003;
            return 0;
        }
        self->field_8 = one;
        arm3 = (REG_SIOCNT >> 2) ^ one1;
        asm("" : "+r"(arm3)); /* keep eor/and, not bic */
        arm3 &= one1;
        REG_IME = 0;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~0x80;
        REG_IME = saved;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~0x40;
        REG_IME = saved;
        IrqClearHandler(INTR_INDEX_TIMER3);
        IrqSetHandler(INTR_INDEX_SERIAL, (irq_handler_t *)LinkSerialIntr);
        REG_IE |= 0x80;
        if (arm3) {
            IrqSetHandler(INTR_INDEX_TIMER3, (irq_handler_t *)LinkTimer3Intr);
            REG_IE |= 0x40;
            REG_TM3CNT = 0x00C0BBBC;
        }
        REG_IME = one;
        self->field_3fc = -1;
        self->field_14 = 0;
        self->field_404 = 0;
        self->field_18 = 0;
    }
    if (self->field_404 > 15) {
        self->field_c = -15;
        self->field_14 = 0x708;
    }
    self->field_404++;
    if (!self->field_7) {
        if (self->field_3fc < 0)
            self->field_c = self->field_c - 1;
        else
            self->field_c = self->field_c + 1;
        if (self->field_c > 14) {
            self->field_7 = 1;
            self->field_14 = 0;
            self->field_10 = 0;
        } else if (self->field_c > -15) {
            return 0;
        } else {
            LinkStop(self);
            ResetLinkSessionState(self);
        }
    }
    {
        s32 a = self->field_10;
        s32 b = self->field_14;

        if (a < b)
            a = b;
        self->field_10 = a;
        b = self->field_18 ? 0 : b + 1;
        self->field_14 = b;
        self->field_18 = 0;
        if (b > 0x1d) {
            LinkStop(self);
            ResetLinkSessionState(self);
        }
    }
    self->field_c++;
    return 1;
}

/* Per-frame SIO data-exchange pump - see docs/rom_map.md's SIO/link-
 * cable section (called from the Serial IRQ handler `LinkSerialIntr` in
 * src/link/link_sio.c, with the session object and SIODATA32's
 * low half as the two arguments). `docs/rom_map.md` confirms the
 * high-level shape: manipulates `REG_SIOCNT`/`SIODATA8`
 * (`0x04000128`/`0x0400012A`), checking specific control bits before
 * touching per-object state - it was "not read to completion" there.
 * This pass read it further without reaching full per-branch
 * confidence, so it's recorded here as a best-effort guide for whoever
 * attempts a real C reconstruction next, not as a verified spec: if
 * `self+4` (a "connected" flag also touched by `UpdateLinkSession` above) is
 * already set, it just mirrors the outgoing word into SIOMLT_SEND and
 * returns; otherwise, on the first call it seeds SIOMLT_SEND from
 * `self+0x20` and sets that flag, and on every call after that it
 * inspects `REG_SIOCNT`'s per-slot bits against the incoming word to
 * work out which of the 4 multiplayer slots are actually present,
 * latching player-count-derived fields once that stabilizes across a
 * few frames (`self+0x1c`) and marking the link "ready" (`self+7`).
 * Past that point each call walks the 4 player sub-records
 * (`self+i*0xc8`, i=0..3, same region as `ResetLinkSessionState`'s struct above)
 * and, once every 4 samples, runs the exact same CRC-16 table walk
 * `MakeLinkHandshakeId` uses (`gCrc16Table`) over each slot's 8-byte
 * handshake-id mirror - this part is a confident read, since the loop
 * body is textually identical to `MakeLinkHandshakeId`'s - feeding the result
 * into per-slot bookkeeping this pass did not fully trace (fields
 * around `self+0x30`/`self+0xc4`/`self+0xbc`/`self+0x36`/`self+0x38`/
 * `self+0x3c`), before deciding what to send out next over SIOMLT_SEND.
 *
 * Once a NAKED transcription (1488 bytes, this project's biggest); it
 * now matches as plain C under old_agbcc (link_handshake.o is on the
 * Makefile's OLD_AGBCC_OBJS). History: docs/matching/big-naked-retry-3.md,
 * early-rom-naked-retry-2.md, last-four-naked-retry.md,
 * last-six-naked-retry.md and last-seven-naked-retry.md (the first
 * receive loop, closed last: the load goes through a pointer biv and
 * the compare constants use the constant-init form).
 * `data` is SIOMULTI0-3 (link_sio.c passes 0x04000120). */
/* A received SIOMULTI word, read back from a stack copy. */
struct link_rx_word {
    u32 lo:4;
    u32 hi:12;
    u32 unused_10:16;
};

#define LINK_NIB(p) (*(struct nibble_pair *)(p))

/* The CRC-16 walk MakeLinkHandshakeId also uses, over bytes 1-5 of an id. */
#define LINK_HASH(hash, p)                                                     \
    {                                                                          \
        s32 _k;                                                                \
        u8 *_p = (p);                                                          \
                                                                               \
        for (_k = 4; _k != -1; _k--) {                                         \
            hash = gCrc16Table[((hash >> 8) ^ *_p) & 0xff] ^ (hash << 8); \
            _p++;                                                              \
        }                                                                      \
    }

/* Copies an 8-byte id as four byte-assembled halfwords. */
#define LINK_COPY_ID(dst, src)                                                 \
    {                                                                          \
        s32 _j;                                                                \
                                                                               \
        for (_j = 0; _j <= 3; _j++) {                                          \
            u32 _v = ((src)[_j * 2 + 1] << 8) | (src)[_j * 2];                 \
            u32 _lo = _v & 0xff;                                               \
                                                                               \
            (dst)[_j * 2] = _lo;                                               \
            (dst)[_j * 2 + 1] = _v >> 8;                                       \
        }                                                                      \
    }

/* The session ring pop. `rf` is a second copy of the ring pointer for
 * the fast loop: the ROM builds that loop's field addresses from a copy
 * made right after the id copy (`adds r4, r7, #0`), and the wrap loop's
 * from the original. The bounds test goes through `rd`, the caller's
 * `&ring.field_88`. */
static inline void LinkRingPop(struct link_ring *r, struct link_ring *rf, u8 *dst, s32 n, s32 *rd)
{
    s32 k;

    if (*rd < 0x80 - n) {
        for (k = n - 1; k != -1; k--) {
            *dst++ = rf->buf[rf->field_88];
            rf->field_88++;
            rf->field_84--;
        }
    } else {
        for (k = n - 1; k != -1; k--) {
            s32 old = r->field_88;
            s32 nw = 0;

            if (old != 0x7f)
                nw = old + 1;
            r->field_88 = nw;
            r->field_84--;
            *dst++ = r->buf[old];
        }
    }
}

void HandleLinkSerial(struct link_session *self, u16 *data)
{
    struct link_rx_word w[4];
    s32 i;
    s32 changed;
    s32 nib;
    u16 siocnt;
    u32 one;

    /* Instruction-count padding (no code): shifts gcc's temporary
     * numbering so two stack slots come out in the ROM's order. */
    asm("");
    self->field_404 = 0;
    if (self->field_4) {
        u16 v = self->field_400;

        REG_SIOMLT_SEND = v;
        return;
    }
    one = 1;
    self->field_4 = one;
    siocnt = REG_SIOCNT;
    changed = 0;
    nib = LINK_NIB(&self->id[1]).lo + 1;
    nib &= 0xf;
    if ((siocnt >> 6) & one)
        goto send;
    if (!self->field_7) {
        s32 nId, nFree, same;
        struct link_id_word *f20;

        if (!((siocnt >> 3) & one))
            goto send;
        nId = 0;
        nFree = 0;
        {
            u16 *d2 = data;
            u16 *p;
            u32 kid, kfree;

            /* Set before the loop, in the ROM's order: `&self->field_20`,
             * then the two compare constants, then the load pointer. The
             * constants use the constant-init form (no code beyond the
             * `movs`/`ldr`) so loop.c doesn't hoist them after `p`'s init. */
            f20 = &self->field_20;
            asm("" : "=r"(kid) : "0"(0xF0B));
            asm("" : "=r"(kfree) : "0"(0xffff));
            p = data;
            for (i = 0; i <= 3; i++) {
                /* The test address is taken first, so its giv is found
                 * before `w[i]`'s; the load goes through the pointer biv
                 * `p`, which isn't strength-reduced. That gives the ROM's
                 * giv order (w in r2, test in r3). */
                u16 *t = &d2[i];

                w[i] = *(struct link_rx_word *)p;
                if (w[i].hi == kid)
                    nId++;
                if (*t == kfree)
                    nFree++;
                p++;
            }
        }
        same = 1;
        for (i = 0; i < nId; i++) {
            if (w[i].lo != nId)
                same = 0;
        }
        if (nFree + nId == 4 && same && nId > 1) {
            self->field_1c = nId;
            {
                s32 me = (REG_SIOCNT & 0x30) >> 4;

                self->field_3fc = me;
            }
            self->field_3f8 = ~(-1 << nId);
            self->field_3f8 &= ~(1 << self->field_3fc);
        }
        f20->lo = nId;
        {
            u16 *dst = &self->field_400;

            *dst = *(u16 *)&self->field_20;
        }
        goto send;
    }

    for (i = 0; i < self->field_1c; i++) {
        struct link_player *p;
        u8 *q;
        s32 ok;

        if (i == self->field_3fc)
            continue;
        p = &self->players[i];
        p->rx[p->field_2c] = data[i];
        p->field_2c = (p->field_2c + 1) & 0xf;
        if (p->field_2c <= 3)
            continue;
        q = (u8 *)&p->rx[p->field_2c - 4];
        ok = 0;
        if (LINK_NIB(&q[1]).lo == LINK_NIB(&q[0]).hi
         || LINK_NIB(&q[1]).lo == ((LINK_NIB(&q[0]).hi - 1) & 0xf)) {
            if (LINK_NIB(&q[1]).hi <= 4) {
                /* A u8 against a u16: the compare is done in HImode, so
                 * `lo`'s zero-extension is emitted at the compare (the
                 * ROM's split lsls/lsrs #28 pair). */
                u8 lo = LINK_NIB(&q[0]).lo;
                u16 sum = (u16)(LINK_NIB(&q[0]).hi + ((q[7] << 8) | q[6])) % 16;

                if (lo == sum)
                    ok = 1;
            }
        }
        if (!ok)
            continue;
        {
            if ((q[1] & 0xf) == (p->id[1] & 0xf)) {
                u16 want = (q[7] << 8) | q[6];
                u16 hash = p->field_8;

                LINK_HASH(hash, &q[1]);
                if (want == hash)
                    goto copy;
                continue;
            } else {
                u16 want;
                u16 hash;
                s32 n, k;
                u8 *src;

                if (LINK_NIB(&q[1]).lo != p->field_30)
                    continue;
                want = (q[7] << 8) | q[6];
                hash = p->field_6;
                LINK_HASH(hash, &q[1]);
                if (want != hash)
                    continue;
                n = p->id[1] >> 4;
                /* Extra reference (no code): raises `n`'s priority so it
                 * gets its own register (r7) instead of reusing the
                 * id-byte one. */
                asm("" : : "r"(n));
                src = &p->id[2];
                /* The bounds test reaches the ring through an escaped copy
                 * of `p` (no code), so CSE doesn't share its address with
                 * the loop pre-headers, which recompute it as the ROM does. */
                if (({ struct link_player *_q = p; asm("" : "+r"(_q)); _q; })->ring.field_8c < 0x80 - n) {
                    for (k = n - 1; k != -1; k--) {
                        p->ring.field_8c++;
                        p->ring.field_84++;
                        p->ring.buf[p->ring.field_8c] = *src++;
                    }
                } else {
                    for (k = n - 1; k != -1; k--) {
                        u8 b = *src++;

                        p->ring.field_8c = p->ring.field_8c == 0x7f ? 0 : p->ring.field_8c + 1;
                        p->ring.field_84++;
                        p->ring.buf[p->ring.field_8c] = b;
                    }
                }
                p->field_34 += n;
                p->field_8 = p->field_6;
                p->field_30 = (p->field_30 + 1) & 0xf;
                self->field_3f4 |= 1 << i;
            }
        }
    copy:
        LINK_COPY_ID(p->id, q);
        if (LINK_NIB(&p->id[0]).hi == nib)
            self->field_3f0 |= 1 << i;
        p->field_2c = 0;
        changed = 1;
    }

    if (changed) {
        changed = 0;
        if (self->field_3f0 == self->field_3f8) {
            s32 n, k;
            u8 *dst;
            u16 hash;
            u8 *id;
            struct link_ring *ring;
            s32 *cnt;
            s32 *rd;
            struct link_ring *rf;

            self->field_3f0 = 0;
            id = self->id;
            ring = &self->ring;
            cnt = &self->ring.field_84;
            dst = &self->id[2];
            rd = &self->ring.field_88;
            {
                u8 *d = self->field_28;
                u8 *s = id;

                for (k = 0; k <= 3; k++) {
                    u32 v = (s[1] << 8) | s[0];
                    u32 lo = v & 0xff;

                    d[0] = lo;
                    d[1] = v >> 8;
                    d += 2;
                    s += 2;
                }
            }
            rf = ring;
            /* A distinct copy of the ring pointer (no code), taken here
             * like the ROM's `adds r4, r7, #0`. */
            asm("" : "+r"(rf));
            n = *cnt;
            if (n > 4)
                n = 4;
            LINK_NIB(&id[1]).hi = n;
            LinkRingPop(ring, rf, dst, n, rd);
            self->field_24 += n;
            {
                /* A signed QImode read-modify-write: the ROM's mask is
                 * -16 and `nib` (already masked) is not masked again. */
                s8 *b = (s8 *)&self->id[1];

                *b = (*b & ~0xf) | nib;
            }
            hash = *(u16 *)&self->id[6];
            LINK_HASH(hash, &self->id[1]);
            {
                /* The ROM sets this 0 in r0 before the hash store. */
                register s32 z asm("r0") = 0;

                *(u16 *)&self->id[6] = hash;
                self->field_3c = z;
            }
            changed = 1;
        }
        if (self->field_3f4 == self->field_3f8) {
            self->field_3f4 = 0;
            LINK_NIB(&self->id[0]).hi = (LINK_NIB(&self->id[0]).hi + 1) & 0xf;
            changed = 1;
        }
        if (changed) {
            self->field_38 = 0;
            self->field_18 = 1;
            {
                u8 *id = self->id;
                u32 n = LINK_NIB(&id[0]).hi;
                u32 v = (id[7] << 8) | id[6];
                s32 w = (n + v) & 0xf;

                LINK_NIB(&id[0]).lo = w;
            }
        }
    }
    {
        u8 *lo, *hi;

        if ((self->field_3c & 3) == 3) {
            u8 *b = (u8 *)self + self->field_38 * 2;

            lo = b + 0x28;
            hi = b + 0x29;
        } else {
            u8 *b = (u8 *)self + self->field_38 * 2;

            lo = b + 0x30;
            hi = b + 0x31;
        }
        self->field_400 = (*hi << 8) | *lo;
    }
    if (++self->field_38 == 4) {
        self->field_38 = 0;
        self->field_3c++;
    }
send:
    {
        u16 v = self->field_400;

        REG_SIOMLT_SEND = v;
    }
    self->field_4 = 0;
}
