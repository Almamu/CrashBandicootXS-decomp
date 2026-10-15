/* The pc platform's lib/gax/src/gax_fatal_error.c (platform/pc/
 * platform.mk, "Units"): GAX2's fatal error. The original stops the DMAs,
 * draws GAX2's halt screen (its Huffman-compressed font, through the BIOS
 * call in gax_swi.c) with the failing function's name and the error, and
 * hangs; the host prints the two and ends the run. (The game sets
 * GaxSongHeader.showErrors to 0, so only GAX2_new(NULL) gets here.) */

#include <stdio.h>

#include "gax_host.h"
#include "host_platform.h"

void GaxFatalError(const char *function, const char *message)
{
    fprintf(stderr, "GAX2: %s: %s\n", function, message);
    HostExit(3, "GAX2 fatal error");
}
