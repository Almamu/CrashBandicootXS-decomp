#ifndef GUARD_LINK_H
#define GUARD_LINK_H

/* The link-cable subsystem (src/link/): the SIO session object, its IRQ
 * handlers and the handshake. The session layouts are C++, in
 * link_session.hpp; C (src/data/link_crc_16af10.cpp) needs only the tables
 * and the C-linkage functions below.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"

/* The link session the link IRQ handlers and the save transfer work on:
 * made by SaveMenu's constructor (`new LinkSession`), deleted by its
 * destructor (src/save/save_menu_input.cpp). Defined in
 * src/iwram/iwram_data.cpp. The C++ files see it as its class,
 * LinkSession (link_session.hpp); C sees a tag. */
#ifdef __cplusplus
extern class LinkSession *gLinkSession;
#else
extern struct link_session *gLinkSession;
#endif

/* Set by LinkSession::ResetState, cleared by LinkExchangeSaveData
 * (src/save/save_menu_draw.cpp). Defined in src/iwram/iwram_data.cpp. */
extern u8 gLinkSessionReset;

/* The CRC-16 table MakeLinkHandshakeId and LinkSession::HandleSerial hash with
 * (src/data/link_crc_16af10.cpp). */
extern const u16 gCrc16Table[256];

/* The link-up texts (src/data/link_crc_16af10.cpp), pointed at by save.h's
 * gCrash2LinkTextPtr/gCrash3LinkTextPtr. */
extern const char gCrash2LinkText[];
extern const char gCrash3LinkText[];

/* The link session's methods (C++, include/link_session.hpp: class
 * LinkSession) have no C caller and no C prototype. Its C-linkage
 * functions: */

/* src/link/link_sio.cpp */
extern s32 LinkSetupSio(void);
extern void LinkSerialIntr(void);
extern void LinkTimer3Intr(void);

/* src/link/link_handshake.cpp */
extern void MakeLinkHandshakeId(u8 *self);

#endif /* GUARD_LINK_H */
