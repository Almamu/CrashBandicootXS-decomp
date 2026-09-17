#include "core.h"

extern void *sub_8026EDC(s32 size);
extern void *sub_8022230(void *arg0);

/* Lazily allocates `gUnknown_03000828` (0x1cc bytes) through
 * `sub_8022230` the first time it's needed, then returns it. Its own
 * file: ROM-adjacent to the still-raw (not yet confidently understood)
 * `sub_802375C`/`sub_8023A1C` pair on both sides
 * (asm/code_3_2_17_236ec.s before it, asm/code_3_2_17_2375c.s after),
 * so it can't share an object file with either matched neighbor
 * without splitting the ROM-contiguous layout. */
extern void *gUnknown_03000828;
void *sub_8023738(void)
{
    if (gUnknown_03000828 == NULL) {
        gUnknown_03000828 = sub_8022230(sub_8026EDC(0x1cc));
    }
    return gUnknown_03000828;
}
