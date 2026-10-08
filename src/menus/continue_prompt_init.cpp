#include "frontend.hpp"
#include "audio.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "graphics_package.h"
#include "memory.h"
#include "globals.h"
}

/* The continue prompt's constructor (ContinuePrompt, frontend.hpp;
 * RunContinuePrompt allocates it): three BG buffers (BG0 the smoke, BG1
 * the Uka Uka background, BG2 the glow) and their graphics packages,
 * palette color 0 cleared, DISPCNT with BG0-BG2 on, the graphics
 * (InitGraphics, continue_prompt.cpp), then BLDCNT/BLDALPHA blending BG2
 * over BG0 (EVA 8/16, EVB 16/16), every register applied, and the music
 * faded out. */
ContinuePrompt::ContinuePrompt()
{
    bg0Buf = new BgSetup(0, 0x1f, 0, 3);
    bg1Buf = new BgSetup(3, 0x1e, 0, 1);
    bg2Buf = new BgSetup(2, 0x1d, 1, 2);
    bg1Buf->Load(&gContinuePromptUkaUkaBg);
    bg0Buf->Load(&gContinuePromptSmokeBg);
    bg2Buf->Load(&gContinuePromptGlowBg);
    *(vu16 *)PLTT = 0;
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = 0;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.bg1 = 1;
    dispcnt.bits.bg2 = 1;
    InitGraphics();
    blend.raw = 0;
    blend.bits.bg2First = 1;
    blend.bits.bg0Second = 1;
    blend.bits.eva = 8;
    blend.bits.evb = 16;
    blend.bits.effect = 1;
    REG_BG0CNT = bg0Buf->GetControl();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    REG_BG1CNT = bg1Buf->GetControl();
    *(vu32 *)REG_ADDR_BG1HOFS = 0;
    REG_BG2CNT = bg2Buf->GetControl();
    *(vu32 *)REG_ADDR_BG2HOFS = 0;
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    blinkCounter = 0;
    selection = 0;
    gAudioContext->FadeOutMusic(0);
}
