#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;

/* Conditionally updates a currently-active channel voice's "note
 * period"-looking field (+0x26): reached by walking the current
 * channel's own two-level object chain (+0->0->0xc gives a count, added
 * to `channel`, indexed into +0->8's array to land on the target voice)
 * and only takes effect if that voice's +0x3c field is non-zero (looks
 * like an "active" flag). The chained objects' own shapes aren't
 * understood yet - kept as raw offsets, same as sub_80381FC. */
void sub_8038F94(s32 channel, u32 period)
{
    struct GaxPlayerState *p;
    u8 *cur;
    u8 *obj2;
    u8 *obj3;
    u8 *voice;

    if (period <= 0xeec) {
        p = gUnknown_03001630;
        cur = p->channels[p->curChannelIdx];
        obj2 = *(u8 **)cur;
        obj3 = *(u8 **)obj2;
        voice = *(u8 **)(*(u32 *)(obj2 + 8) + (*(u32 *)(obj3 + 0xc) + channel) * 4);
        if (*(u32 *)(voice + 0x3c) != 0) {
            *(u16 *)(voice + 0x26) = period;
        }
    }
}
asm(".align 2, 0");
