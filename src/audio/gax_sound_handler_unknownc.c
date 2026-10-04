#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;
extern void *sub_803AD7C(void *arg0, void *fn);

/* `nullsub_41` (ROM `0x0803A228`) sits immediately before the GAX2_
 * SoundHandler "UnknownC" type's function-pointer trio (see
 * docs/audio.md's per-type table: `init_fn`/`unknown_fn`/`play_fn` =
 * `0x0803A22D`/`0x0803A275`/`0x0803A325`, i.e. `sub_803A22C`+1/
 * `nullsub_42`+1/`sub_803A324`+1, the Thumb bit). Unlike its two
 * neighbors it isn't one of that trio itself - no caller found for it
 * anywhere in the ROM (checked every `asm/*.s`, `expected/*.s`, and every
 * `src/` source file for a `bl nullsub_41`/raw `0x0803A229` reference) - kept
 * unnamed/undescribed beyond that since nothing calls it. */
void nullsub_41(void)
{
}
asm(".align 2, 0");

/* GAX2_SoundHandler "UnknownC" type's `init_fn` (ROM `0x0803A22D`, see
 * docs/audio.md) - `self` is the mixer handler. Sets `pos` to 1, then
 * runs every child's `init` callback (`child->type->init`) through the
 * `sub_803AD7C` trampoline. The children are the song's channels
 * (`type->childCount`), plus the `extraChildren` sound-effect voices
 * when this is the music player (`curChannelIdx == 0`). */
void sub_803A22C(void *self)
{
    struct GaxMixerHandler *p = self;
    u32 i;
    register u32 limit asm("r0");

    p->pos = 1;
    for (i = 0; ; i++) {
        if (gUnknown_03001630->curChannelIdx == 0) {
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
            sub_803AD7C(elem, fn);
        }
    }
}

/* GAX2_SoundHandler "UnknownC" type's `unknown_fn` (ROM `0x0803A275`,
 * see docs/audio.md) - a no-op stub, same as the "Info"/"Channel" types'
 * `unknown_fn`s (`nullsub_39`/`nullsub_40`). `play_fn` (`sub_803A324`)
 * is still raw. */
void nullsub_42(void)
{
}
asm(".align 2, 0");
