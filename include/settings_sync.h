#ifndef __SETTINGS_SYNC_H__
#define __SETTINGS_SYNC_H__

/* The composite pause/options screen's checksummed 0x200-byte settings
 * record - `self->field_8c`/`field_90`'s pointee (see
 * pause_options_screen.h). Per-row "selected" flags, a couple of fixed
 * marker bytes, a bitmask, and a running additive checksum
 * (sub_8002B70/sub_8002B44, both still raw, outside this chunk) over
 * the first 0x1f8 bytes. See docs/matching/issue-5-overlay-ui-sync.md.
 * Shared (via this header) between src/graphics/settings_menu8.c and
 * settings_menu8b.c/settings_menu8c.c, split apart so the two parked
 * functions between them (sub_8002D44/sub_8002E20/sub_8002EFC,
 * sub_8003698) can stay raw asm without breaking ROM link order. */
struct settings_sync_record {
    u8 unused_000[0x1f4];
    u8 rowSelected[4]; /* 0x1f4 - sub_8002CE8 here; sub_8002C6C (still raw) sets it */
    u8 field_1f8;       /* 0x1f8 - init'd to 'C' (0x43) by sub_8002C84 */
    u8 versionNibble;     /* 0x1f9 - init'd to 0x12; high nibble read by sub_8002B94 (still raw) */
    u8 flags;               /* 0x1fa - bitmask, sub_8002CF4/sub_8002D0C/sub_8002D28 */
    u8 field_1fb;             /* 0x1fb - zeroed by sub_8002C84, otherwise untouched in this chunk */
    u32 checksum;               /* 0x1fc - sub_8002B70/sub_8002B44 (still raw) */
};
COMPILE_TIME_ASSERT(sizeof(struct settings_sync_record) == 0x200);

/* A transient SIO send/receive envelope wrapping a settings_sync_record
 * copy - allocated per "connecting..." spinner-dialog session
 * (sub_8003B40, src/graphics/settings_menu.c, parked) and torn down
 * with it. `tmpl`/`cursor` stream a settings_sync_record's bytes out to
 * the SIO session's per-player ring buffer (sub_8002D44); `data`
 * receives the remote side's copy of the same shape from its own ring
 * buffer (sub_8002E20), with `writePtr` as the fill cursor. See
 * docs/matching/issue-5-overlay-ui-sync.md for the full protocol
 * write-up. */
struct settings_sync_pump {
    u32 remaining;      /* 0x000 - bytes left to send out of `tmpl`, reset to sizeof(data) */
    u32 totalReceived;    /* 0x004 - running total of bytes received into `data` */
    struct settings_sync_record *tmpl; /* 0x008 - the record sub_8002FCC copies in */
    u8 *cursor;                          /* 0x00c - read cursor into `tmpl` while draining `remaining` */
    u8 data[sizeof(struct settings_sync_record)]; /* 0x010 - the received record's raw bytes */
    u8 *writePtr;                                    /* 0x210 - write cursor into `data` */
    u32 field_214;                                     /* 0x214 - set 1 once `remaining` fully drains (send complete) */
    u32 field_218;                                       /* 0x218 - set 1 once `totalReceived` reaches sizeof(data) (receive complete) */
    u32 field_21c;                                         /* 0x21c - elapsed-poll counter, sub_8002EFC */
};
COMPILE_TIME_ASSERT(sizeof(struct settings_sync_pump) == 0x220);

#endif /* __SETTINGS_SYNC_H__ */
