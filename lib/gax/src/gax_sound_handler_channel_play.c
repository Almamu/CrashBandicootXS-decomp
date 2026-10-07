#include "gax_internal.h"

void GaxChannelDecodeRow(struct GaxChannelState *self, struct GaxInfoHandler *info, u8 retrigger);

/* GAX2_SoundHandler "Channel" type's play_fn (ROM 0x080395A5, see
 * docs/audio.md's per-type function-pointer table). Runs the shared
 * Info handler (`children[0]`) first, then - on a new row - decodes
 * this channel's next pattern row (`GaxChannelDecodeRow`, also re-fired with
 * `retrigger` once an E-Dx note delay runs out), ticks the note-cut
 * countdown (`GaxChannelStepInstrumentSeq`), the envelope/portamento
 * (`GaxChannelTick`), and finally mixes the channel (`GaxChannelMix`)
 * unless it's `muted`.
 *
 * Both functions in this file were NAKED ("parameter-homing order",
 * "r8/sb allocation ceiling"); written plainly against the handler
 * structs in gax_internal.h they match outright - see
 * docs/matching/archive/gax-toolchain-retry.md. */
u8 GaxChannelPlay(struct GaxChannelState *self, void *buf, u32 arg)
{
    struct GaxInfoHandler *info = (struct GaxInfoHandler *)self->children[0];

    info->type->play(info, buf, arg);
    if (info->muteTicks != 0)
        self->instrument = NULL;
    if (info->playing != 0) {
        if (self->retriggerDelay != 0 && --self->retriggerDelay == 0)
            GaxChannelDecodeRow(self, info, 1);
        if (info->speed != 0 && info->newRow != 0)
            GaxChannelDecodeRow(self, info, 0);
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
    return self->muted == 0 ? (u8)GaxChannelMix(self, info, buf, arg, info->type->data.song, 0) : 0;
}

/* Decodes one row of this channel's packed pattern stream and applies
 * it. A row is either a skip marker (`0xff, n`: skip n rows), a packed
 * form (bit 7 set: low 7 bits 1-121 = note + instrument, 122+ = effect
 * command + parameter, 0 = empty row) or a full 4-byte note/instrument/
 * command/parameter record. An E-Dx effect (`cmd 14`, parameter `0xDx`)
 * only stores the note/instrument and arms `retriggerDelay` - the
 * caller re-enters with `retrigger` set once it runs out, replaying
 * them. Otherwise the note is started (`GaxChannelSetNote`, except for
 * tone-portamento `cmd 3`), the instrument bound (`GaxChannelSetInstrument`), and
 * the effect command applied. */
void GaxChannelDecodeRow(struct GaxChannelState *self, struct GaxInfoHandler *info, u8 retrigger)
{
    u32 note, instrument, cmd, param;
    u8 *p, *next;

    self->volStep15 = 0;
    self->pitchStep = 0;
    self->retriggerDelay = 0;
    if (retrigger == 0) {
        if (info->newOrder != 0) {
            self->patternPtr = info->type->data.song->patterns +
                               self->type->data.orders[info->orderPos].patternOffset;
            self->rowSkip = 0;
            self->emptyPattern = *self->patternPtr++;
        }
        if (self->emptyPattern != 0)
            return;
        if (self->rowSkip != 0) {
            self->rowSkip--;
            return;
        }
        p = self->patternPtr;
        if (p[0] == 0xff) {
            self->rowSkip = p[1] - 1;
            self->patternPtr = p + 2;
            return;
        }
        if (p[0] & 0x80) {
            u32 packed = p[0] & 0x7f;

            if (packed == 0) {
                self->patternPtr = p + 1;
                return;
            }
            if (packed <= 121) {
                note = packed;
                instrument = p[1];
                param = 0;
                cmd = 0;
                next = p + 2;
            } else {
                note = 0;
                instrument = 0;
                cmd = p[1];
                param = p[2];
                next = p + 3;
            }
        } else {
            note = p[0];
            instrument = p[1];
            cmd = p[2];
            param = p[3];
            next = p + 4;
        }
        self->patternPtr = next;
        if (cmd == 14 && (param >> 4) == 13) {
            self->retriggerDelay = param & 15;
            self->delayedNote = note;
            self->delayedInstrument = instrument;
            return;
        }
    } else {
        note = self->delayedNote;
        instrument = self->delayedInstrument;
        param = 0;
        cmd = 0;
    }

    if (cmd != 3)
        GaxChannelSetNote(self, note);
    GaxChannelSetInstrument(self, info, instrument, info->type->data.song);
    switch (cmd) {
    case 1:
        self->pitchStep = param;
        break;
    case 2:
        self->pitchStep = -param;
        break;
    case 3:
        if (param != 0) {
            self->slideTarget = (note - 2) << 5;
            self->slideRate = (self->slideTarget - self->pitch) / (s32)param;
        }
        break;
    case 7:
        info->speed = (param >> 4) | ((param << 8) & 0xf00);
        info->tickCounter = info->speed - 1;
        break;
    case 10:
        self->volStep15 = param;
        break;
    case 11:
        self->volStep15 = -param;
        break;
    case 12:
        self->vol15 = param;
        break;
    case 13:
        info->patternBreak = 1;
        info->breakRow = param;
        break;
    case 15:
        info->speed = param;
        info->tickCounter = param - 1;
        break;
    }
}
