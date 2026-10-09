#include "sprite_obj.hpp"
#include "frontend.hpp"
#include "audio.hpp"

extern "C" {
#include "gba/dma_macros.h"
#include "system.h"
#include "text.h"
#include "gfx.h"
#include "globals.h"
}

/* The last of the company-logo screen's methods (CompanyLogos, #664 part
 * 10b, include/frontend.hpp: LoadAssetBuffered, the constructor and the
 * destructor) and the logo actor's destructor, LogoActor's key method:
 * g++ emits gLogoActorVtable here. ROM 0x08037110-0x080371B4; the rest
 * of both classes is in company_logos.cpp. Split from the head of
 * language_select.cpp (#770), whose flags it keeps: agbcp with
 * -fno-implement-inlines (the Makefile's NO_IMPLEMENT_INLINES_OBJS), so
 * that g++ emits none of LogoActor's inline methods here. */

/* Loads a "tagged" asset (see LoadTaggedAsset, src/system/asset.cpp)
 * into a freshly allocated buffer, then DMAs it to `dest`. A method of
 * the logo screen (LoadVvLogoGraphics's), which it doesn't use. */
void CompanyLogos::LoadAssetBuffered(const void *asset, void *dest)
{
    u32 val = *(const u32 *)asset;
    struct dma_regs *dma;
    u8 *buf;

    val >>= 8;
    buf = new u8[val];
    LoadTaggedAsset(asset, buf);
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)buf;
    dma->dst = (u32)dest;
    val >>= 1;
    dma->cnt = val | 0x80000000;
    dma->cnt;
    delete[] buf;
}

/* The company-logo screen's constructor and destructor, both empty
 * (ShowCompanyLogos, level_state.cpp, allocates the screen, runs it and
 * deletes it). */
CompanyLogos::CompanyLogos()
{
}

CompanyLogos::~CompanyLogos()
{
}

/* The company-logo actor's destructor (slot 1; RunCompanyLogos deletes
 * it): frees the two VRAM tile blocks the constructor allocated. g++ adds
 * ActorSelf's inline destructor (the unlink) and the class's operator
 * delete (mem_free). */
LogoActor::~LogoActor()
{
    FreeVramTileBlock(gLogoActorTiles[0]);
    FreeVramTileBlock(gLogoActorTiles[1]);
}
