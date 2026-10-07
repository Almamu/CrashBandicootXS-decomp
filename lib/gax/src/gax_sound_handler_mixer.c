#include "gax_internal.h"
#include "match.h"

extern void *_call_via_r1(void *arg0, void *fn);

/* The sound-effect voice type's `unknown_fn` (ROM `0x0803A228`): a no-op
 * stub like the other types' `unknown_fn`s. It isn't called by name; its
 * only reference is the voice handler type at the end of the sound-effect
 * data set (`0x0855BC98`: `GaxFxChannelInit`/`GaxFxChannelUnknown`/
 * `GaxFxChannelPlay`, built by tools/gax_audio.py's `HANDLER_FUNCS['sfx']`
 * as `0x0803A229`; see docs/audio.md's "Sound effects"). It sits right
 * before the mixer type's trio (`init_fn`/`unknown_fn`/
 * `play_fn` = `0x0803A22D`/`0x0803A275`/`0x0803A325`, i.e.
 * `GaxMixerInit`+1/`GaxMixerUnknown`+1/`GaxMixerPlay`+1, the Thumb bit).
 * Nothing in the engine calls a type's `unknown_fn` slot. */
void GaxFxChannelUnknown(void)
{
}
asm(".align 2, 0");

/* The GAX2_SoundHandler mixer type's `init_fn` (ROM `0x0803A22D`, see
 * docs/audio.md) - `self` is the mixer handler. Sets `pos` to 1, then
 * runs every child's `init` callback (`child->type->init`) through the
 * `_call_via_r1` trampoline. The children are the song's channels
 * (`type->childCount`), plus the `extraChildren` sound-effect voices
 * when this is the music player (`curChannelIdx == 0`). */
void GaxMixerInit(void *self)
{
    struct GaxMixerHandler *p = self;
    u32 i;
    MATCH_HOLD_REG(u32, limit, r0);

    p->pos = 1;
    for (i = 0;; i++) {
        if (gGaxPlayerState->curChannelIdx == 0) {
            limit = p->type->childCount;
            limit = limit + p->extraChildren;
        } else {
            limit = p->type->childCount;
        }
        if (i >= limit) {
            break;
        }
        {
            struct GaxHandler *elem = p->children[i];
            void *fn = elem->type->init;
            _call_via_r1(elem, fn);
        }
    }
}

/* The GAX2_SoundHandler mixer type's `unknown_fn` (ROM `0x0803A275`,
 * see docs/audio.md) - a no-op stub, same as the "Info"/"Channel"/FX
 * voice types' `unknown_fn`s (`GaxInfoUnknown`/`GaxChannelUnknown`/
 * `GaxFxChannelUnknown`). `play_fn` is `GaxMixerPlay`
 * (gax_sound_handler_mixer_play.c). */
void GaxMixerUnknown(void)
{
}
asm(".align 2, 0");
