#include "frontend.hpp"

extern "C" {
#include "gba/io_reg.h"
#include "graphics_package.h"
#include "audio.h"
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
    bg0Buf = InitBgSetup(new bg_setup, 0, 0x1f, 0, 3);
    bg1Buf = InitBgSetup(new bg_setup, 3, 0x1e, 0, 1);
    bg2Buf = InitBgSetup(new bg_setup, 2, 0x1d, 1, 2);
    LoadGraphicsPackage(bg1Buf, &gContinuePromptUkaUkaBg);
    LoadGraphicsPackage(bg0Buf, &gContinuePromptSmokeBg);
    LoadGraphicsPackage(bg2Buf, &gContinuePromptGlowBg);
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
    REG_BG0CNT = GetBgSetupControl(bg0Buf);
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    REG_BG1CNT = GetBgSetupControl(bg1Buf);
    *(vu32 *)REG_ADDR_BG1HOFS = 0;
    REG_BG2CNT = GetBgSetupControl(bg2Buf);
    *(vu32 *)REG_ADDR_BG2HOFS = 0;
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    blinkCounter = 0;
    selection = 0;
    FadeOutMusic(gAudioContext, 0);
}
