#include "gax_internal.h"

/* GAX2 per-channel play routine for directly-triggered notes (issue
 * #68) - the sibling of the Channel type's play_fn `GaxChannelPlay`
 * (gax_sound_handler_channel_play.c) for a channel that isn't fed by a
 * pattern: instead of decoding a pattern row it starts the note/
 * instrument queued in `pendingNote`/`pendingInstrument` (note 1 alone, or any note
 * with an instrument) on a new row, then runs the same note-cut
 * countdown / envelope tick / mix (`GaxChannelMix` with its own type data
 * and `flag` 1) tail.
 *
 * Was NAKED ("r8/sb/sl allocation ceiling"); written plainly against
 * the handler structs in gax_internal.h it matches outright - see
 * docs/matching/archive/gax-toolchain-retry.md. */
u8 GaxFxChannelPlay(struct GaxChannelState *self, void *buf, u32 arg)
{
    struct GaxInfoHandler *info = (struct GaxInfoHandler *)self->children[0];

    if (info->field_1b != 0)
        self->instrument = NULL;
    if (info->playing != 0) {
        if ((self->pendingNote == 1 || (self->pendingNote != 0 && self->pendingInstrument != 0)) &&
            info->field_1b == 0) {
            GaxChannelSetNote(self, self->pendingNote);
            GaxChannelSetInstrument(self, info, self->pendingInstrument, self->type->data.song);
            self->volStep15 = 0;
            self->pitchStep = 0;
            self->pendingInstrument = 0;
            self->pendingNote = 0;
        }
    }
    if (self->instrument != NULL && self->cutTimer == 0) {
        if (self->cutDelay != 0) {
            GaxChannelStepInstrumentSeq(self, info);
            self->cutTimer = self->cutDelay - 1;
        }
    } else {
        self->cutTimer--;
    }
    GaxChannelTick(self, info);
    return self->field_0c == 0 ? (u8)GaxChannelMix(self, info, buf, arg, self->type->data.song, 1)
                               : 0;
}
asm(".align 2, 0");
