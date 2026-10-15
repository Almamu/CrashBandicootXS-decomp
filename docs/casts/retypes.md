# Small retypes: fields and results the casts already said (#855)

A field or a result that every read cast the same way gets that type, and
the casts go. Each one was checked with the full build, object by object.

## Casts to the type the value already has (21)

| thing | declared | casts | sites |
|---|---|---:|---|
| `RandRange()` | `u16` (util.h) | 18 `(u16)` | the menus, the starfield, the zoom BG twinkles, the bosses, the polar and jetpack stages, the player |
| `rand()` | `u16` (util.h) | 3 `(u16)` | Crate::OpenMystery, ExtraLife::PickUp, Wumpa::PickUp |

A `u16` is promoted to `int` in an expression either way, so the casts
changed nothing. PolarPenguin::Update's `velY = -(s32)RandRange(0x300)`
keeps its `(s32)`: it says that the negation is a signed one.

## Signedness, from the ROM's reads (23)

| field | was | is | casts | the ROM |
|---|---|---|---:|---|
| `AnimPart::animTimer` (actor_self.hpp, 0x10) | `u16` | `s16` | 12 `(s16)` | every reader adds it to `animTime` with `ldrsh`. The writers copy `anims[i].duration` with `strh`, the same for either type. |
| `bg_package::width`, `height` (graphics_package.hpp) | `u32` | `s32` | 11 `(s32)` | the loops compare them with `blt`/`bge`/`ble` (signed). TitleScreen's `count = height * width` and BgSetup::Load's `src += width` are the same code for either type. The package tables in engine/src/*/data are unchanged. |
