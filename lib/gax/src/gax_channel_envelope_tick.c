#include "gax_internal.h"

/* Per-tick per-channel envelope/portamento update: if an instrument is
 * bound, advances the envelope (`GaxEnvelopeTick`, result into `envOut`),
 * then the effect-table tick (`GaxChannelTickVibrato`) and the position sweep
 * (`GaxChannelTickSweep`). Independently of that, ramps the two 8-bit volume
 * fields by their signed per-tick steps (clamped to 0-0xff), advances
 * `pitch`/`note` by their steps, and applies an armed portamento
 * (`slideRate`) to `pitch` - snapping to `slideTarget` and disarming
 * once a step crosses it (the sign of `target - pitch` flips).
 *
 * Was NAKED behind a heavily register-pinned 99.7% draft; written
 * plainly against a struct (fields accessed directly, no cached
 * locals) it matches outright - see docs/matching/archive/gax-toolchain-retry.md. */
void GaxChannelTick(struct GaxChannelState *self, struct GaxInfoHandler *info)
{
    s32 v;

    if (self->instrument != NULL) {
        self->envOut = GaxEnvelopeTick(self, self->instrument->envelope, &self->envPos);
        GaxChannelTickVibrato(self);
        GaxChannelTickSweep(self);
    }

    v = self->volStep15 + self->vol15;
    if (v > 0xff)
        v = 0xff;
    if (v < 0)
        v = 0;
    self->vol15 = v;

    v = self->volStep17 + self->vol17;
    if (v > 0xff)
        v = 0xff;
    if (v < 0)
        v = 0;
    self->vol17 = v;

    self->pitch += self->pitchStep;
    self->note += self->noteStep;

    if (self->slideRate != 0) {
        u32 before = (self->slideTarget - self->pitch) & 0x80000000;

        self->pitch += self->slideRate;
        if (before != ((self->slideTarget - self->pitch) & 0x80000000)) {
            self->slideRate = 0;
            self->pitch = self->slideTarget;
            self->slideTarget = 0;
        }
    }
}
