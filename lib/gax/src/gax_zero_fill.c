#include "gax_internal.h"
#include <agb_syscall.h>

/* Split out of lib/libgcc/libgcc2.c: that object is GAX2's bundled
 * libgcc code, built without -mthumb-interwork, while this helper (GAX2
 * engine code, called from GAX2_init/GaxChannelMix) uses the ordinary
 * interworking `pop {reg}; bx reg` return like the rest of the ROM. */

/* Zero-fills `count` bytes at `dest` - a plain memset helper. Byte-fills up to 3 leading bytes one at a time to
 * reach 4-byte alignment, zero-fills the largest 32-byte-aligned chunk
 * of what's left via the BIOS `CpuFastSet` SWI (fixed
 * source address so the same zero word is repeated - `FIXED_SRC`,
 * `0x01000000`), then finishes any trailing remainder one byte at a
 * time. The word count passed to `CpuFastSet` is computed as a signed
 * divide-by-4 of the 32-byte-aligned byte count (correct as-is since
 * that value is always non-negative in every real call site) then
 * masked to `CpuFastSet`'s 21-bit length field - both the signed
 * divide-by-4 and the mask are written out explicitly (rather than as
 * a plain `count / 4`) because that's what reproduces the ROM's own
 * `lsl #9`/`lsr #0xb` combined shift-and-mask codegen; a plain `/ 4`
 * compiles to a bare `asr #2` instead. The `cnt` copy of `count` and
 * the separate `remaining` local reproduce the ROM's register choices
 * (r6/r1); counting down `count` itself does not. */
void GaxZeroFill(void *destArg, s32 count)
{
    u8 *dest = destArg;
    s32 cnt = count;
    s32 aligned;
    u32 zero;

    while ((u32)dest & 3) {
        *dest++ = 0;
        cnt--;
    }

    zero = 0;
    aligned = cnt & ~0x1F;
    {
        s32 corrected = aligned;
        if (corrected < 0) {
            corrected += 3;
        }
        CpuFastSet(&zero, dest, 0x01000000 | (((u32)corrected >> 2) & 0x1FFFFF));
    }

    {
        u8 *tail = dest + aligned;
        s32 remaining = cnt - aligned;

        if (remaining > 0) {
            do {
                *tail++ = 0;
                remaining--;
            } while (remaining != 0);
        }
    }
}
