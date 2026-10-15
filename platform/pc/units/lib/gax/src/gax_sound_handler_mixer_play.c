/* The pc platform's lib/gax/src/gax_sound_handler_mixer_play.c
 * (platform/pc/platform.mk, "Units"): the mixer type's play_fn and the
 * three ARM calls next to it, as the original's code, with two
 * differences:
 * - the original reads the handlers through its own views (struct
 *   GaxMixerView and friends, with u32 fields and padding where the
 *   pointers are), the GBA's layout; this reads them as gax_internal.h's
 *   structures, which the host lays out;
 * - the downmix, echo and filter are C calls of gax_dsp.c's routines
 *   with the work items' fields, where the original calls the ARM
 *   routines' IWRAM copies through GAX_CALL_ARM.
 * GaxMixFrame then hands the 8-bit block to the platform's audio output
 * (audio.c), which on the GBA DMA1 plays to Direct Sound A. */

#include "gax_host.h"

/* The mixer type's data (GaxHandlerType.data.dsp): the song's echo
 * settings. */
struct GaxMixerEchoParams {
    u32 primary; /* 0x00 - channels played before the echo pass */
    u32 echo;    /* 0x04 - nonzero: run the echo pass (GAX2_init's fxEcho test too) */
};

#define MIXER_ECHO(self) ((const struct GaxMixerEchoParams *)(self)->type->data.dsp)

void GaxMixerApplyEcho(struct GaxMixerView *view, u32 *buf)
{
    struct GaxMixerHandler *self = (struct GaxMixerHandler *)view;
    u32 bytes = self->format->frames * self->format->channels * 2;
    struct GaxPlayerState *st = gGaxPlayerState;

    HostGaxEcho(bytes, (s16 *)buf, (s16 *)st->echoBuf, st->echoLen, (const u32 *)st->echoTaps);
}

void GaxMixerApplyFilter(struct GaxMixerView *view, u32 *buf, u32 clampArg, u32 count)
{
    struct GaxMixerHandler *self = (struct GaxMixerHandler *)view;
    u32 bytes = self->format->frames * self->format->channels * 2;

    HostGaxFilter((s16 *)buf, bytes, 0x55 - clampArg, count);
}

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

#define CHILD(self, i) ((struct GaxChannelState *)(self)->children[i])

#define GAX_MIXER_PLAY_CHILD(self, i, buf, arg2)                                    \
    {                                                                              \
        CHILD(self, i)->isFirst = (result == 0);                                   \
        result |= CHILD(self, i)->type->play(CHILD(self, i), buf, arg2);           \
    }

u8 GaxMixerPlay(struct GaxMixerView *view, u32 *buf, u32 arg2)
{
    struct GaxMixerHandler *self = (struct GaxMixerHandler *)view;
    u8 result = 0;
    u32 i;

    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = 0; i < MIXER_ECHO(self)->primary; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    if (gGaxPlayerState->fxEcho != 0) {
        if (result == 0) {
            struct GaxSongHeader *song = gGaxPlayerState->songPtr;
            if (song->layout->types[1]->data.song->halfRateFx != 0 || (song->flags & 0x20))
                GAX_MIXER_CLEAR(self, buf);
        }
        for (i = self->type->childCount; i < self->type->childCount + self->extraChildren; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    /* The echo pass is skipped while the song's master volume or every
     * channel's volume is 0. */
    if (MIXER_ECHO(self)->echo != 0) {
        u8 allMuted;
        struct GaxHandler **handlers;

        if (result == 0)
            GAX_MIXER_CLEAR(self, buf);
        allMuted = 1;
        handlers = GAX_PLAYER();
        if (((struct GaxInfoHandler *)handlers[1])->volume != 0) {
            for (i = 0; i < self->type->childCount && allMuted; i++) {
                if ((u8)((struct GaxChannelState *)handlers[3 + i])->volume != 0)
                    allMuted = 0;
            }
        }
        if (allMuted == 0)
            GaxMixerApplyEcho(view, buf);
    }
    if (gGaxPlayerState->skipSongChannels == 0) {
        for (i = MIXER_ECHO(self)->primary; i < self->type->childCount; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    {
        u32 clamp = gGaxPlayerState->filter;
        if (clamp > 0x55)
            clamp = 0x55;
        gGaxPlayerState->filter = clamp;
        if (gGaxPlayerState->filterCode != NULL && clamp != 0)
            GaxMixerApplyFilter(view, buf, clamp, self->type->childCount);
    }
    if (gGaxPlayerState->fxEcho == 0) {
        if (result == 0) {
            struct GaxSongHeader *song = gGaxPlayerState->songPtr;
            if (song->layout->types[1]->data.song->halfRateFx != 0 || (song->flags & 0x20))
                GAX_MIXER_CLEAR(self, buf);
        }
        for (i = self->type->childCount; i < self->type->childCount + self->extraChildren; i++)
            GAX_MIXER_PLAY_CHILD(self, i, buf, arg2);
    }
    gGaxPlayerState->skipSongChannels = 0;
    return result;
}

void GaxMixFrame(struct GaxMixerHandler *self, u32 *buf)
{
    u32 n = self->type->childCount + self->extraChildren;
    u32 step;
    s32 samples;

    if (n != 1)
        step = (n + 8) << 6;
    else
        step = 0x400;
    samples = self->format->frames * self->format->channels;
    if (self->type->play(self, (void *)self->mixBuf, self->pos++)) {
        HostGaxDownmix((const s16 *)self->mixBuf, (s8 *)buf, samples, step);
    } else {
        s32 left = samples;
        u32 *p = buf;

        while (left > 0) {
            *p++ = 0;
            left -= 4;
        }
    }
    HostAudioMixed((const s8 *)buf, samples, gGaxPlayerState->format->mixRate);
}
