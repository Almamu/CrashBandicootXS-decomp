#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;

/* This whole file re-derives `gUnknown_03001630->channels[gUnknown_03001630
 * ->curChannelIdx]` fresh at every single use, never through a cached local
 * pointer - that's not a style choice, it's load-bearing: the ROM's own
 * codegen for these three functions never spills anything into a callee-
 * saved register (no `push {r4-r7}` at all for the loop-shaped two, and
 * only `r4` - the clamped volume parameter itself - for the volume-setter
 * pair), reusing just r0-r3 throughout, even across a loop. Caching the
 * channel-chase into a named local (the obvious, idiomatic way to write
 * this) makes gcc-2.9 hoist it into r4-r7 as a loop invariant instead,
 * which never reproduces the ROM's bytes - this was previously documented
 * (docs/status/audio.md, issue #67) as a "confirmed many-register
 * loop-allocation ceiling" no C rephrasing could close. Writing every
 * occurrence as this same macro instead - textually re-expanded, so gcc
 * never gets the chance to treat it as one shared value - reproduces the
 * ROM's redundant-reload byte pattern exactly. */
#define GAX_CHAN() (gUnknown_03001630->channels[gUnknown_03001630->curChannelIdx])

/* Sets the "muted" flag (+0x24) on one voice entry of the current channel's
 * bank, or on every entry (`idx == -1`). The entry array is reached via
 * `chan->0` ("obj"), `obj->0` ("table"), `table->0xc` (a base index added
 * to the loop/argument index) and `obj->8` (the actual entry-pointer
 * array); channel/voice object shape isn't modeled yet, so every field
 * stays a raw offset like the rest of this GAX2 engine cluster
 * (gax_channel_note_cut.c and neighbors). */
void sub_8038FD0(s32 idx)
{
    if (idx == -1) {
        s32 i;

        for (i = 0; i < *(u32 *)((u8 *)(*(void **)GAX_CHAN()) + 0x14); i++) {
            void *obj = *(void **)GAX_CHAN();
            u32 base = *(u32 *)((u8 *)(*(void **)obj) + 0xc) + i;
            u32 stride = *(u32 *)((u8 *)obj + 8);
            u8 *entry = *(u8 **)((base << 2) + stride);

            entry[0x24] = 1;
        }
    } else {
        void *obj = *(void **)GAX_CHAN();

        if (idx < *(u32 *)((u8 *)obj + 0x14) && idx >= 0) {
            u32 base = *(u32 *)((u8 *)(*(void **)obj) + 0xc) + idx;
            u32 stride = *(u32 *)((u8 *)obj + 8);
            u8 *entry = *(u8 **)((base << 2) + stride);

            entry[0x24] = 1;
        }
    }
}

/* Sets a volume byte (+0x18, clamped to 0xff) on one entry of a *different*
 * embedded array than sub_8038FD0 above - here reached directly as
 * `chan[idx*4 + 0xc]` (an inline array living inside the channel object
 * itself, not through the `obj`/`table` indirection), while still bounds-
 * checking against `chan->0->0->0xc` (the same "table" object's own
 * count). `idx == -1` sets every entry; any `idx > -2` (i.e. `idx >= 0`,
 * written this way to match the ROM's own signed compare against -2 byte
 * for byte) sets just that one, bounds-checked; `idx <= -2` is a no-op. */
void sub_8039064(s32 idx, u32 vol)
{
    if (vol > 0xff) {
        vol = 0xff;
    }

    if (idx == -1) {
        s32 i;

        for (i = 0; i < *(u32 *)((u8 *)(*(void **)(*(void **)GAX_CHAN())) + 0xc); i++) {
            register u8 *chan asm("r1") = GAX_CHAN();
            register u32 off asm("r0") = (u32)i << 2;
            register u8 *entryAddr asm("r0");
            u8 *entry;

            /* Forces the ROM's exact "adds r0, r0, r1" register-operand
             * order (chan/off pinned to r1/r0 above) - gcc-2.9 always
             * canonicalizes this pointer+offset add with the pointer
             * operand first (`adds r0, r1, r0`) regardless of C-level
             * source order, so only a raw instruction closes this gap
             * (see docs/workflow.md step 3 / matching_decomp_register_
             * pinning memory's inline-asm-anchor technique). The operand
             * list (rather than a bare asm string) keeps gcc from
             * treating `chan`/`off`'s defining loads as dead. */
            asm("add %0, %0, %1" : "=r"(entryAddr) : "r"(chan), "0"(off));
            entry = *(u8 **)(entryAddr + 0xc);
            entry[0x18] = vol;
        }
    } else if (idx > -2) {
        register u8 *chan asm("r1") = GAX_CHAN();
        void *obj = *(void **)chan;

        if (idx < *(u32 *)((u8 *)(*(void **)obj) + 0xc)) {
            register u32 off asm("r0") = (u32)idx << 2;
            register u8 *entryAddr asm("r0");
            u8 *entry;

            /* Same "adds r0, r0, r1" operand-order gap as the loop body
             * above. */
            asm("add %0, %0, %1" : "=r"(entryAddr) : "r"(chan), "0"(off));
            entry = *(u8 **)(entryAddr + 0xc);
            entry[0x18] = vol;
        }
    }
}

/* The same "volume byte" shape as sub_8039064 above, but through
 * sub_8038FD0's `obj`/`table`/`stride` entry-array indirection instead of
 * chan's own inline array, and its single-index bounds check is a signed
 * compare against `obj->0x14` directly (`bge`, matching sub_8038FD0's
 * `idx`/`limit` field) rather than sub_8039064's unsigned one against
 * `table->0xc`. */
void sub_80390F8(s32 idx, u32 vol)
{
    if (vol > 0xff) {
        vol = 0xff;
    }

    if (idx == -1) {
        s32 i;

        for (i = 0; i < *(u32 *)((u8 *)(*(void **)GAX_CHAN()) + 0x14); i++) {
            void *obj = *(void **)GAX_CHAN();
            u32 base = *(u32 *)((u8 *)(*(void **)obj) + 0xc) + i;
            u32 stride = *(u32 *)((u8 *)obj + 8);
            u8 *entry = *(u8 **)((base << 2) + stride);

            entry[0x18] = vol;
        }
    } else if (idx > -2) {
        void *obj = *(void **)GAX_CHAN();
        s32 limit = *(s32 *)((u8 *)obj + 0x14);

        if (idx < limit) {
            u32 base = *(u32 *)((u8 *)(*(void **)obj) + 0xc) + idx;
            u32 stride = *(u32 *)((u8 *)obj + 8);
            u8 *entry = *(u8 **)((base << 2) + stride);

            entry[0x18] = vol;
        }
    }
}
