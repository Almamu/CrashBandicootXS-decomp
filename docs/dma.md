# DMA

The game code issues every DMA transfer through the macros in
`platform/gba/include/gba/dma_macros.h`. The registers are never written field by
field (#912). All game transfers use channel 3 and start immediately.
None of them repeat or raise an IRQ.

## The helpers

Each macro expands to `DmaSet`'s store sequence: a `vu32 *` to the
channel's `SAD`, then the stores to `SAD`, `DAD` and `CNT`, then one
volatile read of `CNT`. They differ only in the control word and in
where a fill's value comes from. Sizes are in **bytes**; the macro
divides by the unit.

| macro | transfer | control |
|---|---|---|
| `DmaCopy16(n, src, dest, size)` | copy, 16-bit units | `0x8000xxxx` |
| `DmaCopy32(n, src, dest, size)` | copy, 32-bit units | `0x8400xxxx` |
| `DmaFill16(n, value, dest, size)` | fill from the helper's own `vu16` temporary | `0x8100xxxx` |
| `DmaFill32(n, value, dest, size)` | fill from the helper's own `vu32` temporary | `0x8500xxxx` |
| `DmaClear16/32(n, dest, size)` | `DmaFill16/32` of 0, through `_dest`/`_size` locals | as the fill |
| `DmaFill16From/32From(n, src, dest, size)` | fill from `*src`, a fixed source the caller owns | as the fill |
| `DmaWait(n)` | spin while `CNT`'s enable bit is set | - |
| `DmaSet(n, src, dest, control)` | the raw three stores and the read | given |

## Picking a fill

All three fill spellings produce the same register stores. They differ
only in the stack slot holding the value:

- `DmaFill16/32` declares a volatile temporary inside the macro. Use it
  first. The ROM stores the value, then takes its address.
- `DmaFill16From/32From` reads from the caller's variable instead. Use
  it when the ROM keeps the value in a plain local, which lets one `#0`
  register serve both that store and a neighbouring one
  (`Starfield::Starfield`). Use it too when the source address is
  computed well before the fill (the same function's word fill), when
  the value is a struct field (`LevelState::PlayCutscene`'s `f.fill`),
  or when a macro reuses a caller's variable (`CLEAR_OAM`,
  engine/include/game/ui/frontend.hpp).
- `DmaClear16/32` copies the destination and size into locals first,
  which can move registers around (`SaveData::Validate`, where
  `DmaFill16` swaps three of them).

## Spelling notes

- Anything evaluated between two stores goes inside the argument that
  is stored next. `DmaCopy32(3, (const u8 *)asset + 4, dest, (*(const u32
  *)asset >> 8) - 4)` loads the header between `DAD` and `CNT`, as the
  ROM does.
- A local set from the same expression after the transfer reuses the
  macro's load. `CompanyLogos::DrawVvLogoPieces` fills `scratch`, then
  sets `buf = scratch` for its loop, and the ROM's one `scratch` load
  sits between `SAD` and `DAD` in `buf`'s register. Setting `buf`
  first moves the load ahead of the `SAD` store.
- `DmaSet` and the fills expand to a `{ }` block, so an `if`/`else`
  around them needs braces (`FlushVramDmaQueue`).

## Left alone

The SDK libraries write the registers themselves, and that is their
original source:

- AgbEeprom's `DMA3Transfer` (`lib/agb_eeprom/src/eeprom_timer_stop.c`).
- GAX's Direct Sound DMA1 re-arm with `GAX_DMA_WAIT` (`lib/gax`).
