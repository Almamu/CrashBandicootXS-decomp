#include "core.h"
#include "vram_pool.h"
#include "actor.h"

extern void *gUnknown_030012D8;
extern void *gUnknown_030012B4;
extern void *gUnknown_03001318;
extern struct tile_asset_cache *gUnknown_030012B8;

extern s32 sub_8023414(void *self);
extern void sub_80232EC(void *self);
extern void sub_80232FC(void *self);
extern void *sub_800014C(void *dest, void *src, s32 size);
extern void sub_803A94C(void *src, void *dst, s32 control);
extern void sub_80232B0(void *self);
extern void sub_80232C0(void *self);
extern void sub_8007398(struct actor *self, s32 arg1, s32 arg2);
extern void sub_8028568(void *state, s32 arg1);
extern void sub_8022CA0(void *self, u8 arg1);

#if NON_MATCHING
/* Called at level start/checkpoint-restore: `arg1` selects whether to
 * accumulate this attempt's progress into the running totals
 * (`self+0x70`/`0x6c`/`0x74`, carrying every 100 units of `+0x6c` into
 * `+0x74` the same way `sub_8022F2C`'s odometer carries) or to just
 * reset those three fields back from their `+0xb4`/`0xb0`/`0xb8`
 * "level start" snapshot. Either way it re-syncs the player's stored
 * position (`self+0xd4`/`0xd8` -> `sub_8007398`) and re-runs
 * `sub_8022CA0`, then flushes `self+0xbc` into the `gUnknown_03001318`
 * cache and clears the `+0xa4` busy flag.
 *
 * NOT YET BYTE-MATCHING: semantics/field offsets/calls all confirmed;
 * this compiler's natural register allocation for the four cached
 * field addresses spills into `r8`/`r9`/`sl` (needing extra push/pop
 * pairs) where the ROM keeps everything in `r4`-`r7`. Not yet found a
 * phrasing that reproduces the ROM's exact low-register reuse. */
void sub_8022BF0(void *self, u8 arg1)
{
    s32 *field70 = (s32 *)((u8 *)self + 0x70);
    s32 *field6c = (s32 *)((u8 *)self + 0x6c);
    s32 *field74 = (s32 *)((u8 *)self + 0x74);
    s32 *fieldbc = (s32 *)((u8 *)self + 0xbc);

    sub_80232FC(self);

    if (arg1 != 0) {
        s32 *fieldb8 = (s32 *)((u8 *)self + 0xb8);
        s32 total;

        *field70 += *(s32 *)((u8 *)self + 0xb4);
        *field6c += *(s32 *)((u8 *)self + 0xb0);

        if (*field6c > 0x63) {
            s32 carry = *field74;
            s32 value = *field6c;

            do {
                carry++;
                value -= 100;
            } while (value > 0x63);

            *field6c = value;
            *field74 = carry;
        }

        total = *field74 + *fieldb8;
        if (total > 0x63) {
            total = 0x63;
        }
        *field74 = total;

        sub_80232B0(self);
        sub_8007398((struct actor *)gUnknown_030012D8, *(s32 *)((u8 *)self + 0xd4),
                    *(s32 *)((u8 *)self + 0xd8));
        sub_8022CA0(self, *((u8 *)self + 0xe0));
    } else {
        *field70 = *(s32 *)((u8 *)self + 0xb4);
        *field6c = *(s32 *)((u8 *)self + 0xb0);
        *field74 = *(s32 *)((u8 *)self + 0xb8);
    }

    sub_8028568(gUnknown_03001318, *fieldbc);
    sub_80232C0(self);
}
#endif

#if NON_MATCHING
/* `arg1` truncated to a byte, matching the ROM's own `lsls/lsrs #0x18`
 * parameter normalization. If `self->levelPtr->mode == 3`, just
 * refreshes `self+0xcc`/`0xd0` (a cached frame count / a copy of the
 * `+0xa9` byte) and snapshots the first `0x68` bytes of `self` into
 * `self+0xe4`. Otherwise it also stashes the camera's `{x, y}`
 * (`gUnknown_030012D8`) into `self+0xd4`/`0xd8`, clears two flag
 * bytes, and syncs two spans of the `gUnknown_030012B4` bitmap
 * (`+0x108`->`+8`, `+0x308`->`+0x208`) via the BIOS `CpuSet` wrapper -
 * reads like an end-of-level "freeze the HUD/save state" snapshot.
 *
 * NOT YET BYTE-MATCHING: every field offset/call/argument is confirmed
 * correct, but two cosmetic register-allocation gaps remain - the
 * `self+0xa9`-byte-to-`+0xd0` copy computes its destination from a
 * persisted `self+0xa9` address instead of the ROM's "derive +0xd0 by
 * adding 0x27 to the same register right after the `sub_8023414`
 * call" shape, and the repeated `0x04000040` `sub_803A94C` control
 * word gets hoisted into a shared register across both calls where
 * the ROM reloads it from its literal pool each time. Neither changes
 * behavior; parked rather than guess further at the exact phrasing
 * this compiler needs. */
void sub_8022CA0(void *self, u8 arg1)
{
    void *level = *(void **)((u8 *)self + 0xdc);

    if (*(s32 *)((u8 *)level + 8) == 3) {
        u8 *a9 = (u8 *)self + 0xa9;

        *(s32 *)((u8 *)self + 0xcc) = sub_8023414(self);
        *(a9 + 0x27) = *a9;
        sub_800014C((u8 *)self + 0xe4, self, 0x68);
    } else {
        void *player = gUnknown_030012D8;
        s32 x = *(s32 *)player;
        s32 y = *(s32 *)((u8 *)player + 4);
        void *base;
        u8 *a9;

        *((u8 *)self + 0xe0) = arg1;
        *(s32 *)((u8 *)self + 0xcc) = sub_8023414(self);
        a9 = (u8 *)self + 0xa9;
        *(a9 + 0x27) = *a9;
        sub_80232FC(self);
        sub_80232EC(self);
        *(s32 *)((u8 *)self + 0xd4) = x;
        *(s32 *)((u8 *)self + 0xd8) = y;

        base = gUnknown_030012B4;
        sub_803A94C((u8 *)base + 0x108, (u8 *)base + 8, 0x04000040);
        sub_803A94C((u8 *)base + 0x308, (u8 *)base + 0x208, 0x4000040);

        sub_800014C((u8 *)self + 0xe4, self, 0x68);
    }
}
#endif
