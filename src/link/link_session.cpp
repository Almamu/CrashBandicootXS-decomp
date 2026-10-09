#include "link_session.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "system.h"
#include "link.h"
#include "math_util.h"
}

/* The link-cable session's per-frame handshake driver and SIO data pump
 * (split from link_handshake.cpp so LinkSession::ResetState can sit in its own object;
 * see link_session_reset.cpp). */

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
 * (`LinkSession::Stop` + `LinkSession::ResetState`), in between it returns 0.
 * Then `peakIdleFrames` keeps the larger of itself and `idleFrames`,
 * `idleFrames` restarts at 0 if the serial IRQ made `progressed` and
 * counts up otherwise, and past 0x1d idle frames the session is reset.
 * Finally increments `connectCounter` and returns 1.
 *
 * The ready flag and `arm3` are bytes, as Start's `u8 arm3` is. The
 * ROM's two 1s after the SIOCNT read (r1 for arm3's eor and and, r9 for
 * `started` and REG_IME) come from the byte AND: Thumb has no QImode AND,
 * so expand loads a QImode 1, gives up and redoes the AND in SImode with
 * a new 1 (the one the test uses and keeps in r9). cse gives arm3's
 * byte-wide eor and and the QImode 1 through paradoxical subregs, which
 * it can't fold to a constant, and `started` and REG_IME the SImode one.
 * Until #662 round 8 the function had a `one1` local held by a
 * MATCH_KEEP and a second MATCH_KEEP against a bic; with `u16` flags
 * (round 7) it was 6 lines off, the HImode 1 then going to `started`.
 * Found by tools/natural_enum.py over the flags' and locals' types and
 * forms: every match has a byte `ready` and a byte `arm3`. */
s32 LinkSession::Update()
{
    u8 arm3;
    u16 saved;

    if (!enabled)
        return 0;
    if (!sioConfigured) {
        REG_RCNT = 0;
        REG_SIOCNT = SIO_MULTI_MODE;
        REG_SIOCNT |= SIO_115200_BPS | SIO_INTR_ENABLE;
        sioConfigured = 1;
    }
    if (!started) {
        u8 ready = (REG_SIOCNT >> 3) & 1;

        if (!ready) {
            Stop();
            REG_RCNT = 0;
            REG_SIOCNT = SIO_MULTI_MODE;
            REG_SIOCNT |= SIO_115200_BPS | SIO_INTR_ENABLE;
            return 0;
        }
        started = 1;
        arm3 = ((REG_SIOCNT >> 2) ^ 1) & 1;
        REG_IME = 0;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_SERIAL;
        REG_IME = saved;
        saved = REG_IME;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_TIMER3;
        REG_IME = saved;
        IrqClearHandler(INTR_INDEX_TIMER3);
        IrqSetHandler(INTR_INDEX_SERIAL, LinkSerialIntr);
        REG_IE |= INTR_FLAG_SERIAL;
        if (arm3) {
            IrqSetHandler(INTR_INDEX_TIMER3, LinkTimer3Intr);
            REG_IE |= INTR_FLAG_TIMER3;
            REG_TM3CNT = 0x00C0BBBC;
        }
        REG_IME = 1;
        playerId = -1;
        idleFrames = 0;
        framesSinceIrq = 0;
        progressed = 0;
    }
    if (framesSinceIrq > 15) {
        connectCounter = -15;
        idleFrames = 0x708;
    }
    framesSinceIrq++;
    if (!connected) {
        if (playerId < 0)
            connectCounter = connectCounter - 1;
        else
            connectCounter = connectCounter + 1;
        if (connectCounter > 14) {
            connected = 1;
            idleFrames = 0;
            peakIdleFrames = 0;
        } else if (connectCounter > -15) {
            return 0;
        } else {
            Stop();
            ResetState();
        }
    }
    {
        s32 a = peakIdleFrames;
        s32 b = idleFrames;

        LIMIT_MIN(a, b);
        peakIdleFrames = a;
        b = progressed ? 0 : b + 1;
        idleFrames = b;
        progressed = 0;
        if (b > 0x1d) {
            Stop();
            ResetState();
        }
    }
    connectCounter++;
    return 1;
}

/* The Serial IRQ's half of the link (called from `LinkSerialIntr` in
 * src/link/link_sio.cpp with the SIOMULTI0-3 words). `inSerialIrq` guards
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
 * packet (`prevPacket`). `progressed` tells LinkSession::Update the link
 * moved.
 *
 * Once a NAKED transcription (1488 bytes, this project's biggest); it
 * now matches as plain C under old_agbcc (link_handshake.o is on the
 * Makefile's OLD_AGBCC_OBJS). History: docs/matching/archive/big-naked-retry-3.md,
 * early-rom-naked-retry-2.md, last-four-naked-retry.md,
 * last-six-naked-retry.md and last-seven-naked-retry.md (the first
 * receive loop, closed last; it is a plain loop over the registers as
 * volatile since #662 round 2).
 * `data` is SIOMULTI0-3 (link_sio.cpp passes 0x04000120). */
/* A received SIOMULTI word, read back from a stack copy. */
struct link_rx_word {
    u32 lo:4;
    u32 hi:12;
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
static inline void LinkRingPop(LinkRing *r, LinkRing *rf, u8 *dst, s32 n, s32 *rd)
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

void LinkSession::HandleSerial(u16 *data)
{
    struct link_rx_word w[4];
    s32 i;
    s32 changed;
    s32 nib;
    u16 siocnt;
    u32 one;

    framesSinceIrq = 0;
    if (inSerialIrq) {
        u16 v = sendWord;

        REG_SIOMLT_SEND = v;
        return;
    }
    one = 1;
    inSerialIrq = one;
    siocnt = REG_SIOCNT;
    changed = 0;
    nib = LINK_NIB(&id[1]).lo + 1;
    nib &= 0xf;
    if ((siocnt >> 6) & one)
        goto send;
    if (!connected) {
        s32 nId, nFree, same;
        struct link_id_word *f20;

        if (!((siocnt >> 3) & one))
            goto send;
        nId = 0;
        nFree = 0;
        {
            /* SIOMULTI0-3 are hardware registers: the ROM reads each one
             * twice, as the word copy into `w` and for the 0xffff test
             * (#662 round 2: the C read them through a pointer biv, with
             * the test's constant in a MATCH_CONST). */
            vu16 *multi = data;

            f20 = &handshakeWord;
            for (i = 0; i <= 3; i++) {
                w[i] = *(struct link_rx_word *)&multi[i];
                if (w[i].hi == 0xF0B)
                    nId++;
                if (multi[i] == 0xffff)
                    nFree++;
            }
        }
        same = 1;
        for (i = 0; i < nId; i++) {
            if ((s32)w[i].lo != nId) /* g++ keeps the bitfield unsigned */
                same = 0;
        }
        if (nFree + nId == 4 && same && nId > 1) {
            playerCount = nId;
            {
                s32 me = (REG_SIOCNT & SIO_ID) >> 4;

                playerId = me;
            }
            peerMask = ~(-1 << nId);
            peerMask &= ~(1 << playerId);
        }
        f20->lo = nId;
        {
            u16 *dst = &sendWord;

            *dst = *(u16 *)&handshakeWord;
        }
        goto send;
    }

    for (i = 0; i < playerCount; i++) {
        LinkPlayer *p;
        u8 *q;
        s32 ok;

        if (i == playerId)
            continue;
        p = &players[i];
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
                s32 n;

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
                 * id-byte one. #662 round 3: unreferenced, global-alloc
                 * gives `n` the register of the `p->id[1]` byte it is
                 * shifted from (r8, which dies there) and copies it to
                 * low registers at each use. Tried: `n` as u8/u32, from
                 * the nibble bitfield, declared first, block-local
                 * around the push, and totalReceived before the push.
                 * #662 round 7: no other global.c priority formula gives
                 * it either (see LinkSession::ResetState). Round 8
                 * (tools/natural_enum.py, 2160 variants: `n` and `want`
                 * in all six integer types, `n` as `p->id[1] >> 4`, the
                 * nibble bitfield or `(u8)` cast, the push, the
                 * totalReceived, prevHash and rxSeq updates in either
                 * order): 42 lines off; `(p->id[1] >> 4) & 0xf` gives
                 * `n` r7 (28 off) but with an `ands` the ROM doesn't
                 * have. */
                MATCH_USE(n);
                p->ring.Push(&p->id[2], n);
                p->totalReceived += n;
                p->prevHash = p->hash;
                p->rxSeq = (p->rxSeq + 1) & 0xf;
                receivedMask |= 1 << i;
            }
        }
    copy:
        LINK_COPY_ID(p->id, q);
        if (LINK_NIB(&p->id[0]).hi == nib)
            ackedMask |= 1 << i;
        p->rxCount = 0;
        changed = 1;
    }

    if (changed) {
        changed = 0;
        if (ackedMask == peerMask) {
            s32 n, k;
            u8 *dst;
            u16 hash;
            u8 *id;
            LinkRing *ring;
            s32 *cnt;
            s32 *rd;
            LinkRing *rf;

            ackedMask = 0;
            id = this->id;
            ring = &this->ring;
            cnt = &this->ring.count;
            dst = &this->id[2];
            rd = &this->ring.readPos;
            {
                u8 *d = prevPacket;
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
             * like the ROM's `adds r4, r7, #0`. #662 round 3: a plain
             * copy is propagated away by cse1, and the fast loop's
             * addresses are then built from `ring` itself; the ROM's
             * copy is a value cse did not see as equal to `ring` (its
             * fast loop recomputes `&readPos` beside `rd`). Tried: the
             * pop with `ring` passed twice, one-pointer inline pops
             * (bounds via `r->readPos` or `rd`) and a LinkRing::Pop
             * member on `ring`/`this->ring`. #662 round 4: `rf =
             * &this->ring` here does stay a second register: gcse's
             * PRE finds the address redundant, computes it into a
             * reaching register at the end of an earlier block and
             * makes `rf` a copy of that. But that register then lives
             * across the id copy loop (r8) and the allocation after it
             * moves. #662 round 5: an inline call whose ring argument is
             * the expression `&this->ring` gives the ROM's copy without
             * the escape. integrate.c copies an argument that isn't a
             * register into a fresh pseudo; cse1 can't tell that it
             * equals `ring` (the id copy loop ended its extended basic
             * block), and gcse's PRE then turns it into a copy of the
             * reaching register, so the fast loop works from the copy
             * and the wrap loop from the PRE register, as in the ROM.
             * But the copy is emitted at the call. `this->ring.Pop(dst,
             * n)` (LinkRing's Pop) is 56 lines off, and this pop with one
             * ring pointer, called with `&this->ring` and `rd`, is 52:
             * the copy comes after the clamp and the nibble store, where
             * the ROM has it before `n = *cnt`. Moving the count read, the
             * clamp and the nibble store into the inline as well puts the
             * copy in place (32 lines), but then `&this->ring` is a PRE
             * insertion after the loop setup instead of the second of
             * the address locals, and that helper (session id, count and
             * destination pointers as parameters) is no natural code. */
            MATCH_KEEP(rf);
            n = *cnt;
            LIMIT_MAX(n, 4);
            LINK_NIB(&id[1]).hi = n;
            LinkRingPop(ring, rf, dst, n, rd);
            totalSent += n;
            {
                /* A signed QImode read-modify-write: the ROM's mask is
                 * -16 and `nib` (already masked) is not masked again. */
                s8 *b = (s8 *)&this->id[1];

                *b = (*b & ~0xf) | nib;
            }
            hash = this->hash;
            LINK_HASH(hash, &this->id[1]);
            {
                /* The ROM sets this 0 before the hash store. */
                s32 z = 0;

                this->hash = hash;
                sendRound = z;
            }
            changed = 1;
        }
        if (receivedMask == peerMask) {
            receivedMask = 0;
            LINK_NIB(&id[0]).hi = (LINK_NIB(&id[0]).hi + 1) & 0xf;
            changed = 1;
        }
        if (changed) {
            sendWordIndex = 0;
            progressed = 1;
            {
                u8 *id = this->id;
                u32 n = LINK_NIB(&id[0]).hi;
                u32 v = (id[7] << 8) | id[6];
                s32 w = (n + v) & 0xf;

                LINK_NIB(&id[0]).lo = w;
            }
        }
    }
    {
        u8 *lo, *hi;

        if ((sendRound & 3) == 3) {
            u8 *b = (u8 *)this + sendWordIndex * 2;

            lo = b + 0x28;
            hi = b + 0x29;
        } else {
            u8 *b = (u8 *)this + sendWordIndex * 2;

            lo = b + 0x30;
            hi = b + 0x31;
        }
        sendWord = (*hi << 8) | *lo;
    }
    if (++sendWordIndex == 4) {
        sendWordIndex = 0;
        sendRound++;
    }
send:
    {
        u16 v = sendWord;

        REG_SIOMLT_SEND = v;
    }
    inSerialIrq = 0;
}
