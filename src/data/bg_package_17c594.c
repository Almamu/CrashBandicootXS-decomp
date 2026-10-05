#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0817C594-0x0817C5D0: the fade overlay's three BG packages,
 * loaded by InitContinuePrompt (actor_part87.c) into its BG0/BG2/BG1 buffers.
 * Linked in ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

extern const u8 gContinuePromptSmokeBgPalette[];
extern const u8 gContinuePromptGlowBgPalette[];
extern const u8 gContinuePromptUkaUkaBgPalette[];
extern const u8 gContinuePromptSmokeBgTiles[];
extern const u8 gContinuePromptGlowBgTiles[];
extern const u8 gContinuePromptUkaUkaBgTiles[];
extern const u8 gContinuePromptSmokeBgMap[];
extern const u8 gContinuePromptGlowBgMap[];
extern const u8 gContinuePromptUkaUkaBgMap[];

/* BG0. */
const struct bg_package gContinuePromptSmokeBg = {
    0x1e,
    0x14,
    (void *)gContinuePromptSmokeBgPalette,
    (void *)gContinuePromptSmokeBgTiles,
    (void *)gContinuePromptSmokeBgMap,
};

/* BG2. */
const struct bg_package gContinuePromptGlowBg = {
    0x1e,
    0x14,
    (void *)gContinuePromptGlowBgPalette,
    (void *)gContinuePromptGlowBgTiles,
    (void *)gContinuePromptGlowBgMap,
};

/* BG1. */
const struct bg_package gContinuePromptUkaUkaBg = {
    0x1e,
    0x14,
    (void *)gContinuePromptUkaUkaBgPalette,
    (void *)gContinuePromptUkaUkaBgTiles,
    (void *)gContinuePromptUkaUkaBgMap,
};
