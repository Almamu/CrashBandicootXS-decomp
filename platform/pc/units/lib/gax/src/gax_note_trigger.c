/* The pc platform's lib/gax/src/gax_note_trigger.c (platform/pc/
 * platform.mk, "Units"): GaxChannelMix as the original, but for the ARM
 * resampler it runs. The GBA's patches the two instructions of its IWRAM
 * copy that step and end-test the loop it is about to enter (forward
 * `add`/`blt`, backward `sub`/`bgt`) and calls the copy through
 * GAX_CALL_ARM_R; here the patches set which way the C resampler
 * (gax_dsp.c) steps each loop, and the call is a C call with the same
 * work item. The rest is the original's code (see lib/gax for its notes
 * on matching, which don't apply here). */

#include "gax_host.h"

/* The resampler copy's state that GAX_PATCH_MIXER changes. */
static int sStoreBackward;
static int sMixBackward;

u32 GaxChannelMix(struct GaxChannelState *self, struct GaxInfoHandler *info, void *buf, u32 arg,
                  struct GaxSongData *song, u8 flag)
{
    struct GaxChannelInstrument *inst;
    struct GaxWave *wave;
    s32 pitch;
    u32 idx;
    u32 period;
    u32 vol;
    u32 step;
    u32 len;
    u32 pingpong;

    (void)arg;
    if (self->instrument == NULL || self->note == (s16)0x8ad0 || self->row > 3)
        return 0;
    wave = &song->waves[self->instrument->waveIdx[self->row]];
    if (wave->data == NULL)
        return 0;

    pitch = self->note + self->vibratoOffset;
    if (self->fixedPitch == 0) {
        pitch += self->pitch;
        if (flag == 0)
            pitch += self->type->data.orders[info->orderPos].transpose << 5;
    }
    inst = self->instrument;
    idx = pitch + inst->rows[self->row].tune;
    if (idx > 0xef3)
        idx = 0xef3;
    period = gGaxPeriodTable[idx];
    vol = self->envOut != 0xff ? self->envOut : 0x100;
    if (self->vol17 != 0xff)
        vol = vol * self->vol17 >> 8;
    if (self->vol15 != 0xff)
        vol = vol * self->vol15 >> 8;
    if ((u8)self->volume != 0xff)
        vol = vol * (u8)self->volume >> 8;
    if (info->volume != 0xff)
        vol = vol * info->volume >> 8;
    if (flag == 0)
        vol = vol * info->type->data.song->volume >> 8;
    {
        s64 prod = (s32)period;

        prod = prod * gGaxMixRateReciprocal >> 32;
        step = prod;
    }
    len = wave->length;
    pingpong = 0;
    if (inst->rows[self->row].sweep == 0 &&
        inst->rows[self->row].sweepMin < inst->rows[self->row].sweepMax)
        pingpong = 1;
    {
        struct HostGaxMixItem item = {
            wave->data,
            buf,
            self->samplePos,
            len << 11,
            self->format->frames,
            0,
            vol,
            step,
            0,
            self->sweepOn ? self->instrument->rows[self->row].sweepLen << 11 : 0,
        };

        while (item.done < self->format->frames) {
            if (self->direction > 0) {
                if (pingpong)
                    item.end = self->instrument->rows[self->row].sweepMax << 11;
                else if (self->sweepOn)
                    item.end = (self->sweepPos + self->instrument->rows[self->row].sweepLen) << 11;
                else
                    item.end = wave->length << 11;
                if (self->isFirst) {
                    item.mode = 0;
                    sStoreBackward = 0;
                } else {
                    item.mode = self->mixMode;
                    sMixBackward = 0;
                }
            } else {
                len = self->instrument->rows[self->row].sweepMin;
                item.end = len << 11;
                if (self->isFirst) {
                    item.mode = 0;
                    sStoreBackward = 1;
                } else {
                    item.mode = 1;
                    sMixBackward = 1;
                }
            }
            HostGaxResample(&item, sStoreBackward, sMixBackward);
            if (item.done == self->format->frames)
                break;
            if (pingpong) {
                if (self->instrument->rows[self->row].pingPong != 0) {
                    if (self->direction > 0)
                        item.pos -= step * 2;
                    else
                        item.pos += step * 2;
                    self->direction = ~self->direction;
                } else {
                    item.pos -= (self->instrument->rows[self->row].sweepMax -
                                 self->instrument->rows[self->row].sweepMin)
                                << 11;
                }
            } else if (self->sweepOn) {
                item.pos -= self->instrument->rows[self->row].sweepLen << 11;
            } else {
                if (self->isFirst)
                    GaxZeroFill((u16 *)buf + item.done, (self->format->frames - item.done + 1) * 2);
                self->note = 0x8ad0;
                self->noteStep = 0;
                self->priority = 0x80000000;
                break;
            }
        }
        self->samplePos = item.pos;
    }
    return 1;
}
