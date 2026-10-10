#ifndef GUARD_GRAPHICS_PACKAGE_HPP
#define GUARD_GRAPHICS_PACKAGE_HPP

/* The BG setup and the sprite box as C++ (#753; src/gfx/graphics_package.cpp
 * and scaled_sprite.cpp).
 * Neither has a vtable. cxx_symbols.txt maps the methods' mangled names
 * onto their C names. */

extern "C" {
#include "core.h"
#include "gfx.h"
#include "graphics_package.h"
}

/* REG_BGnCNT as bitfields. As a stack variable it is 4 bytes (agbcc pads
 * every union to a word); level_select.hpp's `ZoomBg` holds
 * a packed 2-byte copy. */
union bgcnt {
    u16 raw;
    struct {
        u16 priority:2;
        u16 charBase:2;
        u16:2; // bits 4-5, unused by the hardware
        u16 mosaic:1;
        u16 colorMode:1;
        u16 screenBase:5;
        u16 wrap:1;
        u16 size:2;
    } bits;
};

/* A BG's setup (0x10 bytes): the char block, screen block and palette bank
 * a graphics package is loaded into, and the BGnCNT value built from them.
 * The menus construct one (on the stack, as a member or with `new`), load
 * a package into it (Load) and write GetControl() to REG_BGnCNT. */
class BgSetup
{
public:
    u32 charBlock;    // 0x00
    u32 screenBlock;  // 0x04
    u32 paletteBank;  // 0x08
    union bgcnt ctrl; // 0x0C - BGnCNT

    BgSetup(u32 charBlock, u32 screenBlock, u32 paletteBank, u32 priority); // InitBgSetup
    void Load(const struct bg_package *pkg);                                // LoadGraphicsPackage
    u16 GetControl();                                                       // GetBgSetupControl
};

COMPILE_TIME_ASSERT(graphics_package_hpp, sizeof(BgSetup) == 0x10);

/* A sprite box (0x28 bytes): position, requested size, its OAM template and
 * the preset box it was fitted to. UNUSED: nothing in the ROM calls any of
 * its methods. */
class ScaledSprite
{
public:
    s32 x;                // 0x00
    s32 y;                // 0x04
    s32 width;            // 0x08
    s32 height;           // 0x0C
    struct oam_attrs oam; // 0x10 (gfx.h)
    s32 sizeIndex;        // 0x18
    s32 color;            // 0x1C - SetColor
    s32 scaleX;           // 0x20 - Q8
    s32 scaleY;           // 0x24 - Q8

    void Fit(s32 width, s32 height); // FitScaledSprite
    void Draw();                     // DrawScaledSprite
    void SetColor(s32 color);        // SetScaledSpriteColor
    void SetPriority(u32 priority);  // SetScaledSpritePriority
    void SetPos(u32 x, u32 y);       // SetScaledSpritePos
    void ResetAttrs();               // ResetScaledSpriteAttrs
};

COMPILE_TIME_ASSERT(graphics_package_hpp, sizeof(ScaledSprite) == 0x28);

#endif /* !GUARD_GRAPHICS_PACKAGE_HPP */
