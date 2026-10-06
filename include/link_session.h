#ifndef GUARD_LINK_SESSION_H
#define GUARD_LINK_SESSION_H

#include "core.h"

/* The link-cable session layouts (src/link/*.c), as the
 * NON_MATCHING drafts of MakeLinkHandshakeId/ResetLinkSessionState/UpdateLinkSession establish
 * (docs/matching/archive/issue-4-6-8-naked-retry.md). */
struct nibble_pair {
    u8 lo:4;
    u8 hi:4;
} __attribute__((packed));

/* A 0x90-byte byte ring: the session's outgoing ring at +0x40 and each
 * player record's incoming ring at +0x38 (SendSaveTransferChunk and
 * ReceiveSaveTransferChunk, src/save/save_transfer.c, stream the save
 * data through them). */
struct link_ring {
    u8 unused_00[4];
    u8 buf[0x80]; /* 0x04 - written at writePos, read at readPos */
    s32 count;    /* 0x84 - bytes pending */
    s32 readPos;  /* 0x88 */
    s32 writePos; /* 0x8c - reset to 0x7f */
};

/* One per-player 0xc8-byte record of the link session. */
struct link_player {
    /* 0x00 - with `hash`, the peer's last 8-byte packet (first a copy of
     * the session's handshake id) */
    u8 id[6];
    u16 hash;     /* 0x06 - the packet's CRC-16; 0x1234 after ResetLinkSessionState */
    u16 prevHash; /* 0x08 - the hash before the latest packet */
    u16 rx[16];   /* 0x0a - last 16 received words, written at rxCount */
    u8 unused_2a[2];
    s32 rxCount;           /* 0x2c - write index into rx (& 0xf) */
    s32 rxSeq;             /* 0x30 - the sequence nibble expected next */
    s32 totalReceived;     /* 0x34 - payload bytes pushed into `ring` */
    struct link_ring ring; /* 0x38 */
};

struct link_id_word {
    u16 lo:4;
    u16 hi:12;
} __attribute__((packed));

/* The link session object (`*gLinkSession`). UpdateLinkSession and
 * HandleLinkSerial (src/link/link_session.c) describe the protocol. */
struct link_session {
    u8 unused_00[4];
    u8 inSerialIrq;   /* 0x04 - HandleLinkSerial's re-entrancy guard */
    u8 enabled;       /* 0x05 - Begin/EndLinkSaveTransfer; UpdateLinkSession idles while 0 */
    u8 sioConfigured; /* 0x06 - SIO put in multiplayer mode */
    u8 connected;     /* 0x07 - connectCounter got past 14 */
    u8 started;       /* 0x08 - every terminal was ready, IRQ handlers installed */
    u8 unused_09[3];
    s32 connectCounter; /* 0x0c - steps toward +15 (connected) or -15 (reset) */
    s32 peakIdleFrames; /* 0x10 - largest idleFrames seen, write-only */
    s32 idleFrames;     /* 0x14 - frames without `progressed`; > 0x1d resets */
    u8 progressed;      /* 0x18 - set by HandleLinkSerial when a packet moved */
    u8 unused_19[3];
    s32 playerCount;                   /* 0x1c - consoles that answered the handshake */
    struct link_id_word handshakeWord; /* 0x20 - 0xF0B, low nibble = answers seen */
    u8 unused_22[2];
    s32 totalSent;                 /* 0x24 - payload bytes popped from `ring` */
    u8 prevPacket[8];              /* 0x28 - the previous `id`, re-sent every fourth round */
    u8 id[8];                      /* 0x30 - the outgoing packet; first MakeLinkHandshakeId's id */
    s32 sendWordIndex;             /* 0x38 - which of the packet's 4 halfwords goes out next */
    s32 sendRound;                 /* 0x3c - full packets sent since the last new one */
    struct link_ring ring;         /* 0x40 - outgoing bytes */
    struct link_player players[4]; /* 0xd0 */
    s32 ackedMask;                 /* 0x3f0 - peers that acknowledged our packet */
    s32 receivedMask;              /* 0x3f4 - peers whose new packet we accepted */
    s32 peerMask;                  /* 0x3f8 - every player's bit but ours */
    s32 playerId;                  /* 0x3fc - SIOCNT's multiplayer id, -1 = none yet */
    u16 sendWord;                  /* 0x400 - the halfword written to SIOMLT_SEND */
    u8 unused_402[2];
    s32 framesSinceIrq; /* 0x404 - UpdateLinkSession calls since the last serial IRQ */
};
COMPILE_TIME_ASSERT(link_session_h, sizeof(struct link_player) == 0xc8);
COMPILE_TIME_ASSERT(link_session_h, sizeof(struct link_session) == 0x408);

#endif /* GUARD_LINK_SESSION_H */
