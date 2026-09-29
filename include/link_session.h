#ifndef GUARD_LINK_SESSION_H
#define GUARD_LINK_SESSION_H

#include "core.h"

/* The link-cable session layouts (src/system/link_cable*.c), as the
 * NON_MATCHING drafts of sub_8001CB8/sub_8001DB4/sub_8001F50 establish
 * (docs/matching/issue-4-6-8-naked-retry.md). */
struct nibble_pair {
    u8 lo:4;
    u8 hi:4;
} __attribute__((packed));

/* A 0x90-byte receive ring (the session header has one at +0x40, each
 * player record one at +0x38). */
struct link_ring {
    u8 unused_00[4];
    u8 buf[0x80];       /* 0x04 - written/read at field_8c/field_88 */
    s32 field_84;
    s32 field_88;
    s32 field_8c;       /* reset to 0x7f */
};

/* One per-player 0xc8-byte record of the link session. */
struct link_player {
    u8 id[6];           /* 0x00 - copy of the session handshake id */
    u16 field_6;        /* 0x06 - overwrites the id's hash with 0x1234 */
    u16 field_8;        /* 0x08 */
    u16 rx[16];         /* 0x0a - last 16 received words, at field_2c */
    u8 unused_2a[2];
    s32 field_2c;       /* 0x2c */
    s32 field_30;       /* 0x30 */
    s32 field_34;       /* 0x34 */
    struct link_ring ring; /* 0x38 */
};

struct link_id_word {
    u16 lo:4;
    u16 hi:12;
} __attribute__((packed));

/* The link session object (`*gUnknown_03000804`). */
struct link_session {
    u8 unused_00[4];
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 unused_09[3];
    s32 field_c;
    s32 field_10;
    s32 field_14;
    u8 field_18;
    u8 unused_19[3];
    s32 field_1c;
    struct link_id_word field_20;
    u8 unused_22[2];
    s32 field_24;
    u8 field_28[8];
    u8 id[8];           /* 0x30 - sub_8001CB8's handshake id */
    s32 field_38;
    s32 field_3c;
    struct link_ring ring;         /* 0x40 */
    struct link_player players[4]; /* 0xd0 */
    s32 field_3f0;
    s32 field_3f4;
    s32 field_3f8;
    s32 field_3fc;
    u16 field_400;
    u8 unused_402[2];
    s32 field_404;
};
COMPILE_TIME_ASSERT(sizeof(struct link_player) == 0xc8);
COMPILE_TIME_ASSERT(sizeof(struct link_session) == 0x408);

#endif /* GUARD_LINK_SESSION_H */
