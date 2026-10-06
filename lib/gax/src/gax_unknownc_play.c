#include "gax_internal.h"

/* MemCopy32 is this ROM's memcpy (reached from the non-constant
 * aggregate initializers below). */
asm(".set memcpy, MemCopy32");

/* This file's functions (issue #68's remainder past
 * `gax_sound_handler_unknownc.c`) finish out the GAX2_SoundHandler
 * "UnknownC" type's function-pointer table: `GaxMixerPlay` is its
 * `play_fn`, and the other three hand a small work item to one of the
 * ARM DSP routines copied into `gGaxPlayerState` (see the raw block at
 * the end of this file) through `GAX_CALL_ARM` (gax_internal.h) - a
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

/* Only the parts of the song/channel/handler objects this file touches. */
struct GaxSongInfo3 {
    u8 pad_00[0x1b];
    u8 field_1b; /* 0x1b */
};

struct GaxSongInfo2 {
    u8 pad_00[0x18];
    struct GaxSongInfo3 *song; /* 0x18 - GaxHandlerType.data.song */
};

struct GaxSongInfo1 {
    u8 pad_00[8];
    struct GaxSongInfo2 *infoType; /* 0x08 - GaxHandlerLayout.types[1], the Info type */
};

struct GaxSong {
    u8 pad_00[0xc];
    u16 flags; /* 0x0c */
    u8 pad_0e[0x30 - 0xe];
    struct GaxSongInfo1 *layout; /* 0x30 - GaxSongHeader.layout */
};

struct GaxVoice {
    u8 pad_00[0x18];
    u8 active; /* 0x18 */
};

struct GaxChanInfo {
    u8 pad_00[0x1f];
    u8 volume; /* 0x1f - GaxInfoHandler.volume */
};

struct GaxChannel {
    u32 field_00;
    struct GaxChanInfo *info; /* 0x04 */
    u32 field_08;
    struct GaxVoice *voices[1]; /* 0x0c - really hdr->childCount long */
};

struct UnknownCFormat {
    u8 pad_00;
    u8 channels; /* 0x01 */
    u8 pad_02[2];
    u16 frames; /* 0x04 */
};

struct UnknownCChild;

struct UnknownCChildOps {
    u32 field_00[2];
    u8 (*play)(struct UnknownCChild *child, u32 *buf, u32 arg); /* 0x08 */
};

struct UnknownCChild {
    struct UnknownCChildOps *ops;
    u8 pad_04[9];
    u8 isFirst; /* 0x0d */
};

struct UnknownCCounts {
    u32 primary; /* 0x00 */
    u32 echo;    /* 0x04 - nonzero: run the echo pass (GAX2_init's fxEcho test too) */
};

struct UnknownCHdr {
    u8 pad_00[8];
    u8 (*step)(void *self, u32 a, u32 b); /* 0x08 */
    u32 childCount;                       /* 0x0c */
    u8 pad_10[8];
    struct UnknownCCounts *counts; /* 0x18 */
};

struct UnknownC {
    struct UnknownCHdr *hdr;         /* 0x00 */
    struct UnknownCFormat *format;   /* 0x04 */
    struct UnknownCChild **children; /* 0x08 */
    u32 pos;                         /* 0x0c */
    u32 mixBuf;                      /* 0x10 - GaxMixerHandler.mixBuf */
    u32 extraChildren;               /* 0x14 */
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
void GaxMixerApplyEcho(struct UnknownC *self, u32 *buf)
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
void GaxMixerApplyFilter(struct UnknownC *self, u32 *buf, u32 clampArg, u32 count)
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
#define UNKNOWNC_CLEAR(self, buf)                                          \
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
#define UNKNOWNC_PLAY_CHILD(self, i, buf, arg2)                                  \
    {                                                                            \
        (self)->children[i]->isFirst = (result == 0);                            \
        result |= (self)->children[i]->ops->play((self)->children[i], buf, arg2); \
    }

/* UnknownC type's `play_fn`: plays the primary children (unless
 * `skipSongChannels` is set), then the extra children either
 * before or after the `GaxMixerApplyEcho`/`GaxMixerApplyFilter` DSP passes depending
 * on `fxEcho`, zero-filling `buf` first whenever nothing
 * has produced output yet. Returns whether anything did. */
u8 GaxMixerPlay(struct UnknownC *self, u32 *buf, u32 arg2)
{
    u8 result = 0;
    u32 i;

    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = 0; i < self->hdr->counts->primary; i++)
            UNKNOWNC_PLAY_CHILD(self, i, buf, arg2);
    }
    if (gGaxPlayerState->fxEcho != 0) {
        if (result == 0) {
            struct GaxSong *song = gGaxPlayerState->songPtr;
            if (song->layout->infoType->song->field_1b != 0 || (song->flags & 0x20))
                UNKNOWNC_CLEAR(self, buf);
        }
        for (i = self->hdr->childCount; i < self->hdr->childCount + self->extraChildren; i++)
            UNKNOWNC_PLAY_CHILD(self, i, buf, arg2);
    }
    if (self->hdr->counts->echo != 0) {
        u8 allIdle;
        struct GaxChannel *chan;

        if (result == 0)
            UNKNOWNC_CLEAR(self, buf);
        allIdle = 1;
        chan = gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx];
        if (chan->info->volume != 0) {
            /* Walks `chan->voices[]` by advancing `chan` itself one
             * pointer at a time - this is what keeps the ROM's
             * `ldr rX, [chan, #0xc]` addressing inside the loop. */
            for (i = 0; i < self->hdr->childCount && allIdle; i++) {
                if (chan->voices[0]->active != 0)
                    allIdle = 0;
                chan = (struct GaxChannel *)((struct GaxVoice **)chan + 1);
            }
        }
        if (allIdle == 0)
            GaxMixerApplyEcho(self, buf);
    }
    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = self->hdr->counts->primary; i < self->hdr->childCount; i++)
            UNKNOWNC_PLAY_CHILD(self, i, buf, arg2);
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
            struct GaxSong *song = gGaxPlayerState->songPtr;
            if (song->layout->infoType->song->field_1b != 0 || (song->flags & 0x20))
                UNKNOWNC_CLEAR(self, buf);
        }
        for (i = self->hdr->childCount; i < self->hdr->childCount + self->extraChildren; i++)
            UNKNOWNC_PLAY_CHILD(self, i, buf, arg2);
    }
    gGaxPlayerState->skipSongChannels = 0;
    return result;
}

/* Advances the mixer's position through its type's `play` (GaxMixerPlay,
 * into `mixBuf`); if that produced a block, hands a 4-word work item to
 * `downmixCode` (the IWRAM copy of gGaxArmDownmix), otherwise zero-fills `buf` for
 * the block's length. Called by GAX_play with the mixer as a `struct
 * GaxMixerHandler`; the rest of this file still reads it through its
 * own `struct UnknownC` view (`hdr` is `type`, `mixBuf` is
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

/* Immediately past this file's code (0x0803A628 onward): raw ARM-mode
 * (not Thumb) DSP/mixer routines docs/audio.md documents
 * (`gGaxArmDownmix` onward, preceded by an 8-byte unlabeled
 * lead-in) - not disassembled as real code at all yet, so this is kept
 * as an untouched raw byte transcription rather than guessed-at ARM
 * instructions (hand-written ARM asm in the original, like the other
 * GAX2 mixer routines - the "FILT"/"BART" tags between them aren't
 * compiler output); folded into this translation unit rather than a
 * separate object purely so this whole issue #68 chunk's
 * report_units.py boundary lands on an address the frozen
 * expected/code_3.s disassembly actually labels (this raw span's own
 * start has no such label). The code copies of these at
 * `downmixCode`/`echoCode`/`filterCode` are what `GAX_CALL_ARM` enters.
 *
 * In ROM order: `gGaxArmDownmix` (16-bit mix to 8-bit output),
 * `gGaxArmFilter` (the optional low-pass filter, see
 * GaxMixerApplyFilter; two state words, then "FILT"), `gGaxArmEcho` (two
 * state words, then "BART") and `gGaxArmResample`. GaxChannelMix patches
 * four instructions of the resampler's copy: the sample-position step
 * (`add`/`sub r2, r2, r8`, forward or backward playback) and the loop's
 * end test (`blt`/`bgt`) of its store loop (the first channel to mix,
 * `gGaxArmResampleStoreStep`/`StoreEndTest`) and of its add loop (the
 * channels after it, `gGaxArmResampleMixStep`/`MixEndTest`). */
// clang-format off
asm(
        ".align 2, 0\n\t"
        "_0803A628:\n\t"
        ".byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00\n\t"
        ".global gGaxArmDownmix\n\t"
        "gGaxArmDownmix:\n\t"
        ".byte 0x60, 0x00, 0x2D, 0xE9, 0x0C, 0x50, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5\n\t"
        ".byte 0x00, 0x00, 0x90, 0xE5, 0x01, 0x20, 0x82, 0xE0, 0xF2, 0x60, 0xD0, 0xE0, 0x95, 0x06, 0x06, 0xE0\n\t"
        ".byte 0x46, 0x65, 0xA0, 0xE1, 0x80, 0x00, 0x76, 0xE3, 0x7F, 0x60, 0xE0, 0xB3, 0x7F, 0x00, 0x56, 0xE3\n\t"
        ".byte 0x7F, 0x60, 0xA0, 0xC3, 0x01, 0x60, 0xC1, 0xE4, 0x02, 0x00, 0x51, 0xE1, 0xF5, 0xFF, 0xFF, 0xBA\n\t"
        ".byte 0x60, 0x00, 0xBD, 0xE8, 0x0E, 0x00, 0xA0, 0xE1, 0x10, 0xFF, 0x2F, 0xE1\n\t"
        ".global gGaxArmFilter\n\t"
        "gGaxArmFilter:\n\t"
        ".byte 0xF0, 0x0F, 0x2D, 0xE9, 0x0C, 0x10, 0x90, 0xE5, 0x08, 0x70, 0x90, 0xE5, 0x04, 0x20, 0x90, 0xE5\n\t"
        ".byte 0x00, 0x00, 0x90, 0xE5, 0x98, 0x40, 0x8F, 0xE2, 0x04, 0x50, 0x94, 0xE5, 0x00, 0x40, 0x94, 0xE5\n\t"
        ".byte 0xCD, 0x8F, 0xA0, 0xE3, 0x98, 0x07, 0x07, 0xE0, 0x47, 0x74, 0xA0, 0xE1, 0x7F, 0x90, 0xA0, 0xE3\n\t"
        ".byte 0x09, 0x94, 0xA0, 0xE1, 0x91, 0x09, 0x09, 0xE0, 0xF0, 0x80, 0xD0, 0xE1, 0x08, 0x84, 0xA0, 0xE1\n\t"
        ".byte 0x05, 0x60, 0x48, 0xE0, 0x04, 0x60, 0x46, 0xE0, 0x09, 0x00, 0x76, 0xE1, 0x09, 0x60, 0xE0, 0xB1\n\t"
        ".byte 0x09, 0x00, 0x56, 0xE1, 0x09, 0x60, 0xA0, 0xC1, 0x96, 0x07, 0x0A, 0xE0, 0x4A, 0x54, 0x85, 0xE0\n\t"
        ".byte 0x09, 0x00, 0x75, 0xE1, 0x09, 0x50, 0xE0, 0xB1, 0x09, 0x00, 0x55, 0xE1, 0x09, 0x50, 0xA0, 0xC1\n\t"
        ".byte 0x95, 0x07, 0x0A, 0xE0, 0x4A, 0x44, 0x84, 0xE0, 0x09, 0x00, 0x74, 0xE1, 0x09, 0x40, 0xE0, 0xB1\n\t"
        ".byte 0x09, 0x00, 0x54, 0xE1, 0x09, 0x40, 0xA0, 0xC1, 0x44, 0xA4, 0xA0, 0xE1, 0xB0, 0xA0, 0xC0, 0xE1\n\t"
        ".byte 0x02, 0x00, 0x80, 0xE2, 0x02, 0x20, 0x52, 0xE2, 0x00, 0x00, 0x52, 0xE3, 0xE5, 0xFF, 0xFF, 0xCA\n\t"
        ".byte 0x0C, 0x00, 0x8F, 0xE2, 0x00, 0x40, 0x80, 0xE5, 0x04, 0x50, 0x80, 0xE5, 0xF0, 0x0F, 0xBD, 0xE8\n\t"
        ".byte 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x49, 0x4C, 0x54\n\t"
        ".global gGaxArmEcho\n\t"
        "gGaxArmEcho:\n\t"
        ".byte 0xF0, 0x01, 0x2D, 0xE9\n\t"
        ".byte 0xC8, 0x50, 0x9F, 0xE5, 0x00, 0x40, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5, 0x10, 0x20, 0x90, 0xE5\n\t"
        ".byte 0x08, 0x10, 0x90, 0xE5, 0x04, 0x00, 0x90, 0xE5, 0x00, 0x40, 0x84, 0xE0, 0x03, 0x00, 0x55, 0xE1\n\t"
        ".byte 0x00, 0x50, 0xA0, 0x03, 0x03, 0x70, 0x85, 0xE0, 0x00, 0x80, 0x92, 0xE5, 0x08, 0x70, 0x47, 0xE0\n\t"
        ".byte 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x04, 0x80, 0x92, 0xE5\n\t"
        ".byte 0x98, 0x07, 0x17, 0xE0, 0x47, 0x62, 0xA0, 0xE1, 0x08, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3\n\t"
        ".byte 0x11, 0x00, 0x00, 0x0A, 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1\n\t"
        ".byte 0x03, 0x70, 0x47, 0xA0, 0xF7, 0x70, 0x91, 0xE1, 0x0C, 0x80, 0x92, 0xE5, 0x98, 0x07, 0x17, 0xE0\n\t"
        ".byte 0x47, 0x62, 0x86, 0xE0, 0x10, 0x80, 0x92, 0xE5, 0x00, 0x00, 0x58, 0xE3, 0x06, 0x00, 0x00, 0x0A\n\t"
        ".byte 0x03, 0x70, 0x85, 0xE0, 0x08, 0x70, 0x47, 0xE0, 0x03, 0x00, 0x57, 0xE1, 0x03, 0x70, 0x47, 0xA0\n\t"
        ".byte 0xF7, 0x70, 0x91, 0xE1, 0x14, 0x80, 0x92, 0xE5, 0x47, 0x62, 0x86, 0xE0, 0xF0, 0x80, 0xD0, 0xE1\n\t"
        ".byte 0x06, 0x80, 0x88, 0xE0, 0xB0, 0x80, 0xC0, 0xE1, 0xB5, 0x80, 0x81, 0xE1, 0x02, 0x00, 0x80, 0xE2\n\t"
        ".byte 0x02, 0x50, 0x85, 0xE2, 0x04, 0x00, 0x50, 0xE1, 0xD7, 0xFF, 0xFF, 0xBA, 0x0C, 0x00, 0x8F, 0xE2\n\t"
        ".byte 0x00, 0x50, 0x80, 0xE5, 0xF0, 0x01, 0xBD, 0xE8, 0x1E, 0xFF, 0x2F, 0xE1, 0x00, 0x00, 0x00, 0x00\n\t"
        ".byte 0x00, 0x00, 0x00, 0x00, 0x42, 0x41, 0x52, 0x54\n\t"
        ".global gGaxArmResample\n\t"
        "gGaxArmResample:\n\t"
        ".byte 0xF1, 0x0F, 0x2D, 0xE9, 0x18, 0x70, 0x90, 0xE5\n\t"
        ".byte 0x1C, 0x80, 0x90, 0xE5, 0x04, 0x10, 0x90, 0xE5, 0x08, 0x20, 0x90, 0xE5, 0x0C, 0x30, 0x90, 0xE5\n\t"
        ".byte 0x10, 0x40, 0x90, 0xE5, 0x84, 0x40, 0x81, 0xE0, 0x14, 0x50, 0x90, 0xE5, 0x85, 0x10, 0x81, 0xE0\n\t"
        ".byte 0x20, 0xA0, 0x90, 0xE5, 0x24, 0xB0, 0x90, 0xE5, 0x00, 0x00, 0x90, 0xE5, 0x0A, 0xA1, 0x9F, 0xE7\n\t"
        ".byte 0x0A, 0xF0, 0x8F, 0xE0, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00\n\t"
        ".byte 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0, 0x46, 0x64, 0xA0, 0xE1\n\t"
        ".byte 0xB2, 0x60, 0xC1, 0xE0\n\t"
        ".global gGaxArmResampleStoreStep\n\t"
        "gGaxArmResampleStoreStep:\n\t"
        ".byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x14, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1\n\t"
        ".global gGaxArmResampleStoreEndTest\n\t"
        "gGaxArmResampleStoreEndTest:\n\t"
        ".byte 0xF5, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x10, 0x00, 0x00, 0x0A, 0x0B, 0x20, 0x42, 0xE0\n\t"
        ".byte 0xF1, 0xFF, 0xFF, 0xEA, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0\n\t"
        ".byte 0x46, 0x64, 0xA0, 0xE1, 0xF0, 0x90, 0xD1, 0xE1, 0x09, 0x60, 0x86, 0xE0, 0xB2, 0x60, 0xC1, 0xE0\n\t"
        ".global gGaxArmResampleMixStep\n\t"
        "gGaxArmResampleMixStep:\n\t"
        ".byte 0x08, 0x20, 0x82, 0xE0, 0x04, 0x00, 0x51, 0xE1, 0x04, 0x00, 0x00, 0xAA, 0x03, 0x00, 0x52, 0xE1\n\t"
        ".global gGaxArmResampleMixEndTest\n\t"
        "gGaxArmResampleMixEndTest:\n\t"
        ".byte 0xF3, 0xFF, 0xFF, 0xBA, 0x00, 0x00, 0x5B, 0xE3, 0x0B, 0x20, 0x42, 0x10, 0xF0, 0xFF, 0xFF, 0x1A\n\t"
        ".byte 0xF1, 0x0F, 0xBD, 0xE8, 0x04, 0x30, 0x90, 0xE5, 0x03, 0x30, 0x41, 0xE0, 0xA3, 0x30, 0xA0, 0xE1\n\t"
        ".byte 0x08, 0x20, 0x80, 0xE5, 0x14, 0x30, 0x80, 0xE5, 0x0E, 0x00, 0xA0, 0xE1\n\t"
        ".byte 0x10, 0xFF, 0x2F, 0xE1, 0xC2, 0x65, 0xA0, 0xE1, 0xD6, 0x60, 0x90, 0xE1, 0x97, 0x06, 0x06, 0xE0\n\t"
        ".byte 0x46, 0x64, 0xA0, 0xE1, 0x06, 0x68, 0xA0, 0xE1, 0x26, 0x68, 0x86, 0xE1, 0x00, 0x90, 0x91, 0xE5\n\t"
        ".byte 0x09, 0x90, 0x86, 0xE0, 0x04, 0x90, 0x81, 0xE4, 0x88, 0x20, 0x82, 0xE0, 0x03, 0x00, 0x52, 0xE1\n\t"
        ".byte 0x04, 0x00, 0x51, 0xB1, 0xF2, 0xFF, 0xFF, 0xBA, 0x03, 0x00, 0x52, 0xE1, 0x01, 0x00, 0x00, 0xCA\n\t"
        ".byte 0x04, 0x00, 0x51, 0xE1, 0xE6, 0xFF, 0xFF, 0xDA, 0x02, 0x10, 0x41, 0xE2, 0x08, 0x20, 0x42, 0xE0\n\t"
        ".byte 0xE3, 0xFF, 0xFF, 0xEA\n\t"
);
// clang-format on
