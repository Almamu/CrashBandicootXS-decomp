#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gGaxPlayerState;

/* This whole file re-derives `gGaxPlayerState->channels[gGaxPlayerState
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
#define GAX_CHAN() (gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx])

/* Queues a key-off (`pendingNote = 1`) on one sound-effect voice of the
 * current player, or on every one (`idx == -1`). The voices are the
 * mixer handler's (`handlers[0]`) children after the song's own
 * channels: `children[type->childCount + idx]`, `extraChildren` of them. */
void GAX_stop_fx(s32 idx)
{
    if (idx == -1) {
        s32 i;

        for (i = 0; i < GAX_MIXER()->extraChildren; i++) {
            struct GaxMixerHandler *obj = GAX_MIXER();
            u32 base = obj->type->childCount + i;
            struct GaxChannelState **children = (struct GaxChannelState **)obj->children;

            children[base]->pendingNote = 1;
        }
    } else {
        struct GaxMixerHandler *obj = GAX_MIXER();

        if (idx < obj->extraChildren && idx >= 0) {
            u32 base = obj->type->childCount + idx;
            struct GaxChannelState **children = (struct GaxChannelState **)obj->children;

            children[base]->pendingNote = 1;
        }
    }
}

/* Sets a volume byte (`volume`, clamped to 0xff) on one of the
 * player's song channels - `handlers[idx + 3]`, reached directly as
 * `chan[idx*4 + 0xc]` rather than through the mixer's children like
 * GAX_stop_fx above - bounds-checked against the mixer type's
 * `childCount` (the number of song channels). `idx == -1` sets every
 * entry; any `idx > -2` (i.e. `idx >= 0`, written this way to match the
 * ROM's own signed compare against -2 byte for byte) sets just that one,
 * bounds-checked; `idx <= -2` is a no-op. */
void GAX_set_music_volume(s32 idx, u32 vol)
{
    if (vol > 0xff) {
        vol = 0xff;
    }

    if (idx == -1) {
        s32 i;

        for (i = 0; i < GAX_MIXER()->type->childCount; i++) {
            register u8 *chan asm("r1") = GAX_CHAN();
            register u32 off asm("r0") = (u32)i << 2;
            register u8 *entryAddr asm("r0");
            struct GaxChannelState *entry;

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
            entry = ((struct GaxChannelState **)entryAddr)[3];
            entry->volume = vol;
        }
    } else if (idx > -2) {
        register u8 *chan asm("r1") = GAX_CHAN();
        struct GaxMixerHandler *obj = *(struct GaxMixerHandler **)chan;

        if (idx < obj->type->childCount) {
            register u32 off asm("r0") = (u32)idx << 2;
            register u8 *entryAddr asm("r0");
            struct GaxChannelState *entry;

            /* Same "adds r0, r0, r1" operand-order gap as the loop body
             * above. */
            asm("add %0, %0, %1" : "=r"(entryAddr) : "r"(chan), "0"(off));
            entry = ((struct GaxChannelState **)entryAddr)[3];
            entry->volume = vol;
        }
    }
}

/* The same "volume byte" shape as GAX_set_music_volume above, but on the mixer's
 * sound-effect voices (GAX_stop_fx's `children[type->childCount + idx]`)
 * instead of the player's channel handlers, and its single-index bounds
 * check is a signed compare against `extraChildren` directly (`bge`,
 * matching GAX_stop_fx's `idx`/`limit` field) rather than GAX_set_music_volume's
 * unsigned one against `type->childCount`. */
void GAX_set_fx_volume(s32 idx, u32 vol)
{
    if (vol > 0xff) {
        vol = 0xff;
    }

    if (idx == -1) {
        s32 i;

        for (i = 0; i < GAX_MIXER()->extraChildren; i++) {
            struct GaxMixerHandler *obj = GAX_MIXER();
            u32 base = obj->type->childCount + i;
            struct GaxChannelState **children = (struct GaxChannelState **)obj->children;

            children[base]->volume = vol;
        }
    } else if (idx > -2) {
        struct GaxMixerHandler *obj = GAX_MIXER();
        s32 limit = obj->extraChildren;

        if (idx < limit) {
            u32 base = obj->type->childCount + idx;
            struct GaxChannelState **children = (struct GaxChannelState **)obj->children;

            children[base]->volume = vol;
        }
    }
}
