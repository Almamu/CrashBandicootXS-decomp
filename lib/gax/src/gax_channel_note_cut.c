#include "gax_internal.h"
#include "match.h"

/* Called with a small command value (`cmd`, 0-3 seen at the call site in
 * `GaxChannelDecodeRow`, gax_sound_handler_channel_play.c) against a
 * channel. `cmd == 1` is note-off: if the bound instrument's envelope has
 * no sustain point (`sustain == 0xff`), the note is cut outright (the
 * `0x8AD0` "no note" sentinel, lowest voice-steal priority); either way
 * the note is marked released. `cmd > 1` sets the pitch from `cmd` and
 * clears `released` again. */
void GaxChannelSetNote(struct GaxChannelState *self, u32 cmd)
{
    MATCH_HOLD_REG(u32, v, r3) = cmd;

    if (v == 1) {
        struct GaxChannelInstrument *inst = self->instrument;
        if (inst != NULL) {
            struct GaxEnvelope *env = inst->envelope;
            if (env->sustain == 0xff) {
                u16 zero = 0;
                u16 val = 0x8AD0;

                self->note = val;
                self->noteStep = zero;
                self->priority = 0x80000000;
            }
        }
        self->released = 1;
    }
    if (v > 1) {
        MATCH_HOLD_REG(u32, tmp, r0) = v - 2;
        u16 shifted = tmp << 5;
        u8 zero = 0;

        self->pitch = shifted;
        self->released = zero;
    }
}
