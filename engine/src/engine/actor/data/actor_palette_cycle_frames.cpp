#include "engine/actor/actor_palette_cycle.hpp"

using Engine::Actor::ActorPaletteCycle;

/*
 * ROM 0x08175760-0x08178F80. Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* The BG palette-cycle animation: 32 frames of BG palettes 0-13
 * (0x1C0 bytes each): ActorPaletteCycle::frames (engine/include/engine/actor/actor_palette_cycle.hpp).
 * While it's armed, ActorPaletteCycle::Update DMAs the cursor's frame to BG
 * palette RAM every call and every 0x24 calls steps that cursor towards
 * its bound. The sources are games/xs/graphics/vehicles/polar/palette_cycle/00.pal to
 * 31.pal (JASC, 224 colours each: the manifest's palette_cycle entry), built
 * with tools/jasc_pal.py and concatenated in frame order, a row per frame
 * (graphics.mk). A polar category can name its own (docs/levels.md,
 * "Category graphics"). */
const u16 ActorPaletteCycle::frames[32][14 * 16] = {
#include "vehicles/polar/palette_cycle.bin.inc"
};

/* The cursor start and bound ActorPaletteCycle::Set seeds for each
 * cycle: 0->15, 15->0, 16->31 and 31->16 (the first or second half of
 * the frames, forwards or backwards). */
const s32 ActorPaletteCycle::startFrames[4] = { 0, 15, 16, 31 };
const s32 ActorPaletteCycle::targetFrames[4] = { 15, 0, 31, 16 };
