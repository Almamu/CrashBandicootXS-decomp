#ifndef GUARD_LINK_SESSION_H
#define GUARD_LINK_SESSION_H

#include "core.h"

/* The link-cable session's packed bit views (src/link/*.cpp); the
 * session, player and ring layouts are LinkSession's, LinkPlayer's and
 * LinkRing's (include/link_session.hpp). See
 * docs/matching/archive/issue-4-6-8-naked-retry.md. */
struct nibble_pair {
    u8 lo:4;
    u8 hi:4;
} __attribute__((packed));

struct link_id_word {
    u16 lo:4;
    u16 hi:12;
} __attribute__((packed));

/* The link session object (`*gLinkSession`), as the C side sees it: a
 * tag. Its layout and code are C++, class LinkSession
 * (include/link_session.hpp). */
struct link_session;

#endif /* GUARD_LINK_SESSION_H */
