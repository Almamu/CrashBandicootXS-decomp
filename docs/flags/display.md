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
