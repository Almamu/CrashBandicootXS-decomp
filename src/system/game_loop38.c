#include "core.h"

/* GitHub issue #38: 0x08024790-0x080247EB (game_loop) - continuation of
 * the sound-channel-handle helper family (game_loop37.c). See
 * docs/matching/issue-38-medal-results-tally.md for the original chunk
 * write-up and docs/matching/issue-38-sound-channel-family.md for this
 * follow-up pass. */

struct AudioContext;

/* Same `SoundChannelItem`/`SoundChannelList` shape game_loop37.c's
 * sub_8024590/sub_8024640/sub_8024708 operate on - kept as this file's
 * own local copy (only the three fields this function reads), per this
 * project's established per-translation-unit convention for structs
 * shared across non-adjacent files. */
struct SoundChannelItem {
    u8 unused_00[0xc];
    s32 field_0c; /* +0x0c: sentinel -1 means "none" */
    u8 unused_10;
    u8 field_11;    /* +0x11: nonzero triggers a duck-out */
    u8 field_12;      /* +0x12: nonzero (and field_18 != 0x63) triggers a
                        * re-arm */
    u8 unused_13;
    u8 unused_14[4];
    u32 field_18; /* +0x18: sentinel 0x63 (99) means "no sfx" */
};

struct SoundChannelList {
    struct SoundChannelItem **items; /* +0x00 */
};

extern struct AudioContext *gUnknown_030012BC;
extern void sub_8001AC4(struct AudioContext *self, u32 value);
extern void sub_800132C(u8 flags, s32 frameDelay, u8 sync);
extern void sub_80019A8(struct AudioContext *self, u32 id);

/* Tail half of sub_8024640's per-item body (game_loop37.c) - duck-out
 * (`field_11`), fade-start (`field_0c`), and re-arm (`field_12`/
 * `field_18`) - reused standalone against a caller-supplied index. Each
 * of the three checks re-reads `self->items[idx]` fresh rather than
 * sharing one cached pointer across all three, matching sub_8024640's
 * own inlined copy of this same body. */
void sub_8024790(struct SoundChannelList *self0, s32 idx)
{
    register struct SoundChannelList *self asm("r5") = self0;

    if (self->items[idx]->field_11 != 0) {
        sub_8001AC4(gUnknown_030012BC, 0);
    }

    {
        s32 v = self->items[idx]->field_0c;

        if (v != -1) {
            sub_800132C((u8)v, 1, 0);
        }
    }

    {
        struct SoundChannelItem *item = self->items[idx];

        if (item->field_12 != 0 && item->field_18 != 0x63) {
            sub_80019A8(gUnknown_030012BC, item->field_18);
        }
    }
}
