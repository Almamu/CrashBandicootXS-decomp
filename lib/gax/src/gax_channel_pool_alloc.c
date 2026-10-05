#include "gax_internal.h"

/* GAX2's fatal-error screen messages. */
extern const char gGaxErrNameJingle[];
extern const char gGaxErrNoJingle[];
extern const char gGaxErrNameInit[];
extern const char gGaxErrOutOfMemory[];
extern void GaxFatalError(const char *a, const char *b);
extern void GaxZeroFill(void *dest, s32 count);
extern u8 GaxCreateHandlers(struct GaxHandlerLayout *layout, struct GaxHandlerType **sfx, u32 numSfx, u8 **bufp,
                      u32 *sizep);

/* GAX2's `GAX2_jingle(song)` (named by its "GAX2_JINGLE" / "GAX_NO_JINGLE
 * FLAG IS SET" error report): plays `layout` as a jingle over the music.
 * Builds player 1 (the jingle player) out of the caller-supplied
 * work buffer (`gGaxPlayerState->workBuf`/`workSize`): refuses (with
 * GAX2's fatal-error screen, if the song asks for it) when the song's
 * flag 0x10 is set; otherwise clears the buffer, carves player 1's
 * handler array plus every handler out of it (`GaxCreateHandlers`), links
 * the song's channels in as the mixer's SFX voices, and runs the
 * mixer's `init`. Returns 1 on success, 0 (again with the optional
 * fatal-error screen) if the buffer is too small.
 *
 * Was NAKED ("r8/sb allocation ceiling"); written plainly against the
 * player/handler structs in gax_internal.h it matches outright - the
 * one subtlety is writing the "0" stores as constants (the ROM reuses
 * the register holding the known-zero flag test for them) - see
 * docs/matching/gax-toolchain-retry.md. */
u32 GAX2_jingle(struct GaxHandlerLayout *layout)
{
    u8 *buf = gGaxPlayerState->workBuf;
    u32 size = gGaxPlayerState->workSize;
    u16 busy = GAX_SONG()->flags & 0x10;
    u32 n;
    struct GaxHandlerType **sfx;
    struct GaxSongHeader *song;

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
    n *= 4;
    if (size < n) {
        gGaxPlayerState->state = 1;
        gGaxPlayerState->curChannelIdx = 0;
    } else {
        buf += n;
        size -= n;
        if (GaxCreateHandlers(layout, sfx, song->numSfx, &buf, &size)) {
            GAX_MIXER()->extraChildren = GAX_SONG()->numSfx;
            GAX_MIXER()->mixBuf = gGaxPlayerState->mixBuf;
            GAX_MIXER()->type->init(GAX_MIXER());
            GAX_INFO()->stopAtEnd = 1;
            GAX_SONG()->songEnded = 0;
            GAX_SONG()->jingleEnded = 0;
            gGaxPlayerState->field_41 = 1;
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
