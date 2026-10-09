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
 * Both asm statements emit no code (#662 round 2 also tried the tests as
 * `!(REG_SIOCNT & 8)`/`!(REG_SIOCNT & 4)`-style bool expressions and as a
 * volatile SioMultiCnt bitfield struct). Matches under both compilers. As
 * C++, the ready test is written `(v & 1) == 0`: g++'s `!(v & 1)` is a
 * bool negation, which combine turns into an eor/and pair.
 * #662 round 3 (RTL dumps): without MATCH_KEEP(one1), cse1 already puts
 * the eor/and on `one`'s pseudo and cse2 (the rerun after loop) then
 * canonicalizes every 1 to the first one, so one register is left. The
 * ROM has two registers holding 1, both set before the ready test and
 * one used only after it, so its second 1 was a value cse did not know
 * to be constant. Without MATCH_KEEP(arm3), combine rewrites
 * `(x ^ c) & c` as a bic. Tried: `arm3` as s32/u8/bool from
 * `!(REG_SIOCNT & 4)`, `!((REG_SIOCNT >> 2) & 1)`, `((REG_SIOCNT >> 2) ^ 1)
 * & 1` (one or two statements) and `== 0` forms; no flag of the brief's
 * list (-fno-gcse ... -fno-function-cse, -fno-regmove, -fno-force-mem)
 * matches the object without the keeps.
 * #662 round 4: cse hashes a constant by mode, so a QImode or HImode 1
 * would stay apart from the test's SImode one; `u8`/`u16` copies of the
 * flag still come out as one register. A `bit = 1` local set before
 * the test for the eor/and (with or without the test using it too) is
 * canonicalized to the test's 1, and combine still makes the bic. The
 * decomp-permuter on a C port (45 minutes) got from 1030 to 575 with no
 * natural change.
 * #662 round 5: `arm3 = (REG_SIOCNT & 4) == 0` is expr.c's store-flag of
 * a single bit (shift, xor 1, and 1: the ROM's eor/and), but its two 1s
 * are then the test's register, which is why combine makes the bic. The
 * ROM's 1 in r9 lives across IrqClearHandler/IrqSetHandler as a register
 * (`started` and REG_IME are stored from it), so cse saw it as one
 * value; the r1 copy, set before the test and used only after it, is the
 * one it did not know was 1.
 * #662 round 6: the started block is LinkSession::Start (link_sio.cpp,
 * unused) and the SIOCNT setup LinkSetupSio, step for step. Update
 * written with inline copies of both (`Start(((REG_SIOCNT >> 2) & 1) ==
 * 0)`, `Stop(); SetupSio(); return 0;`) compiles to this code except
 * the 1s (48 lines off without the keeps): the argument copy is cse'd
 * like the local, so there is still one 1 and a bic.
 * #662 round 7: the ROM's two 1s are the halfword-AND idiom of the
 * matched ActionCtrl states: Thumb has no HImode AND, so expand loads
 * an HImode 1, gives up and redoes the AND in SImode with a new 1. With
 * the ready test on a `u16` (`u16 v = REG_SIOCNT >> 3; if ((v & 1) ==
 * 0)`), `started = 1`, `arm3 = (REG_SIOCNT >> 2) ^ 1; arm3 &= 1;`,
 * `REG_IME = 1` and no keeps, the code is 6 lines off: r1 and r9 hold
 * the two 1s as in the ROM, but the ROM stores `started` from r9 (the
 * AND's) and does the eor and the and with r1, where cse gives
 * `started` (QImode, wider modes tried narrowest first) the HImode 1
 * and the and the SImode one. About 300 variants (the test's and
 * arm3's types and forms, statement order, an inline copy of Start, a
 * `MATCH_KEEP` between eor and and) don't move them, nor does a
 * private cse that tries the wider modes widest first (66 lines). */
s32 LinkSession::Update()
{
    s32 arm3;
    u16 saved;
    s32 one;

    if (!enabled)
        return 0;
    if (!sioConfigured) {
        REG_RCNT = 0;
        REG_SIOCNT = SIO_MULTI_MODE;
        REG_SIOCNT |= SIO_115200_BPS | SIO_INTR_ENABLE;
        sioConfigured = 1;
    }
    if (!started) {
        s32 one1;
        u32 v = REG_SIOCNT >> 3;

        one1 = 1;
        MATCH_KEEP(one1); /* keep the flag's own 1 (r1) */
        one = 1;
        if ((v & 1) == 0) {
            Stop();
            REG_RCNT = 0;
            REG_SIOCNT = SIO_MULTI_MODE;
            REG_SIOCNT |= SIO_115200_BPS | SIO_INTR_ENABLE;
            return 0;
        }
        started = one;
        arm3 = (REG_SIOCNT >> 2) ^ one1;
        MATCH_KEEP(arm3); /* keep eor/and, not bic */
        arm3 &= one1;
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
        REG_IME = one;
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
                 * it either (see LinkSession::ResetState). */
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
