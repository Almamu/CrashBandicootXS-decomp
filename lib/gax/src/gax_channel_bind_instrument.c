#include "gax_internal.h"

/* Binds instrument `cmd` of the song's `instruments[]` table to a channel
 * and resets the channel's per-note state (envelope, vibrato, sequence,
 * portamento); an `empty` placeholder instrument leaves the channel
 * unbound. If an instrument is bound, `cmd` is also recorded in the song
 * header's `scratch` table at the channel's `index` (4 bytes per
 * channel).
 *
 * Plain C: the ROM's `self` in ip (re-read with `mov rX, ip` before
 * each group of stores) is agbcc's own allocation here. */
void GaxChannelSetInstrument(struct GaxChannelState *self, struct GaxInfoHandler *info, u32 cmd,
                             struct GaxSongData *table)
{
    if (cmd != 0) {
        self->instrument = table->instruments[cmd];
        self->envPos = 0;
        self->released = 0;
        self->vibratoPhase = 0;
        self->vibratoDelay = self->instrument->vibratoDelay;
        self->seqPos = 0;
        self->cutTimer = 0;
        self->seqLoopCount = 0;
        self->vol15 = 0xff;
        self->cutDelay = self->instrument->seqSpeed;
        self->slideRate = 0;
        self->slideTarget = 0;
        if (self->instrument->empty != 0)
            self->instrument = NULL;
        if (self->instrument != NULL) {
            u8 *scratch = GAX_SONG()->scratch;

            if (scratch != NULL)
                scratch[self->index * 4] = cmd;
        }
    }
}
