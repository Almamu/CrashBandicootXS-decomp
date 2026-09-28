#include "core.h"
#include "audio.h"

extern void sub_8039818(struct GaxChannelState *self, u32 note);
extern void sub_803985C(struct GaxChannelState *self, struct GaxInfoHandler *info, u32 instrument,
                        struct GaxSongData *song);
extern void sub_80398DC(struct GaxChannelState *self, struct GaxInfoHandler *info);
extern void sub_8039AA4(struct GaxChannelState *self, struct GaxInfoHandler *info);
extern u32 sub_8039B44(struct GaxChannelState *self, struct GaxInfoHandler *info, void *buf, u32 arg,
                       struct GaxSongData *song, u32 flag);

/* GAX2 per-channel play routine for directly-triggered notes (issue
 * #68) - the sibling of the Channel type's play_fn `sub_80395A4`
 * (gax_sound_handler_channel_play.c) for a channel that isn't fed by a
 * pattern: instead of decoding a pattern row it starts the note/
 * instrument queued in `field_24`/`field_25` (note 1 alone, or any note
 * with an instrument) on a new row, then runs the same note-cut
 * countdown / envelope tick / mix (`sub_8039B44` with its own type data
 * and `flag` 1) tail.
 *
 * Was NAKED ("r8/sb/sl allocation ceiling"); written plainly against
 * the handler structs in include/audio.h it matches outright - see
 * docs/matching/gax-toolchain-retry.md. */
u8 sub_803A158(struct GaxChannelState *self, void *buf, u32 arg)
{
    struct GaxInfoHandler *info = (struct GaxInfoHandler *)self->children[0];

    if (info->field_1b != 0)
        self->instrument = NULL;
    if (info->field_1a != 0) {
        if ((self->field_24 == 1 || (self->field_24 != 0 && self->field_25 != 0)) && info->field_1b == 0) {
            sub_8039818(self, self->field_24);
            sub_803985C(self, info, self->field_25, self->type->data.song);
            self->volStep15 = 0;
            self->pitchStep = 0;
            self->field_25 = 0;
            self->field_24 = 0;
        }
    }
    if (self->instrument != NULL && self->cutTimer == 0) {
        if (self->cutDelay != 0) {
            sub_80398DC(self, info);
            self->cutTimer = self->cutDelay - 1;
        }
    } else {
        self->cutTimer--;
    }
    sub_8039AA4(self, info);
    return self->field_0c == 0 ? (u8)sub_8039B44(self, info, buf, arg, self->type->data.song, 1) : 0;
}
asm(".align 2, 0");
