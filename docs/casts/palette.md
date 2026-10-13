# Palette RAM, the palette cache and the DISPCNT shadow (#855)

## gDispcnt is a union dispcnt

`gDispcnt` (sym_iwram.txt, 2 bytes) was declared `u8 [2]` in globals.h,
and every reader cast it: to `struct dispcnt_bits *` for a field, to
`u16 *` for the whole value. It is now `union dispcnt` (gfx.h, next to the
union), the type the menus' own DISPCNT copies already had:

| was | is |
|---|---|
| `((struct dispcnt_bits *)gDispcnt)->bg2 = 1` (company_logos.cpp; display.cpp's `DISPCNT_BITS` macro) | `gDispcnt.bits.bg2 = 1` |
| `*(u16 *)gDispcnt = 0`, `u16 *dispcnt = (u16 *)gDispcnt` | `gDispcnt.raw = 0`, `u16 *dispcnt = &gDispcnt.raw` |
| `gDispcnt[1] \|= 0x10` (Credits) | `gDispcnt.bits.obj = 1` |

The field stores are the same byte-wide `ldrb`/`orr`/`strb` as before
(Credits' too: its literal is still `gDispcnt` with the `#1` in the
`ldrb`). The union's 4-byte alignment in a class doesn't apply: gDispcnt
is only declared here.

## PaletteCache::slots is u16 [16][16]

`slots` (sprite_obj.hpp, 0x2C) holds each OBJ palette bank's 16 colours,
and was `u8 [16][TILE_SIZE_4BPP]`. The language select and the continue
prompt fill it through `(u16 *)cache->slots[0]`; they now take
`cache->slots[0]` as it is. Credits' CopyPaletteToSlot takes a
`u16 (*)[16]`.

LoadSlot and GetSlot keep their `dst = slots[0]; ... dst += slot * 16;`
(in bytes before): the ROM adds `slots`' offset to `this` first, and
`dst = slots[slot]` adds the bank's offset first, which changes
palette_cache.o.

The cache reaches palette RAM through the `Dma*` helpers
(gba/dma_macros.h): LoadSlot and GetSlot copy a palette into a bank
with `DmaCopy16(3, src, dst, PALETTE_SIZE_16)`, UploadSlot copies one
bank, `DmaCopy16(3, slots[slot], OBJ_PLTT + (slot << 5), ...)`, and
Upload all of them, `DmaCopy16(3, slots, OBJ_PLTT, OBJ_PLTT_SIZE)`.
`slots` goes in as it is: `DmaSet` converts the addresses to `vu32`
itself. (Its `DMA3.src = (u32)slots` became these copies in #917, and
the `DMA3` register view itself went in #998.)

## Palette RAM: BG_PALETTE(n), OBJ_PALETTE(n), BACKDROP_COLOR

`PLTT`, `BG_PLTT` and `OBJ_PLTT` (gba/defines.h) are addresses, as
`REG_ADDR_*` are, and every pointer use of them cast them: 37 casts, to
`vu16 *` (the backdrop), `u16 *` or `void *` (a palette).

gfx.h now names palette RAM as the `REG_*` macros name the I/O
registers, with the one conversion from the address in the macro:

```c
#define BG_PALETTE(n)  ((u16 *)(BG_PLTT + (n) * PALETTE_SIZE_16))
#define OBJ_PALETTE(n) ((u16 *)(OBJ_PLTT + (n) * PALETTE_SIZE_16))
#define BACKDROP_COLOR (*(vu16 *)PLTT)
```

- `*(vu16 *)PLTT = 0` (8 menus and init paths) is `BACKDROP_COLOR = 0`.
- `(void *)(OBJ_PLTT + 14 * PALETTE_SIZE_16)` is `OBJ_PALETTE(14)`,
  `(u16 *)(BG_PLTT + PALETTE_SIZE_16)` is `BG_PALETTE(1)`, and the
  starfield's `(u16 *)(BG_PLTT + 240 * sizeof(u16))` (colours 240-255)
  is `BG_PALETTE(15)`. play_room.cpp's own `PAL_RAM` went.
- The `DmaCopy16`/`DmaFill16` destinations lost their `(void *)`: the
  macros cast to `vu32` themselves (zoom_bg.cpp already passed `BG_PLTT`).
- The DMA register writes (`dma->dst = OBJ_PLTT`) keep the addresses.

TitleScreen::LoadObjTiles' `u8 *paletteDest = (u8 *)OBJ_PLTT` is left: it
is a byte cursor, as its `tileDest` is.

## QueueVramDmaTransfer takes a const source

Every palette passed to QueueVramDmaTransfer is ROM data (`const u16`),
and the callers cast it to `void *`. Its `src` (and the queue entry's) is
now `const void *` (it has C linkage, so no mangled name changes), and
the 10 casts go. PolarAkuAku::Refresh keeps `(const u8 *)gPolarAkuAkuPalette1
+ (level - 1) * PALETTE_SIZE_16`: it reads past the array into
gPolarAkuAkuPalette2 and 3.
