#include "core.h"

extern void sub_80392E0(void *arg0, void *arg1);
extern void sub_8037F3C(void *arg0, s32 size);
extern u8 gStaticData_085A61B0[];
extern u8 gStaticData_085A61BC[];

/* `self == NULL` takes a completely different path (a 2-arg call into
 * still-unread GAX2 engine internals) - otherwise clears/zero-fills
 * `self` (via sub_8037F3C, a memset-like helper) and resets a handful of
 * fields to their "empty" sentinel values. Meaning of the individual
 * fields isn't understood yet; looks like a SoundHandler/channel-object
 * constructor (see docs/audio.md's GAX2_SoundHandler notes) given the
 * neighboring GAX2 code, but not confirmed. */
void sub_80381FC(void *self)
{
    u16 val;

    if (self == NULL) {
        sub_80392E0(gStaticData_085A61B0, gStaticData_085A61BC);
        return;
    }
    sub_8037F3C(self, 0x3c);
    val = 0xFFFF;
    *(u16 *)((u8 *)self + 8) = val;
    val = 0;
    *(u16 *)((u8 *)self + 0xa) = val;
    val -= 1;
    *(u16 *)((u8 *)self + 0xe) = val;
    *(u16 *)((u8 *)self + 0x10) = val;
    *(u8 *)((u8 *)self + 0x38) = 1;
}
