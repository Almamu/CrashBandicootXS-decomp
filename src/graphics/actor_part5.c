#include "core.h"
#include "actor.h"

extern s32 sub_8007114(struct actor *self, void *box);

/* `part+0x25 == 1` is the same fast override seen in
 * sub_8007F78/sub_8007FD8; otherwise defers to `sub_8007114` (already
 * matched in graphics.c), forwarding `box` straight through
 * unmodified. */
s32 sub_8008304(struct actor *part, void *box)
{
    s32 result = 0;
    register u8 *addr asm("r2") = (u8 *)part + 0x25;
    register u8 byteVal asm("r2");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if ((u8)sub_8007114(part, box)) {
        result = 1;
    }
    return result;
}

extern u8 sub_8006FE4(struct actor *self);

/* Same `part+0x25` fast-override shape as `sub_8008304` above,
 * deferring to `sub_8006FE4` (already matched in `graphics.c`)
 * instead - a single-argument sibling, so the address scratch
 * naturally lands in `r1` instead of `r2` (no second call argument to
 * keep out of the way). */
s32 sub_8008328(struct actor *part)
{
    s32 result = 0;
    register u8 *addr asm("r1") = (u8 *)part + 0x25;
    register u8 byteVal asm("r1");

    byteVal = *addr;
    if (byteVal == 1) {
        result = 1;
    } else if (sub_8006FE4(part)) {
        result = 1;
    }
    return result;
}

/* Always-true stub. */
s32 sub_800834C(void)
{
    return 1;
}

extern void sub_8007A84(void *self, void *part);
extern void *gUnknown_030012CC;

/* Tail-calls `sub_8007A84` (already matched in `actor_part.c`) with
 * the global `gUnknown_030012CC` as `self`. */
void sub_8008350(void *part)
{
    sub_8007A84(gUnknown_030012CC, part);
}
asm(".align 2, 0");
