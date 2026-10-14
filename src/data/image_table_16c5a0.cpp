#include "graphics.hpp"
#include "game/ui/menus.hpp"

extern "C" {
#include "core.h"
}

using Engine::Gfx::LevelSelectPicture0;
using Engine::Gfx::LevelSelectPicture1;
using Engine::Gfx::LevelSelectPicture4;
using Engine::Gfx::LevelSelectPicture2;
using Engine::Gfx::LevelSelectPicture5;
using Engine::Gfx::LevelSelectPicture3;
using Engine::Gfx::LevelSelectPicture9;
using Engine::Gfx::LevelSelectPicture7;
using Engine::Gfx::LevelSelectPicture8;
using Engine::Gfx::LevelSelectPicture6;

/*
 * ROM 0x0816C5A0-0x0816C5F0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* Ten {palette, tiles} tagged-asset pairs, indexed by image number:
 * UpdateZoomBg (zoom_bg.cpp) loads both through
 * LoadTaggedAsset. */
const struct image_pair gLevelSelectPictures[10] = {
    { LevelSelectPicture0::palette, LevelSelectPicture0::tiles },
    { LevelSelectPicture1::palette, LevelSelectPicture1::tiles },
    { LevelSelectPicture2::palette, LevelSelectPicture2::tiles },
    { LevelSelectPicture3::palette, LevelSelectPicture3::tiles },
    { LevelSelectPicture4::palette, LevelSelectPicture4::tiles },
    { LevelSelectPicture5::palette, LevelSelectPicture5::tiles },
    { LevelSelectPicture6::palette, LevelSelectPicture6::tiles },
    { LevelSelectPicture7::palette, LevelSelectPicture7::tiles },
    { LevelSelectPicture8::palette, LevelSelectPicture8::tiles },
    { LevelSelectPicture9::palette, LevelSelectPicture9::tiles },
};
