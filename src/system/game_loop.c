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
 * `self+0x70`/`0x6c`/`0x74` are accessed with direct `self`-relative
 * offsets throughout (no cached pointer) because those three offsets
 * fit the Thumb `ldr`/`str` immediate range (0-124); only the
 * `0xb4`/`0xb0`/`0xb8`/`0xd4`/`0xe0`/`0xbc` fields (all past that
 * range) need an explicit address computed into a local pointer -
 * matching the ROM exactly. Caching *all* of them in pointers (the
 * earlier attempt here) forced 3 extra always-live locals the natural
 * allocator had to spill into `r8`/`r9`/`sl`. */
void sub_8022BF0(void *self, u8 arg1)
{
    s32 *fieldbc;

    sub_80232FC(self);

    if (arg1 != 0) {
        s32 *fieldb4 = (s32 *)((u8 *)self + 0xb4);

        *(s32 *)((u8 *)self + 0x70) += *fieldb4;

        {
            s32 *fieldb0 = (s32 *)((u8 *)self + 0xb0);

            *(s32 *)((u8 *)self + 0x6c) += *fieldb0;
        }

        {
            s32 *fieldb8 = (s32 *)((u8 *)self + 0xb8);
            s32 *fieldd4 = (s32 *)((u8 *)self + 0xd4);
            u8 *fielde0 = (u8 *)self + 0xe0;
            s32 total;

            fieldbc = (s32 *)((u8 *)self + 0xbc);

            if (*(s32 *)((u8 *)self + 0x6c) > 0x63) {
                s32 carry = *(s32 *)((u8 *)self + 0x74);
                s32 value = *(s32 *)((u8 *)self + 0x6c);

                do {
                    carry++;
                    value -= 100;
                } while (value > 0x63);

                *(s32 *)((u8 *)self + 0x6c) = value;
                *(s32 *)((u8 *)self + 0x74) = carry;
            }

            total = *(s32 *)((u8 *)self + 0x74) + *fieldb8;
            if (total > 0x63) {
                total = 0x63;
            }
            *(s32 *)((u8 *)self + 0x74) = total;

            sub_80232B0(self);
            sub_8007398((struct actor *)gUnknown_030012D8, fieldd4[0], fieldd4[1]);
            sub_8022CA0(self, *fielde0);
        }
    } else {
        *(s32 *)((u8 *)self + 0x70) = *(s32 *)((u8 *)self + 0xb4);
        *(s32 *)((u8 *)self + 0x6c) = *(s32 *)((u8 *)self + 0xb0);
        *(s32 *)((u8 *)self + 0x74) = *(s32 *)((u8 *)self + 0xb8);

        fieldbc = (s32 *)((u8 *)self + 0xbc);
    }

    sub_8028568(gUnknown_03001318, *fieldbc);
    sub_80232C0(self);
}

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
 * The `self+0xa9`-byte-to-`+0xd0` copy is written as a read, then
 * `p += 0x27` on the *same* pointer, then the store - matching the
 * ROM's "derive `+0xd0` by adding `0x27` to the register that still
 * holds `+0xa9`" shape (a plain `*(p+0x27) = *p;` computed the
 * destination address before the read instead). In the `mode == 3`
 * branch that same pointer is then bumped again (`p += 0x14`) to
 * become the `self+0xe4` destination for the trailing `sub_800014C`
 * copy, reusing the register chain exactly like the ROM. */
void sub_8022CA0(void *self, u8 arg1)
{
    void *level = *(void **)((u8 *)self + 0xdc);

    if (*(s32 *)((u8 *)level + 8) == 3) {
        *(s32 *)((u8 *)self + 0xcc) = sub_8023414(self);

        /* Barrier: without this, the compiler notices `self + 0xa9`
         * is `(self + 0xcc) - 0x23` and reuses the field-0xcc pointer
         * (`subs r1, #0x23`) instead of recomputing fresh from `self`
         * the way the ROM does (`adds r0, r6, #0; adds r0, #0xa9`). */
        asm volatile("" : "+r"(self));

        {
            register u8 *p asm("r0") = (u8 *)self + 0xa9;
            u8 value = *p;

            p += 0x27;
            *p = value;
            p += 0x14;
            sub_800014C(p, self, 0x68);
        }
    } else {
        void *player = gUnknown_030012D8;
        s32 x = *(s32 *)player;
        s32 y = *(s32 *)((u8 *)player + 4);
        void *base;

        *((u8 *)self + 0xe0) = arg1;
        *(s32 *)((u8 *)self + 0xcc) = sub_8023414(self);

        asm volatile("" : "+r"(self));

        {
            register u8 *p asm("r0") = (u8 *)self + 0xa9;
            u8 value = *p;

            p += 0x27;
            *p = value;
        }

        sub_80232FC(self);
        sub_80232EC(self);
        {
            /* Two-word field copy: a plain `self->0xd4 = x; self->0xd8
             * = y;` re-derives the destination address from scratch
             * for the second store (`adds r0, #4` then `str r5,
             * [r0]`); the ROM instead keeps the base pointer from the
             * first store and uses its `[r0, #4]` immediate-offset
             * form. Reproduced with a local pointer and indexed
             * stores - same gotcha as `sub_8023500`/`sub_802356C` in
             * docs/matching/issue-37-game-loop-234e8.md. */
            s32 *dst = (s32 *)((u8 *)self + 0xd4);

            dst[0] = x;
            dst[1] = y;
        }

        base = gUnknown_030012B4;
        {
            /* The control word is loaded from the literal pool fresh
             * for each call (matching the ROM's two separate `ldr
             * r2, =0x04000040`) rather than hoisted into one shared
             * register across both calls - see `sub_802356C`'s
             * identical gotcha in docs/matching/issue-37-game-loop-
             * 234e8.md for why this needs its own `register` block
             * declared right before each call, after the two pointer
             * arguments are already computed. */
            void *a = (u8 *)base + 0x108;
            void *b = (u8 *)base + 8;
            register u32 ctrl asm("r2") = 0x04000040;

            sub_803A94C(a, b, ctrl);
        }
        {
            void *a = (u8 *)base + 0x308;
            void *b = (u8 *)base + 0x208;
            register u32 ctrl asm("r2") = 0x04000040;

            sub_803A94C(a, b, ctrl);
        }

        sub_800014C((u8 *)self + 0xe4, self, 0x68);
    }
}
