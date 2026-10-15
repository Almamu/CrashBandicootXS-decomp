/* The pc platform's lib/gax/src/gax_channel_pool_alloc.c (platform/pc/
 * platform.mk, "Units"): GAX2_jingle as the original, with the host's
 * two differences from the GBA's GAX2_init (gax_playstart.c): the jingle's
 * layout is the converted one (a GBA address in the music block gives its
 * host structures), and the handler array is carved by the host's pointer
 * size, with its slot 2 the handler of no type. (The game plays no
 * jingle.) */

#include "gax_host.h"

u32 GAX2_jingle(struct GaxHandlerLayout *layout)
{
    u8 *buf = gGaxPlayerState->workBuf;
    u32 size = gGaxPlayerState->workSize;
    u16 busy = GAX_SONG()->flags & 0x10;
    u32 n;
    struct GaxHandlerType **sfx;
    struct GaxSongHeader *song;

    layout = HostGaxLayout(layout);
    if (busy) {
        if (GAX_SONG()->showErrors)
            GaxFatalError(gGaxErrNameJingle, gGaxErrNoJingle);
        return 0;
    }
    GaxZeroFill(buf, size);
    gGaxPlayerState->state = 0;
    gGaxPlayerState->curChannelIdx = 1;
    n = layout->count;
    song = GAX_SONG();
    sfx = song->sfxTypes;
    if (sfx)
        n += song->numSfx;
    gGaxPlayerState->channels[1] = buf;
    n *= sizeof(struct GaxHandler *);
    if (size < n) {
        gGaxPlayerState->state = 1;
        gGaxPlayerState->curChannelIdx = 0;
    } else {
        ((struct GaxHandler **)buf)[2] = &gHostGaxNoHandler;
        buf += n;
        size -= n;
        if (GaxCreateHandlers(layout, sfx, song->numSfx, &buf, &size)) {
            GAX_MIXER()->extraChildren = GAX_SONG()->numSfx;
            GAX_MIXER()->mixBuf = gGaxPlayerState->mixBuf;
            GAX_MIXER()->type->init(GAX_MIXER());
            GAX_INFO()->stopAtEnd = 1;
            GAX_SONG()->songEnded = 0;
            GAX_SONG()->jingleEnded = 0;
            gGaxPlayerState->skipSongChannels = 1;
            gGaxPlayerState->state = 1;
            return 1;
        }
        gGaxPlayerState->state = 1;
        gGaxPlayerState->curChannelIdx = 0;
    }
    if (GAX_SONG()->showErrors)
        GaxFatalError(gGaxErrNameInit, gGaxErrOutOfMemory);
    return 0;
}
