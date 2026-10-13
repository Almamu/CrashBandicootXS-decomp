#include "frame_limiter.hpp"
#include "fade.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "gfx.h"
}

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". This cluster's
 * parked functions interleave with the matched ones - see
 * docs/matching.md for the full split. */

/* Backs the real palette (`PLTT`) up into `backup`,
 * then, for each factor 0/2/4/.../16, blends it toward black via the
 * already-matched `Darken` into `buffer` and DMAs
 * that result into the real palette, waiting one VBlank between each
 * step - a textbook fade-to-black animation. Once fully faded, sets
 * up the hardware blend registers (`REG_BLDCNT`/`REG_BLDY`) and
 * restores the original backed-up palette.
 *
 * Was NAKED asm, then C with pins and instruction asm (see
 * docs/matching/archive/naked-sub_80014a4-matched.md); the plain
 * DmaCopy16 calls match (#662). */
void PaletteFade::FadeToBlack()
{
    s32 factor;

    DmaCopy16(3, PLTT, backup, PLTT_SIZE);
    for (factor = 0; factor <= 0x10; factor += 2) {
        Darken(factor);
        FrameLimiter::WaitForVBlank();
        DmaCopy16(3, buffer, PLTT, PLTT_SIZE);
    }
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_BLDY = 0x10;
    DmaCopy16(3, backup, PLTT, PLTT_SIZE);
}

/* `state.period != -1`: the "idle" sentinel (gfx.h). Between
 * PaletteFade's functions in the ROM, but BrightnessFade's. */

bool BrightnessFade::IsActive()
{
    return state.period != -1;
}
