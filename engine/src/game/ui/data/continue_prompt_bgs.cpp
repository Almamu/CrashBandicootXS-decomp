#include "graphics.hpp"
#include "engine/gfx/graphics_package.hpp"
#include "game/ui/menus.hpp"

extern "C" {
#include "core.h"
}

using Engine::Gfx::ContinuePromptSmokeBg;
using Engine::Gfx::ContinuePromptGlowBg;
using Engine::Gfx::ContinuePromptUkaUkaBg;

/*
 * ROM 0x0817C594-0x0817C5D0: the fade overlay's three BG packages,
 * loaded by InitContinuePrompt (continue_prompt.cpp) into its BG0/BG2/BG1 buffers.
 * Linked in ROM order between games/xs/data/data.s sections by games/xs/ldscript.txt - see
 * docs/data.md.
 */

/* BG0. */
const struct bg_package gContinuePromptSmokeBg = {
    0x1e,
    0x14,
    (void *)ContinuePromptSmokeBg::palette,
    (void *)ContinuePromptSmokeBg::tiles,
    (void *)ContinuePromptSmokeBg::map,
};

/* BG2. */
const struct bg_package gContinuePromptGlowBg = {
    0x1e,
    0x14,
    (void *)ContinuePromptGlowBg::palette,
    (void *)ContinuePromptGlowBg::tiles,
    (void *)ContinuePromptGlowBg::map,
};

/* BG1. */
const struct bg_package gContinuePromptUkaUkaBg = {
    0x1e,
    0x14,
    (void *)ContinuePromptUkaUkaBg::palette,
    (void *)ContinuePromptUkaUkaBg::tiles,
    (void *)ContinuePromptUkaUkaBg::map,
};
