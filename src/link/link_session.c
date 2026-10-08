#include "core.h"
#include "match.h"
#include "system.h"
#include "link.h"
#include "math_util.h"

/* The link-cable session's per-frame handshake driver and SIO data pump
 * (split from link_handshake.c so ResetLinkSessionState can sit in its own object;
 * see link_session_reset.c). */

/* Link-connection/handshake driver - see docs/rom_map.md's SIO/link-
 * cable section. Called once per frame. Does nothing until the session
 * is `enabled` (BeginLinkSaveTransfer). The first call puts the SIO in
 * multiplayer mode (RCNT 0, SIOCNT 0x2000 | 0x4003; `sioConfigured`).
 * Until `started`, it waits for `REG_SIOCNT` bit 3 (every terminal
 * ready), re-priming SIOCNT and returning 0 while it is clear; once set,
 * it marks the session `started`, installs the Serial IRQ handler
 * (`LinkSerialIntr`) and, on the parent (SIOCNT bit 2 clear), the Timer3
 * handler (`LinkTimer3Intr`) with Timer3 running at `0x00C0BBBC`, and
 * resets `playerId` (-1), `idleFrames`, `framesSinceIrq` and `progressed`.
 * After that, every call: more than 15 frames without a serial IRQ
 * (`framesSinceIrq`) forces `connectCounter` to -15 and `idleFrames` to
 * 0x708. Until `connected`, `connectCounter` steps down while there is
 * no `playerId` and up once there is: above 14 the link is `connected`
 * (`idleFrames`/`peakIdleFrames` cleared), at -15 the session is reset
 * (`LinkStop` + `ResetLinkSessionState`), in between it returns 0.
 * Then `peakIdleFrames` keeps the larger of itself and `idleFrames`,
 * `idleFrames` restarts at 0 if the serial IRQ made `progressed` and
 * counts up otherwise, and past 0x1d idle frames the session is reset.
 * Finally increments `connectCounter` and returns 1.
 *
 * Matched in the second near-miss sweep (37 halfwords before). The ROM
 * materializes two separate 1s after reading SIOCNT: r2 (copied to sb)
 * for `started`/IME and r1 for the arm3 flag. Three things reproduce
 * that:
 * - `MATCH_KEEP(one1)` keeps the flag's 1 from being merged into
 *   `one`.
 * - The ready test uses a literal 1, so `one` is a copy of that
 *   constant's register.
 * - `MATCH_KEEP(arm3)` between the eor and the and stops combine
 *   from folding `(x ^ 1) & 1` into a `bic`, which the ROM doesn't have.
 * Both asm statements emit no code. Matches under both compilers. */
s32 UpdateLinkSession(struct link_session *self)
{
    s32 arm3;
    u16 saved;
    s32 one;

    if (!self->enabled)
        return 0;
    if (!self->sioConfigured) {
        REG_RCNT = 0;
        REG_SIOCNT = 0x2000;
        REG_SIOCNT |= 0x4003;
        self->sioConfigured = 1;
    }
    if (!self->started) {
        s32 one1;
        u32 v = REG_SIOCNT >> 3;

        one1 = 1;
        MATCH_KEEP(one1); /* keep the flag's own 1 (r1) */
        one = 1;
        if (!(v & 1)) {
            LinkStop(self);
            REG_RCNT = 0;
            REG_SIOCNT = 0x2000;
            REG_SIOCNT |= 0x4003;
            return 0;
        }
        self->started = one;
        arm3 = (REG_SIOCNT >> 2) ^ one1;
        MATCH_KEEP(arm3); /* keep eor/and, not bic */
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
        IrqSetHandler(INTR_INDEX_SERIAL, LinkSerialIntr);
        REG_IE |= 0x80;
        if (arm3) {
            IrqSetHandler(INTR_INDEX_TIMER3, LinkTimer3Intr);
            REG_IE |= 0x40;
            REG_TM3CNT = 0x00C0BBBC;
        }
        REG_IME = one;
        self->playerId = -1;
        self->idleFrames = 0;
        self->framesSinceIrq = 0;
        self->progressed = 0;
    }
    if (self->framesSinceIrq > 15) {
        self->connectCounter = -15;
        self->idleFrames = 0x708;
    }
    self->framesSinceIrq++;
    if (!self->connected) {
        if (self->playerId < 0)
            self->connectCounter = self->connectCounter - 1;
        else
            self->connectCounter = self->connectCounter + 1;
        if (self->connectCounter > 14) {
            self->connected = 1;
            self->idleFrames = 0;
            self->peakIdleFrames = 0;
        } else if (self->connectCounter > -15) {
            return 0;
        } else {
            LinkStop(self);
            ResetLinkSessionState(self);
        }
    }
    {
        s32 a = self->peakIdleFrames;
        s32 b = self->idleFrames;

        LIMIT_MIN(a, b);
        self->peakIdleFrames = a;
        b = self->progressed ? 0 : b + 1;
        self->idleFrames = b;
        self->progressed = 0;
        if (b > 0x1d) {
            LinkStop(self);
            ResetLinkSessionState(self);
        }
    }
    self->connectCounter++;
    return 1;
}

/* The Serial IRQ's half of the link (called from `LinkSerialIntr` in
 * src/link/link_sio.c with the SIOMULTI0-3 words). `inSerialIrq` guards
 * against re-entry: a nested call only re-sends `sendWord`. Before the
 * session is `connected` it runs the handshake: it counts the consoles
 * answering with `handshakeWord` (0xF0B), takes `playerCount` (more than
 * one), `playerId` (SIOCNT bits 4-5) and `peerMask` (every other
 * player's bit) from that. Once connected, every exchange moves one
 * 8-byte packet per console, four SIOMULTI halfwords at a time
 * (`sendWordIndex`, `sendRound`). The outgoing packet is `id`: byte 0
 * holds a check nibble and an ack counter, byte 1 a sequence number and
 * the payload length (0-4 bytes popped from the outgoing `ring`, counted
 * in `totalSent`), bytes 2-5 the payload and bytes 6-7 a CRC-16
 * (`gCrc16Table`, as in MakeLinkHandshakeId) chained from the previous
 * packet. Each player record keeps the peer's last packet (`id`, `hash`,
 * `prevHash`), its incoming words (`rx`, `rxCount`), the expected
 * sequence (`rxSeq`) and its incoming `ring` (`totalReceived`). A new
 * packet is built once every peer has acknowledged ours (`ackedMask` ==
 * `peerMask`), our ack counter is bumped once every peer's new data has
 * arrived (`receivedMask`), and every fourth round re-sends the previous
 * packet (`prevPacket`). `progressed` tells UpdateLinkSession the link
 * moved.
 *
 * Once a NAKED transcription (1488 bytes, this project's biggest); it
 * now matches as plain C under old_agbcc (link_handshake.o is on the
 * Makefile's OLD_AGBCC_OBJS). History: docs/matching/archive/big-naked-retry-3.md,
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
 * `&ring.readPos`. */
static inline void LinkRingPop(struct link_ring *r, struct link_ring *rf, u8 *dst, s32 n, s32 *rd)
{
    s32 k;

    if (*rd < 0x80 - n) {
        for (k = n - 1; k != -1; k--) {
            *dst++ = rf->buf[rf->readPos];
            rf->readPos++;
            rf->count--;
        }
    } else {
        for (k = n - 1; k != -1; k--) {
            s32 old = r->readPos;
            s32 nw = 0;

            if (old != 0x7f)
                nw = old + 1;
            r->readPos = nw;
            r->count--;
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

    self->framesSinceIrq = 0;
    if (self->inSerialIrq) {
        u16 v = self->sendWord;

        REG_SIOMLT_SEND = v;
        return;
    }
    one = 1;
    self->inSerialIrq = one;
    siocnt = REG_SIOCNT;
    changed = 0;
    nib = LINK_NIB(&self->id[1]).lo + 1;
    nib &= 0xf;
    if ((siocnt >> 6) & one)
        goto send;
    if (!self->connected) {
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

            /* Set before the loop, in the ROM's order: `&self->handshakeWord`,
             * then the two compare constants, then the load pointer.
             * `kfree` uses the constant-init form (no code beyond the
             * `ldr`) so loop.c doesn't hoist it after `p`'s init; `kid`
             * stays in place as a plain assignment. */
            f20 = &self->handshakeWord;
            kid = 0xF0B;
            MATCH_CONST(kfree, 0xffff);
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
            self->playerCount = nId;
            {
                s32 me = (REG_SIOCNT & SIO_ID) >> 4;

                self->playerId = me;
            }
            self->peerMask = ~(-1 << nId);
            self->peerMask &= ~(1 << self->playerId);
        }
        f20->lo = nId;
        {
            u16 *dst = &self->sendWord;

            *dst = *(u16 *)&self->handshakeWord;
        }
        goto send;
    }

    for (i = 0; i < self->playerCount; i++) {
        struct link_player *p;
        u8 *q;
        s32 ok;

        if (i == self->playerId)
            continue;
        p = &self->players[i];
        p->rx[p->rxCount] = data[i];
        p->rxCount = (p->rxCount + 1) & 0xf;
        if (p->rxCount <= 3)
            continue;
        q = (u8 *)&p->rx[p->rxCount - 4];
        ok = 0;
        if (LINK_NIB(&q[1]).lo == LINK_NIB(&q[0]).hi ||
            LINK_NIB(&q[1]).lo == ((LINK_NIB(&q[0]).hi - 1) & 0xf)) {
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
                u16 hash = p->prevHash;

                LINK_HASH(hash, &q[1]);
                if (want == hash)
                    goto copy;
                continue;
            } else {
                u16 want;
                u16 hash;
                s32 n, k;
                u8 *src;

                if (LINK_NIB(&q[1]).lo != p->rxSeq)
                    continue;
                want = (q[7] << 8) | q[6];
                hash = p->hash;
                LINK_HASH(hash, &q[1]);
                if (want != hash)
                    continue;
                n = p->id[1] >> 4;
                /* Extra reference (no code): raises `n`'s priority so it
                 * gets its own register (r7) instead of reusing the
                 * id-byte one. */
                MATCH_USE(n);
                src = &p->id[2];
                /* The bounds test reaches the ring through an escaped copy
                 * of `p` (no code), so CSE doesn't share its address with
                 * the loop pre-headers, which recompute it as the ROM does. */
                if (MATCH_KEEP_EXPR(struct link_player *, p)->ring.writePos < 0x80 - n) {
                    for (k = n - 1; k != -1; k--) {
                        p->ring.writePos++;
                        p->ring.count++;
                        p->ring.buf[p->ring.writePos] = *src++;
                    }
                } else {
                    for (k = n - 1; k != -1; k--) {
                        u8 b = *src++;

                        p->ring.writePos = p->ring.writePos == 0x7f ? 0 : p->ring.writePos + 1;
                        p->ring.count++;
                        p->ring.buf[p->ring.writePos] = b;
                    }
                }
                p->totalReceived += n;
                p->prevHash = p->hash;
                p->rxSeq = (p->rxSeq + 1) & 0xf;
                self->receivedMask |= 1 << i;
            }
        }
    copy:
        LINK_COPY_ID(p->id, q);
        if (LINK_NIB(&p->id[0]).hi == nib)
            self->ackedMask |= 1 << i;
        p->rxCount = 0;
        changed = 1;
    }

    if (changed) {
        changed = 0;
        if (self->ackedMask == self->peerMask) {
            s32 n, k;
            u8 *dst;
            u16 hash;
            u8 *id;
            struct link_ring *ring;
            s32 *cnt;
            s32 *rd;
            struct link_ring *rf;

            self->ackedMask = 0;
            id = self->id;
            ring = &self->ring;
            cnt = &self->ring.count;
            dst = &self->id[2];
            rd = &self->ring.readPos;
            {
                u8 *d = self->prevPacket;
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
            MATCH_KEEP(rf);
            n = *cnt;
            LIMIT_MAX(n, 4);
            LINK_NIB(&id[1]).hi = n;
            LinkRingPop(ring, rf, dst, n, rd);
            self->totalSent += n;
            {
                /* A signed QImode read-modify-write: the ROM's mask is
                 * -16 and `nib` (already masked) is not masked again. */
                s8 *b = (s8 *)&self->id[1];

                *b = (*b & ~0xf) | nib;
            }
            hash = *(u16 *)&self->id[6];
            LINK_HASH(hash, &self->id[1]);
            {
                /* The ROM sets this 0 before the hash store. */
                s32 z = 0;

                *(u16 *)&self->id[6] = hash;
                self->sendRound = z;
            }
            changed = 1;
        }
        if (self->receivedMask == self->peerMask) {
            self->receivedMask = 0;
            LINK_NIB(&self->id[0]).hi = (LINK_NIB(&self->id[0]).hi + 1) & 0xf;
            changed = 1;
        }
        if (changed) {
            self->sendWordIndex = 0;
            self->progressed = 1;
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

        if ((self->sendRound & 3) == 3) {
            u8 *b = (u8 *)self + self->sendWordIndex * 2;

            lo = b + 0x28;
            hi = b + 0x29;
        } else {
            u8 *b = (u8 *)self + self->sendWordIndex * 2;

            lo = b + 0x30;
            hi = b + 0x31;
        }
        self->sendWord = (*hi << 8) | *lo;
    }
    if (++self->sendWordIndex == 4) {
        self->sendWordIndex = 0;
        self->sendRound++;
    }
send:
    {
        u16 v = self->sendWord;

        REG_SIOMLT_SEND = v;
    }
    self->inSerialIrq = 0;
}
