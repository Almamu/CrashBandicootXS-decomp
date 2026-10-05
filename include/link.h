#ifndef GUARD_LINK_H
#define GUARD_LINK_H

/* The link-cable subsystem (src/link/): the SIO session object, its IRQ
 * handlers and the handshake. The session layouts are in link_session.h.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md). */

#include "core.h"
#include "link_session.h"

/* The link session the link IRQ handlers and the save transfer work on:
 * allocated by InitSaveMenu, freed by DestroySaveMenu
 * (src/save/save_menu_input.c). Defined in src/iwram/iwram_data.c. */
extern struct link_session *gLinkSession;

/* Set by ResetLinkSessionState, cleared by LinkExchangeSaveData
 * (src/save/save_menu_draw.c). Defined in src/iwram/iwram_data.c. */
extern u8 gLinkSessionReset;

/* The CRC-16 table MakeLinkHandshakeId and HandleLinkSerial hash with
 * (src/data/link_crc_16af10.c). */
extern const u16 gCrc16Table[256];

/* The link-up texts (src/data/link_crc_16af10.c), pointed at by save.h's
 * gCrash2LinkTextPtr/gCrash3LinkTextPtr. */
extern const char gCrash2LinkText[];
extern const char gCrash3LinkText[];

/* src/link/link_sio.c */
extern s32 LinkStart(struct link_session *self, u32 flags);
extern s32 LinkSetupSio(void);
extern s32 ResetLinkSession(struct link_session *self);
extern void DestroyLinkSession(struct link_session *self, u32 flags);
extern struct link_session *InitLinkSession(struct link_session *self);
extern void LinkSerialIntr(void);
extern void LinkTimer3Intr(void);

/* src/link/link_handshake.c */
extern void MakeLinkHandshakeId(u8 *self);
extern s32 LinkStop(struct link_session *self);

/* src/link/link_session.c */
extern s32 UpdateLinkSession(struct link_session *self);
extern void HandleLinkSerial(struct link_session *self, u16 *data);

/* src/link/link_session_reset.c */
extern s32 ResetLinkSessionState(struct link_session *self);

#endif /* GUARD_LINK_H */
