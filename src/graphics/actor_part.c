#include "core.h"
#include "actor.h"

extern void sub_8007174(void *arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
extern void sub_80073DC(void *unused, void *part, s32 *posPtr);

/* `part+0x25` selects whether (x, y) are already screen-relative
 * (nonzero - used as-is) or need the camera-relative conversion
 * sub_8007174 applies (zero - the common case). Either way, the
 * resolved {x, y} pair is forwarded to sub_80073DC (parked as
 * NON_MATCHING in src/graphics/graphics.c) to build/queue this part's
 * OAM entries. */
void sub_8007A48(void *self, void *part, s32 x, s32 y)
{
    s32 pos[2];

    if (*((u8 *)part + 0x25) == 0) {
        sub_8007174(part, x, y, &pos[0], &pos[1]);
    } else {
        pos[0] = x;
        pos[1] = y;
    }
    sub_80073DC(self, part, pos);
}
asm(".align 2, 0");

/* `part`'s own leading {x, y} pair (the same Q8 fixed-point position
 * fields struct actor has at 0x00/0x04) becomes the explicit position
 * passed to sub_8007A48 - confirms `part` embeds a struct-actor-shaped
 * position at its own start. */
void sub_8007A84(void *self, void *part)
{
    sub_8007A48(self, part, *(s32 *)part >> 8, *(s32 *)((u8 *)part + 4) >> 8);
}
asm(".align 2, 0");
