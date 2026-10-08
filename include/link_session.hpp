#ifndef GUARD_LINK_SESSION_HPP
#define GUARD_LINK_SESSION_HPP

/* The link-cable session as C++ (#751, docs/cplusplus.md, "The link
 * session, the save data and the save transfer"): class LinkSession, the
 * object the save menu's constructor
 * makes with `new LinkSession` (src/save/save_menu_input.cpp) into
 * gLinkSession (link.h declares it as a `LinkSession *` to C++) and its
 * destructor deletes. Its code is src/link/: link_sio.cpp (Start, Reset,
 * the constructor and destructor), link_handshake.cpp (Stop),
 * link_session.cpp (Update, HandleSerial) and link_session_reset.cpp
 * (ResetState); cxx_symbols.txt maps the methods onto their C names. It
 * has no vtable.
 *
 * The C++ traits the C had: the constructor returns `this`
 * (InitLinkSession) and its caller allocated with IwramAlloc first (the
 * class's own operator new and delete, the IWRAM heap's, as
 * AudioContext's); the destructor takes `__in_chrg`, frees on bit 0 and
 * was called with 3 (`delete gLinkSession`). The constructor's ring
 * resets are LinkRing's constructor, run for the session's `ring` and in
 * g++'s array loop for the four players' (`i = 3 ... while (i != -1)`),
 * and the destructor's dead loop from `&players[4]` down to `players` is
 * the array's destructor loop: LinkRing has an empty destructor, which
 * makes LinkPlayer's (implicit) one non-trivial.
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * the inline methods. */
#pragma interface

extern "C" {
#include "core.h"
#include "link.h"
#include "util.h"
}

/* A 0x90-byte byte ring: the session's outgoing ring at +0x40 and each
 * player record's incoming ring at +0x38 (SaveTransfer::SendChunk and
 * ReceiveChunk, src/save/save_transfer.cpp, stream the save data through
 * them). */
class LinkRing
{
public:
    u8 unused_00[4];
    u8 buf[0x80]; // 0x04 - written at writePos, read at readPos
    s32 count;    // 0x84 - bytes pending
    s32 readPos;  // 0x88
    s32 writePos; // 0x8c - reset to 0x7f

    // Empties the ring (LinkSession::ResetState).
    void Reset()
    {
        count = 0;
        readPos = 0;
        writePos = 0x7f;
    }
    /* Appends `n` bytes (SaveTransfer::SendChunk, LinkSession::HandleSerial). */
    void Push(u8 *src, s32 n)
    {
        s32 i;

        if (writePos < 0x80 - n) {
            for (i = n - 1; i != -1; i--) {
                writePos++;
                count++;
                buf[writePos] = *src++;
            }
        } else {
            for (i = n - 1; i != -1; i--) {
                u8 b = *src++;

                writePos = writePos == 0x7f ? 0 : writePos + 1;
                count++;
                buf[writePos] = b;
            }
        }
    }
    /* Takes `n` bytes (SaveTransfer::ReceiveChunk, LinkSession::HandleSerial). */
    void Pop(u8 *dst, s32 n)
    {
        s32 i;

        if (readPos < 0x80 - n) {
            for (i = n - 1; i != -1; i--) {
                *dst++ = buf[readPos];
                readPos++;
                count--;
            }
        } else {
            for (i = n - 1; i != -1; i--) {
                s32 old = readPos;
                s32 next = 0;

                if (old != 0x7f)
                    next = old + 1;
                readPos = next;
                count--;
                *dst++ = buf[old];
            }
        }
    }
    LinkRing()
    {
        Reset();
    }
    ~LinkRing()
    {
    }
};

/* One per-player 0xc8-byte record of the link session. */
class LinkPlayer
{
public:
    /* 0x00 - with `hash`, the peer's last 8-byte packet (first a copy of
     * the session's handshake id) */
    u8 id[6];
    u16 hash;          // 0x06 - the packet's CRC-16; 0x1234 after ResetState
    u16 prevHash;      // 0x08 - the hash before the latest packet
    u16 rx[16];        // 0x0a - last 16 received words, written at rxCount
    s32 rxCount;       // 0x2c - write index into rx (& 0xf)
    s32 rxSeq;         // 0x30 - the sequence nibble expected next
    s32 totalReceived; // 0x34 - payload bytes pushed into `ring`
    LinkRing ring;     // 0x38
};

/* The link session object (`*gLinkSession`). Update and HandleSerial
 * (src/link/link_session.cpp) describe the protocol. */
class LinkSession
{
public:
    u8 unused_00[4];
    u8 inSerialIrq;     // 0x04 - HandleSerial's re-entrancy guard
    u8 enabled;         // 0x05 - SaveMenu's Begin/EndLinkTransfer; Update idles while 0
    u8 sioConfigured;   // 0x06 - SIO put in multiplayer mode
    u8 connected;       // 0x07 - connectCounter got past 14
    u8 started;         // 0x08 - every terminal was ready, IRQ handlers installed
    s32 connectCounter; // 0x0c - steps toward +15 (connected) or -15 (reset)
    s32 peakIdleFrames; // 0x10 - largest idleFrames seen, write-only
    s32 idleFrames;     // 0x14 - frames without `progressed`; > 0x1d resets
    u8 progressed;      // 0x18 - set by HandleSerial when a packet moved
    s32 playerCount;    // 0x1c - consoles that answered the handshake
    struct link_id_word handshakeWord; // 0x20 - 0xF0B, low nibble = answers seen
    s32 totalSent;                     // 0x24 - payload bytes popped from `ring`
    u8 prevPacket[8];                  // 0x28 - the previous `id`, re-sent every fourth round
    u8 id[8];                          // 0x30 - the outgoing packet; first MakeLinkHandshakeId's id
    s32 sendWordIndex;                 // 0x38 - which of the packet's 4 halfwords goes out next
    s32 sendRound;                     // 0x3c - full packets sent since the last new one
    LinkRing ring;                     // 0x40 - outgoing bytes
    LinkPlayer players[4];             // 0xd0
    s32 ackedMask;                     // 0x3f0 - peers that acknowledged our packet
    s32 receivedMask;                  // 0x3f4 - peers whose new packet we accepted
    s32 peerMask;                      // 0x3f8 - every player's bit but ours
    /* 0x3fc - SIOCNT's multiplayer id, -1 = none yet. volatile: HandleSerial
     * (the serial IRQ) sets it, and SaveTransfer::Poll reads it twice. */
    volatile s32 playerId;
    u16 sendWord;       // 0x400 - the halfword written to SIOMLT_SEND
    s32 framesSinceIrq; // 0x404 - Update calls since the last serial IRQ

    LinkSession();  // InitLinkSession
    ~LinkSession(); // DestroyLinkSession

    /* src/link/link_sio.cpp */
    s32 Start(u32 flags); // LinkStart (UNUSED): `this` is unused
    s32 Reset();          // ResetLinkSession

    /* src/link/link_handshake.cpp */
    s32 Stop(); // LinkStop: `this` is unused

    /* src/link/link_session.cpp */
    s32 Update();                 // UpdateLinkSession
    void HandleSerial(u16 *data); // HandleLinkSerial

    /* src/link/link_session_reset.cpp */
    s32 ResetState(); // ResetLinkSessionState

    static void *operator new(size_t size)
    {
        return IwramAlloc(size);
    }
    static void operator delete(void *p)
    {
        IwramFree((u8 *)p);
    }
};

COMPILE_TIME_ASSERT(link_session_hpp, sizeof(LinkRing) == 0x90);
COMPILE_TIME_ASSERT(link_session_hpp, sizeof(LinkPlayer) == 0xc8);
COMPILE_TIME_ASSERT(link_session_hpp, sizeof(LinkSession) == 0x408);

#endif /* !GUARD_LINK_SESSION_HPP */
