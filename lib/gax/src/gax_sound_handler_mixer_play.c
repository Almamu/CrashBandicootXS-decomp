#include "gax_internal.h"

/* MemCopy32 is this ROM's memcpy (reached from the non-constant
 * aggregate initializers below). */
asm(".set memcpy, MemCopy32");

/* This file's functions (issue #68's remainder past
 * `gax_sound_handler_mixer.c`) finish out the GAX2_SoundHandler
 * mixer type's function-pointer table: `GaxMixerPlay` is its
 * `play_fn`, and the other three hand a small work item to one of the
 * ARM DSP routines copied into `gGaxPlayerState` (hand-written ARM,
 * lib/gax/asm/gax_arm_dsp.s, linked right after this file) through
 * `GAX_CALL_ARM` (gax_internal.h), Shin'en's own inline asm: a
 * hand-computed return address plus `bx`, since ARMv4T Thumb has no
 * `blx reg`.
 *
 * The ROM disassembly shows two extra labels, `sub_803A318` and
 * `sub_803A608`, sitting right at `GAX_CALL_ARM`'s return point inside
 * `GaxMixerApplyFilter`/`GaxMixFrame` (the trailing `nop` and the shared
 * epilogue). Neither is ever called from anywhere in the ROM - they're
 * disassembler artifacts of the hand-computed return address, not real
 * functions, and disappear now that both are plain C.
 *
 * All four were NAKED until a later pass found the call idiom is
 * reproducible as narrow inline asm with a `"m"` operand (see
 * docs/matching/archive/gax-toolchain-retry.md). */

/* Only the parts of the channel objects this file touches. */
struct GaxChannelView {
    u8 pad_00[0x18];
    u8 volume; /* 0x18 - GaxChannelState.volume (GAX_set_music_volume) */
};

/* The player's handler array (GAX_PLAYER()): the mixer, the Info
 * handler, the handler of the layout's third slot, then the song's
 * channels. */
struct GaxPlayerHandlers {
    u32 mixer;                   /* 0x00 */
    struct GaxInfoHandler *info; /* 0x04 */
    u32 field_08;
    struct GaxChannelView *channels[1]; /* 0x0c - really hdr->childCount long */
};

/* The mixer handler (struct GaxMixerHandler, gax_internal.h) as this
 * file's functions read it: its type as GaxMixerViewType, its children
 * (the song's channels, then the SFX voices) as GaxMixerViewChild. */
struct GaxMixerViewChild;

struct GaxMixerViewChildType {
    u32 field_00[2];
    u8 (*play)(struct GaxMixerViewChild *child, u32 *buf, u32 arg); /* 0x08 */
};

struct GaxMixerViewChild {
    struct GaxMixerViewChildType *ops;
    u8 pad_04[9];
    u8 isFirst; /* 0x0d */
};

/* The mixer type's data (GaxHandlerType.data.dsp): the song's echo
 * settings. */
struct GaxMixerEchoParams {
    u32 primary; /* 0x00 - channels played before the echo pass */
    u32 echo;    /* 0x04 - nonzero: run the echo pass (GAX2_init's fxEcho test too) */
};

struct GaxMixerViewType {
    u8 pad_00[8];
    u8 (*step)(void *self, u32 a, u32 b); /* 0x08 */
    u32 childCount;                       /* 0x0c */
    u8 pad_10[8];
    struct GaxMixerEchoParams *counts; /* 0x18 */
};

struct GaxMixerView {
    struct GaxMixerViewType *hdr;        /* 0x00 */
    struct GaxMixerFormat *format;       /* 0x04 */
    struct GaxMixerViewChild **children; /* 0x08 */
    u32 pos;                             /* 0x0c */
    u32 mixBuf;                          /* 0x10 - GaxMixerHandler.mixBuf */
    u32 extraChildren;                   /* 0x14 */
};

struct GaxWorkItem5 {
    u32 bytes;
    u32 *buf;
    u32 echoBuf;
    u32 echoLen;
    u32 echoTaps;
};

struct GaxWorkItem7 {
    u32 *buf;
    u32 bytes;
    u32 clamp;
    u32 count;
    u32 echoBuf;
    u32 echoLen;
    u32 echoTaps;
};

struct GaxWorkItem4 {
    u32 mixBuf;
    u32 *buf;
    u32 samples;
    u32 step;
};

/* Runs `echoCode` (the IWRAM copy of gGaxArmEcho) over a 5-word work item
 * built from the buffer size and three player-state fields. */
void GaxMixerApplyEcho(struct GaxMixerView *self, u32 *buf)
{
    u32 bytes = self->format->frames * self->format->channels * 2;
    struct GaxPlayerState *st = gGaxPlayerState;
    u32 f20 = st->echoBuf;
    struct GaxWorkItem5 item = { bytes, buf, f20, st->echoLen, st->echoTaps };
    void *arg;

    arg = &item;
    GAX_CALL_ARM(gGaxPlayerState->echoCode, arg);
}

/* Same shape as `GaxMixerApplyEcho` (a 7-word work item this time, with an
 * extra `0x55 - clampArg` word) through `filterCode`, the IWRAM copy of
 * the ARM low-pass filter `gGaxArmFilter`: two cascaded one-pole stages
 * over the mixed buffer, with a coefficient of `0x334 * (0x55 - filter)
 * >> 8`, so a larger `filter` (at most 0x55) cuts more. The routine is
 * followed by its "FILT" tag. */
void GaxMixerApplyFilter(struct GaxMixerView *self, u32 *buf, u32 clampArg, u32 count)
{
    u32 bytes = self->format->frames * self->format->channels * 2;
    struct GaxPlayerState *st = gGaxPlayerState;
    u32 f20 = st->echoBuf;
    struct GaxWorkItem7 item = {
        buf, bytes, 0x55 - clampArg, count, f20, st->echoLen, st->echoTaps
    };
    void *arg;

    arg = &item;
    GAX_CALL_ARM(gGaxPlayerState->filterCode, arg);
}

/* Zero-fills `format->frames * format->channels` halfwords of `buf`,
 * a word at a time. */
#define GAX_MIXER_CLEAR(self, buf)                                        \
    {                                                                      \
        s32 len_ = (self)->format->frames * (self)->format->channels * 2; \
        u32 *p_ = (buf);                                                   \
        result = 1;                                                        \
        while (len_ > 0) {                                                 \
            *p_++ = 0;                                                     \
            len_ -= 4;                                                     \
        }                                                                  \
    }

/* Plays children `[first, end)`, ORing their "produced output" results. */
#define GAX_MIXER_PLAY_CHILD(self, i, buf, arg2)                                  \
    {                                                                            \
        (self)->children[i]->isFirst = (result == 0);                            \
        result |= (self)->children[i]->ops->play((self)->children[i], buf, arg2); \
    }

/* The mixer type's `play_fn`: plays the primary children (unless
 * `skipSongChannels` is set), then the extra children either
 * before or after the `GaxMixerApplyEcho`/`GaxMixerApplyFilter` DSP passes depending
 * on `fxEcho`, zero-filling `buf` first whenever nothing
 * has produced output yet. Returns whether anything did.
 *
 * Before the SFX voices it clears `buf` only for half-rate SFX
 * (`GaxSongData.halfRateFx` or the parameter block's `flags` bit 5).
 * A voice mixed first stores at full rate (resampler mode 0); after the
 * clear every voice adds, in its `mixMode`, which is the half-rate mode 2
 * there (GaxFxChannelInit). Mode 2 has no store form. */
u8 GaxMixerPlay(struct GaxMixerView *self, u32 *buf, u32 arg2)
{
    u8 result = 0;
    u32 i;

    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = 0; i < self->hdr->counts->primary; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    if (gGaxPlayerState->fxEcho != 0) {
        if (result == 0) {
            struct GaxSongHeader *song = gGaxPlayerState->songPtr;
            if (song->layout->types[1]->data.song->halfRateFx != 0 || (song->flags & 0x20))
                GAX_MIXER_CLEAR(self, buf);
        }
        for (i = self->hdr->childCount; i < self->hdr->childCount + self->extraChildren; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    /* The echo pass is skipped while the song's master volume or every
     * channel's volume is 0. */
    if (self->hdr->counts->echo != 0) {
        u8 allMuted;
        struct GaxPlayerHandlers *chan;

        if (result == 0)
            GAX_MIXER_CLEAR(self, buf);
        allMuted = 1;
        chan = gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx];
        if (chan->info->volume != 0) {
            /* Walks `chan->channels[]` by advancing `chan` itself one
             * pointer at a time - this is what keeps the ROM's
             * `ldr rX, [chan, #0xc]` addressing inside the loop. */
            for (i = 0; i < self->hdr->childCount && allMuted; i++) {
                if (chan->channels[0]->volume != 0)
                    allMuted = 0;
                chan = (struct GaxPlayerHandlers *)((struct GaxChannelView **)chan + 1);
            }
        }
        if (allMuted == 0)
            GaxMixerApplyEcho(self, buf);
    }
    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = self->hdr->counts->primary; i < self->hdr->childCount; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    {
        u32 clamp = gGaxPlayerState->filter;
        if (clamp > 0x55)
            clamp = 0x55;
        gGaxPlayerState->filter = clamp;
        if (gGaxPlayerState->filterCode != NULL && clamp != 0)
            GaxMixerApplyFilter(self, buf, clamp, self->hdr->childCount);
    }
    if (gGaxPlayerState->fxEcho == 0) {
        if (result == 0) {
            struct GaxSongHeader *song = gGaxPlayerState->songPtr;
            if (song->layout->types[1]->data.song->halfRateFx != 0 || (song->flags & 0x20))
                GAX_MIXER_CLEAR(self, buf);
        }
        for (i = self->hdr->childCount; i < self->hdr->childCount + self->extraChildren; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    gGaxPlayerState->skipSongChannels = 0;
    return result;
}

/* Advances the mixer's position through its type's `play` (GaxMixerPlay,
 * into `mixBuf`); if that produced a block, hands a 4-word work item to
 * `downmixCode` (the IWRAM copy of gGaxArmDownmix), otherwise zero-fills `buf` for
 * the block's length. Called by GAX_play with the mixer as a `struct
 * GaxMixerHandler`; the rest of this file still reads it through its
 * own `struct GaxMixerView` view (`hdr` is `type`, `mixBuf` is
 * `mixBuf`). */
void GaxMixFrame(struct GaxMixerHandler *self, u32 *buf)
{
    u32 n = self->type->childCount + self->extraChildren;
    u32 step;
    s32 samples;
    struct GaxWorkItem4 item;

    if (n != 1)
        step = (n + 8) << 6;
    else
        step = 0x400;
    {
        u32 f10 = self->mixBuf;
        samples = self->format->frames * self->format->channels;
        item.mixBuf = f10;
    }
    item.buf = buf;
    item.samples = samples;
    item.step = step;
    if (self->type->play(self, (void *)self->mixBuf, self->pos++)) {
        void *arg = &item;
        GAX_CALL_ARM(gGaxPlayerState->downmixCode, arg);
    } else {
        while (samples > 0) {
            *buf++ = 0;
            samples -= 4;
        }
    }
}
