#include "gax_internal.h"

/* GAX2's `GAX2_new(params)` (named by its own "GAX2_NEW" / "PARAMS ARG
 * IS NULL" error report): fills a `struct GaxSongHeader` (the GAX2
 * params block GAX2_estimate/GAX2_init take) with defaults - zeroes it
 * (GaxZeroFill), then mix rate and SFX-voice count 0xffff ("the song's
 * default"), `filter` 0, volume 0xffff and `showErrors` 1. A NULL
 * `self` shows the fatal-error screen instead. Kept on raw offsets
 * here (byte-exact as written). */
void GAX2_new(void *self)
{
    u16 val;

    if (self == NULL) {
        GaxFatalError(gGaxErrNameNew, gGaxErrParamsNull);
        return;
    }
    GaxZeroFill(self, 0x3c);
    val = 0xFFFF;
    *(u16 *)((u8 *)self + 8) = val;
    val = 0;
    *(u16 *)((u8 *)self + 0xa) = val;
    val -= 1;
    *(u16 *)((u8 *)self + 0xe) = val;
    *(u16 *)((u8 *)self + 0x10) = val;
    *(u8 *)((u8 *)self + 0x38) = 1;
}
