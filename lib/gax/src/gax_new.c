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

    if (params == NULL) {
        GaxFatalError(gGaxErrNameNew, gGaxErrParamsNull);
        return;
    }
    GaxZeroFill(params, 0x3c);
    params->mixRate = 0xFFFF;
    params->filter = 0;
    params->numSfx = 0xFFFF;
    params->volume = 0xFFFF;
    params->showErrors = 1;
}
