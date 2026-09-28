#include "core.h"
#include "audio.h"

/* sub_803AD7C is libgcc's `_call_via_r1` (the `ops->init` calls). */
asm(".set _call_via_r1, sub_803AD7C\n");

/* Q32 reciprocal of the current mix rate, read by sub_8039B44. */
extern u64 gUnknown_03001618;
/* `__udivdi3` - see src/util/math_div64_util.c. */
extern u64 sub_8037A7C(u64 n, u64 d);

/* Stores `2^32 / self->format->mixRate`. An inline helper taking the
 * destination pointer first is what makes the ROM load
 * `&gUnknown_03001618` into a callee-saved register *before* reading
 * the mix rate - plain `gUnknown_03001618 = ...` (or any struct/array
 * spelling of it) loads the address after the call instead. */
static inline void SetMixRateReciprocal(u64 *dst, struct GaxChannelState *self)
{
    *dst = sub_8037A7C((u64)1 << 32, self->format->mixRate);
}

/* GAX2_SoundHandler "Channel" type's init_fn (ROM 0x08039519, see
 * docs/audio.md's per-type function-pointer table): resets the
 * channel's playback fields (no note, no instrument, full volume,
 * portamento off), recomputes the mix-rate reciprocal, then runs every
 * child's own `init` callback.
 *
 * Was NAKED (the division call's register choreography "resisted every
 * plain-C form"); the inline destination-pointer helper above closes it
 * - see docs/matching/gax-toolchain-retry.md. */
void sub_8039518(struct GaxChannelState *self)
{
    u32 i;

    self->samplePos = 0;
    self->row = 0;
    self->instrument = NULL;
    self->note = 0x8ad0;
    self->field_11 = 1;
    self->vol15 = 0xff;
    self->field_18 = -1;
    self->field_0c = 0;
    self->sweepOn = 0;
    self->field_0d = 0;
    self->rowSkip = 0;
    self->emptyPattern = 0;
    self->field_24 = 0;
    self->field_25 = 0;
    self->retriggerDelay = 0;
    self->slideRate = 0;
    self->slideTarget = 0;
    self->field_52 = 1;
    SetMixRateReciprocal(&gUnknown_03001618, self);
    for (i = 0; i < self->type->childCount; i++)
        self->children[i]->type->init(self->children[i]);
}
