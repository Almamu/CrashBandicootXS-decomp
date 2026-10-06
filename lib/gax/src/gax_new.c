#include "gax_internal.h"

/* GAX2's `GAX2_new(params)` (named by its own "GAX2_NEW" / "PARAMS ARG
 * IS NULL" error report): fills a `struct GaxSongHeader` (the GAX2
 * params block GAX2_estimate/GAX2_init take) with defaults - zeroes it
 * (GaxZeroFill), then mix rate and SFX-voice count 0xffff ("the song's
 * default"), `filter` 0, volume 0xffff and `showErrors` 1. A NULL
 * `self` shows the fatal-error screen instead. */
void GAX2_new(void *self)
{
    struct GaxSongHeader *params = self;
    u16 val;

    if (self == NULL) {
        GaxFatalError(gGaxErrNameNew, gGaxErrParamsNull);
        return;
    }
    GaxZeroFill(self, 0x3c);
    val = 0xFFFF;
    /* not a plain member store: that makes gcc build the 0 and -1 below
     * as fresh constants instead of reusing val's register */
    *(u16 *)&params->mixRate = val;
    val = 0;
    params->filter = val;
    val -= 1;
    params->numSfx = val;
    params->volume = val;
    params->showErrors = 1;
}
