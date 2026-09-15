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
asm(".align 2, 0");
