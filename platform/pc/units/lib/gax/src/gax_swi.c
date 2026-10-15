/* The pc platform's lib/gax/src/gax_swi.c (platform/pc/platform.mk,
 * "Units"): nothing. The original is GaxHuffUnComp, an inline BIOS
 * HuffUnComp (`swi 0x13`) for the halt screen's font, which only the
 * GBA's fatal-error screen uses; the host's GaxFatalError
 * (gax_fatal_error.c) prints the error instead. */

typedef int GaxSwiUnused;
