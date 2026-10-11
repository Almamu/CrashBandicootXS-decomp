# The display mode (#843)

`struct dispcnt_bits.mode` (gfx.h), DISPCNT bits 0-2 in the `gDispcnt`
shadow and in the menus' own `union dispcnt` copies, is the GBA's BG mode.
gba/io_reg.h already names the hardware values, `DISPCNT_MODE_0` to
`DISPCNT_MODE_5`, so the sites use those.

| Value | Where |
|---|---|
| `DISPCNT_MODE_0` (four text BGs) | the rooms (SetDispcntMode in room.cpp and RunRoom), the starfield, the pause menu, the power dialog, the continue prompt |
| `DISPCNT_MODE_1` (BG2 affine) | the title screen, the language select, the level select (its zooming picture), the company logos |
| `DISPCNT_MODE_4` (an 8bpp bitmap) | the level cutscenes |

## The BG colour mode

`union bgcnt`'s `colorMode` (graphics_package.hpp), BGnCNT bit 7, is the
hardware's 16/256-colour switch. gba/io_reg.h has the register values
(`BGCNT_16COLOR`, `BGCNT_256COLOR`); the bitfield's values are
`BGCNT_COLOR_MODE_16` (0) and `BGCNT_COLOR_MODE_256` (1).

| Value | Where |
|---|---|
| `BGCNT_COLOR_MODE_16` (4bpp) | BgSetup::Load, when the palette asset's header gives a size of 0x20 or less (one 16-colour palette) |
| `BGCNT_COLOR_MODE_256` (8bpp) | BgSetup::Load for a bigger palette; the title screen's and the company logos' BG2 |
