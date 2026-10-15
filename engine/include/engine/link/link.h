#ifndef GUARD_LINK_H
#define GUARD_LINK_H

/* The link-cable subsystem (engine/src/engine/link/): the SIO session object, its IRQ
 * handlers and the handshake. The session layouts are C++, in
 * link_session.hpp; C (engine/src/engine/link/data/link_crc_table.cpp) needs only the tables
 * and the C-linkage functions below.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md). */

#include "core.h"

/* The link session the link IRQ handlers and the save transfer work on:
 * made by SaveMenu's constructor (`new LinkSession`), deleted by its
 * destructor (engine/src/game/ui/save_menu_input.cpp). Defined in
 * engine/src/engine/sys/iwram_data.cpp. The C++ files see it as its class,
 * LinkSession (link_session.hpp); C sees a tag. */
#ifdef __cplusplus
/* In its namespace (link_session.hpp; docs/cplusplus.md, "Namespaces"). */
namespace Engine
{
namespace Link
{
class LinkSession;
} // namespace Link
} // namespace Engine
extern Engine::Link::LinkSession *gLinkSession;
#else
extern struct link_session *gLinkSession;
#endif

/* Set by LinkSession::ResetState, cleared by LinkExchangeSaveData
 * (engine/src/game/ui/save_menu_draw.cpp). Defined in engine/src/engine/sys/iwram_data.cpp. */
extern u8 gLinkSessionReset;

/* The CRC-16 table MakeLinkHandshakeId and LinkSession::HandleSerial hash with
 * (engine/src/engine/link/data/link_crc_table.cpp). */
extern const u16 gCrc16Table[256];

/* The link-up texts (engine/src/engine/link/data/link_crc_table.cpp), pointed at by save.h's
 * gCrash2LinkTextPtr/gCrash3LinkTextPtr. */
extern const char gCrash2LinkText[];
extern const char gCrash3LinkText[];

/* The link session's methods (C++, engine/include/engine/link/link_session.hpp: class
 * LinkSession) have no C caller and no C prototype. Its C-linkage
 * functions: */

/* engine/src/engine/link/link_sio.cpp */
extern s32 LinkSetupSio(void);
extern void LinkSerialIntr(void);
extern void LinkTimer3Intr(void);

/* engine/src/engine/link/link_handshake.cpp */
extern void MakeLinkHandshakeId(u8 *self);

#endif /* GUARD_LINK_H */
