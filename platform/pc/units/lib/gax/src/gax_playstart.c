/* The pc platform's lib/gax/src/gax_playstart.c (platform/pc/platform.mk,
 * "Units"): GAX2_init, which builds the player in its work RAM, as the
 * original does it, with three differences:
 * - the song: the game hands it the GBA layout's address in the music
 *   block (gSongTable's entry) and the block itself as the sound-effect
 *   voices' types; it plays the host's converted structures for them
 *   (gHostGaxLayouts, gax_host_data.c, platform/pc/tools/
 *   gax_host_data.py), and keeps them in the parameter block, which the
 *   engine reads its song from from then on;
 * - the work RAM: the original carves the player state, the handler
 *   array and the handlers out of the caller's buffer by the GBA's sizes
 *   (0x18c, 4-byte pointers; AudioContext's 0x2000 bytes); on a 64-bit
 *   host the structures are larger, so it carves them by the host's sizes
 *   (sizeof) out of a buffer of its own, aligned for 8-byte pointers;
 * - the ARM routines: there is no code to copy into the player state
 *   (gax_dsp.c is C); the echo and filter state they keep in their copies
 *   is reset instead. mixCode and filterCode stay what the engine tests:
 *   non-NULL when the full resampler or the filter is in use.
 * The rest is the original's order and arithmetic. */

#include "gax_host.h"

extern const u8 gGaxMusicData[];

#define HOST_GAX_WORK 0x10000

static u8 sWork[HOST_GAX_WORK] __attribute__((aligned(16)));

/* What mixCode and filterCode point at: no code (gax_dsp.c runs it). */
static u32 sNoCode[1];

/* The handler array's slot 2, which holds no handler (the layout's slot 2
 * is its list of alternative layouts): GaxCreateHandlers skips it when it
 * carves the handlers, but its search for a handler by type reads every
 * slot's `type`, this one's too. On the GBA the slot holds whatever the
 * work RAM did, and the read finds no match; here it is a handler of no
 * type. */
struct GaxHandler gHostGaxNoHandler;

#define ALIGN8(buf, size)                                            \
    {                                                                \
        u32 pad_ = (u32)((((uintptr_t)(buf) + 7) & ~(uintptr_t)7) - \
                         (uintptr_t)(buf));                          \
        (buf) += pad_;                                               \
        (size) -= pad_;                                              \
    }

/* The converted structures for an address in the music block, or NULL. */
static const struct HostGaxLayout *HostLayout(const void *gba)
{
    u32 i;

    for (i = 0; i < gHostGaxLayoutCount; i++) {
        if ((const u8 *)gba == gGaxMusicData + gHostGaxLayouts[i].offset) {
            return &gHostGaxLayouts[i];
        }
    }
    return NULL;
}

struct GaxHandlerLayout *HostGaxLayout(struct GaxHandlerLayout *layout)
{
    const struct HostGaxLayout *h = HostLayout(layout);

    return h != NULL && h->layout != NULL ? h->layout : layout;
}

static void HostConvertSong(struct GaxSongHeader *p)
{
    const struct HostGaxLayout *h;

    if (p->layout != NULL) {
        p->layout = HostGaxLayout(p->layout);
    }
    if (p->sfxTypes != NULL) {
        h = HostLayout(p->sfxTypes);
        if (h != NULL && h->types != NULL) {
            p->sfxTypes = h->types;
        }
    }
}

u8 GAX2_init(struct GaxSongHeader *p)
{
    struct GaxChannelFormat *fmt;
    u32 maxRate = 0;
    u8 *buf = sWork;
    u32 size = sizeof(sWork);
    u32 n;
    u32 i;
    s32 idx;
    struct GaxHandlerLayout *layout;

    HostConvertSong(p);
    gGaxPlayerState = (struct GaxPlayerState *)buf;
    buf += sizeof(struct GaxPlayerState);
    size -= sizeof(struct GaxPlayerState);
    ALIGN8(buf, size);
    if (p->layout == NULL)
        p->layout = &gGaxDefaultSong;
    if (p->sfxTypes == NULL)
        p->numSfx = 0;
    if (p->mixRate == 0xffff)
        p->mixRate = p->layout->types[1]->data.song->mixRate;
    if (p->numSfx == 0xffff)
        p->numSfx = p->layout->types[1]->data.song->numSfx;
    if (p->volume == 0xffff)
        p->volume = 0xff;
    gGaxPlayerState->magic = 0x47415832;
    gGaxPlayerState->songPtr = p;
    gGaxPlayerState->state = 0;
    gGaxPlayerState->curChannelIdx = 0;
    gGaxPlayerState->echoTaps = 0;
    gGaxPlayerState->skipSongChannels = 0;
    gGaxPlayerState->playDone = 1;
    n = p->layout->count;
    if (p->sfxTypes != NULL)
        n += p->numSfx;
    gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx] = buf;
    ((struct GaxHandler **)buf)[2] = &gHostGaxNoHandler;
    size -= n * sizeof(struct GaxHandler *);
    fmt = (struct GaxChannelFormat *)(buf + n * sizeof(struct GaxHandler *));
    buf = (u8 *)fmt + sizeof(*fmt);
    size -= sizeof(*fmt);
    ALIGN8(buf, size);
    gGaxPlayerState->format = fmt;
    idx = GaxFindMixRate(p->mixRate);
    fmt->bits = 8;
    fmt->channels = 1;
    fmt->mixRate = gGaxMixRates[idx].rate;
    fmt->frames = fmt->mixRate * 1000 / 0xe94f;
    gGaxPlayerState->timerReload = gGaxMixRates[idx].timer;
    gGaxPlayerState->fxEcho = 0;
    if (gGaxVersionStringPtr[2] != 'X' || gGaxVersionStringPtr[1] != 'A' ||
        gGaxVersionStringPtr[0] != 'G')
        gGaxPlayerState->timerReload <<= 1;
    if (size < (u32)(fmt->frames + 4) * 2)
        goto fail;
    gGaxPlayerState->mixBuf = (uintptr_t)buf;
    buf += (fmt->frames + 4) * 2;
    size -= (fmt->frames + 4) * 2;
    ALIGN8(buf, size);
    gGaxPlayerState->outHalf = 0;
    if (size < (u32)fmt->frames * 2)
        return 0;
    gGaxPlayerState->outBuf = (s8 *)buf;
    buf += fmt->frames * 2;
    size -= fmt->frames * 2;
    GaxZeroFill(gGaxPlayerState->outBuf, fmt->frames * 2);
    ALIGN8(buf, size);
    layout = p->layout;
    {
        struct GaxDspTap *tap = layout->types[0]->data.dsp->taps;

        for (i = 0; i <= 2; i++, tap++) {
            if (tap->rate > maxRate)
                maxRate = tap->rate;
        }
    }
    if (!(p->flags & 0x10) && layout->types[2] != NULL) {
        struct GaxLayoutList *list = (struct GaxLayoutList *)layout->types[2];
        s32 j;

        for (j = 0; j < (s32)list->count; j++) {
            struct GaxDspTap *tap = list->layouts[j]->types[0]->data.dsp->taps;

            for (i = 0; i <= 2; i++, tap++) {
                if (tap->rate > maxRate)
                    maxRate = tap->rate;
            }
        }
    }
    if (maxRate != 0) {
        u32 len;

        if (size <= 23)
            goto fail;
        gGaxPlayerState->echoTaps = (uintptr_t)buf;
        buf += 24;
        size -= 24;
        len = maxRate * fmt->mixRate / 1000 * 2;
        if (size < len)
            goto fail;
        gGaxPlayerState->echoBuf = (uintptr_t)buf;
        gGaxPlayerState->echoLen = len;
        GaxZeroFill(buf, len);
        buf += len;
        size -= len;
        ALIGN8(buf, size);
    }
    /* The ARM routines' copies: their state, and the two pointers the
     * engine tests. */
    HostGaxDspReset();
    if (p->layout->types[1]->data.song->halfRateFx != 0 || (u16)(p->flags & 0x20))
        gGaxPlayerState->fullResampler = 1;
    else
        gGaxPlayerState->fullResampler = 0;
    gGaxPlayerState->mixCode = sNoCode;
    if ((u16)(p->flags & 4))
        gGaxPlayerState->filterCode = sNoCode;
    else
        gGaxPlayerState->filterCode = NULL;
    gGaxPlayerState->filter = 0;
    if (!GaxCreateHandlers(p->layout, p->sfxTypes, p->numSfx, &buf, &size))
        goto fail;
    ALIGN8(buf, size);
    GAX_MIXER()->extraChildren = p->numSfx;
    GAX_MIXER()->mixBuf = gGaxPlayerState->mixBuf;
    GAX_MIXER()->type->init(GAX_MIXER());
    gGaxPlayerState->workBuf = buf;
    gGaxPlayerState->workSize = size;
    GaxResetSoundHardware();
    gGaxPlayerState->state = 1;
    GAX_INFO()->stopAtEnd = (p->flags >> 3) & 1;
    GAX_SONG()->songEnded = 0;
    GAX_SONG()->jingleEnded = 0;
    if ((u16)(p->flags & 2) && ((u32 *)GAX_MIXER()->type->data.dsp)[1] != 0)
        gGaxPlayerState->fxEcho = 1;
    else
        gGaxPlayerState->fxEcho = 0;
    return 1;
fail:
    if (p->showErrors)
        GaxFatalError(gGaxErrNameInit, gGaxErrOutOfMemory);
    return 0;
}
